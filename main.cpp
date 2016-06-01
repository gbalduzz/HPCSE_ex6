#include <iostream>
#include <string>
#include <vector>
#include "include/particles.h"
#include "include/timing.h"
#include "include/p2p.h"
#include "include/e2p.h"
#include "include/p2e.h"

using std::cout; using std::endl;
using std::vector;
using vd = vector<double>;
void Print(const vd& ,int n=-1);
void generateRandomData(Particles& ,Particles&);

#define ORDER 8

#define EXECUTE(p2e,e2p)\
  {						\
vector<double> cr(ORDER+1,0),ci(ORDER+1,0);	\
(p2e)(particles,cr,ci);				\
(e2p)(targets,cr,ci);				\
cout<<"cr: "; Print(cr);			\
 cout<<"ci: "; Print(ci);			\
Print(targets,3);				\
}

int main(int argc, char** argv) {
  constexpr int exp_order = 8;
  int Np=1e3;
  int Nt=1e2;
  if(argc==3){
    Np=atoi(argv[1]);
    Nt=atoi(argv[2]);
  }
  Particles particles(Np),targets(Nt);
  generateRandomData(particles,targets);
 
  cout<<"N# of particles: "<<particles.N<<endl;
  cout<<"N# of targets: "<<targets.N<<endl;
  
  
  // compute expansion with gcc only
    cout<<"\ngcc:\n";
    EXECUTE(p2e_gcc<ORDER>,e2p_gcc<ORDER>)
  
    cout<<"\nm4+ispc:\n";
    EXECUTE(p2e,e2p)
  
  //compute target locations with direct evaluations
  for(int i=0;i<targets.N;i++) targets.w[i]=p2p_gcc(particles,targets.x[i],targets.y[i]);
    Print(targets,5);
 
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
