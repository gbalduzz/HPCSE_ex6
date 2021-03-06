#include <iostream>
#include <string>
#include <vector>
#include "particles.h"
#include "timing.h"
#include "p2p.h"
#include "e2p.h"
#include "p2e.h"
#include "cuda_potential.hpp"

using std::cout; using std::endl;
using std::vector;
using vd = vector<double>;
void Print(const vd& ,int n=-1);
void generateRandomData(Particles& ,Particles&);

// the macro ORDER is defined by cmake

#define EXECUTE(p2e,e2p)\
  {						\
vector<double> cr(ORDER+1,0),ci(ORDER+1,0);	\
reset_and_start_timer();			\
(p2e)(particles,cr,ci);				\
double t1 = get_elapsed_mcycles();		\
(e2p)(targets,cr,ci);				\
double t2 = get_elapsed_mcycles();		\
cout<<"P2e milion cycles:  "<<t1<<endl;	\
cout<<"E2p milion cycles:  "<<t2-t1<<endl;	\
Print(targets,3);				\
  }

int main(int argc, char** argv) {
  constexpr int exp_order = 8;
  int Np=1e6;
  int Nt=1e6;
  if(argc==3){
    Np=atoi(argv[1]);
    Nt=atoi(argv[2]);
  }
  Particles particles(Np),targets(Nt);
  generateRandomData(particles,targets);
 
  cout<<"N# of particles: "<<particles.N<<endl;
  cout<<"N# of targets: "<<targets.N<<endl;

  
  // compute expansion with c++ only
    cout<<"\nc++ only:\n";
    EXECUTE(p2e_cxx<ORDER>,e2p_cxx<ORDER>)
  
    cout<<"\nm4+ispc:\n";
    EXECUTE(p2e,e2p)

  cout<<"\nCUDA:\n";
  cudaPotential(particles,targets,exp_order);
  Print(targets,3);
  
  //compute target locations with direct evaluations
  //for(int i=0;i<targets.N;i++) targets.w[i]=p2p_cxx(particles,targets.x[i],targets.y[i]);
  //Print(targets,5);
 
}

#include <random>
void generateRandomData(Particles& p,Particles& t){
  std::mt19937_64 mt(0);
  std::uniform_real_distribution<double> ran(-1,1);
  for(int i=0;i<p.N;i++){
    p.x[i]=ran(mt);
    p.y[i]=ran(mt);
    p.w[i]=ran(mt);
  }
  const double x0=4;
  const double y0=1;
   for(int i=0;i<t.N;i++){
     const double theta= ran(mt)*M_PI;
     t.x[i]=x0+std::cos(theta);
     t.y[i]=y0+std::sin(theta);
     t.w[i]=0;
  }
}

void Print(const vd& v,int n){
  n= n<=0 ? v.size() : std::min((int)v.size(),n);
  for(int i=0;i<n;i++)  cout<<v[i]<<"\t";
  cout<<endl;
}
