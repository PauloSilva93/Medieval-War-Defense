#version 330 core
in vec2 TexCoord;
out vec4 fragColor; 

uniform sampler2D texture0;

uniform bool isInverted;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 uv;

uniform vec4 color;


void main(){ 

	vec2 uvTexCoord; 
	uvTexCoord =  (uv * vec4(TexCoord, vec2(1.0f))).xy;

	// converte espaço das coordenadas de textura pro mesmo espaço da posição em que o objeto foi transformado
	vec2 objSpaceTexCoord = vec2((TexCoord * 2.0) - 1.0); 
	// transforma as coordenadas de textura neste espaço para equivalerem as coordenadas do objeto
	vec2 objTexCoord = (projection * view * model * vec4(objSpaceTexCoord, vec2(1.0f))).xy;
	// "desconverte" as coordenadas de textura para o espaço original, uma vez feita a transformação
	objTexCoord = (objTexCoord + 1.0) / 2.0;

	vec4 diffuse_material = texture2D(texture0, uvTexCoord) * color.a;

	fragColor = (diffuse_material + vec4(color.xyz, 0.0f));
}