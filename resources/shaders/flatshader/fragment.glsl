#version 330 core
in vec4 gsin;

layout(location=0) out vec4 outColor;

void main()
{
	outColor = gsin;
}