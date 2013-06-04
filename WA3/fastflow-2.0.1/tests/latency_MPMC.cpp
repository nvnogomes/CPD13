/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 */
/*
 * Simple test for the Multi-Producer/Multi-Consumer based on a modular visit
 * of a spsc queue vector.
 *
 * Author: Massimiliano Meneghin (MaX)
 *
 */
#include <iostream>
#include <ff/node.hpp>   // for Barrier
#include <ff/cycle.h>
#include <ff/atomic/atomic.h>
#include <ff/platforms/platform.h>
#include <ff/module_based_MPMC.hpp>
#include <ff/mapping_utils.hpp>


#include <ff/buffer.hpp>
#include <ff/ubuffer.hpp>
#include <ff/dynqueue.hpp>



#define NO_PADDING
#define INDEX
#define NO_POST_POLLING

#include <ff/staticlinkedlist.hpp>
#include "mytime.h"
//#include <ff/mapping_utils.hpp>

using namespace ff;

struct task_t{
	int id;
    double tot_usec;
    double mean_nsec; 
};


#define QUEUE_TYPE SWSR_Ptr_Buffer
//#define QUEUE_TYPE staticlinkedlist

#if defined(MSQUEUE)
 #include <ff/MPMCqueues.hpp>
 #if defined(SCALABLE_QUEUE)
  multiMSqueue    * global_mpmc;
 #else
  MSqueue         * global_mpmc;
 #endif //SCALABLE_QUEUE
#else
 module_based_MPMC<QUEUE_TYPE> * global_mpmc;
#endif //MSQUEUE

unsigned long global_iterations;


/*
 * SENSER ENGINE STUFF
 */
template<class spsc_queue>
void sender_run(spsc_queue * sq) {
    for (unsigned long i = 0; i < global_iterations; i++) {
		global_mpmc->push(sq);
		//printf("sent %p!!\n", sq);
		void * recv_data;
		do; while(!sq->pop(&recv_data));
        /*
		 * REMOVE THE CHECK FOR PERFORMANCE
		 */
        if(recv_data != (void*)sq){
         			printf("ERRROR!!\n");
         			exit(3);
        }
	}
}

template<class spsc_queue>
void * sender_engine(void * arg) {
	task_t *task = (task_t *)arg;
	/*
	 * Init SPSC queue
	 */
	spsc_queue * sq = new spsc_queue(1024);
    if (!sq->init()) abort();
	
	t_new_clock(crono);
	/*
	 * Run the while Test TODO take time here...
	 */
	t_start(crono);
	sender_run<spsc_queue> (sq);
	t_stop(crono);
	task->tot_usec = t_get_total_usec(crono);
	task->mean_nsec = (task->tot_usec/(double)global_iterations)*1000;
	//printf("Total time was %f uSec Totla mean time %f nsec.\n", task->tot_usec,  (task->tot_usec/global_iterations)*1000);
    return NULL;
}

/*
 * RECEIVER ENGINE STUFF
 */
__thread void *last_get_p;
__thread void *last_get_p_old;
template<class spsc_queue>
void receiver_run() {
	while(1) {
		void * recv_data=NULL;
#if defined(MSQUEUE)
        do ; while( !global_mpmc->pop(recv_data));
#else
		global_mpmc->pop(&recv_data);
#endif

		spsc_queue * sq = (spsc_queue *)recv_data;
        last_get_p_old = last_get_p;
        last_get_p = recv_data;
		do; while(!sq->push((void*)sq));
	}
}

template<class spsc_queue>
void * receiver_engine(void * arg) {
	task_t * task = (task_t *)arg;

    ff_mapThreadToCpu(task->id%ff_numCores());

	/*
	 * Run the while Test
	 */
	receiver_run<spsc_queue> ();
	/*
	 * Close Barrier
	 */
    return NULL;
}


int main(int argc, char** argv) {
    /*
     * Default configurations
     */
	int numC = 1;
	int numP = 1;
    int help = 0;
	global_iterations =9000000;

    int opt;
    while ((opt = getopt(argc, argv, "p:c:i:")) != -1) {
        switch (opt) {
        case 'p':
            numP = strtol(optarg, NULL, 0);
            if (numP < 1){
                printf("Error num producer must be >= 1\n");
                help =1;
            }
            break;

        case 'c':
            numC = strtol(optarg, NULL, 0);
            if (numC < 1){
                printf("Error num consumer must be >= 1\n");
                help =1;
            }
            break;

        case 'i':
            global_iterations = strtol(optarg, NULL, 0);
            if (global_iterations < 0){
                printf("Error num of iterations must be >= 1\n");
                help =1;
            }
            break;
        default:
            help = 1;
            break;
        }
    }

    if(help){
        printf(
               "Latency Test for MPMC queues \n"
               "Options:\n"
               "\t -c CONS_NUM  : number of consumers.  [1]\n"
               "\t -c PROD_NUM  : number of producers.  [1]\n"
               "\t -c ITERATION : number of iterations. [9000000]\n"
               );
        exit(0);
    }

    printf("Number of PROD %d CONS %d %lu iterations\n", numP, numC, global_iterations);

	/*
	 * Init of the MPMC queue to be tested
	 */

#if defined(MSQUEUE)
 #if defined(SCALABLE_QUEUE)

    const int nqueues = 2;
    global_mpmc = new multiMSqueue(nqueues);
 #else
    global_mpmc = new MSqueue;
    if (!global_mpmc->init()) abort();
 #endif //SCALABLE_QUEUE
#else
	global_mpmc = new module_based_MPMC<QUEUE_TYPE> (numP, numC);
#endif
	task_t * tasks_v;
	int num_threads = numC + numP;
	tasks_v = (task_t *)malloc((sizeof(task_t) + sizeof(pthread_t)) * num_threads);
	if (!tasks_v) {
		printf("Malloc error in the Test support \n");
		exit(1);
	}

	int i = 0;
	pthread_t * thread_v = (pthread_t *) &tasks_v[num_threads];
	for (i = 0; i < num_threads; i++) {
		/*
		 * IMPORTANT!! Senders will have lower ID than receivers
		 */
		tasks_v[i].id = i;
	}

		int iret;
	for (i = 0; i < num_threads; i++) {
		if(i<numP){
		iret = pthread_create(&thread_v[i], NULL, sender_engine<QUEUE_TYPE>, (void*) &tasks_v[i]);
		}else{
		iret = pthread_create(&thread_v[i], NULL, receiver_engine<QUEUE_TYPE>, (void*) &tasks_v[i]);
		}

		if (iret) {
			printf("Thread error in the Test support \n");
			exit(1);
		}
	}

    double tot_usec =0;
    double mean_nsec =0;
	for (i = 0; i < numP; i++) {
		void * res;
		pthread_join(thread_v[i], (void**) &res);
        tot_usec += tasks_v[i].tot_usec;
        mean_nsec += tasks_v[i].mean_nsec;
		//errors += res;
	}

    tot_usec /= numP;
    mean_nsec /= numP;
    
    printf("[GENERAL] \t %d Producers %d Consumers %lu iratations \n",numP, numC, global_iterations);
    printf("[ MEAN T ] \t ping-pong time %.2f msec (= %.2f usec), ping-pong latency %.2f (go and back)\n", 
           tot_usec/(double)1000,tot_usec,mean_nsec
           );
	for (i = 0; i < numP; i++) {
        printf("[PRODUCER %d] \t ping-pong engine time %f, ping-pong latency %f (go and back)\n", 
               i, tasks_v[i].tot_usec,tasks_v[i].mean_nsec
           );

	}


	return 0;
}
