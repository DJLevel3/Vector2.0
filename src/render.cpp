#include "render.hpp"

using namespace vector2;

/*
vertex * vector2::devectorize(std::vector<vertex> v)
{
	vertex out[v.size()];
	std::copy(v.begin(), v.end(), out);
	return out;
}
GLuint * vector2::devectorize(std::vector<GLuint> v, bool null)
{
	GLuint out[v.size()];
	std::copy(v.begin(), v.end(), out);
	return out;
}
*/

float rFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
float rFloat(float bottom, float top)
{
	return bottom + static_cast <float> (rand()) / static_cast <float> ((RAND_MAX) / top);
}

glm::quat vector2::rQuat()
{
	double x,y,z,u,v,w,s;
	x = y = z = u = v = w = s = 5;
	while (x > 1) { x = rFloat(-1,1); y = rFloat(-1,1); z = x*x + y*y; }
	while (w > 1) { u = rFloat(-1,1); v = rFloat(-1,1); w = u*u + v*v; }
	s = glm::sqrt((1-z) / w);
	return glm::quat(x, y, s*u, s*v);
}

std::vector<glm::quat> vector2::splash(int instances)
{
	std::vector<glm::quat> v = {};
	for (int i = 0; i < instances / 2; i ++) {v.push_back(rQuat()); v.push_back(rQuat() * glm::quat(0, 0, 1, 0));}
	return v;
}
std::vector<glm::quat> vector2::drawSplash(int instances, float scale, Object &o, int id, shader &s, Camera &c)
{
	std::vector<glm::quat> v = splash(instances);
	std::vector<glm::quat> r = splash(instances);

	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	o.setScale({scale, scale, scale});

	for (int i = 0; i < instances; i ++)
	{
		o.setRotation(r.at(i));
		glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * o.getParentMatrix() * (glm::toMat4(v.at(i)) * o.getSelfMatrix());
		glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );
		glDrawElements( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
	}

	glUseProgram(0);
	glBindVertexArray(0);

	return v;
}
void vector2::drawSplash(std::vector<glm::quat> v, int instances, float scale, float movement, Object &o, int id, shader &s, Camera &c)
{
	std::vector<glm::quat> r = splash(v.size());

	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::vec3 p = o.getPosition();

	o.setPosition({0, 0, movement});
	o.setScale({scale, scale, scale});

	for (int i = 0; i < instances; i ++)
	{
		o.setRotation(r.at(i));
		glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * o.getParentMatrix() * (glm::toMat4(v.at(i)) * o.getSelfMatrix());
		glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );
		glDrawElements( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );
	}

	o.setPosition(p);

	glUseProgram(0);
	glBindVertexArray(0);
}

void vector2::drawModel(Object &o, int id, shader &s, Camera &c)
{
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElements( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

	glUseProgram(0);
	glBindVertexArray(0);
}
void vector2::drawModel(TileObject &o, int id, shader &s, Camera &c)
{
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElementsInstanced( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0), o.getInstances() );

	glUseProgram(0);
	glBindVertexArray(0);
}
void vector2::drawModelOver(Object &o, int id, shader &s, Camera &c)
{
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElements( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

	glUseProgram(0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
void vector2::drawModelOver(TileObject &o, int id, shader &s, Camera &c)
{
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElementsInstanced( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0), o.getInstances() );

	glUseProgram(0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void vector2::drawModel(Object2D &o, int id, shader &s, Camera2D &c)
{
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix()  * glm::toMat4(glm::quat(0,0,1,0)) * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElements( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

	glUseProgram(0);
	glBindVertexArray(0);
}
void vector2::drawModel(TileObject2D &o, int id, shader &s, Camera2D &c)
{
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElementsInstanced( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0), o.getInstances() );

	glUseProgram(0);
	glBindVertexArray(0);
}
void vector2::drawModelOver(Object2D &o, int id, shader &s, Camera2D &c)
{
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElements( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

	glUseProgram(0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
void vector2::drawModelOver(TileObject2D &o, int id, shader &s, Camera2D &c)
{
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray( o.getVAO(id) );
	glUseProgram( s.program );

	glm::mat4 MVP = c.GetProjectionMatrix() * o.getMatrix(); // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElementsInstanced( GL_LINES_ADJACENCY, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0), o.getInstances() );

	glUseProgram(0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
