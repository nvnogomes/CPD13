#include <cstdlib>
#include <iostream>
#include <ff/utils.hpp>


int main(int argc, char* argv[]) {
    if (argc != 3) {
	std::cerr << "use: " << argv[0] << " tasksize streamlen\n";
	return -1;
    }
    
    unsigned  taskSize = atoi(argv[1]);
    unsigned  nTasks=atoi(argv[2]);

#if !defined(OPENMP)
    double* t = new double[taskSize*taskSize];
    assert(t);
    double* myt = new double[taskSize*taskSize];
    assert(myt);
#endif   
    srandom(0); //::getpid()+(getusec()%4999));
    ff::ffTime(ff::START_TIME);

#if defined(OPENMP)
#pragma omp parallel for
#endif
    for(unsigned w=0;w<nTasks;++w) {
#if defined(OPENMP)
      double* t = new double[taskSize*taskSize];
      assert(t);
      double* myt = new double[taskSize*taskSize];
      assert(myt);
#endif   
      for(unsigned z=0;z<(taskSize*taskSize);++z) {
#if !defined(OPENMP)
	t[z] = 1.0*random() / (double)(taskSize);
#else
	t[z] = 1.0*(w+1) / (double)(taskSize);
#endif
      }

      bzero(myt,taskSize*taskSize*sizeof(double));
	
      for(unsigned i=0;i<taskSize;++i)
	for(unsigned j=0;j<taskSize;++j)
	  for(unsigned k=0;k<taskSize;++k)
#if defined(OPTIMIZE_CACHE)
	    myt[j*taskSize+k] += t[j*taskSize+i]*t[i*taskSize+k];
#else
	    myt[i*taskSize+j] += t[i*taskSize+k]*t[k*taskSize+j];
#endif
      
      memcpy(t,myt,taskSize*taskSize*sizeof(double));

#if defined(OPENMP)
      free(t);
      free(myt);
#endif
    }

    ff::ffTime(ff::STOP_TIME);
    printf("Time= %f ms\n", ff::ffTime(ff::GET_TIME));

    return 0;
}
    
