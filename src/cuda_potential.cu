#include "cuda_potential.hpp"
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

__global__ divide(double* cr,double* ci, const int order){
  //call with one block
  const int i = threadIdx.x;
  if(i>1 && i<=order){
    ci[i] /= i;
    cr[i] /= i;
  }
}

__global__  e2p(const double* xv,const double* yv, double* rv,
                const double* cr,const double* ci, const int N, const int order){
  const int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N) {
    //c_re[0]=Q
    double result = c_re[0] * 0.5 * std::log(zr * zr + zi * zi);
    double zr = 1, zi = 0;
    const double x = xv[i];
    const double y = yv[i];
#pragma unroll(4)
    for (int k = 1; k < order + 1; k++) {
      const double temp = zr * zk_re - zi * zk_im;
      zi = zr * zk_im + zi * zk_re;
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
  CudaVector<double> dpx(Np,p.x),dpy(Np,p.y),dpw(Np,p.w),dtx(Nt,t.x),dty(Nt,t.y),dtw(Nt);
  CudaVector<double> cr(order),ci(order);

  p2e<<<Np/threadsPerBlock,threadsPerBlock>>>(p.x,p.w,p.w,cr,ci,Np,order);
  divide<<<1,order>>>(cr,ci,order);
  e2p<<<Np/threadsPerBlock,threadsPerBlock>>>(t.x,t.y,t.w,cr,ci,Nt,order);

  t.w = dtw;
}

