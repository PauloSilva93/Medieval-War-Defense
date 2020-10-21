#version 330 core
in vec2 TexCoord;
out vec4 fragColor; 

uniform sampler2D texture0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;

void main(){ 

	vec4 diffuse_material = texture2D(texture0, TexCoord) * color.a;

	fragColor = diffuse_material + vec4(color.rgb, 0.0f);
}