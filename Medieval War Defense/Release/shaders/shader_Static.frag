#version 330 core
in vec2 TexCoord;
in vec3 viewCoord;
out vec4 fragColor; 

uniform sampler2D dif;
uniform sampler2D norm;
uniform sampler2D spec;
uniform sampler2D amb;
uniform sampler2D emit;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;

#define MAX_LIGHTS 50

uniform int totalLights;

uniform struct Light {
    vec3 lightPos;
    vec4 diffuse_lightColor;
	vec4 specular_lightColor;
	vec4 ambient_lightColor;
	float shininess;
	bool pointLight;
	bool spotLight;
	vec3 falloff;
	vec3 light_Spot;
	float spotCutOff;
	float outerCutOff;
} LightSource[MAX_LIGHTS];

void main(){ 

	
	// converte espaço das coordenadas de textura pro mesmo espaço da posição em que o objeto foi transformado
	vec2 objSpaceTexCoord = vec2((TexCoord * 2.0) - 1.0); 
	// transforma as coordenadas de textura neste espaço para equivalerem as coordenadas do objeto
	vec2 objTexCoord = (projection * view * model * vec4(objSpaceTexCoord, vec2(1.0f))).xy;
	// "desconverte" as coordenadas de textura para o espaço original, uma vez feita a transformação
	objTexCoord = (objTexCoord + 1.0) / 2.0;

	vec4 diffuse_material = texture2D(dif, TexCoord);
	vec3 normal = texture2D(norm, TexCoord).rgb;
	vec4 specular_material = texture2D(spec, TexCoord);
	vec4 ambient_material = texture2D(amb, TexCoord);
	vec4 emit_material = texture2D(emit, TexCoord);

vec4 lastColor = vec4(0.0);

	normal -= 0.5; // transformamos o intervalo de [0,1] para [-0.5,0.5]. Pois cada pixel representa uma direção, não cor.

	for(int i = 0; i < totalLights; i++)
	{
		
		vec2 lightPosition = LightSource[i].lightPos.xy; /// resolution.xy; // normalizamos o vetor lightPos, das coordenadas de tela para [0, 1];
		//lightPosition.y = 1.0 - lightPosition.y; // invertemos o y pra corresponder as coordenadas de textura.
		
		vec3 L = vec3(0.0);
		if (LightSource[i].pointLight)
		{
			// Luz de ponto
			L = vec3(lightPosition.x - objTexCoord.x, lightPosition.y - objTexCoord.y, LightSource[i].lightPos.z);
		}
		else
		{
			// Luz direcional
			L = vec3(lightPosition.x, lightPosition.y, LightSource[0].lightPos.z);
			L.xy -= 0.5;
		}
		

		// calculamos um vetor de distância entre o pixel da textura e a posição da luz;
		vec3 N = normalize(normal); // normalizamos esse vetor, pois só precisamos de sua direção.

		//Calculo da atenuação
		float D = length(L);
		float Attenuation = 1.0 / ( LightSource[i].falloff.x + (LightSource[i].falloff.y*D) + (LightSource[i].falloff.z*D*D) );

		// normalizamos o vetor que corresponde a essa distância.
		L = normalize(L);

		vec3 E = normalize(-viewCoord);
		vec3 H = normalize(L + E);

		// DIFFUSE COMPONENT
		float diffuse_intensity = max(dot(N, L), 0.0);
		vec4 diffuse_light = diffuse_intensity * LightSource[i].diffuse_lightColor * diffuse_material * color.a;

		// SPECULAR COMPONENT
		float specular_intensity = pow(max(dot(N, H), 0.0), LightSource[i].shininess);
		vec4 specular_light = specular_intensity * LightSource[i].specular_lightColor * specular_material * color.a;

		//AMBIENT COMPONENT
		vec4 ambient_light = LightSource[i].ambient_lightColor * ambient_material * color.a;

		//EMIT COMPONENT
		float emit_intensity = 0.0;
		vec4 emit_light = vec4(emit_intensity * emit_material.xyz, 0.0);

		if (LightSource[i].spotLight)
		{
			// Luz holofote
			vec3 light_Spot = LightSource[i].light_Spot;
			float spotCutOff = LightSource[i].spotCutOff;
			float outerCutOff = LightSource[i].outerCutOff;
			float theta = dot(L, normalize(-light_Spot));
			float epsilon = (spotCutOff - outerCutOff);
			float spot_intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
			diffuse_light *= spot_intensity;
			specular_light *= spot_intensity;
		}
	
		vec4 finalColor =  (diffuse_light + specular_light ) * Attenuation + ambient_light + emit_light;
		lastColor += finalColor;
	}

	fragColor = lastColor + vec4(color.rgb, 0.0f);
}