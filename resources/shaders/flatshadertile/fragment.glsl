#version 330 core
in vec4 gsin;

layout(location=0) out vec4 outColor;

in vec4 fColor;

void main()
{
	outColor = fColor;
}