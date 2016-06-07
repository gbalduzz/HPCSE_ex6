#include <cmath>
#include "cuda_potential.hpp"
#include "cuda_vector.hpp"
#include "cuda_kernels.cuh"
#include "p2e.h"
constexpr int threadsPerBlock = 128;




void cudaPotential(const Particles& p, Particles& t, const int order){
  const int Np= p.N;
  const int Nt= t.N;
  // start copying while compuing p2e
  CudaVector<double> dtx(Nt,t.x),dty(Nt,t.y);
  CudaVector<double> dtw(Nt);
  std::vector<double> crd(order+1,0),cid(order+1,0);
  p2e(p, crd,cid);
  CudaVector<double> cr(order+1,crd.data()),ci(order+1,cid.data());
//todo time
  e2p<<<Np/threadsPerBlock,threadsPerBlock>>>(t.x,t.y,t.w,cr,ci,Nt);

  dtw.copyTo(t.w);
}
 
