// classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour; // ambient light colour
uniform vec3 LightColour; // light colour
uniform vec3 LightDirection;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

uniform sampler2D diffuseTex;

out vec4 FragColour;

void main() {
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );

	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

	vec3 textureColour = texture(diffuseTex, vTexCoords).rgb;

	// calculate each colour property
	vec3 ambient = AmbientColour * Ka * textureColour;
	vec3 diffuse = LightColour * Kd * lambertTerm * textureColour;
	vec3 specular = LightColour * Ks * specularTerm;

	FragColour = vec4( ambient + diffuse + specular, 1);
}