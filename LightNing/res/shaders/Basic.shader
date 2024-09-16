#shader vertex
#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord; 
out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjection * u_Transform *  vec4(aPos,1.0f);
    v_TexCoord = texCoord;
}

#shader fragment
#version 410

layout (location = 0) out vec4 FragColor;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 color = texture(u_Texture,v_TexCoord);
    FragColor = color * u_Color;
}
