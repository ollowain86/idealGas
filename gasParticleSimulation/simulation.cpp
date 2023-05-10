#define _USE_MATH_DEFINES
#include <iostream>
#include <random>
#include <vector>
#include "simulation.h"
#include "GasParticle.h"
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

std::vector<GasParticle> gasParticleContainer;

posStruct posDiff = { 0.0, 0.0 };
velStruct velDiff = { 0.0, 0.0 };
bool isAHit = false;
float particleDistance = 0.0;
float parameter1 = 0.0; // 2*m2/(m1+m2) for v1
float parameter2 = 0.0; // <	(v1-v2), (x1-x2)	>
float parameter3 = 0.0; // length(x1-x2)
size_t velDistrArraySize = 0U;
std::vector<float> velDistrArray;
float kB = 1.380649e-23f; // boltmann constant

/*
void Simulation::reDistributePos(GasParticle & tmp_particle, const std::vector<GasParticle>& gasParticleContainer, const float i_deltaRadiustoRadius)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> disPos(-m_worldSideLength / 2.0, m_worldSideLength / 2.0);
	float xVal, yVal;
	xVal = static_cast<float>(disPos(gen));
	yVal = static_cast<float>(disPos(gen));
	posStruct pos{ xVal, yVal };

	while ()
	{

	}
}
*/

void Simulation::distributeParticle2D(std::vector<GasParticle> & gasParticleContainer, const uint32_t i_totalNumPart, const float i_deltaRadiustoRadius, const float i_particleRadius)
{	
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> disPos(-m_worldSideLength / 2.0, m_worldSideLength / 2.0);
	std::uniform_real_distribution<> disVel(-1.0, 1.0);
	float xVal, yVal;
	float velScalar;
	for (size_t i = 0; i < i_totalNumPart; i++)
	{
		xVal = static_cast<float>(disPos(gen));
		yVal = static_cast<float>(disPos(gen));
		posStruct pos{ xVal, yVal };
		//reuse xVal, yVal
		xVal = static_cast<float>(disVel(gen));
		yVal = static_cast<float>(disVel(gen));
		// ToDo what should be vel? Here it depends on the world size
		velStruct vel{ xVal * m_worldSideLength / 10.0f, yVal * m_worldSideLength / 10.0f };
		velScalar = std::sqrt((vel.xVal * vel.xVal) + (vel.yVal * vel.yVal));
		GasParticle tmp_particle{ 1.0, i_particleRadius, i_particleRadius * i_deltaRadiustoRadius, vel, pos, velScalar};
		gasParticleContainer.push_back(tmp_particle);
	}

	// check if there is a overlap. If overlap try to reassign a new position with a protective while loop
	for (size_t i = 0; i < i_totalNumPart; i++)
	{
		for (size_t j = 0; j < i_totalNumPart; j++)
		{
			if (i != j)
			{
				// if distance < (radiusA + radiusB) * deltaRadiusToRadius
				if (distanceCal(gasParticleContainer[i].pos, gasParticleContainer[j].pos) < ((gasParticleContainer[i].radius + gasParticleContainer[j].radius)  * i_deltaRadiustoRadius))
				{
					//repositioning of particle i
				}
			}
		}
	}
	size_t thresholdReAssignPosition = 10U;
}

void Simulation::setParameters(const float i_worldSideLength, const float i_binSize, const float i_deltaRadiustoRadius)
{
	m_worldSideLength = i_worldSideLength;
	m_velDistrBinSize = i_binSize;
	m_halfWorldSideLength = i_worldSideLength / 2.0f;
	m_deltaRadiustoRadius = i_deltaRadiustoRadius;
}

bool Simulation::surfaceCheck(const uint32_t i_totalNumPart, const float i_maxSurfaceRatioCirclesRectangle, const float i_worldSideLength, const float i_particleRadius, const float i_deltaRadiustoRadius)
{
	bool tooManyParticles = true;
	// for all circle radii are the same
	float ratio = static_cast<float>(i_totalNumPart) * M_PI * ((i_particleRadius+i_deltaRadiustoRadius) * (i_particleRadius + i_deltaRadiustoRadius)) / (i_worldSideLength * i_worldSideLength); // N*(pi*r^2)/(surface world)
	if (ratio <= i_maxSurfaceRatioCirclesRectangle)
	{
		tooManyParticles = false;
	}

	return tooManyParticles;
}

