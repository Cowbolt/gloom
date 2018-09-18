#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 color;

uniform layout(location=2) mat4 afTrans;
out layout(location=1) vec4 vertColor;

void main()
{
    gl_Position = afTrans * vec4(position, 1.0f);
    vertColor = color;
}
