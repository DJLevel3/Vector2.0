#ifndef STRUCTS_HPP_
#define STRUCTS_HPP_

#include "main.hpp"

namespace vector2 {
	struct vertex {
		glm::vec3 pos;
		glm::vec3 color;
	};
	struct shader {
		GLuint program;
		GLuint mvp;
		GLint attribIDPosition;
		GLint attribIDColor;
	};
}

#endif /* INC_STRUCTS_HPP_ */
