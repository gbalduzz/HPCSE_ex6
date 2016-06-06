#include <cmath>
#include "cuda_potential.hpp"
#include "cuda_vector.hpp"
constexpr int threadsPerBlock = 128;

__global__ void p2e(double* xv,double* yv, double* wv,double* cr,double* ci, const int N,const int order){
  //cr and ci must have order+1 reserved spaces
  const int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N){
      const double x=xv[i];
      const double y=yv[i];
      const double w=wv[i];
      cr[0]+=w;
      double z_re = x;
      double z_im = y;

    for(int k=1;k<=order;k++){
      cr[k]-=w*z_re;
      ci[k]-=w*z_im;
      //compute z=(x+i y)^k
      const double temp = z_re*x-z_im*y;
      z_im= z_re*y+z_im*x;
      z_re = temp;
    }
  }
}

__global__ void divide(double* cr,double* ci, const int order){
  //call with one block
  const int i = threadIdx.x;
  if(i>1 && i<=order){
    ci[i] /= i;
    cr[i] /= i;
  }
}

__global__  void e2p(const double* xv,const double* yv, double* rv,
                const double* cr,const double* ci, const int N, const int order){
  const int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N) {
    //cr[0]=Q
    const double x = xv[i];		   
    const double y = yv[i];
    double result = cr[0] * 0.5 * std::log(x * x + y * y);
    double zr = 1, zi = 0;
   
//#pragma unroll(4)
    for (int k = 1; k < order + 1; k++) {
      const double temp = zr * x - zi * y;
      zi = zr * y + zi * x;
      zr = temp;
      //result += real(a[k]/z**k)
      result += (cr[k] * zr + ci[k] * zi) / (zr * zr + zi * zi);
    }
    rv[i] = result;
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

  p2e<<<Np/threadsPerBlock,threadsPerBlock>>>(p.x,p.w,p.w,cr,ci,Np,order);
  divide<<<1,order+1>>>(cr,ci,order);
  e2p<<<Np/threadsPerBlock,threadsPerBlock>>>(t.x,t.y,t.w,cr,ci,Nt,order);

  dtw.copyTo(t.w);
}
 
