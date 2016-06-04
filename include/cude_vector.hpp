#pragma once
#include<cuda.h>

template<class T>
class CudaVector{
public:
  CudaVector(int n):size(n* sizeof(T)){cudaMalloc((**void)&dp,n);}
  CudaVector(int n,const T* p);
  ~CudaVector(){cudaFree(dp);}
  operator T*(){return dp;}
  friend void operator = (T*,const CudaVector<T>&);
  friend void operator = (CudaVector<T>& ,const T* );
private:
  T* dp=NULL;
  int size;
};

template<class T>
void operator = (T* p, const CudaVector<T>& cv){
  cudaMemcpy(p,cv.dp,cv.size,cudaMemcpyDeviceToHost);
}

template<class T>
void operator = (CudaVector<T>& cv, const T* p){
  cudaMemcpy(cv.dp,p,cv.size,cudaMemcpyHostToDevice);
}

template<class T>
CudaVector<T>::CudaVector(int n, const T *p): size(n* sizeof(T){
  cudaMalloc((**void)&dp,n);
  *this = p;
}

