#include <iostream>
#include "GasParticle.h"
#include "WorldClass.h"
#include "simulation.h"

int main()
{
	const uint32_t totalNumPart = 10U;
	const float worldSideLength = 10;
	const float totalTime = 3.0F;
	const float deltaTime = 1.0F;

	initSimulation(totalNumPart, worldSideLength);
	runSimulation(totalTime, deltaTime);

	getchar();
	return 0;
}