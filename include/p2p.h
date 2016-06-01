#pragma once
#include "p2p.ispc.h"
inline double diffMod(double x1,double x2,double y1,double y2){
  const double dx=x1-x2;
  const double dy=y1-y2;
  return dx*dx+dy*dy;
}

inline double p2p(const Particles& p, const double x, const double y){
  return ispc::p2p(p.x,p.y,p.w,x,y,p.N);
}


double p2p_cxx(const Particles& p, const double x, const double y){
  double res=0;
  for(int i=0;i<p.N;i++) res+=p.w[i]*std::log(diffMod(x,p.x[i],y,p.y[i]));
  return res/2.;
}
