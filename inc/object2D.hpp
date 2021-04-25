#ifndef INC_OBJECT2D_HPP_
#define INC_OBJECT2D_HPP_

#include "structs.hpp"
#include "main.hpp"

namespace vector2 {
	class Empty2D {
	protected:
		Empty2D * parent;

		glm::vec3 pos = {0,0,0};
		glm::quat rot = {1,0,0,0};
		glm::vec2 scl = {1,1};

		glm::mat4 modelMatrix;

		glm::mat4 getMatrix(bool head);
		bool stale = true;
	public:
		Empty2D( );
		Empty2D( Empty2D * p );
		Empty2D( glm::vec3 p, glm::quat r, glm::vec2 sc);

		void setPosition(glm::vec3 position);
		void setRotation(glm::quat rotation);
		void setScale(glm::vec2 scale);

		void move(glm::vec3 position);
		void rotate(glm::quat rotation);
		void scale(glm::vec2 scale);

		void genMatrix();
		bool checkTree();

		glm::vec3 getPosition();
		glm::quat getRotation();
		glm::vec2 getScale();

		glm::mat4 getMatrix();
	};

	class Object2D : public Empty2D {
	protected:
		std::vector<vertex> vertices;
		std::vector<GLuint> indices;

		int size;
		int numVertices;

		std::vector<GLuint> vao = {0};
		std::vector<GLuint> vBuf = {0};
		std::vector<GLuint> iBuf = {0};
	public:
		Object2D( );
		Object2D( Empty2D * p );
		Object2D(  std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 p, glm::quat r, glm::vec2 sc);

		void setVertices(std::vector<vertex> v);
		void setIndices(std::vector<GLuint> i);

		void genVAO(unsigned id, shader &shd);

		std::vector<vertex> getVertices();
		std::vector<GLuint> getIndices();

		GLuint getVAO(unsigned id);
		int getSize();
		int getNumVertices();
		GLuint getVBuf(unsigned id);
		GLuint getIBuf(unsigned id);
	};

	void changeData(Object2D &o, letter &l, letter &x);

	class TileObject2D : public Object2D
	{
	protected:
		std::vector<glm::vec2> offsets;
		int instances = 0;
	public:
		TileObject2D( );
		TileObject2D( Empty2D * p );
		TileObject2D( std::vector<vertex> v, std::vector<GLuint> i, std::vector<glm::vec2> o, glm::vec3 p, glm::quat r, glm::vec2 sc);
		void setOffsets(std::vector<glm::vec2> o);
		int getInstances();
		void genVAO(unsigned id, shader &shd);
	};
}


#endif /* INC_OBJECT_HPP_ */
