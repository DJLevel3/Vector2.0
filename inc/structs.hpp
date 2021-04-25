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
		GLuint mvp = -1;
		GLint attribIDPosition;
		GLint attribIDColor;
		GLint attribIDOffset;
	};
	struct sounds {
		std::map<std::string, Mix_Music*> mus;
		std::map<std::string, Mix_Chunk*> eff;
	};
	struct letter {
		std::vector<vertex> points;
		std::vector<GLuint> lines;
	};
}

#endif /* INC_STRUCTS_HPP_ */
