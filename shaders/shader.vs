#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

out vec3 ourColor;
out vec3 newPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    newPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = vec3(1, 1, 1);
    TexCoord = aTextCoord;
}

