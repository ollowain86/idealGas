#pragma once

struct velStruct
{
	float velX;
	float velY;
};

struct posStruct
{
	float xPos;
	float yPos;
};

class GasParticle
{
public:
	GasParticle(float i_mass, float i_radius, velStruct i_vel, posStruct i_pos);
	velStruct vel{ 0.0F , 0.0F };
	posStruct pos{ 0.0F , 0.0F };
	bool hittedLastStep = false;
private:
	const float mass = 0.0F;
	const float radius = 0.0F;
};

