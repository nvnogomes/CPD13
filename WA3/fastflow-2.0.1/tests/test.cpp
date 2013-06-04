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
 * Mixing FastFlow pipeline with farm stages in the accelerator mode
 *
 *
 *                        |(stage1)|        |(stage2)|   
 *                        |        |        |        |  
 *    main-flow ---->farm |(stage1)|---farm |(stage2)|--------   
 *        .               |        |        |        |       | 
 *        .               |(stage1)|        |(stage2)|       | 
 *        .                                                  |
 *    main-flow <---------------------------------------------         
 *
 *
 */

#include <stdlib.h>
#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <ff/allocator.hpp>
#include <ff/mapping_utils.hpp>


#if __linux__
#include <sys/time.h>
#include <sys/resource.h>
#include <asm/unistd.h>
#define gettid() syscall(__NR_gettid)
#else
#define gettid() 0
#endif

using namespace ff;

enum { IN_QUEUE_SIZE=1024, OUT_QUEUE_SIZE=2048};

// define your task
typedef int my_task_t;

class Stage1: public ff_node {
public:
    Stage1(int id):num_tasks(0), id(id){}

    void * svc(void * task) {
        ++num_tasks;
        return task;
    }

    void svc_end() {
        std::cout << "Stage1-Worker" << id << " Received " << num_tasks << " tasks." << std::endl;
    }
private:
    int num_tasks;
    int id;
};

class Stage2: public ff_node {
public:
    Stage2(int id):num_tasks(0), id(id){}

    void * svc(void * task) {
        ++num_tasks;
        return task;
    }

    void svc_end() {
        std::cout << "Stage2-Worker" << id << " Received " << num_tasks << " tasks." << std::endl;
    }
private:
    int num_tasks;
    int id;
};
class Emitter: public ff_node {
public:
    Emitter(int id):ntasks(0),id(id) {}
    void * svc(void * task) { 
        if (id ==1) {
            for(int i=0;i<10000;++i) {
                my_task_t * task = new my_task_t(i);
                ff_send_out(task);
            }
            return NULL;
        }
        ++ntasks;
        return task;
    }
    void svc_end() {
        printf("Emitter %d ntasks=%d\n", id, ntasks);
    }
    void eosnotify(int) { 
        printf("Emitter %d EOS arrived, ntasks=%d\n", id,ntasks);
    }   
private:
    int ntasks;
    int id;
};

class Collector: public ff_node {
public:
    Collector(int id):ntasks(0),id(id) {}
    void * svc(void * task) { 
        ++ntasks;
        return task;
    }
    void svc_end() {
        printf("Collector %d ntasks=%d\n", id, ntasks);
    }
    void eosnotify(int) { 
        printf("Collector %d EOS arrived, ntasks=%d\n", id,ntasks);
    }       

private:
    int ntasks;
    int id;
};



int main(int argc, char * argv[]) {
    void * result = NULL;

    int streamlen= 0;
    int nworkers  = 0;

    if (argc != 3) {
        std::cerr << "use:\n" << " " << argv[0] << " stream-length num-farm-workers\n";
        return -1;
    }
    streamlen=atoi(argv[1]);
    nworkers  =atoi(argv[2]);

    ff_farm<> farm_1;

    Emitter E(1);
    Collector C(1);
    farm_1.add_emitter(&E); 
    //farm_1.add_collector(NULL);
    farm_1.add_collector(&C);
    std::vector<ff_node *> w;
    for(int i=0;i<nworkers;++i) {
        w.push_back(new Stage1(i));
    }
    farm_1.add_workers(w);

    ff_farm<> farm_2;

    Emitter E2(2);
    Collector C2(2);
    farm_2.add_emitter(&E2); 
    farm_2.add_collector(&C2);
    std::vector<ff_node *> w2;
    for(int i=0;i<nworkers;++i) {
        w2.push_back(new Stage2(i));
    }
    farm_2.add_workers(w2);

    ff_pipeline * pipe = new ff_pipeline;
    pipe->add_stage(&farm_1);
    pipe->add_stage(&farm_2);


    pipe->run_and_wait_end();
    return 0;

}
