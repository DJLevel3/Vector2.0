#include "object.hpp"

using namespace vector2;

void Object::genVAO(shader s)
{
	vertex v[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), v);

	GLuint i[indices.size()];
	std::copy(indices.begin(), indices.end(), i);

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	GLuint vertexBuffer, indexBuffer;
	glGenBuffers( 1, &vertexBuffer );
	glGenBuffers( 1, &indexBuffer );

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(i), i, GL_STATIC_DRAW );

	glVertexAttribPointer( s.attribIDPosition, 3, GL_FLOAT, false, sizeof(vertex), MEMBER_OFFSET(vertex,pos) );
	glEnableVertexAttribArray( s.attribIDPosition );

	glVertexAttribPointer( s.attribIDColor, 3, GL_FLOAT, false, sizeof(vertex), MEMBER_OFFSET(vertex, color) );
	glEnableVertexAttribArray( s.attribIDColor );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray( s.attribIDPosition );
	glDisableVertexAttribArray( s.attribIDColor );
}

void Object::genMatrix()
{
	modelMatrix = glm::translate(glm::mat4(), pos) * glm::toMat4(rot) * glm::scale(scl);
}

Object::Object( )
{
	vertices = {};
	indices = {};
	size = 0;
	genMatrix();
}
Object::Object( glm::vec3 position, glm::quat rotation, glm::vec3 scale )
{
	vertices = {};
	indices = {};
	size = 0;
	pos = position;
	rot = rotation;
	scl = scale;
	genMatrix();
}
Object::Object( std::vector<vertex> v, std::vector<GLuint> i )
{
	vertices = v;
	indices = i;
	size = i.size()/3;
	genMatrix();
}
Object::Object( glm::vec3 position, glm::quat rotation, glm::vec3 scale, std::vector<vertex> v, std::vector<GLuint> i )
{
	vertices = v;
	indices = i;
	size = i.size()/3;
	pos = position;
	rot = rotation;
	scl = scale;
	genMatrix();
}
Object::Object( std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 position, glm::quat rotation, glm::vec3 scale )
{
	vertices = v;
	indices = i;
	size = i.size()/3;
	pos = position;
	rot = rotation;
	scl = scale;
	genMatrix();
}

void Object::setVertices(std::vector<vertex> v)
{
	vertices = v;
}

void Object::setIndices(std::vector<GLuint> i)
{
	indices = i;
}

void Object::setPosition(glm::vec3 position)
{
	pos = position;
}

void Object::setRotation(glm::quat rotation)
{
	rot = rotation;
}

void Object::move(glm::vec3 position)
{
	pos += position;
}

void Object::rotate(glm::quat rotation)
{
	rot *= rotation;
}

std::vector<vertex> Object::getVertices() {return vertices;}
std::vector<GLuint> Object::getIndices() {return indices;}
glm::vec3           Object::getPosition() {return pos;}
glm::quat           Object::getRotation() {return rot;}
glm::vec3           Object::getScale() {return scl;}
glm::mat4           Object::getMatrix() {return modelMatrix;}
GLuint              Object::getVAO() {return vao;}
int                 Object::getSize() {return size;}
