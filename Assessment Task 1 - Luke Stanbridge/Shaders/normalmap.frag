#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoords;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour; // ambient light colour
uniform vec3 LightColour; // light colour
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

out vec4 FragColour;

vec3 getDiffuse(vec3 direction, vec3 colour, vec3 normal)
{
	return colour * max( 0, dot( normal, -direction ) );
}

vec3 getSpecular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
	vec3 R = reflect( direction, normal );
	float specularTerm = pow( max( 0, dot( R, view ) ), specularPower );
	return specularTerm * colour;
}

void main() 
{
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);

	mat3 TBN = mat3(T,B,N);
	vec3 texNormal = texture( normalTex, vTexCoords).rgb;
	N = TBN * (texNormal * 2 - 1);

	// calculate diffuse lighting from sunlight
	vec3 diffuseTotal = getDiffuse(L, LightColour, N);

	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);

	// calculate specular light from directional light
	vec3 specularTotal = getSpecular(L, LightColour, N, V);

	vec3 texDiffuse = texture(diffuseTex, vTexCoords).rgb;
	vec3 texSpecular = texture( specularTex, vTexCoords).rgb;

	for (int i=0; i<numLights; i++)
	{
		vec3 direction = vPosition.xyz - PointLightPosition[i];
		float distance = length(direction);
		direction = direction/distance;

		// attenuate the light intensity with inverse square law
		vec3 colour = PointLightColour[i]/(distance * distance);

		diffuseTotal += getDiffuse(direction, colour, N);
		specularTotal += getSpecular(direction, colour, N, V);
	}

	// calculate each colour property
	vec3 ambient = AmbientColour * Ka * texDiffuse;
	vec3 diffuse = Kd * texDiffuse * diffuseTotal;
	vec3 specular = Ks * texSpecular * specularTotal;

	FragColour = vec4( ambient + diffuse + specular, 1);
	
}

