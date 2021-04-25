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
	genMatrix();
	return wasStale;
}

void Empty::genMatrix()
{
	modelMatrix = glm::scale(glm::translate(glm::mat4(1.0), pos) * glm::toMat4(rot), scl);
	if (parent != NULL) modelMatrix = parent->getMatrix(false) * modelMatrix;
	stale = false;
}

glm::vec3 Empty::getPosition() {return pos;}
glm::quat Empty::getRotation() {return rot;}
glm::vec3 Empty::getScale() {return scl;}
glm::mat4 Empty::getMatrix() {return getMatrix(true);}



// Object

Object::Object()
{
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object::Object( Empty * p ) : Empty ( p )
{
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object::Object( std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 p, glm::quat r, glm::vec3 sc) : Empty ( p, r, sc )
{
	vertices = v;
	indices = i;
	numVertices = vertices.size();
	size = indices.size();
}

void Object::genVAO(unsigned id, shader &shd)
{

	if (vao.size() == id) vao.push_back((GLuint)0);
	if (vBuf.size() == id) vBuf.push_back((GLuint)0);
	if (iBuf.size() == id) iBuf.push_back((GLuint)0);
	vertex v[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), v);

	GLuint i[indices.size()];
	std::copy(indices.begin(), indices.end(), i);

	glGenVertexArrays( 1, &vao.at(id) );
	glBindVertexArray( vao.at(id) );

	GLuint vertexBuffer, indexBuffer;
	glGenBuffers( 1, &vertexBuffer );
	glGenBuffers( 1, &indexBuffer );

	vBuf.at(id) = vertexBuffer;
	iBuf.at(id) = indexBuffer;

	glBindBuffer( GL_ARRAY_BUFFER, vBuf.at(id) );
	glBufferData( GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iBuf.at(id) );
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
GLuint              Object::getVAO(unsigned id) {return vao.at(id);}
int                 Object::getSize() {return size;}
int                 Object::getNumVertices() {return numVertices;}
GLuint              Object::getVBuf(unsigned id) {GLuint ret = vBuf.at(id); return ret;}
GLuint              Object::getIBuf(unsigned id) {GLuint ret = iBuf.at(id); return ret;}

void hChangeData(Object &o, letter &l) {
	vertex v[l.points.size()];
	std::copy(l.points.begin(), l.points.end(), v);
	GLuint i[l.lines.size()];
	std::copy(l.lines.begin(), l.lines.end(), i);
	glBindVertexArray(o.getVAO(0));
	glBindBuffer(GL_ARRAY_BUFFER, o.getVBuf(0));
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, o.getIBuf(0));
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(i), i);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void vector2::changeData(Object &o, letter &l, letter &x) {
	hChangeData(o, x);
	vertex v[l.points.size()];
	std::copy(l.points.begin(), l.points.end(), v);
	GLuint i[l.lines.size()];
	std::copy(l.lines.begin(), l.lines.end(), i);
	glBindVertexArray(o.getVAO(0));
	glBindBuffer(GL_ARRAY_BUFFER, o.getVBuf(0));
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v), v);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, o.getIBuf(0));
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(i), i);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

// TileObject

TileObject::TileObject() : Object () {}
TileObject::TileObject( Empty * p ) : Object ( p ) {}
TileObject::TileObject( std::vector<vertex> v, std::vector<GLuint> i, std::vector<glm::vec3> o, glm::vec3 p, glm::quat r, glm::vec3 sc) : Object ( v, i, p, r, sc )
{
	offsets = o;
	instances = offsets.size();
}

void TileObject::setOffsets(std::vector<glm::vec3> o)
{
	offsets = o;
	instances = offsets.size();
}

void TileObject::genVAO(unsigned id, shader &shd)
{
	if (vao.size() == id) vao.push_back((GLuint)0);
	vertex v[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), v);

	GLuint i[indices.size()];
	std::copy(indices.begin(), indices.end(), i);

	glm::vec3 o[offsets.size()];
	std::copy(offsets.begin(), offsets.end(), o);

	glGenVertexArrays( 1, &vao.at(id) );
	glBindVertexArray( vao.at(id) );

	GLuint vertexBuffer, indexBuffer, instanceBuffer;
	glGenBuffers( 1, &vertexBuffer );
	glGenBuffers( 1, &indexBuffer );
	glGenBuffers( 1, &instanceBuffer );

	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(i), i, GL_STATIC_DRAW );

	glVertexAttribPointer( shd.attribIDPosition, 3, GL_FLOAT, false, sizeof(vertex), MEMBER_OFFSET(vertex,pos) );
	glEnableVertexAttribArray( shd.attribIDPosition );

	glVertexAttribPointer( shd.attribIDColor, 3, GL_FLOAT, false, sizeof(vertex), MEMBER_OFFSET(vertex,color) );
	glEnableVertexAttribArray( shd.attribIDColor );

	glBindBuffer( GL_ARRAY_BUFFER, instanceBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(o), o, GL_STATIC_DRAW);

	glVertexAttribPointer( shd.attribIDOffset, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( shd.attribIDOffset );
	glVertexAttribDivisor(2, 1);

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray( shd.attribIDPosition );
	glDisableVertexAttribArray( shd.attribIDColor );
	glDisableVertexAttribArray( shd.attribIDOffset );
}

int TileObject::getInstances() {return instances;}
