/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

/*! 
 *  \link
 *  \file map.hpp
 *  \ingroup high_level_patterns_shared_memory
 *
 *  \brief This file describes the map skeleton.
 */
 
#ifndef _FF_MAP_HPP_
#define _FF_MAP_HPP_
/* ***************************************************************************
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License version 3 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 */


#include <vector>
#include <ff/svector.hpp>
#include <ff/gt.hpp>
#include <ff/lb.hpp>
#include <ff/node.hpp>
#include <ff/farm.hpp>
#include <ff/partitioners.hpp>

#if defined(OCL)
#include <ff/oclnode.hpp>
#endif


namespace ff {

/*!
 *  \ingroup high_level_patterns_shared_memory
 *
 *  @{
 */

/*!
 * \class map_lb
 *  \ingroup high_level_patterns_shared_memory
 *
 * \brief A loadbalancer for the \p map skeleton.
 *
 * The map loadbalancer extends the \p ff_loadbalancer and uses
 * ff_loadbalancer's method \p broadcast_task() to send task to all workers.
 *
 * This class is defined in \ref map.hpp
 *
 */
class map_lb: public ff_loadbalancer {
public:
    /**
     * Default constructor
     *
     * \param max_num_workers max number of workers
     */
    map_lb(int max_num_workers):ff_loadbalancer(max_num_workers) {}
    
    /// Broadcast the task to all workers
    void broadcast(void * task) {
        ff_loadbalancer::broadcast_task(task);
    }   
};

/*!
 * \class map_gt
 *  \ingroup high_level_patterns_shared_memory
 *
 * \brief A gatherer for the \p map skeleton.
 *
 * The map gatherer extends the \p ff_gatherer and uses ff_gatherer's
 * method \p all_gather() to collect the result from all workers.
 *
 * This class is defined in \ref map.hpp
 */
class map_gt: public ff_gatherer {
public:
    /**
     * Default constructor
     *
     * \param max_num_workers max number of workers
     */
    map_gt(int max_num_workers):ff_gatherer(max_num_workers) {}
    
    /**
     * It collects results from all tasks.
     *
     * \return TODO
     */
    int all_gather(void * task, void **V) {
        return ff_gatherer::all_gather(task,V);
    }   
};

/*!
 * \class ff_map
 *  \ingroup high_level_patterns_shared_memory
 *
 * \brief The map skeleton.
 *
 * The map skeleton, that extends the \p farm skeleton.
 *
 * This class is defined in \ref map.hpp
 * 
 */
class ff_map: public ff_farm<map_lb,map_gt> {
public:
    // REW
    /**
     *  worker function type
     *  Function called by each worker thread as soon as an input task is received.
     *  The first parameter is the partitioner (that can be user-defined or one 
     *  of those provided in the partitioners.hpp file) used the get a task 
     *  partition for the worker.
     *  tid is the worker/thread id (from 0 to mapWorkers-1).
     */
    typedef void* (*map_worker_F_t) (basePartitioner*const, int tid);

    /**
     *  reduce function type
     *  It gets in input the array of tasks sent by each worker 
     *  (one for each worker).
     *  vsize is the size of the V array.
     */
    typedef void* (*reduce_F_t) (void** V, int vsize);
    
private:
    // Emitter, Collector and Worker of the farm.
    /**
     * Emitter
     *
     * \return TODO
     */
    class mapE: public ff_node {
    public:
        mapE(map_lb * const lb, void* oneShotTask): lb(lb),ost(oneShotTask) {}	
        void * svc(void * task) {
            if (task==NULL) { 
                if (ost) lb->broadcast(ost);
                return NULL;
            }
            lb->broadcast(task);
            return GO_ON;
        }
    private:
        map_lb* lb;
        void*   ost;
    };
    
    /**
     * Collector
     *
     * \return TODO
     */
    class mapC: public ff_node {
    public:
        mapC(map_gt * const gt, reduce_F_t reduceF): gt(gt),reduceF(reduceF) {}	
        
        void * svc(void *task) {
            int nw= gt->getnworkers();
            svector<void*> Task(nw);
            gt->all_gather(task, &Task[0]);
            if (reduceF) return reduceF(Task.begin(), nw);
            return Task[0];  // "default" reduceF
        }
    private:
        map_gt* const gt;
        reduce_F_t reduceF;
    };
    
    /**
     * Worker
     *
     * \return TODO
     */
    class mapW: public ff_node {
    public:
        mapW(map_worker_F_t mapF, basePartitioner *const P):mapF(mapF),P(P) {}
        void * svc(void * task) {
            P->setTask(task);
            return mapF(P,ff_node::get_my_id());
        }
    private:
        map_worker_F_t  mapF;
        basePartitioner * const P;
    };

public:

