# modules needed for compiling on daint
# NB:source this file BEFORE running cmake
module swap PrgEnv-cray PrgEnv-gnu
module swap gcc gcc/4.9.3
module load cudatoolkit/7.0.28-1.0502.10742.5.1 
module load cmake/3.5.2 
module load ispc/1.6.0