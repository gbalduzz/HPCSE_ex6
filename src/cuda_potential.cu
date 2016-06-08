#include <cmath>
#include "cuda_potential.hpp"
#include "cuda_vector.hpp"
#include "timing.h"
#include "cuda_kernels.cuh"
#include "p2e.h"
#include <iostream>
using std::cout; using std::endl;
constexpr int threadsPerBlock = 128;




void cudaPotential(const Particles& p, Particles& t, const int order){
  const int Np= p.N;
  const int Nt= t.N;
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);  
 
  reset_and_start_timer();
  // start copying while compuing p2e
  CudaVector<double> dtx(Nt,t.x),dty(Nt,t.y);
  CudaVector<double> dtw(Nt);
  std::vector<double> crd(order+1,0),cid(order+1,0);
  p2e(p, crd,cid);
  CudaVector<double> cr(order+1,crd.data()),ci(order+1,cid.data());
//time e2p
  cudaEventRecord(start);
  e2p<<<Np/threadsPerBlock,threadsPerBlock>>>(dtx,dty,dtw,cr,ci,Nt);
  cudaEventRecord(stop);
  dtw.copyTo(t.w);
  cudaEventSynchronize(stop);
  double tf = get_elapsed_mcycles();


  cout<<"CUDA total M cycles (CPU p2e + GPU e2p) : "<<tf<<endl;
  float GPU_ms; cudaEventElapsedTime(&GPU_ms, start, stop );
  cout<<"Time for e2p kernel: "<<GPU_ms<<endl;
}
 
