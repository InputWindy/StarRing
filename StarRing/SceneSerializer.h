#pragma once

#include "Scene.h"

namespace StarRing {

	class SR_API SceneSerializer
	{
	public:
		SceneSerializer(const SceneRef& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		SceneRef m_Scene;
	};

}
