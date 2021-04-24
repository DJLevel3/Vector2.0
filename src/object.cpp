#include "object.hpp"

using namespace vector2;



// Empty

Empty::Empty( )
{
	parent = NULL;
}

Empty::Empty( Empty * p )
{
	parent = p;
}

Empty::Empty( glm::vec3 p, glm::quat r, glm::vec3 sc )
{
	parent = NULL;
	stale = true;
	pos = p;
	rot = r;
	scl = sc;
}

glm::mat4 Empty::getMatrix(bool head)
{
	if (head) checkTree();
	return modelMatrix;
}

void Empty::setPosition(glm::vec3 position)
{
	pos = position;
	stale = true;
}

void Empty::setRotation(glm::quat rotation)
{
	rot = rotation;
	stale = true;
}

void Empty::setScale(glm::vec3 scale)
{
	scl = scale;
	stale = true;
}

void Empty::move(glm::vec3 position)
{
	pos += position;
	stale = true;
}

void Empty::rotate(glm::quat rotation)
{
	rot *= rotation;
	stale = true;
}

void Empty::scale(glm::vec3 scale)
{
	scl *= scale;
	stale = true;
}

bool Empty::checkTree()
{
	bool wasStale;
	if (parent != NULL){
		stale = wasStale = parent->checkTree() || stale;
	} else {
		wasStale = stale;
	}
	if (stale) {
		genMatrix();
	}
	return wasStale;
}

void Empty::genMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1.0), pos) * glm::scale(scl) * glm::toMat4(rot);
	if (parent != NULL) modelMatrix = parent->getMatrix(false) * modelMatrix;
	stale = false;
}

glm::vec3 Empty::getPosition() {return pos;}
glm::quat Empty::getRotation() {return rot;}
glm::vec3 Empty::getScale() {return scl;}
glm::mat4 Empty::getMatrix() {glm::mat4 m = getMatrix(true); return m;}



// Object

Object::Object()
{
	shd = {0,0,0,0};
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object::Object( shader s ) : Empty ( )
{
	shd = s;
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object::Object( shader s, Empty * p ) : Empty ( p )
{
	shd = s;
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object::Object( shader s, std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 p, glm::quat r, glm::vec3 sc) : Empty ( p, r, sc )
{
	shd = s;
	vertices = v;
	indices = i;
	numVertices = vertices.size();
	size = indices.size();
}

void Object::genVAO()
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

	glVertexAttribPointer( shd.attribIDPosition, 3, GL_FLOAT, false, sizeof(vertex), MEMBER_OFFSET(vertex,pos) );
	glEnableVertexAttribArray( shd.attribIDPosition );

	glVertexAttribPointer( shd.attribIDColor, 3, GL_FLOAT, false, sizeof(vertex), MEMBER_OFFSET(vertex,color) );
	glEnableVertexAttribArray( shd.attribIDColor );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray( shd.attribIDPosition );
	glDisableVertexAttribArray( shd.attribIDColor );
}

void Object::setVertices(std::vector<vertex> v)
{
	vertices = v;
	numVertices = v.size();
}

void Object::setIndices(std::vector<GLuint> i)
{
	indices = i;
	size = i.size();
}

std::vector<vertex> Object::getVertices() {return vertices;}
std::vector<GLuint> Object::getIndices() {return indices;}
GLuint              Object::getVAO() {return vao;}
int                 Object::getSize() {return size;}
int                 Object::getNumVertices() {return numVertices;}
