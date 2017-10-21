#pragma once

#include "IComponent.h"
#include "Vector2.h"

class PhysicsComponent : public IComponent
{
	friend class Physics;
	friend class ProxMap;
public:
	PhysicsComponent()
	{}
	void initialize(const std::vector<std::string> &input) override
	{
		initSerial(&m_colliderPos.x, Serializable::Type::FLOAT,
			&m_colliderPos.y, Serializable::Type::FLOAT,
			&m_colliderSize.x, Serializable::Type::FLOAT,
			&m_colliderSize.y, Serializable::Type::FLOAT,
			&m_moveVec.x, Serializable::Type::FLOAT,
			&m_moveVec.y, Serializable::Type::FLOAT,
			&m_speed, Serializable::Type::FLOAT);
		readSerial(input);

		normalize(m_moveVec);
		m_moveVec.x *= m_speed;
		m_moveVec.y *= m_speed;
	};
	void setDir(float x, float y)
	{
		m_moveVec.x = x;
		m_moveVec.y = y;
		normalize(m_moveVec);
		m_moveVec.x *= m_speed;
		m_moveVec.y *= m_speed;
	}
private:
	Vector2 m_colliderPos;
	Vector2 m_colliderSize;
	Vector2 m_moveVec;
	float m_speed;

	void normalize(Vector2 &v)
	{
		float m = sqrt(pow(v.x, 2) + pow(v.y, 2));
		if (m != 0)
		{
			v.x /= m;
			v.y /= m;
		}
	}
};