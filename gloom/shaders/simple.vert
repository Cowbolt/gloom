#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec3 color;

out layout(location=0) vec3 vertColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    vertColor = color;
}
