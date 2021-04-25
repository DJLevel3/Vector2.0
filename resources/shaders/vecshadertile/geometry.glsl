#version 330 core
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 5) out;

in vec4 gsin[];

out vec4 fColor;

void main() {
	fColor = gsin[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	fColor = gsin[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	fColor = gsin[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	fColor = gsin[3];
	gl_Position = gl_in[3].gl_Position;
	EmitVertex();
	fColor = gsin[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();
}