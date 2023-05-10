#include "SimStateCalc.h"
#include <vector>
#include "GasParticle.h"
#include <iostream>

SimStateCalc::SimStateCalc()
{

}

void SimStateCalc::totalEnergyCalc(std::vector<GasParticle> & gasParticleContainer)
{
	float totalEnergy = 0.0F;
	for (const auto element : gasParticleContainer)
	{
		// sum over 1/2*m_i*(v_i)^2
		totalEnergy += 0.5F * element.mass * (element.velScalar * element.velScalar);
	}
	std::cout << totalEnergy << std::endl;
}