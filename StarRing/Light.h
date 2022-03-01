#pragma once
#include "Core.h"
#include <glm/glm.hpp>
using namespace glm;
namespace StarRing {
	enum class LightType
	{
		DirectionalLight = 0,
		PointLight = 1,
		SpotLight = 2,
		AreaLight = 3
	};

	DECLARE_REF(DirectionalLight)
	DECLARE_SCOPE(DirectionalLight)
	class SR_API DirectionalLight
	{
	public:
		DirectionalLight() = default;
		~DirectionalLight() = default;

		vec3 dColor ;
		vec3 dLightDir ;

		static LightType ms_LightType;
	};

	DECLARE_REF(PointLight)
	DECLARE_SCOPE(PointLight)
	class SR_API PointLight
	{
	public:
		PointLight() = default;
		~PointLight() = default;
		static LightType ms_LightType;

		vec3 pColor ;
		vec3 pPosition ;
		float pCutOff = 20.0f;
	};

	DECLARE_REF(SpotLight)
	DECLARE_SCOPE(SpotLight)
	class SR_API SpotLight
	{
	public:
		SpotLight() = default;
		~SpotLight() = default;
		static LightType ms_LightType;

		vec3 sColor ;
		vec3 sPosition ;
		vec3 sLightDir ;
		float sAngle = 45.0f;
		float sCutOff = 20.0f;
	};

	DECLARE_REF(AreaLight)
	DECLARE_SCOPE(AreaLight)
	class SR_API AreaLight
	{
	public:
		AreaLight() = default;
		~AreaLight() = default;
		static LightType ms_LightType;

		vec3 aColor ;
		vec3 aPosition ;
		vec3 aNormal ;
		float awidth = 1.0f;
		float aheight =1.0f;
		float aCutOff = 20.0f;
	};

}

