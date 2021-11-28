#pragma once
#include "../types.h"
#include "shader.h"

class Font
{
private:
	GLuint	m_TexId;
	GLuint	m_VAO;
	GLuint	m_VBO;
	Shader	m_shader;
	float	m_Coords[6 * (2 + 2)];	// 6 vertices XY+UV
	float m_Width;
	float m_Height;
	float m_CharWidth;
	float m_CharHeight;

public:
	Font();
	~Font();

	bool Load(const string& filename);
	void Draw(const char* text, int x, int y, float size);
	
		
};
