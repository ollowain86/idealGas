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

float vecLengthsCalc2d(velStruct & vel)
{
	return std::sqrt((vel.velX * vel.velX) + (vel.velY * vel.velY));
}

void calcElasticCollision(GasParticle& firstGasParticle, GasParticle& secondGasParticle)
{
	velVecLengthFirst = vecLengthsCalc2d(firstGasParticle.vel);
	velVecLengthSecond = vecLengthsCalc2d(secondGasParticle.vel);
	centralLine.xPos = secondGasParticle.pos.xPos - firstGasParticle.pos.xPos;
	centralLine.yPos = secondGasParticle.pos.yPos - firstGasParticle.pos.yPos;
	centralLineLength = std::sqrt((centralLine.xPos * centralLine.xPos) + (centralLine.yPos * centralLine.yPos));

	angleVelVecFirstCentralLine = ((centralLine.xPos * firstGasParticle.pos.xPos) + (centralLine.yPos * firstGasParticle.pos.yPos));
}

float distanceCal(const posStruct & posA, const posStruct & posB)
{
	return (static_cast<float>(std::sqrt(std::pow((posA.xPos - posB.xPos), 2.0) + std::pow((posA.yPos - posB.yPos), 2.0))));
}

bool hasHitted(const int i, const GasParticle & particle_i, const std::vector<GasParticle>& gasParticleContainer, int& particleNo_j)
{
	// check if particle_i hitted a particle_j
	for (size_t j = 0; j < gasParticleContainer.size(); j++)
	{
		// check if i & j are not the same particle
		if (i != j)
		{
			particleDistance = distanceCal(particle_i.pos, gasParticleContainer[j].pos);
			if (particleDistance < (particle_i.radius + gasParticleContainer[j].radius))
			{
				particleNo_j = j;
				return true;
			}
			else
			{
				particleNo_j = -1;
				return false;
			}
		}
	}
}

void moveParticle(std::vector<GasParticle>& gasParticleContainer, const float deltaTime)
{
	int particle_j = -1;
	for (size_t i = 0; i < gasParticleContainer.size(); i++)
	{
		//moving particle i by vel*dt
		gasParticleContainer[i].pos.xPos += gasParticleContainer[i].vel.velX * deltaTime;
		gasParticleContainer[i].pos.yPos += gasParticleContainer[i].vel.velY * deltaTime;
		
		// zum Testen
		gasParticleContainer[0].pos.xPos = 0.0;
		gasParticleContainer[0].pos.yPos = 0.0;
		gasParticleContainer[0].vel.velX = 1.0;
		gasParticleContainer[0].vel.velY = 1.0;

		gasParticleContainer[1].pos.xPos = 1.999;
		gasParticleContainer[1].pos.yPos = 0.0;
		gasParticleContainer[1].vel.velX = -1.0;
		gasParticleContainer[1].vel.velY = -1.0;

		// check if a hit has taken place
		if (hasHitted(i, gasParticleContainer[i], gasParticleContainer, particle_j))
		{
			calcElasticCollision(gasParticleContainer[i], gasParticleContainer[particle_j]);
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