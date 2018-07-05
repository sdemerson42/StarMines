#pragma once

class ColorState
{
public:
	struct Color
	{
		int r;
		int g;
		int b;
		int a;
	};
	Color m_color;
	virtual void setColor(int r, int g, int b, int a)
	{
		m_color.r = r;
		m_color.g = g;
		m_color.b = b;
		m_color.a = a;
	}
	const Color &color() const
	{
		return m_color;
	}
protected:
	ColorState()
	{}
};