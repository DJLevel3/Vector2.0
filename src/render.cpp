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

void vector2::drawModel(Object o, shader s)
{
	glBindVertexArray( o.getVAO() );
	glUseProgram( s.program );

	glm::mat4 mvp = g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix() * o.getMatrix();
	glUniformMatrix4fv( s.mvp, 1, GL_FALSE, glm::value_ptr(mvp) );

	glDrawElements( GL_TRIANGLES, o.getSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0) );

	glUseProgram(0);
	glBindVertexArray(0);
}

void test()
{
	float vertices[] = {
	    -0.5f, -0.5f, 0.0f,
	     0.5f, -0.5f, 0.0f,
	     0.0f,  0.5f, 0.0f
	};
}
