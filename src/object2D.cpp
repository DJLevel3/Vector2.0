#include "object2D.hpp"

using namespace vector2;



// Empty2D

Empty2D::Empty2D( )
{
	parent = NULL;
}

Empty2D::Empty2D( Empty2D * p )
{
	parent = p;
}

Empty2D::Empty2D( glm::vec3 p, glm::quat r, glm::vec2 sc )
{
	parent = NULL;
	stale = true;
	pos = p;
	rot = r;
	scl = sc;
}

glm::mat4 Empty2D::getMatrix(bool head)
{
	if (head) checkTree();
	return modelMatrix;
}

void Empty2D::setPosition(glm::vec3 position)
{
	pos = position;
	stale = true;
}

void Empty2D::setRotation(glm::quat rotation)
{
	rot = rotation;
	stale = true;
}

void Empty2D::setScale(glm::vec2 scale)
{
	scl = scale;
	stale = true;
}

void Empty2D::move(glm::vec3 position)
{
	pos += position;
	stale = true;
}

void Empty2D::rotate(glm::quat rotation)
{
	rot *= rotation;
	stale = true;
}

void Empty2D::scale(glm::vec2 scale)
{
	scl *= scale;
	stale = true;
}

bool Empty2D::checkTree()
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

void Empty2D::genMatrix()
{
	modelMatrix = glm::scale(glm::translate(glm::mat4(1.0), pos) * glm::toMat4(rot), glm::vec3(scl, 1));
	if (parent != NULL) modelMatrix = parent->getMatrix(false) * modelMatrix;
	stale = false;
}

glm::vec3 Empty2D::getPosition() {return pos;}
glm::quat Empty2D::getRotation() {return rot;}
glm::vec2 Empty2D::getScale() {return scl;}
glm::mat4 Empty2D::getMatrix() {return getMatrix(true);}



// Object2D

Object2D::Object2D()
{
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object2D::Object2D( Empty2D * p ) : Empty2D ( p )
{
	vertices = {};
	numVertices = 0;
	indices = {};
	size = 0;
}

Object2D::Object2D( std::vector<vertex> v, std::vector<GLuint> i, glm::vec3 p, glm::quat r, glm::vec2 sc) : Empty2D ( p, r, sc )
{
	vertices = v;
	indices = i;
	numVertices = vertices.size();
	size = indices.size();
}

void Object2D::genVAO(unsigned id, shader &shd)
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

void Object2D::setVertices(std::vector<vertex> v)
{
	vertices = v;
	numVertices = v.size();
}

void Object2D::setIndices(std::vector<GLuint> i)
{
	indices = i;
	size = i.size();
}

std::vector<vertex> Object2D::getVertices() {return vertices;}
std::vector<GLuint> Object2D::getIndices() {return indices;}
GLuint              Object2D::getVAO(unsigned id) {return vao.at(id);}
int                 Object2D::getSize() {return size;}
int                 Object2D::getNumVertices() {return numVertices;}
GLuint              Object2D::getVBuf(unsigned id) {GLuint ret = vBuf.at(id); return ret;}
GLuint              Object2D::getIBuf(unsigned id) {GLuint ret = iBuf.at(id); return ret;}

void hChangeData(Object2D &o, letter &l) {
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

void vector2::changeData(Object2D &o, letter &l, letter &x) {
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

// TileObject2D

TileObject2D::TileObject2D() : Object2D () {}
TileObject2D::TileObject2D( Empty2D * p ) : Object2D ( p ) {}
TileObject2D::TileObject2D( std::vector<vertex> v, std::vector<GLuint> i, std::vector<glm::vec2> o, glm::vec3 p, glm::quat r, glm::vec2 sc) : Object2D ( v, i, p, r, sc )
{
	offsets = o;
	instances = offsets.size();
}

void TileObject2D::setOffsets(std::vector<glm::vec2> o)
{
	offsets = o;
	instances = offsets.size();
}

void TileObject2D::genVAO(unsigned id, shader &shd)
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

int TileObject2D::getInstances() {return instances;}
