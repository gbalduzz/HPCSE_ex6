include(src/unroll.m4)


export void e2p(const uniform double zrv[],const uniform double ziv[],
       const uniform double c_re[], const uniform double c_im[],
       uniform double out[], const uniform int N){
 foreach(p=0 ... N){
  const double z_re = zrv[p];
  const double z_im = ziv[p];
  const  double r2 = z_re*z_re+z_im*z_im;
  double result = c_re[0]*0.5*log(r2);
  const  double pow_re_1 = z_re / r2;
  const  double pow_im_1 = -z_im/r2;

  result += c_re[1]*pow_re_1-c_im[1]*pow_im_1;
  LUNROLL(i,2,eval(ORDER),`
  // loop iteration
    const double TMP(pow_re,i) = TMP(pow_re,eval(i-1))*pow_re_1 - TMP(pow_im, eval(i-1))*pow_im_1;
    const double TMP(pow_im,i) = TMP(pow_im, eval(i-1))*pow_re_1 + TMP(pow_re, eval(i-1))*pow_im_1;
    result += c_re[i]*TMP(pow_re,i)-c_im[i]*TMP(pow_im,i);
  ')
  out[p] = result; 
 }
}      
  
