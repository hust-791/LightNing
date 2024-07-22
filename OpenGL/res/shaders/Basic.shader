#shader vertex
#version 330 core
layout (location = 0) in vec4 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos);
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}
