#ifndef INC_OBJECT_HPP_
#define INC_OBJECT_HPP_

#include "structs.hpp"
#include "main.hpp"

namespace vector2 {
	class Empty {
	protected:
		Empty * parent;

		glm::vec3 pos = {0,0,0};
		glm::quat rot = {1,0,0,0};
		glm::vec3 scl = {1,1,1};

		glm::mat4 modelMatrix;

		glm::mat4 getMatrix(bool head);
		bool stale = true;
	public:
		Empty( );
		Empty( Empty * p );
		Empty( glm::vec3 p, glm::quat r, glm::vec3 sc);

		void setPosition(glm::vec3 position);
		void setRotation(glm::quat rotation);
		void setScale(glm::vec3 scale);

		void move(glm::vec3 position);
		void rotate(glm::quat rotation);
		void scale(glm::vec3 scale);

		void genMatrix();
		bool checkTree();

		glm::vec3 getPosition();
		glm::quat getRotation();
		glm::vec3 getScale();

		glm::mat4 getMatrix();
	};

	class Object : public Empty {
	protected:
		std::vector<vertex> vertices;
		std::vector<GLuint> indices;

		int size;
		int numVertices;
		shader shd;

		GLuint vao = 0;
	public:
		Object();
		Object( shader s );
		Object( shader s, Empty * p );
		Object( shader s, std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 p, glm::quat r, glm::vec3 sc);

		void setVertices(std::vector<vertex> v);
		void setIndices(std::vector<GLuint> i);

		void genVAO( );

		std::vector<vertex> getVertices();
		std::vector<GLuint> getIndices();

		GLuint getVAO();
		int getSize();
		int getNumVertices();
	};
}


#endif /* INC_OBJECT_HPP_ */
