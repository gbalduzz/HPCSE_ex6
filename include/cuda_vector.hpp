#pragma once
#include<cuda.h>

template<class T>
class CudaVector{
public:
  CudaVector(int n);
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
CudaVector<T>::CudaVector(int n):
  size(n* sizeof(T)){
  cudaMalloc((void**)(&dp),size);
}

template<class T>
void CudaVector<T>::operator = (const T* p){
  cudaMemcpy(dp,p,size,cudaMemcpyHostToDevice);
}

template<class T>
CudaVector<T>::CudaVector(int n, const T *p): 
  CudaVector(n){
  *this = p;
}

