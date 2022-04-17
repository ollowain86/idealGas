#include <iostream>
#include <random>
#include <vector>
#include "simulation.h"
#include "GasParticle.h"
#include <cmath>

std::vector<GasParticle> gasParticleContainer;
float particleDistance = -1.0;
float velVecLengthFirst = -1.0;
float velVecLengthSecond = -1.0;
float angleVelVecFirstCentralLine = -1.0;
float angleVelVecSecondCentralLine = -1.0;
float centralLineLength = -1.0;
posStruct centralLine = { 0.0, 0.0 };

void distributeParticle2D(std::vector<GasParticle> & gasParticleContainer, const uint32_t i_totalNumPart, const float i_worldSideLength)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> disPos(-i_worldSideLength / 2.0, i_worldSideLength / 2.0);
	std::uniform_real_distribution<> disVel(-1.0, 1.0);
	
	float xPos, yPos;
	float velX, velY;
	for (size_t i = 0; i < i_totalNumPart; i++)
	{
		xPos = static_cast<float>(disPos(gen));
		yPos = static_cast<float>(disPos(gen));
		velX = static_cast<float>(disVel(gen));
		velY = static_cast<float>(disVel(gen));
		velStruct vel{velX, velY};
		posStruct pos{ xPos, yPos };
		GasParticle tmp_particle{ 1.0, 1.0, vel, pos };
		gasParticleContainer.push_back(tmp_particle);
	}
}

void initSimulation(const uint32_t i_totalNumPart, const float i_worldSideLength)
{
	distributeParticle2D(gasParticleContainer, i_totalNumPart, i_worldSideLength);
}

void calcElasticCollision(GasParticle& firstGasParticle, GasParticle& secondGasParticle)
{
	velVecLengthFirst = std::sqrt((firstGasParticle.vel.velX * firstGasParticle.vel.velX) + (firstGasParticle.vel.velY * firstGasParticle.vel.velY));
	velVecLengthSecond = std::sqrt((secondGasParticle.vel.velX * secondGasParticle.vel.velX) + (secondGasParticle.vel.velY * secondGasParticle.vel.velY));
	centralLine.xPos = secondGasParticle.pos.xPos - firstGasParticle.pos.xPos;
	centralLine.yPos = secondGasParticle.pos.yPos - firstGasParticle.pos.yPos;
	centralLineLength = std::sqrt((centralLine.xPos * centralLine.xPos) + (centralLine.yPos * centralLine.yPos));

	angleVelVecFirstCentralLine = ((centralLine.xPos * firstGasParticle.pos.xPos) + (centralLine.yPos * firstGasParticle.pos.yPos));
}

void moveParticle(std::vector<GasParticle>& gasParticleContainer, const float deltaTime)
{
	for (size_t i = 0; i < gasParticleContainer.size(); i++)
	{
		gasParticleContainer[i].pos.xPos += gasParticleContainer[i].vel.velX * deltaTime;
		gasParticleContainer[i].pos.yPos += gasParticleContainer[i].vel.velY * deltaTime;
		// check if particle_i hitted a particle_j
		for (size_t j = 0; j < gasParticleContainer.size(); j++)
		{
			particleDistance = static_cast<float>(std::pow((gasParticleContainer[i].pos.xPos - gasParticleContainer[j].pos.xPos), 2.0) + std::pow((gasParticleContainer[i].pos.yPos - gasParticleContainer[j].pos.yPos), 2.0));
			particleDistance = static_cast<float>(std::sqrt(particleDistance));
			if (particleDistance<(gasParticleContainer[i].radius + gasParticleContainer[j].radius))
			{
				// check if they are not the same particle
				if (i != j)
				{
					calcElasticCollision(gasParticleContainer[i], gasParticleContainer[j]);
				}
			}
		}
	}
}

void runSimulation(const float totalTime, const float deltaTime)
{
	for (float t = 0; t < totalTime; t+=deltaTime)
	{
		moveParticle(gasParticleContainer, deltaTime);
	}
}