bool Simulation::initSimulation(const uint32_t i_totalNumPart, const float i_worldSideLength, const float i_binSize, const float i_deltaRadiustoRadius, const float i_particleRadius, const float i_maxSurfaceRatioCirclesRectangle)
{
	bool error = false;
	if (surfaceCheck(i_totalNumPart, i_maxSurfaceRatioCirclesRectangle, i_worldSideLength, i_particleRadius, i_deltaRadiustoRadius))
	{
		error = true;
		std::cout << "Too many particles! Aborted." << std::endl;
	}
	else
	{
		setParameters(i_worldSideLength, i_binSize, i_deltaRadiustoRadius);
		distributeParticle2D(gasParticleContainer, i_totalNumPart, i_deltaRadiustoRadius, i_particleRadius);
	}
	return error;
}

float Simulation::vecLengthsCalc2d(vec2d & vec)
{
	return std::sqrt(vec.xVal*vec.xVal + vec.yVal*vec.yVal);
}

float Simulation::scalarProduct2d(vec2d& vec1, vec2d& vec2)
{
	// a1*b1+a2*b2
	return (vec1.xVal*vec2.xVal+ vec1.yVal*vec2.yVal);
}

void Simulation::calcElasticCollision(GasParticle& firstGasParticle, GasParticle& secondGasParticle)
{
	// Calculation for v1
	// x1-x2
	posDiff.xVal = firstGasParticle.pos.xVal - secondGasParticle.pos.xVal;
	posDiff.yVal = firstGasParticle.pos.yVal - secondGasParticle.pos.yVal;
	// v1-v2
	velDiff.xVal = firstGasParticle.vel.xVal - secondGasParticle.vel.xVal;
	velDiff.yVal = firstGasParticle.vel.yVal - secondGasParticle.vel.yVal;

	parameter1 = 2.0F * secondGasParticle.mass / (firstGasParticle.mass + secondGasParticle.mass);
	parameter2 = scalarProduct2d(velDiff, posDiff);
	parameter3 = vecLengthsCalc2d(posDiff);

	float eTot1 = firstGasParticle.vel.xVal * firstGasParticle.vel.xVal + firstGasParticle.vel.yVal * firstGasParticle.vel.yVal;
	float eTot2 = secondGasParticle.vel.xVal * secondGasParticle.vel.xVal + secondGasParticle.vel.yVal * secondGasParticle.vel.yVal;

	firstGasParticle.vel.xVal = firstGasParticle.vel.xVal - (parameter1 * parameter2 / (parameter3 * parameter3)) * posDiff.xVal;
	firstGasParticle.vel.yVal = firstGasParticle.vel.yVal - (parameter1 * parameter2 / (parameter3 * parameter3)) * posDiff.yVal;

	parameter1 = 2.0f * firstGasParticle.mass / (firstGasParticle.mass + secondGasParticle.mass);
	posDiff.xVal *= -1.0;
	posDiff.yVal *= -1.0;
	velDiff.xVal *= -1.0;
	velDiff.yVal *= -1.0;
	parameter2 = scalarProduct2d(velDiff, posDiff);
	parameter3 = vecLengthsCalc2d(posDiff);

	secondGasParticle.vel.xVal = secondGasParticle.vel.xVal - (parameter1 * parameter2 / (parameter3*parameter3)) * posDiff.xVal;
	secondGasParticle.vel.yVal = secondGasParticle.vel.yVal - (parameter1 * parameter2 / (parameter3 * parameter3)) * posDiff.yVal;
}

float Simulation::distanceCal(const posStruct & posA, const posStruct & posB)
{
	return (static_cast<float>(std::sqrt(std::pow((posA.xVal - posB.xVal), 2.0) + std::pow((posA.yVal - posB.yVal), 2.0))));
}

