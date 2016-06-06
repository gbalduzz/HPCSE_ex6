#pragma once
#include <vector>
#include <cmath>
#include "particles.h"
#include "e2p.ispc.h"
using std::vector;

template<int K>
double e2p_cxx(const double z_re,const double z_im,const double* c_re,const double* c_im){
  //c_re[0]=Q
  double result= c_re[0]*0.5*std::log(z_re*z_re+z_im*z_im);
  double zk_re=1,zk_im=0;
#pragma unroll(4)
  for(int k=1;k<K+1;k++) {
    const double temp=z_re*zk_re-z_im*zk_im;
    zk_im=z_re*zk_im+z_im*zk_re;
    zk_re=temp;
    //result += real(a[k]/z**k)
    result += (c_re[k]*zk_re+c_im[k]*zk_im)/(zk_re*zk_re+zk_im*zk_im);
  } 
  return result;
}

template<int ord>
void e2p_cxx(Particles& t,const vector<double>& cr,const vector<double>& ci){
  for(int i=0; i<t.N;i++) t.w[i]= e2p_cxx<ord>(t.x[i],t.y[i],cr.data(),ci.data());  
}

inline void e2p(Particles& t,const vector<double>& cr,const vector<double>& ci){
  ispc::e2p(t.x,t.y,cr.data(),ci.data(),t.w,t.N);
}
