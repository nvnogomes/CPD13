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

#include <cmath>
#include <iostream>
#include <ff/farm.hpp>


// run : farm150 8 1000 0

using namespace ff;

const long MAX_VALUE=1048576;

// generic worker
class Worker: public ff_node {
public:
    Worker():counter(0) {}

    void * svc(void * task) {
        unsigned long N = (unsigned long)task;

        
#if 0        
        //usleep((ff_node::get_my_id()+1)*100000); // <-------       
        double S = 0.0;
        for (unsigned long j=1; j<(N+1); j++) {
            S += j*sin((double)j/(random()*1.0));
        }
#endif

        ticks_wait(N*4096);

        fprintf(stdout, "%d: %lu\n", ff_node::get_my_id(), counter);
        fflush(stdout);

        ++counter;
        return GO_ON;
    }
#if 0    
    void svc_end() {
        std::cout << ff_node::get_my_id() << ": " << counter << " tasks computed\n";
    }
#endif

private:
    unsigned long counter;
};

// the load-balancer filter
class Emitter: public ff_node {
public:
    Emitter(int nworkers, unsigned long maxtasks):sent(0),maxtasks(maxtasks),nworkers(nworkers) {};

    void * svc(void *) {	

        usleep(random()%150000);  // <--------
        
        unsigned long task= (random() % MAX_VALUE) + 1;
        ff_send_out((void *)task);
        ++sent;
        fprintf(stdout, "%lu: %lu\n", nworkers, sent);
        fflush(stdout);

    
        if (sent >= maxtasks) return NULL;
        return GO_ON;
    }
private:
    unsigned long sent;
    const unsigned long maxtasks;
    const unsigned long nworkers;
};


int main(int argc, char * argv[]) {

    if (argc<4) {
        std::cerr << "use: " 
                  << argv[0] 
                  << " nworkers streamlen seed\n";
        return -1;
    }
    
    int nworkers =atoi(argv[1]);
    int streamlen=atoi(argv[2]);
    int seed = atoi(argv[3]);

    if (nworkers<=0 || streamlen<=0) {
        std::cerr << "Wrong parameters values\n";
        return -1;
    }
    
    srandom(seed);

    ff_farm<> farm(false, 1024*nworkers); // farm object
    
    Emitter E(nworkers, streamlen);
    farm.add_emitter(&E);
    farm.set_scheduling_ondemand();

    std::vector<ff_node *> w;
    for(int i=0;i<nworkers;++i) w.push_back(new Worker);
    farm.add_workers(w); // add all workers to the farm

    if (farm.run_and_wait_end()<0) {
        error("running farm\n");
        return -1;
    }

    //std::cerr << "DONE, time= " << farm.ffTime() << " (ms)\n";
    //farm.ffStats(std::cerr);

    return 0;
}
