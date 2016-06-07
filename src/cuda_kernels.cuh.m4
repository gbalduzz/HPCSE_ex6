include(src/unroll.m4)

__global__  void e2p(const double* xv,const double* yv, double* rv,
                const double* cr,const double* ci, const int N){
  const int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N) {
  
   const double z_re = xv[i];
   const double z_im = yv[i];
   const  double r2 = z_re*z_re+z_im*z_im;
   //cr[0]=Q
   double result = cr[0]*0.5*log(r2);
   const  double pow_re_1 = z_re / r2;
   const  double pow_im_1 = -z_im/r2;
   result += c_re[1]*pow_re_1-c_im[1]*pow_im_1;
   
   LUNROLL(k,2,eval(ORDER),`
  // loop iteration
    const double TMP(pow_re,k) = TMP(pow_re,eval(k-1))*pow_re_1 - TMP(pow_im, eval(k-1))*pow_im_1;
    const double TMP(pow_im,k) = TMP(pow_im, eval(k-1))*pow_re_1 + TMP(pow_re, eval(k-1))*pow_im_1;
    result += c_re[k]*TMP(pow_re,k)-c_im[k]*TMP(pow_im,k);
  ')
  rv[i] = result; 
  }
}
