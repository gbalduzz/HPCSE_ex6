#pragma once
#include "particles.h"

void cudaPotential(const Particles& p, Particles& t, const int order);

