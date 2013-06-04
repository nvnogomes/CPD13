#include <stdio.h>
#include <stdlib.h>
#include <ff/utils.hpp>

#define N 1024

static double A[N][N];
static double B[N][N];
static double C[N][N];




int main() {
  
  /* init */
  for(int i=0;i<N;++i) 
	for(int j=0;j<N;++j) {
	  A[i][j] = (i+j)/(i+1);
	  B[i][j] = i*j*3.14;
	  C[i][j] = 0.0;
	}
  
  ff::ffTime(ff::START_TIME);

#if defined(OPTIMIZE_CACHE)
  for(int i=0;i<N;++i) 
	for(int j=0;j<N;++j)
	  for(int k=0;k<N;++k)
                C[j][k] += A[j][i]*B[i][k];

#else
  #pragma omp parallel for
  for(int i=0;i<N;++i) 
	for(int j=0;j<N;++j)
	  for(int k=0;k<N;++k)
		C[i][j] += A[i][k]*B[k][j];
#endif

  ff::ffTime(ff::STOP_TIME);
  std::cerr << "Computation time= " << ff::ffTime(ff::GET_TIME) << " (ms)\n";
  
#if 0
  for(int i=0;i<N;++i)  {
	  for(int j=0;j<N;++j)
	    printf(" %ld", C[i][j]);
	  
	  printf("\n");
    }
#endif
    return 0;
	
}


