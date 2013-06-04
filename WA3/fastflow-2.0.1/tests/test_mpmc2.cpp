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
 * Test for the Multi-Producer/Multi-Consumer bounded and unbounded queues.
 *
 * Author: Massimo Torquati
 *  October  2012  first version
 *
 */
#include <ff/node.hpp> 
#include <ff/sysdep.h>
#include <ff/MPMCqueues.hpp>

using namespace ff;

#define _RAND_H_
#define SIM_RAND_MAX         32767

/*
 * This random generators are implementing 
 * by following POSIX.1-2001 directives.
 */

__thread unsigned long next = 1;

inline static long simRandom(void) {
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}

inline static void simSRandom(unsigned long seed) {
    next = seed;
}

/*
 * In Numerical Recipes in C: The Art of Scientific Computing 
 * (William H. Press, Brian P. Flannery, Saul A. Teukolsky, William T. Vetterling;
 *  New York: Cambridge University Press, 1992 (2nd ed., p. 277))
 */
inline static long simRandomRange(long low, long high) {
    return low + (long) ( ((double) high)* (simRandom() / (SIM_RAND_MAX + 1.0)));
}


#if defined(USE_BOUNDED_QUEUE)
MPMC_Ptr_Queue* Q = NULL;
#else
//uMPMC_Ptr_Queue* Q = NULL;
multiSWSR* Q = NULL;
#endif


class Thread: public ff_node {
public:
    Thread(long runs):runs(runs),fails(0) {}

#if 0
    int svc_init() {
        printf("thread %d running on cpu %d runs=%ld\n", 
               get_my_id(), ff_getMyCpu(), runs);
        return 0;
    }
#endif

    void* svc(void*) {
        void* task = (void*)(get_my_id() + 100);
        for(long i=0;i<runs;++i) {
            while(!Q->push(task,get_my_id())) {PAUSE(); fails++;} 

            // do work
            long v = simRandomRange(1,64);
            for(volatile long j=0;j<v;++j) ;

            while(!Q->pop(&task,get_my_id())) {PAUSE(); fails++;}

            // do work
            v = simRandomRange(1,64);
            for(volatile long j=0;j<v;++j) ;
        }
        return NULL;
    }
    void svc_end() {
        printf("thread %d, fails=%ld\n", get_my_id(), fails);
    }

    void set_id(int id) {
        ff_node::set_id(id);
    }
    void set_barrier(BARRIER_T * const b) { ff_node::set_barrier(b);}

    int run()    { return ff_node::run();}
    int wait()   { return ff_node::wait();}
    
    double wffTime() {
        return ff_node::wffTime();
    }
    
private:
    long runs;
    long fails;
};


const int  nqueues=ff_numCores();
const long RUNS=10000000;
int main(int argc, char * argv[]) {

    if (argc<2) {
        std::cerr << "use: " << argv[0] << " #threads\n";        
        return -1;
    }
    int nthreads= atoi(argv[1]);

#if defined(USE_BOUNDED_QUEUE)
    Q = new MPMC_Ptr_Queue();
    Q->init(RUNS);
#else
    //Q = new uMPMC_Ptr_Queue;
    //if (!Q->init(nqueues, 4096/nqueues)) abort();
    Q = new multiSWSR;
    if (!Q->init(nqueues)) abort();
#endif

    Thread* N[nthreads];

    BARRIER_T barr; 
    barr.barrierSetup(nthreads);


    for(int i=0;i<nthreads;++i) {
        N[i]= new Thread(RUNS/nthreads);
        assert(N[i]);
        N[i]->set_id(i);
        N[i]->set_barrier(&barr);
        N[i]->run();
    }
    for(int i=0;i<nthreads;++i) 
        N[i]->wait();

    std::vector<double > threadTime(nthreads,0.0);
    for(int i=0;i<nthreads;++i) {
        threadTime[i]=N[i]->wffTime();
        printf("thread %d time %.02f (ms)\n", i, threadTime[i]);
    }
    
    std::vector<double >::iterator it=
        std::max_element(threadTime.begin(),threadTime.end(),std::less<double>() );
    
    printf("Time %.02f (ms)\n", *it);
    return 0;
}
