#pragma once
#include <cuda.h>
#include <new>

template<class T>
class CudaVector{
public:
  CudaVector(int n);
  CudaVector(int n,const T* p);
  ~CudaVector(){cudaFree(dp);}
  operator T*(){return dp;}
  void operator = (const T* );
  void copyTo(T* dest){
    cudaMemcpyAsync(dest,dp,size,cudaMemcpyDeviceToHost);
  } 
private:
  T* dp=NULL;
  int size;
};

template<class T>
CudaVector<T>::CudaVector(int n):
  size(n* sizeof(T)){
  cudaError_t err = cudaMalloc((void**)(&dp),size);
  if(err != cudaSuccess) throw(std::bad_alloc());
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

