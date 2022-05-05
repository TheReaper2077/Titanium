#version 400 core

in vec3 normal;
in vec4 color;
in vec2 uv0;
in vec2 uv1;
in vec2 uv2;
in vec2 uv3;
in vec2 uv4;
in vec2 uv5;
in vec2 uv6;
in vec2 uv7;

in vec3 frag_pos;
in vec3 view_pos;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
  
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}


void main() {
	vec3 N = normalize(normal);
	vec3 V = normalize(view_pos - frag_pos);

	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) {
		vec3 L = normalize(lightPositions[i] - frag_pos);
		vec3 H = normalize(V + L);
	
		float distance    = length(lightPositions[i] - frag_pos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance     = lightColors[i] * attenuation;

		vec3 F0 = vec3(0.04); 
		F0 = mix(F0, albedo, metallic);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		float NDF = DistributionGGX(N, H, roughness);       
		float G = GeometrySmith(N, V, L, roughness);

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
		vec3 specular     = numerator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		
		kD *= 1.0 - metallic;
  
		float NdotL = max(dot(N, L), 0.0);        
		Lo += (kD * albedo / 3.14159265359 + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    gl_FragColor = vec4(color, 1.0);
}