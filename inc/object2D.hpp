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

		std::vector<GLuint> vao = {0};
		std::vector<GLuint> vBuf = {0};
		std::vector<GLuint> iBuf = {0};
	public:
		Object( );
		Object( Empty * p );
		Object(  std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 p, glm::quat r, glm::vec3 sc);

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

	void changeData(Object &o, letter &l, letter &x);

	class TileObject : public Object
	{
	protected:
		std::vector<glm::vec3> offsets;
		int instances = 0;
	public:
		TileObject( );
		TileObject( Empty * p );
		TileObject( std::vector<vertex> v, std::vector<GLuint> i, std::vector<glm::vec3> o, glm::vec3 p, glm::quat r, glm::vec3 sc);
		void setOffsets(std::vector<glm::vec3> o);
		int getInstances();
		void genVAO(unsigned id, shader &shd);
	};
}


#endif /* INC_OBJECT_HPP_ */
