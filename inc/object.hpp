#ifndef INC_OBJECT_HPP_
#define INC_OBJECT_HPP_

#include "structs.hpp"
#include "main.hpp"

namespace vector2 {
	class Object {
	protected:
		std::vector<vertex> vertices;
		std::vector<GLuint> indices;

		int size;

		GLuint vao = 0;

		glm::vec3 pos = {0,0,0};
		glm::quat rot = {1,0,0,0};
		glm::vec3 scl = {1,1,1};
		glm::mat4 modelMatrix;
	public:
		Object( );
		Object( glm::vec3 position, glm::quat rotation, glm::vec3 scale );
		Object( std::vector<vertex> v, std::vector<GLuint> i );
		Object( glm::vec3 position, glm::quat rotation, glm::vec3 scale, std::vector<vertex> v, std::vector<GLuint> i );
		Object( std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 position, glm::quat rotation, glm::vec3 scale );

		void setVertices(std::vector<vertex> v);
		void setIndices(std::vector<GLuint> i);

		void setPosition(glm::vec3 position);
		void setRotation(glm::quat rotation);
		void setScale(glm::vec3 scale);

		void move(glm::vec3 position);
		void rotate(glm::quat rotation);
		void scale(glm::vec3 scale);

		void genVAO( shader s );
		void genMatrix();

		std::vector<vertex> getVertices();
		std::vector<GLuint> getIndices();

		glm::vec3 getPosition();
		glm::quat getRotation();
		glm::vec3 getScale();

		glm::mat4 getMatrix();
		GLuint getVAO();
		int getSize();
	};
}


#endif /* INC_OBJECT_HPP_ */
