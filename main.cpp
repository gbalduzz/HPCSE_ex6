#include <iostream>
#include <string>
#include "include/file_IO.h"
#include "include/particles.h"
#include "include/timing.h"
#include "morton_tree/build_tree.h"
#include "expansion/node_to_expansion.hpp"
using std::cout; using std::endl;
using std::vector;
void generateRandomData(Particles& ,Particles&);
#define ORDER 8

int main(int argc, char** argv) {
  constexpr int exp_order = 8;
  int Np=1e3;
  int Nt=1e2;
  if(argc==3){
    Np=atoi(argv[1]);
    Nt=atoi(argv[2]);
  }
  const int maxnodes= 4*Np/exp_order;
  Particles particles(Np),targets(Nt);
  generateRandomData(particles,targets);
 
  cout<<"N# of particles: "<<particles.N<<endl;
  cout<<"N# of targets: "<<targets.N<<endl;
  
  //reset_and_start_timer();
  // compute expansion with gcc only
  vector<int> cr(ORDER+1,0),ci(ORDER+1,0);
  p2e_gcc<ORDER>(particles,cr,ci);
  e2p_gcc<ORDER>(targets,cr,ci);
  Print(targets,5);

  
  //compute target locations with direct evaluations
  for(int i=0;i<targets.N;i++) targets.w[i]=p2p(particles,targets.x[i],targets.y[i]);
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
  const doble x0=4;
  const doble y0=1;
   for(int i=0;i<t.N;i++){
     const double theta= ran(mt)*M_PI;
     t.x[i]=x0+std::cos(theta);
     t.y[i]=y0+std::sin(theta);
     t.w[i]=0;
  }
}
