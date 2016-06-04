#pragma once
#include<cuda.h>

template<class T>
class CudaVector{
public:
  CudaVector(int n):size(n* sizeof(T)){cudaMalloc((**void)&dp,n);}
  CudaVector(int n,const T* p);
  ~CudaVector(){cudaFree(dp);}
  operator T*(){return dp;}
  void operator = (const T* );
  void copyTo(T* dest){
    cudaMemcpy(dest,dp,size,cudaMemcpyDeviceToHost);
  } 
private:
  T* dp=NULL;
  int size;
};

template<class T>
void CudaVector<T>::operator = (const T* p){
  cudaMemcpy(cv.dp,p,cv.size,cudaMemcpyHostToDevice);
}

template<class T>
CudaVector<T>::CudaVector(int n, const T *p): size(n* sizeof(T){
  cudaMalloc((**void)&dp,n);
  *this = p;
}

