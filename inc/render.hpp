#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "main.hpp"
#include "structs.hpp"
#include "camera.hpp"

namespace vector2 {
	extern Camera g_Camera;

	vertex * devectorize(std::vector<vertex> v);
	GLuint * devectorize(std::vector<GLuint> v, bool null);

	void drawModel(int size, GLuint vao, GLuint shader, GLuint uniformMVP, glm::quat rotation);
}


#endif /* RENDER_HPP_ */
