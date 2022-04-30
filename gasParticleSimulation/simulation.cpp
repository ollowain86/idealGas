#include <iostream>
#include <random>
#include <vector>
#include "simulation.h"
#include "GasParticle.h"
#include <cmath>

std::vector<GasParticle> gasParticleContainer;
float particleDistance = -1.0;
float cosVel1VecCentral = 0.0;
float sinVel1VecCentral = 0.0;
float cosVel2VecCentral = 0.0;
float sinVel2VecCentral = 0.0;
posStruct centralLine = { 0.0, 0.0 };

void distributeParticle2D(std::vector<GasParticle> & gasParticleContainer, const uint32_t i_totalNumPart, const float i_worldSideLength)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> disPos(-i_worldSideLength / 2.0, i_worldSideLength / 2.0);
	std::uniform_real_distribution<> disVel(-1.0, 1.0);
	
	float xVal, yVal;
	for (size_t i = 0; i < i_totalNumPart; i++)
	{
		xVal = static_cast<float>(disPos(gen));
		yVal = static_cast<float>(disPos(gen));
		xVal = static_cast<float>(disVel(gen));
		yVal = static_cast<float>(disVel(gen));
		velStruct vel{xVal, yVal};
		posStruct pos{ xVal, yVal };
		GasParticle tmp_particle{ 1.0, 1.0, vel, pos };
		gasParticleContainer.push_back(tmp_particle);
	}
}

void initSimulation(const uint32_t i_totalNumPart, const float i_worldSideLength)
{
	distributeParticle2D(gasParticleContainer, i_totalNumPart, i_worldSideLength);
}

float vecLengthsCalc2d(vec2d & vec)
{
	return std::sqrt(vec.xVal*vec.xVal + vec.yVal*vec.yVal);
}

float scalarProduct2d(vec2d& vec1, vec2d& vec2)
{
	// a1*b1+a2*b2
	return (vec1.xVal*vec2.xVal+ vec1.yVal * vec2.yVal);
}

float cosBetween2Vecs(vec2d& vec1, vec2d& vec2)
{
	// a*b/(|a|*|b|)
	return(scalarProduct2d(vec1, vec2)/(vec1.length * vec2.length));
}

float sinBetween2Vecs(vec2d& vec1, vec2d& vec2)
{
	// |axb|/(|a|*|b|)
	return ((vec1.xVal * vec2.yVal - vec1.yVal * vec2.xVal)/(vec1.length*vec2.length));
}

void calcElasticCollision(GasParticle& firstGasParticle, GasParticle& secondGasParticle)
{
	firstGasParticle.vel.length = vecLengthsCalc2d(firstGasParticle.vel);
	secondGasParticle.vel.length = vecLengthsCalc2d(secondGasParticle.vel);

	centralLine.xVal = secondGasParticle.pos.xVal - firstGasParticle.pos.xVal;
	centralLine.yVal = secondGasParticle.pos.yVal - firstGasParticle.pos.yVal;
	centralLine.length = vecLengthsCalc2d(centralLine);

	cosVel1VecCentral = cosBetween2Vecs(firstGasParticle.vel, centralLine);
	sinVel1VecCentral = sinBetween2Vecs(firstGasParticle.vel, centralLine);

	cosVel2VecCentral = cosBetween2Vecs(secondGasParticle.vel, centralLine);
	sinVel2VecCentral = sinBetween2Vecs(secondGasParticle.vel, centralLine);
}

float distanceCal(const posStruct & posA, const posStruct & posB)
{
	return (static_cast<float>(std::sqrt(std::pow((posA.xVal - posB.xVal), 2.0) + std::pow((posA.yVal - posB.yVal), 2.0))));
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
		gasParticleContainer[i].pos.xVal += gasParticleContainer[i].vel.xVal * deltaTime;
		gasParticleContainer[i].pos.yVal += gasParticleContainer[i].vel.yVal * deltaTime;
		
		// zum Testen
		gasParticleContainer[0].pos.xVal = 2.0;
		gasParticleContainer[0].pos.yVal = 2.0;
		gasParticleContainer[0].vel.xVal = 2.0;
		gasParticleContainer[0].vel.yVal = 1.0;

		gasParticleContainer[1].pos.xVal = 3.0;
		gasParticleContainer[1].pos.yVal = 3.732;
		gasParticleContainer[1].vel.xVal = -2.0;
		gasParticleContainer[1].vel.yVal = 1.0;

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