#pragma once

struct vec2d
{
	float xVal;
	float yVal;
};

using velStruct = vec2d;
using posStruct = vec2d;

class GasParticle
{
public:
	GasParticle(float i_mass, float i_radius, float i_deltaRadius, velStruct i_vel, posStruct i_pos, float i_velScalar);
	velStruct vel{ 0.0F , 0.0F };
	posStruct pos{ 0.0F , 0.0F };
	bool hittedLastStep = false;
	const float mass = 0.0F;
	const float radius = 0.0F;
	const float deltaRadius = 0.0F;
	float velScalar = 0.0F;
private:
};

