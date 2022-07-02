#pragma once

#include <vector>
#include <iostream>
#include "GasParticle.h"

class Simulation
{
public:

	bool surfaceCheck(const float i_totalNumPart, const float i_maxSurfaceRatioCirclesRectangle, const float i_worldSideLength, const float i_particleRadius, const float i_deltaRadiustoRadius);

	bool initSimulation(const uint32_t i_totalNumPart, const float i_worldSideLength, const float i_binSize, const float i_deltaRadiustoRadius, const float i_particleRadius, const float i_maxSurfaceRatioCirclesRectangle);

	void distributeParticle2D(std::vector<GasParticle>& gasParticleContainer, const uint32_t i_totalNumPart, const float deltaRadiustoRadius, const float i_particleRadius);

	void runSimulation(const float totalTime);

	void moveParticle(std::vector<GasParticle>& gasParticleContainer, const float deltaTime);

	void calcElasticCollision(GasParticle& firstGasParticle, GasParticle& secondGasParticle);

	// distance calc between two points: input point a (x & y pos), b (x & y pos)
	float distanceCal(const posStruct& posA, const posStruct& posB);

	bool hasHitted(const int i, const GasParticle& particle_i, const std::vector<GasParticle>& gasParticleContainer, int& j);

	float vecLengthsCalc2d(vec2d& vec);

	float scalarProduct2d(vec2d& vec1, vec2d& vec2);

	void calcTotalEnergy(std::vector<GasParticle>& gasParticleContainer);

	void calcVelDistr(std::vector<GasParticle>& gasParticleContainer);

	void printVelDistr(const std::vector<float>& velDistrArray);

	void setParameters(const float i_worldSideLength, const float i_binSize, const float i_deltaRadiustoRadius);
	
	bool hasHittedWall(const GasParticle& particle_i);

	void calcElasticCollisionWithWall(GasParticle& particle_i);

	void calcDeltaTime(const std::vector<GasParticle>& gasParticleContainer, float & deltaTime);

	float m_worldSideLength = 0.0F;
	float m_halfWorldSideLength = 0.0F;
	float m_velDistrBinSize = 0.0F;
	float m_deltaRadiustoRadius = 0.0F;
	enum class wallSide
	{
		rightSide,
		upperSide,
		leftSide,
		lowerSide
	}m_wallSide;
};