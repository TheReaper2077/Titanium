#include "Serializer.h"

#include <fstream>
#include "Components/Components.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/emitter.h"

YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec2& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec3& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec4& v) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

template<>
struct YAML::convert<glm::vec2> {
	static Node encode(const glm::vec2& rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		return node;
	}

	static bool decode(const Node& node, glm::vec2& rhs) {
		if(!node.IsSequence() || node.size() != 2) {
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();

		return true;
	}
};

template<>
struct YAML::convert<glm::vec3> {
	static Node encode(const glm::vec3& rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		return node;
	}

	static bool decode(const Node& node, glm::vec3& rhs) {
		if(!node.IsSequence() || node.size() != 3) {
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();

		return true;
	}
};

template<>
struct YAML::convert<glm::vec4> {
	static Node encode(const glm::vec4& rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		return node;
	}

	static bool decode(const Node& node, glm::vec4& rhs) {
		if(!node.IsSequence() || node.size() != 4) {
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();

		return true;
	}
};

void ti::Serialize(ECS::Registry* registry, const char* filename) {
	using namespace ti::Component;

	YAML::Emitter out;
	
	out << YAML::BeginSeq;
	for (auto& entity: registry->View<Tag>()) {
		auto& tag = registry->Get<Tag>(entity);

		if (!tag.save) continue;

		out << YAML::BeginMap;

		out << YAML::Key << "Name";
		out << YAML::Value << tag.name;
		out << YAML::Key << "ID";
		out << YAML::Value << tag.id;
		out << YAML::Key << "Components";
		out << YAML::Value;

		out << YAML::BeginMap;
		if (registry->Contains<Transform>(entity)) {
			auto& transform = registry->Get<Transform>(entity);

			out << YAML::Key << "Transform";
			out << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "position";
			out << YAML::Value << transform.position;
			out << YAML::Key << "rotation";
			out << YAML::Value << transform.rotation;
			out << YAML::Key << "scale";
			out << YAML::Value << transform.scale;
			out << YAML::EndMap;
		}

		if (registry->Contains<Camera>(entity)) {
			auto& camera = registry->Get<Camera>(entity);

			out << YAML::Key << "Camera";
			out << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "fov";
			out << YAML::Value << camera.fov;
			out << YAML::Key << "near";
			out << YAML::Value << camera.camera_near;
			out << YAML::Key << "far";
			out << YAML::Value << camera.camera_far;
			out << YAML::Key << "Projection";
			out << YAML::Value << camera.mode;
			out << YAML::Key << "CameraType";
			out << YAML::Value << camera.type;
			out << YAML::EndMap;
		}

		if (registry->Contains<Light>(entity)) {
			auto& light = registry->Get<Light>(entity);

			out << YAML::Key << "Light";
			out << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "ambient";
			out << YAML::Value << light.ambient;
			out << YAML::Key << "diffuse";
			out << YAML::Value << light.diffuse;
			out << YAML::Key << "specular";
			out << YAML::Value << light.specular;
			out << YAML::Key << "constant";
			out << YAML::Value << light.constant;
			out << YAML::Key << "linear";
			out << YAML::Value << light.linear;
			out << YAML::Key << "quadratic";
			out << YAML::Value << light.quadratic;
			out << YAML::Key << "cutOff";
			out << YAML::Value << light.cutOff;
			out << YAML::Key << "outerCutOff";
			out << YAML::Value << light.outerCutOff;
			out << YAML::Key << "active";
			out << YAML::Value << light.active;
			out << YAML::Key << "mode";
			out << YAML::Value << light.mode;
			out << YAML::EndMap;
		}

		if (registry->Contains<MeshFilter>(entity)) {
			auto& meshfilter = registry->Get<MeshFilter>(entity);

			out << YAML::Key << "MeshFilter";
			out << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "mesh";
			out << YAML::Value << meshfilter.mesh;
			out << YAML::EndMap;
		}

		if (registry->Contains<SpriteRenderer>(entity)) {
			auto& spriterenderer = registry->Get<SpriteRenderer>(entity);

			out << YAML::Key << "SpriteRenderer";
			out << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "visible";
			out << YAML::Value << spriterenderer.visible;
			out << YAML::Key << "lighting";
			out << YAML::Value << spriterenderer.lighting;
			out << YAML::Key << "flags";
			out << YAML::Value << spriterenderer.flags;
			out << YAML::Key << "color";
			out << YAML::Value << spriterenderer.color;
			out << YAML::EndMap;
		}

		if (registry->Contains<MeshRenderer>(entity)) {
			auto& meshrenderer = registry->Get<MeshRenderer>(entity);

			out << YAML::Key << "MeshRenderer";
			out << YAML::Value;

			out << YAML::BeginMap;
			out << YAML::Key << "visible";
			out << YAML::Value << meshrenderer.visible;
			out << YAML::Key << "lighting";
			out << YAML::Value << meshrenderer.lighting;
			out << YAML::Key << "flags";
			out << YAML::Value << meshrenderer.flags;
			out << YAML::Key << "material";
			out << YAML::Value << meshrenderer.material;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	std::ofstream fout(filename);
	fout << out.c_str();
	fout.close();
}

void ti::DeSerialize(ECS::Registry* registry, const char* filename) {
	using namespace ti::Component;

	std::ifstream f(filename);
	if (!f.good()) return;
	f.close();

	YAML::Node scene = YAML::LoadFile(filename);

	for (std::size_t i = 0; i < scene.size(); i++) {
		Tag tag;

		tag.name = scene[i]["Name"].as<std::string>();
		tag.id = scene[i]["ID"].as<std::string>();
		tag.save = true;

		auto entity = registry->Create();

		registry->Add(entity, tag);

		for (auto pair: scene[i]["Components"]) {
			if (pair.first.as<std::string>() == "Transform") {
				Transform transform;

				transform.position = pair.second["position"].as<glm::vec3>();
				transform.rotation = pair.second["rotation"].as<glm::vec3>();
				transform.scale = pair.second["scale"].as<glm::vec3>();

				registry->Add(entity, transform);
			}

			if (pair.first.as<std::string>() == "Camera") {
				Camera camera;

				camera.fov = pair.second["fov"].as<float>();
				camera.camera_near = pair.second["near"].as<float>();
				camera.camera_far = pair.second["far"].as<float>();
				camera.mode = (Projection)pair.second["Projection"].as<int>();
				camera.type = (CameraType)pair.second["CameraType"].as<int>();

				registry->Add(entity, camera);
			}

			if (pair.first.as<std::string>() == "Light") {
				Light light;

				light.ambient = pair.second["ambient"].as<glm::vec3>();
				light.diffuse = pair.second["diffuse"].as<glm::vec3>();
				light.specular = pair.second["specular"].as<glm::vec3>();
				light.constant = pair.second["constant"].as<float>();
				light.linear = pair.second["linear"].as<float>();
				light.quadratic = pair.second["quadratic"].as<float>();
				light.cutOff = pair.second["cutOff"].as<float>();
				light.outerCutOff = pair.second["outerCutOff"].as<float>();
				light.active = pair.second["active"].as<bool>();
				light.mode = (LightMode)pair.second["mode"].as<int>();

				registry->Add(entity, light);
			}

			if (pair.first.as<std::string>() == "MeshFilter") {
				MeshFilter meshfilter;

				meshfilter.mesh = pair.second["mesh"].as<std::string>();

				registry->Add(entity, meshfilter);
			}

			if (pair.first.as<std::string>() == "SpriteRenderer") {
				SpriteRenderer spriterenderer;

				spriterenderer.visible = pair.second["visible"].as<bool>();
				spriterenderer.lighting = pair.second["lighting"].as<bool>();
				spriterenderer.flags = pair.second["flags"].as<int>();
				spriterenderer.color = pair.second["color"].as<glm::vec4>();

				registry->Add(entity, spriterenderer);
			}

			if (pair.first.as<std::string>() == "MeshRenderer") {
				MeshRenderer meshrenderer;

				meshrenderer.visible = pair.second["visible"].as<bool>();
				meshrenderer.lighting = pair.second["lighting"].as<bool>();
				meshrenderer.flags = pair.second["flags"].as<int>();
				meshrenderer.material = pair.second["material"].as<std::string>();

				registry->Add(entity, meshrenderer);
			}
		}
	}
}

