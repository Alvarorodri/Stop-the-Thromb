#ifndef _TEXTURED_QUAD_INCLUDE
#define _TEXTURED_QUAD_INCLUDE


#include <glm/glm.hpp>
#include "utilities\Texture.h"
#include "utilities\ShaderProgram.h"


class TexturedQuad
{

private:
	TexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

public:
	// Textured quads can only be created inside an OpenGL context
	static TexturedQuad *createTexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

	void render(const Texture &tex) const;
	void free();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;

};


#endif // _TEXTURED_QUAD_INCLUDE