bool Simulation::hasHitted(const int i, const GasParticle & particle_i, const std::vector<GasParticle>& gasParticleContainer, int& particleNo_j)
{
	// check if particle_i hitted a particle_j
	for (size_t j = 0; j < gasParticleContainer.size(); j++)
	{
		// check if i & j are not the same particle
		if (i != j)
		{
			particleDistance = distanceCal(particle_i.pos, gasParticleContainer[j].pos);
			if (particleDistance < (particle_i.radius+particle_i.deltaRadius + gasParticleContainer[j].radius+ gasParticleContainer[j].deltaRadius))
			{
				particleNo_j = static_cast<int>(j);
				isAHit = true;
				break;
			}
			else
			{
				particleNo_j = -1;
				isAHit = false;
			}
		}
	}
	return isAHit;
}

bool Simulation::hasHittedWall(const GasParticle& particle_i)
{
	bool hittedWall = false;
	if (particle_i.pos.xVal+(particle_i.radius+particle_i.deltaRadius) >= m_halfWorldSideLength)
	{
		m_wallSide = wallSide::rightSide;
		hittedWall = true; 
	}
	else if (particle_i.pos.xVal-(particle_i.radius + particle_i.deltaRadius) <= -m_halfWorldSideLength)
	{
		m_wallSide = wallSide::leftSide;
		hittedWall = true;
	}
	else if(particle_i.pos.yVal+ (particle_i.radius + particle_i.deltaRadius) >= m_halfWorldSideLength)
	{
		m_wallSide = wallSide::upperSide;
		hittedWall = true;
	}
	else if(particle_i.pos.yVal- (particle_i.radius + particle_i.deltaRadius) <= -m_halfWorldSideLength)
	{
		m_wallSide = wallSide::lowerSide;
		hittedWall = true;
	}
	else
	{
		// nothing to do
	}
	return hittedWall;
}

void Simulation::calcElasticCollisionWithWall(GasParticle& particle_i)
{
	if ((m_wallSide == wallSide::leftSide))
	{
		// only if the velocity is going to left
		if (particle_i.vel.xVal < 0.0F)
		{
			particle_i.vel.xVal = -1.0f * particle_i.vel.xVal;
		}
	}
	else if (m_wallSide == wallSide::rightSide)
	{
		// only if the velocity is going to right
		if (particle_i.vel.xVal > 0.0F)
		{
			particle_i.vel.xVal = -1.0f * particle_i.vel.xVal;
		}
	}
	else if(m_wallSide == wallSide::lowerSide)
	{
		// only if the velocity is going to bottom
		if (particle_i.vel.yVal < 0.0F)
		{
			particle_i.vel.yVal = -1.0f * particle_i.vel.yVal;
		}
	}
	else if (m_wallSide == wallSide::upperSide)
	{
		if (particle_i.vel.yVal > 0.0F)
		{
			particle_i.vel.yVal = -1.0f * particle_i.vel.yVal;
		}
	}
	else
	{
		//do nothing
	}
}

void Simulation::moveParticle(std::vector<GasParticle>& gasParticleContainer, const float deltaTime)
{
	int particle_j = -1;
	for (int i = 0; i < gasParticleContainer.size(); i++)
	{
		//moving particle i by vel*dt
		gasParticleContainer[i].pos.xVal += gasParticleContainer[i].vel.xVal * deltaTime;
		gasParticleContainer[i].pos.yVal += gasParticleContainer[i].vel.yVal * deltaTime;

		// check if a hit has taken place
		if (hasHitted(i, gasParticleContainer[i], gasParticleContainer, particle_j))
		{
			calcElasticCollision(gasParticleContainer[i], gasParticleContainer[particle_j]);
		}
		// check if wall is hitted
		else if (hasHittedWall(gasParticleContainer[i]))
		{
			calcElasticCollisionWithWall(gasParticleContainer[i]);
		}
		else
		{
			// nothing to do
		}
	}
}

void Simulation::calcTotalEnergy(std::vector<GasParticle>& gasParticleContainer)
{
	float eTot = 0.0F;
	for (auto & gasParticle : gasParticleContainer)
	{
		gasParticle.velScalar = std::sqrt((gasParticle.vel.xVal * gasParticle.vel.xVal) + (gasParticle.vel.yVal * gasParticle.vel.yVal));
		eTot += 0.5f * gasParticle.mass * (gasParticle.velScalar* gasParticle.velScalar);
	}
	std::cout << "Total energy: " << eTot << "J; Temperature: " << eTot/kB << std::endl;
}

