#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>

using Entity = uint32_t;

class ComponentArrayT {

public:
	virtual void Destroy(Entity entity) = 0;
};

template <typename T>
class ComponentArray: public ComponentArrayT {
private:
	std::unordered_map<Entity, uint32_t> entity_index_map;
	std::unordered_map<uint32_t, Entity> index_entity_map;
	std::vector<T> componentarray;

public:
	void Add(Entity entity, T data) {
		if (entity_index_map.find(entity) != entity_index_map.end()) {
			componentarray[entity_index_map[entity]] = data;
		} else {
			entity_index_map[entity] = componentarray.size();
			entity_index_map[componentarray.size()] = entity;
			componentarray.push_back(data);
		}
	}

	T& Get(Entity entity) {
		assert(entity_index_map.find(entity) != entity_index_map.end());

		return componentarray[entity_index_map[entity]];
	}

	void Destroy(Entity entity) override {
		assert(entity_index_map.find(entity) != entity_index_map.end());

		auto last_index = componentarray.size() - 1;
		auto last_entity = index_entity_map[last_index];
		auto remove_index = entity_index_map[entity];

		std::swap(componentarray[remove_index], componentarray[last_index]);

		index_entity_map[remove_index] = last_entity;
		entity_index_map[last_entity] = remove_index;
		
		componentarray.pop_back();
		index_entity_map.erase(last_index);
		entity_index_map.erase(entity);
	}
};

class Registry {
	std::vector<Entity> available_entities;
	Entity next_entity = 0;

	std::unordered_map<std::size_t, std::shared_ptr<ComponentArrayT>> componentarray_map;

public:
	Entity Create() {
		if (available_entities.size()) {
			auto entity = available_entities.back();
			available_entities.pop_back();
			return entity;
		}

		return next_entity++;
	}

	void Destroy(Entity entity) {
		available_entities.push_back(entity);

		for (auto& pair: componentarray_map) {
			pair.second->Destroy(entity);
		}
	}

	template <typename T, typename ...Args>
	void Add(Entity entity, Args ...params) {
		GetComponentArray<T>()->Add(entity, T{params...});
	}

	template <typename T>
	T& Get(Entity entity) {
		return GetComponentArray<T>()->Get(entity);
	}

private:
	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		if (componentarray_map.find(typeid(T).hash_code()) == componentarray_map.end()) {
			componentarray_map[typeid(T).hash_code()] = std::static_pointer_cast<ComponentArrayT>(std::make_shared<ComponentArray<T>>());
		}

		return std::static_pointer_cast<ComponentArray<T>>(componentarray_map[typeid(T).hash_code()]);
	}
};