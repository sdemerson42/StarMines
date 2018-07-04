#pragma once

#include "RenderComponent.h"

class ParticleComponent : public RenderComponent
{
	friend class Renderer;
	friend class Particle;
public:
	ParticleComponent()
	{
		m_particleData.reserve(m_maxParticles);
	}
	virtual void initialize(const std::vector<std::string> &input) override
	{
		// Initialize RenderComponent first...
		auto b = begin(input);
		auto e = begin(input) + 7;
		std::vector<std::string> renderInput(b, e);
		RenderComponent::initialize(renderInput);

		m_offset.x = stof(input[7]);
		m_offset.y = stof(input[8]);
		m_life = stoi(input[9]);
		m_lifeVariance = stoi(input[10]);
		m_densityPerFrame = stof(input[11]);
		m_speed = stof(input[12]);
		m_speedVariance = stof(input[13]);

		reset();
	}
	const std::string &getTag() const override
	{
		return m_tag;
	}
	void reset()
	{
		m_particleData.clear();
		m_particleData.resize(m_maxParticles);
		m_particleSz = 0;
		m_pCount = 0.0f;
	}

private:
	static std::string m_tag;
	static const int m_maxParticles = 200;
	struct ParticleData
	{
		ParticleData() : 
			active{ false }
		{}
		ParticleData(float x, float y, float dx, float dy, int life) :
			position{ x,y }, direction{ dx,dy }, life{ life }, lifeCounter{ 0 }, active{ true }
		{}
		Vector2 position;
		Vector2 direction;
		int life;
		int lifeCounter;
		bool active;
	};
	std::vector<ParticleData> m_particleData;
	int m_particleSz;

	Vector2 m_offset;
	int m_life;
	int m_lifeVariance;
	float m_densityPerFrame;
	float m_speed;
	float m_speedVariance;
	float m_pCount;
};