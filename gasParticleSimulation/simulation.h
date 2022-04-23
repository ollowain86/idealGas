#pragma once

#include <vector>
#include <iostream>
#include "GasParticle.h"

void initSimulation(const uint32_t i_totalNumPart, const float i_worldSideLength);

void distributeParticle2D(std::vector<GasParticle>& gasParticleContainer, const uint32_t i_totalNumPart, const float i_worldSideLength);

void runSimulation(const float totalTime, const float deltaTime);

void moveParticle(std::vector<GasParticle>& gasParticleContainer, const float deltaTime);

void calcElasticCollision(GasParticle & firstGasParticle, GasParticle & secondGasParticle);

// distance calc between two points: input point a (x & y pos), b (x & y pos)
float distanceCal(posStruct & posA, posStruct & posB);

bool hasHitted(const int i, const GasParticle & particle_i, const std::vector<GasParticle>& gasParticleContainer, int & j);

float vecLengthsCalc2d();