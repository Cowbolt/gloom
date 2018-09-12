#version 430 core

in layout(location=1) vec4 vertColor;

out vec4 fragColor;

void main()
{
    fragColor = vertColor;
}
