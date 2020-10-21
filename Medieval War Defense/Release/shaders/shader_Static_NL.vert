#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec2 aTexCoord; // mais um atributo para textura

out vec2 TexCoord; // output da textura

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){ 
TexCoord = aTexCoord;
gl_Position = projection * view * model * vec4(aPos, 1.0); 
}