    /**  
     *  Public Constructor (1).
     *
     *  This constructor allows to activate the map for working on a stream of
     *  tasks or as a software accelerator by setting \p input_ch \p = \p
     *  true.
     *
     *  \param mapF Specifies the \p Worker object that will execute the
     *  operations.
     *  \param mapP It is the partitioner that is responsible to partition the
     *  problem.
     *  \param reduceF The \p Reduce object. This parameter is optional and is
     *  to be specified when using a \a MapReduce skeleton. Defult is \p
     *  NULL.
     *  \param input_ch Specifies whether the map skeleton is used as an
     *  accelerator. Default is \p false.
     */
    ff_map ( map_worker_F_t mapF, 
             basePartitioner* mapP,
             reduce_F_t reduceF=NULL, 
             bool input_ch=false
           ) : ff_farm<map_lb,map_gt>(input_ch), mapP(mapP) 
    {
        add_emitter(new mapE(getlb(),NULL));
        add_collector(new mapC(getgt(), reduceF));
        std::vector<ff_node *> w;
        for(size_t i=0;i<mapP->getParts();++i) w.push_back(new mapW(mapF,mapP));
        add_workers(w);
    }

    /**  
     *  Public Constructor (2).
     *
     *  This constructor allows to activate the map for the computation of
     *  just one task
     *
     *  \param mapF Specifies the \p Worker object that will execute the operations.
     *  \param mapP It is the partitioner that is responsible to partition the 
     *                 problem.
     *  \param task The task to be executed.
     *  \param reduceF The \p Reduce object. This parameter is optional and is 
     * to be specified when using a \a MapReduce skeleton. Defult is \p NULL.
     */
    ff_map ( map_worker_F_t mapF, 
             basePartitioner* mapP,
             void* task, 
             reduce_F_t reduceF=NULL
           ) : ff_farm<map_lb,map_gt>(false), mapP(mapP) 
    {
        add_emitter(new mapE(getlb(), task));
        add_collector(new mapC(getgt(),reduceF));
        std::vector<ff_node *> w;
        for(size_t i=0;i<mapP->getParts();++i) w.push_back(new mapW(mapF,mapP));
        add_workers(w);
    }
    
    /** 
     * Destructor 
     *
     * \return TODO
     */
    ~ff_map() {
        delete (mapE*)(getEmitter());
        delete (mapC*)(getCollector());
        ff_node** w= getWorkers();
        int nw= getNWorkers();
        for(int i=0;i<nw;++i) delete (mapW*)(w[i]);	
    }

    int   get_my_id() const { return -1; };
    
    /**
     * This method sets the affinity for the emitter and collector threads,
     * both are pinned on the same core.
     *
     * \param cpuID the ID of the cpu to which the threads will be pinned 
     */
    void  setAffinity(int cpuID) { 
        if (cpuID<0 || !threadMapper::instance()->checkCPUId(cpuID) ) {
            error("MAP, setAffinity, invalid cpuID\n");
        }
        ((mapE*)getEmitter())->setAffinity(cpuID);
        ((mapC*)getCollector())->setAffinity(cpuID);

        ff_node::setAffinity(cpuID);
    }

    /**
     * Retrivies the id of the cpu
     *
     * \return the core id
     */
    int   getCPUId() { return ff_node::getCPUId();}

    /**
     * TODO
     */
    int wrap_around() {
        error("MAP, feedback channel between Emitter and Collector not supported\n");
        return -1;
    }

    /**
     * TODO
     */
    double ffTime() {
        return diffmsec(getgt()->getstoptime(),
                        getlb()->getstarttime());
    }
    
    /**
     * TODO
     */
    double ffwTime() {
        return diffmsec(getgt()->getwstoptime(),
                        getlb()->getwstartime());
    }
    
private:
    basePartitioner* mapP;
};



#if defined(OCL)

#define OCLMAPFUNC(name, basictype, param, code)                        \
    static const char name[] =                                          \
" __kernel void kern( __global " #basictype "* " #param ",  const  uint max_items) {\n"   \
"   int i = get_global_id(0); \n"                               \
"   while (i<max_items) { " #code ";\n"                   \
"        i+=1000;    }\n"                                     \
" }"


class baseTask {
public:
    baseTask():task(NULL) {}

    virtual size_t size() const =0;
    virtual size_t bytesize() const =0;
    
    void   setTask(void* t) { if (t) task=t;}
    void*  getTask() { return task;}

