#include <cmath>
#include "cuda_potential.hpp"
#include "cuda_vector.hpp"
#include "cuda_kernels.cuh"
constexpr int threadsPerBlock = 128;


__global__ void divide(double* cr,double* ci, const int order){
  //call with one block
  const int i = threadIdx.x;
  if(i>1 && i<=order){
    ci[i] /= i;
    cr[i] /= i;
  }
}


void cudaPotential(const Particles& p, Particles& t, const int order){
  const int Np= p.N;
  const int Nt= t.N;
  CudaVector<double> dpx(Np,p.x),dpy(Np,p.y),dpw(Np,p.w);
  CudaVector<double> dtx(Nt,t.x),dty(Nt,t.y);
  CudaVector<double> dtw(Nt);
  // are cr,ci set to zero?
  CudaVector<double> cr(order+1),ci(order+1);

  p2e<<<Np/threadsPerBlock,threadsPerBlock>>>(p.x,p.w,p.w,cr,ci,Np);
  divide<<<1,order+1>>>(cr,ci,order);
  e2p<<<Np/threadsPerBlock,threadsPerBlock>>>(t.x,t.y,t.w,cr,ci,Nt);

  dtw.copyTo(t.w);
}
 
