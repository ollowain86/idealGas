#include <iostream>
#include "GasParticle.h"
#include "WorldClass.h"
#include "simulation.h"

int main()
{
	const uint32_t totalNumPart = 100U;
	const float worldSideLength = 1000.0F;
	const float deltaRadiustoRadius = 0.01;
	const float particleRadius = 5.0F;
	const float totalTime = 1.0F;
	const float maxSurfaceRatioCirclesRectangle = 0.4; // circle to surface max ~0.8 (80%)
	const float velBinSize = 0.1F; //bin of vel distr

	Simulation sim;

	// run simulation only if init simulation was successful
	if (!sim.initSimulation(totalNumPart, worldSideLength, velBinSize, deltaRadiustoRadius, particleRadius, maxSurfaceRatioCirclesRectangle))
	{
		sim.runSimulation(totalTime);
	}
	
	return 0;
}