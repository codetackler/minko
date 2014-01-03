#ifdef FRAGMENT_SHADER

#ifdef PRECOMPUTED_AMBIENT
	uniform vec3 sumAmbients;
#endif // PRECOMPUTED_AMBIENT
	
#ifndef MINKO_NO_GLSL_STRUCT

	#ifdef NUM_AMBIENT_LIGHTS
		uniform AmbientLight		ambientLights[NUM_AMBIENT_LIGHTS];
	#endif // NUM_AMBIENT_LIGHTS

	#ifdef NUM_DIRECTIONAL_LIGHTS
		uniform DirectionalLight	directionalLights[NUM_DIRECTIONAL_LIGHTS];
	#endif // NUM_DIRECTIONAL_LIGHTS

	#ifdef NUM_POINT_LIGHTS
		uniform PointLight			pointLights[NUM_POINT_LIGHTS];
	#endif // NUM_POINT_LIGHTS

	#ifdef NUM_SPOT_LIGHTS
		uniform SpotLight			spotLights[NUM_SPOT_LIGHTS];
	#endif // NUM_SPOT_LIGHTS

#else

	#ifdef NUM_AMBIENT_LIGHTS
		uniform vec3	ambientLights_color[NUM_AMBIENT_LIGHTS];
		uniform float	ambientLights_ambient[NUM_AMBIENT_LIGHTS];
	#endif // NUM_AMBIENT_LIGHTS

	#ifdef NUM_DIRECTIONAL_LIGHTS
		uniform vec3	directionalLights_color[NUM_DIRECTIONAL_LIGHTS];
		uniform float	directionalLights_diffuse[NUM_DIRECTIONAL_LIGHTS];
		uniform float	directionalLights_specular[NUM_DIRECTIONAL_LIGHTS];
		uniform vec3	directionalLights_direction[NUM_DIRECTIONAL_LIGHTS];
	#endif // NUM_DIRECTIONAL_LIGHTS

	#ifdef NUM_POINT_LIGHTS
		uniform vec3	pointLights_color[NUM_POINT_LIGHTS];
		uniform float	pointLights_diffuse[NUM_POINT_LIGHTS];
		uniform float	pointLights_specular[NUM_POINT_LIGHTS];
		uniform float	pointLights_attenuationDistance[NUM_POINT_LIGHTS];
		uniform vec3	pointLights_position[NUM_POINT_LIGHTS];
	#endif // NUM_POINT_LIGHTS

	#ifdef NUM_SPOT_LIGHTS
		uniform vec3	spotLights_color[NUM_SPOT_LIGHTS];
		uniform float	spotLights_diffuse[NUM_SPOT_LIGHTS];
		uniform float	spotLights_specular[NUM_SPOT_LIGHTS];
		uniform float	spotLights_attenuationDistance[NUM_SPOT_LIGHTS];
		uniform vec3	spotLights_position[NUM_SPOT_LIGHTS];
		uniform vec3	spotLights_direction[NUM_SPOT_LIGHTS];
		uniform float	spotLights_cosInnerConeAngle[NUM_SPOT_LIGHTS];
		uniform float	spotLights_cosOuterConeAngle[NUM_SPOT_LIGHTS];
	#endif // NUM_SPOT_LIGHTS
	
#endif // MINKO_NO_GLSL_STRUCT
	
// diffuse
uniform vec4 diffuseColor;
uniform sampler2D diffuseMap;

// alpha
uniform sampler2D alphaMap;
uniform float alphaThreshold;

// phong
uniform vec4 specularColor;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform float shininess;
uniform vec3 cameraPosition;

// env. mapping
uniform sampler2D environmentMap;
uniform float environmentAlpha;

varying vec3 vertexPosition;
varying vec2 vertexUV;
varying vec3 vertexNormal;
varying vec3 vertexTangent;

