#pragma once
#include <assert.h>

struct Particles{
  double  *x,*y,*w;
  int N;
  bool is_a_copy;

  Particles(){is_a_copy=true;}
  Particles(int N0);
  inline Particles(double* x0,double* y0,double* w0,const int N0):
    x(x0),y(y0),w(w0),N(N0),is_a_copy(true){}
  ~Particles();
  inline Particles subEnsamble(int i0,int l) const{
    assert(i0+l <= N);
    return Particles(x+i0,y+i0,w+i0,l);
  }
  void resize(int N);
  friend void swap(Particles& a,Particles& b);
};

