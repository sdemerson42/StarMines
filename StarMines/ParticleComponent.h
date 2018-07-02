#pragma once

#include "RenderComponent.h"

class ParticleComponent : public RenderComponent
{
	friend class Renderer;
	friend class Particle;
public:
	virtual void initialize(const std::vector<std::string> &input) override
	{
		// Initialize RenderComponent first...
		auto b = begin(input);
		auto e = begin(input) + 7;
		std::vector<std::string> renderInput(b, e);
		RenderComponent::initialize(renderInput);

		m_life = stoi(input[7]);
		m_lifeVariance = stoi(input[8]);
		m_densityPerFrame = stof(input[9]);
		m_speed = stof(input[10]);
		m_speedVariance = stof(input[11]);

		m_pCount = 0.0f;
		m_particleData.clear();
	}
	const std::string &getTag() const override
	{
		return m_tag;
	}
	void reset()
	{
		m_particleData.clear();
	}

private:
	static std::string m_tag;
	struct ParticleData
	{
		ParticleData(float x, float y, float dx, float dy, int life) :
			position{ x,y }, direction{ dx,dy }, life{ life }, lifeCounter{ 0 }
		{}
		Vector2 position;
		Vector2 direction;
		int life;
		int lifeCounter;
	};
	std::vector<ParticleData> m_particleData;

	int m_life;
	int m_lifeVariance;
	float m_densityPerFrame;
	float m_speed;
	float m_speedVariance;
	float m_pCount;
};