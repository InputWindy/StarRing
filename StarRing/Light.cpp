#include "pch.h"
#include "Light.h"
namespace StarRing {
	LightType DirectionalLight::ms_LightType = LightType::DirectionalLight;
	LightType PointLight::ms_LightType = LightType::PointLight;
	LightType SpotLight::ms_LightType = LightType::SpotLight;
	LightType AreaLight::ms_LightType = LightType::AreaLight;
}