#pragma once
#include "cuda_vector.hpp"
constexpr int threadsPerBlock = 128;

__global__ void p2e(double* x,double* y, double* w,double* cr,double* ci, const int N,const int order){
  //c_re and c_im must have order+1 reserved spaces
  const int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N){
      const double x=p.x[i];
      const double y=p.y[i];
      const double w=p.w[i];
      c_re[0]+=w;
      //sum_kth_coeff<1,k>::execute(c_re.data(),c_im.data(),x,y,w,x,y);
    double z_re = x;
    double z_im = y;

    for(int k=1;k<=order;k++){
      c_re[k]-=w*z_re;
      c_im[k]-=w*z_im;            //compute z=(x+i y)^k
      const double temp = z_re*x-z_im*y;
      z_im= z_re*y+z_im*x;
      z_re = temp;
    }
  }
}

void cudaPotential(const Particles& p, Particles& t, const int order){
  const int Np= p.N;
  const int Nt= t.N;
  CudaVector<double> dpx(Np,p.x),dpy(Np,p.y),dpw(Np,p.w),dtx(Nt,t.x),dty(Nt,t.y),dtw(Nt);
  CudaVector<double> cr(order),ci(order);
//do a bunch of stuff
  //p2e
  p2e<<<Np/threadsPerBlock,threadsPerBlock>>>(p.x,p.w,p.w,cr,ci,Np,order);
  // for(int i=2;i<k+1;i++) {c_re[i]/=i;c_im[i]/=i;}
  //e2p
  t.w = dtw;
}