void Simulation::calcVelDistr(std::vector<GasParticle>& gasParticleContainer)
{
	auto minMaxVelValues = std::minmax_element(gasParticleContainer.begin(), gasParticleContainer.end(), [](GasParticle const& lhs, GasParticle const& rhs) {return lhs.velScalar < rhs.velScalar; });

	velDistrArraySize = static_cast<size_t>(std::ceil((1.0f / m_velDistrBinSize) * minMaxVelValues.second->velScalar));
	velDistrArray.resize(velDistrArraySize);

	float bin = 0.0F;
	float binPlus = bin + m_velDistrBinSize;
	for (auto & gasParticle : gasParticleContainer)
	{
		for (bin = 0; bin < minMaxVelValues.second->velScalar; bin+=binPlus)
		{
			if (gasParticle.velScalar > bin && gasParticle.velScalar <= bin+binPlus)
			{
				velDistrArray[static_cast<size_t>(10.0 * bin)]++;
			}
		}
	}
}

void Simulation::printVelDistr(const std::vector<float> & velDistrArray)
{
	float counter = 0.0f;
	for (const auto & elem: velDistrArray)
	{
		std::cout << "Between " << counter << " and " << counter+ m_velDistrBinSize << ": " <<  elem << std::endl;
		counter += m_velDistrBinSize;
	}
}

void Simulation::calcDeltaTime(const std::vector<GasParticle>& gasParticleContainer, float & deltaTime)
{
	auto fastestParticle = std::max_element(gasParticleContainer.begin(), gasParticleContainer.end(), [](const GasParticle& particleA, const GasParticle& particleB) {return particleA.velScalar < particleB.velScalar; });
	auto smallestParticle = std::min_element(gasParticleContainer.begin(), gasParticleContainer.end(), [](const GasParticle& particleA, const GasParticle& particleB) {return particleA.radius < particleB.radius; });
	// t = x / v
	// a particle with v velocity should be not able to go more then delta radius
	// 2.0F ist an extra threshold to allow even smaller steps
	deltaTime =  ((*smallestParticle).radius *m_deltaRadiustoRadius)/ (2.0F* (*fastestParticle).velScalar);
	std::cout << "max dx = " << deltaTime * std::sqrt((*fastestParticle).velScalar) << std::endl;
}

void Simulation::runSimulation(const float totalTime)
{
	// set sfml parameters
	sf::RenderWindow window(sf::VideoMode(2.0*m_worldSideLength, 2.0 * m_worldSideLength), "Ideal Gas 2D by Fatih Turan");
	sf::Event event;
	//window.setFramerateLimit(24);
	std::vector<sf::CircleShape> visualGasContainer(gasParticleContainer.size());
	float deltaTime = -1.0F;

	float fps;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{

				window.close();
			}
		}
		// ToDo include again initial deltaTime calc
		// calc fastest particle and determine new deltaTime - initially
		calcDeltaTime(gasParticleContainer, deltaTime);
		for (float t = 0; t < totalTime; t+=deltaTime)
		{
			for (size_t i = 0; i < visualGasContainer.size(); i++) // draw loop
			{
				visualGasContainer[i].setPosition(gasParticleContainer[i].pos.xVal+m_halfWorldSideLength, gasParticleContainer[i].pos.yVal+ m_halfWorldSideLength);
				visualGasContainer[i].setRadius(gasParticleContainer[i].radius);
				window.draw(visualGasContainer[i]);
			}
			window.display();
			currentTime = clock.getElapsedTime();
			fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
			std::cout << "fps =" << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
			previousTime = currentTime;
			window.clear();
			// calc fastest particle and determine new deltaTime - in each round
			calcDeltaTime(gasParticleContainer, deltaTime);
			// move particle if deltaTime > 0.0F
			if (deltaTime > 0.0F)
			{
				moveParticle(gasParticleContainer, deltaTime);
			}
			else
			{
				std::cout << "Incorrect deltaTime!" << std::endl;
			}
		}
	}
}