void main(void)
{
	vec4 diffuse = diffuseColor;
	
	#ifdef DIFFUSE_MAP
		diffuse = texture2D(diffuseMap, vertexUV);
	#endif // DIFFUSE_MAP

	#ifdef ALPHA_MAP
		diffuse.a = texture2D(alphaMap, vertexUV).r;
	#endif // ALPHA_MAP

	#ifdef ALPHA_THRESHOLD
		if (diffuse.a < alphaThreshold)
			discard;
	#endif // ALPHA_THRESHOLD
	
	#if defined(SHININESS) || (defined(ENVIRONMENT_MAP) && !defined(ENVIRONMENT_ALPHA))
		vec4 specular = specularColor;

		#ifdef SPECULAR_MAP
			specular = texture2D(specularMap, vertexUV);
		#elif defined NORMAL_MAP
			specular.a = texture2D(normalMap, vertexUV).a;
		#endif // SPECULAR_MAP
		
	#endif
	
	vec3 phong = vec3(0.);
	
	#ifdef PRECOMPUTED_AMBIENT
	//------------------------
		phong += sumAmbients;
	#else
	
		#ifdef NUM_AMBIENT_LIGHTS
			for (int i = 0; i < NUM_AMBIENT_LIGHTS; ++i)
			{
				#ifndef MINKO_NO_GLSL_STRUCT
					phong += ambientLights[i].color * ambientLights[i].ambient;
				#else
					phong += ambientLights_color[i] * ambientLights_ambient[i];
				#endif // MINKO_NO_GLSL_STRUCT
			}
		#endif // NUM_AMBIENT_LIGHTS

	#endif // PRECOMPUTED_AMBIENT
	
	#if defined NUM_DIRECTIONAL_LIGHTS || defined NUM_POINT_LIGHTS || defined NUM_SPOT_LIGHTS || defined ENVIRONMENT_MAP

	vec3 eyeVector	= normalize(cameraPosition - vertexPosition);

	#endif // NUM_DIRECTIONAL_LIGHTS || NUM_POINT_LIGHTS || NUM_SPOT_LIGHTS || ENVIRONMENT_MAP

	#if defined NUM_DIRECTIONAL_LIGHTS || defined NUM_POINT_LIGHTS || defined NUM_SPOT_LIGHTS
		
		vec3	lightColor				= vec3(0.0);
		vec3 	lightDirection			= vec3(0.0);
		vec3	lightSpotDirection		= vec3(0.0);
		vec3 	lightPosition			= vec3(0.0);
		float	lightDiffuseCoeff		= 1.0;
		float	lightSpecularCoeff		= 1.0;
		float	lightAttenuationDist	= -1.0;
		float	lightCosInnerAng		= 0.0;
		float	lightCosOuterAng		= 0.0;
		float 	contribution			= 0.0;
		
		vec3 	normal					= normalize(vertexNormal);
		vec3	phongEyeVector			= eyeVector;
		
		#ifdef NORMAL_MAP
			// warning: the normal vector must be normalized at this point!
			mat3 worldToTangentMatrix 	= getWorldToTangentSpaceMatrix(normal, vertexTangent);
			
			normal						= normalize(2.0*texture2D(normalMap, vertexUV).xyz - 1.0);
			phongEyeVector				= worldToTangentMatrix * eyeVector;
		#endif // NORMAL_MAP
		
		// phongEyeVector	= normalize(phongEyeVector);
		
		#ifdef NUM_DIRECTIONAL_LIGHTS
		//---------------------------
		for (int i = 0; i < NUM_DIRECTIONAL_LIGHTS; ++i)
		{
			#ifndef MINKO_NO_GLSL_STRUCT
				lightColor			= directionalLights[i].color;
				lightDiffuseCoeff	= directionalLights[i].diffuse;
				lightSpecularCoeff	= directionalLights[i].specular;
				lightDirection		= directionalLights[i].direction;
			#else
				lightColor			= directionalLights_color[i];
				lightDiffuseCoeff	= directionalLights_diffuse[i];
				lightSpecularCoeff	= directionalLights_specular[i];
				lightDirection		= directionalLights_direction[i];
			#endif // MINKO_NO_GLSL_STRUCT
	
		
			lightDirection	= normalize(-lightDirection);
			#ifdef NORMAL_MAP
				lightDirection = worldToTangentMatrix * lightDirection;
			#endif // NORMAL_MAP
			
			contribution = phong_diffuseReflection(normal, lightDirection) * lightDiffuseCoeff;

			#ifdef SHININESS
				contribution += phong_specularReflection(
					normal,
					lightDirection,
					phongEyeVector,
					shininess
				) * lightSpecularCoeff * specular.a;
			#endif // SHININESS

			phong += contribution * lightColor;
		}
		#endif // NUM_DIRECTIONAL_LIGHTS
		
		#ifdef NUM_POINT_LIGHTS
		//---------------------
		for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
		{
			#ifndef MINKO_NO_GLSL_STRUCT
				lightColor				= pointLights[i].color;
				lightDiffuseCoeff		= pointLights[i].diffuse;
				lightSpecularCoeff		= pointLights[i].specular;
				lightAttenuationDist	= pointLights[i].attenuationDistance;
				lightPosition			= pointLights[i].position;
			#else
				lightColor				= pointLights_color[i];
				lightDiffuseCoeff		= pointLights_diffuse[i];
				lightSpecularCoeff		= pointLights_specular[i];
				lightAttenuationDist	= pointLights_attenuationDistance[i];
				lightPosition			= pointLights_position[i];
			#endif // MINKO_NO_GLSL_STRUCT
		
			lightDirection			= lightPosition - vertexPosition;
			float distanceToLight 	= length(lightDirection);
			lightDirection 			/= distanceToLight;
			#ifdef NORMAL_MAP
				lightDirection = worldToTangentMatrix * lightDirection;
			#endif // NORMAL_MAP
			
			contribution	= phong_diffuseReflection(normal, lightDirection) * lightDiffuseCoeff;

			#ifdef SHININESS
				contribution += phong_specularReflection(
					normal,
					lightDirection,
					phongEyeVector,
					shininess
				) * lightSpecularCoeff * specular.a;
			#endif // SHININESS
			
			float attenuation = lightAttenuationDist > 0.0
				? max(0.0, 1.0 - distanceToLight / lightAttenuationDist) 
				: 1.0;
				
			phong += attenuation * contribution * lightColor;
		}
		#endif // NUM_POINT_LIGHTS
		
		#ifdef NUM_SPOT_LIGHTS
		//--------------------
		for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
		{
			#ifndef MINKO_NO_GLSL_STRUCT
				lightColor				= spotLights[i].color;
				lightDiffuseCoeff		= spotLights[i].diffuse;
				lightSpecularCoeff		= spotLights[i].specular;
				lightAttenuationDist	= spotLights[i].attenuationDistance;
				lightPosition			= spotLights[i].position;
				lightSpotDirection		= spotLights[i].direction;
				lightCosInnerAng		= spotLights[i].cosInnerConeAngle;
				lightCosOuterAng		= spotLights[i].cosOuterConeAngle;
			#else
				lightColor				= spotLights_color[i];
				lightDiffuseCoeff		= spotLights_diffuse[i];
				lightSpecularCoeff		= spotLights_specular[i];
				lightAttenuationDist	= spotLights_attenuationDistance[i];
				lightPosition			= spotLights_position[i];
				lightSpotDirection		= spotLights_direction[i];
				lightCosInnerAng		= spotLights_cosInnerConeAngle[i];
				lightCosOuterAng		= spotLights_cosOuterConeAngle[i];
			#endif // MINKO_NO_GLSL_STRUCT
			
			
			lightDirection			= lightPosition - vertexPosition;
			float distanceToLight	= length(lightDirection);
			lightDirection			/= distanceToLight;
			
			lightSpotDirection	= normalize(-lightSpotDirection);						
			float cosSpot		= dot(-lightDirection, lightSpotDirection);

			if (lightCosOuterAng < cosSpot)
			{
				#ifdef NORMAL_MAP
					lightDirection		= worldToTangentMatrix * lightDirection;
					// lightSpotDirection	= worldToTangentMatrix * lightSpotDirection;
				#endif // NORMAL_MAP
			
				contribution	= phong_diffuseReflection(normal, lightDirection)
					* lightDiffuseCoeff;

				#ifdef SHININESS
					contribution += phong_specularReflection(
						normal,
						lightDirection,
						phongEyeVector,
						shininess
					) * lightSpecularCoeff * specular.a;
				#endif // SHININESS
				
				float cutoff	= cosSpot < lightCosInnerAng && lightCosOuterAng < lightCosInnerAng 
								? (cosSpot - lightCosOuterAng) / (lightCosInnerAng - lightCosOuterAng) 
								: 1.0;
				contribution	*= cutoff;
				
				float attenuation = lightAttenuationDist > 0.0
					? max(0.0, 1.0 - distanceToLight / lightAttenuationDist)
					: 1.0;
				
				phong += attenuation * contribution * lightColor;
			}
		}
		#endif // NUM_SPOT_LIGHTS
		
	#endif // defined NUM_DIRECTIONAL_LIGHTS || defined NUM_POINT_LIGHTS || defined NUM_SPOT_LIGHTS
	
	#ifdef ENVIRONMENT_MAP
		vec3 ref			= reflect(eyeVector, vertexNormal);
		vec3 refSpherical	= phong_cartesian3DToSpherical3D(ref);
		vec4 env			= texture2D(environmentMap, phong_spherical3DToCartesian2D(ref.y, ref.z));

		#ifdef ENVIRONMENT_ALPHA
			env *= environmentAlpha;
		#else
			env *= specular.a;
		#endif // ENVIRONMENT_ALPHA

		diffuse = vec4(diffuse.rgb + env.rgb, diffuse.a);
	#endif // defined ENVIRONMENT_MAP
	
	diffuse = vec4(diffuse.rgb * phong, diffuse.a);
	
	gl_FragColor = diffuse;
}

#endif // FRAGMENT_SHADER