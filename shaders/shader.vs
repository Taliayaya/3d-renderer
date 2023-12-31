#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;

out vec3 ourColor;
out vec3 newPos;
out vec2 TexCoord;

uniform float offset;

void main()
{
    newPos = vec3(aPos.x + offset, aPos.y, aPos.z);
    gl_Position = vec4(newPos, 1.0);
    ourColor = aColor;
    TexCoord = aTextCoord;
}