    void* task;
};


/*!
 * \class ff_mapOCL
 *  \ingroup high_level_patterns_shared_memory
 *
 * \brief The map skeleton.
 *
 * The map skeleton, that extends the \p farm skeleton.
 *
 * This class is defined in \ref map.hpp
 * 
 */
class ff_mapOCL: public ff_oclNode {
public:

    ff_mapOCL(const char* codestr, baseTask* Task):
        codestr(codestr), Task(Task) { }    


    ff_mapOCL(const char* codestr, baseTask* Task, void* task):
        codestr(codestr), Task(Task) {
        assert(task);
        Task->setTask(task);
        ff_node::skipfirstpop(true);
    }    
    
    int  run(bool=false) { return  ff_node::run(); }    
    int  wait() { return ff_node::wait(); }    

    int run_and_wait_end() {
        if (run()<0) return -1;           
        if (wait()<0) return -1;
        return 0;
    }

    double ffTime()  { return ff_node::ffTime();  }
    double ffwTime() { return ff_node::wffTime(); }

protected:
    inline void checkResult(cl_int s, const char* msg) {
        if(s != CL_SUCCESS) {
            std::cerr << msg << ":";
            printOCLErrorString(s,std::cerr);
        }
    }
    
    void svc_SetUpOclObjects(cl_device_id dId) {	                        
        cl_int status;							
        context = clCreateContext(NULL,1,&dId,NULL,NULL,&status);		
        checkResult(status, "creating context");				
        
        cmd_queue = clCreateCommandQueue (context, dId, 0, &status);	
        checkResult(status, "creating command-queue");			
        
        size_t sourceSize = strlen(codestr);					

        printf("kernel_name=kern\ncode=\n%s\n", codestr);
        
        program = clCreateProgramWithSource(context,1,			
                                            (const char **)&codestr,&sourceSize,&status);    
        checkResult(status, "creating program with source");		
        
        status = clBuildProgram(program,1,&dId,NULL,NULL,NULL);		
        checkResult(status, "building program");				
        
        kernel = clCreateKernel(program, "kern", &status);			
        checkResult(status, "CreateKernel");				
        
        status = clGetKernelWorkGroupInfo(kernel, dId,			
                                          CL_KERNEL_WORK_GROUP_SIZE,sizeof(size_t), &workgroup_size,0); 
        checkResult(status, "GetKernelWorkGroupInfo");			
    }									
    
    void * svc(void* task) {				                        
        cl_int   status;
        cl_event events[2];
        size_t globalThreads[1];
        size_t localThreads[1];
        
        Task->setTask(task);
        size_t size = Task->size();

        if (size  < workgroup_size) {					
            localThreads[0]  = size;					
            globalThreads[0] = size;					
        } else {								
            localThreads[0]=workgroup_size;					
            globalThreads[0] = nextMultipleOfIf(size,workgroup_size);	
        }									


        inputBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,		
                                     Task->bytesize(), NULL, &status);      
        checkResult(status, "CreateBuffer");				
        
        status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);			
        checkResult(status, "setKernelArg 0");				
        status = clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *)&size);				
        checkResult(status, "setKernelArg 1");				
        
        status = clEnqueueWriteBuffer(cmd_queue,inputBuffer,CL_FALSE,0,	
                                      Task->bytesize(), Task->getTask(),0,NULL,NULL);			
        checkResult(status, "copying Task to device input-buffer");		
        
        status = clEnqueueNDRangeKernel(cmd_queue,kernel,1,NULL,		
                                        globalThreads,localThreads,0,NULL,&events[0]);             
        status = clWaitForEvents(1, &events[0]);				
        
        status = clEnqueueReadBuffer(cmd_queue,inputBuffer,CL_TRUE, 0,	
                                     Task->bytesize(), Task->getTask(),0,NULL,&events[1]);          
        status = clWaitForEvents(1, &events[1]);				

        clReleaseEvent(events[0]);					
        clReleaseEvent(events[1]);					

        return task;
    }									
    
    void svc_releaseOclObjects(){						
        clReleaseKernel(kernel);						
        clReleaseProgram(program);						
        clReleaseCommandQueue(cmd_queue);					
        clReleaseMemObject(inputBuffer);					
        clReleaseContext(context);						
    }									

    
private:								
    baseTask* Task;
    const char* codestr;
    size_t workgroup_size;						
    cl_context context;							
    cl_program program;							
    cl_command_queue cmd_queue;						
    cl_mem inputBuffer;							
    cl_kernel kernel;							
    cl_int status;							
};
#endif // OCL 



    
/*!
*  @}
*  \endlink
*/
    
} // namespace ff

#endif /* _FF_MAP_HPP_ */
