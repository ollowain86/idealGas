#pragma once
#include "GasParticle.h"
#include <vector>

class SimStateCalc
{
public:
	SimStateCalc();
	void totalEnergyCalc(std::vector<GasParticle> & gasParticleContainer);
};