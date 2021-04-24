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

void vector2::drawModel(Object &o, shader &s, Camera &c)
{
	glBindVertexArray( o.getVAO() );
	glUseProgram( s.program );

	glm::mat4 t = o.getMatrix();
	glm::mat4 MVP = c.GetProjectionMatrix() * c.GetViewMatrix() * t; // glm::toMat4(glm::quat(1,0,0,0)); //
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(MVP) );

	glDrawElements( GL_TRIANGLES, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

	glUseProgram(0);
	glBindVertexArray(0);
}
