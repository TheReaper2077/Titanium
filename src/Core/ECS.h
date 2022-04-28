#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <set>
#include <iostream>

namespace ti {
	namespace ECS {
		using Entity = uint32_t;
		using ComponentType = uint32_t;
		using ID = uint64_t;

		class ComponentArrayT {
		public:
			ComponentType type;
			virtual void Destroy(Entity entity) = 0;
		};

		template <typename T>
		class ComponentArray: public ComponentArrayT {
		private:
			std::unordered_map<Entity, uint32_t> entity_index_map;
			std::unordered_map<uint32_t, Entity> index_entity_map;
			std::vector<T> componentarray;
			uint32_t next_index = 0;

		public:
			void Add(Entity entity, T data) {
				if (entity_index_map.find(entity) != entity_index_map.end()) {
					componentarray[entity_index_map[entity]] = data;
				} else {
					entity_index_map[entity] = next_index;
					index_entity_map[next_index] = entity;
					componentarray.push_back(data);
					next_index++;
				}
			}

			T& Get(Entity entity) {
				assert(entity_index_map.find(entity) != entity_index_map.end());

				return componentarray[entity_index_map[entity]];
			}

			void Destroy(Entity entity) override {
				assert(entity_index_map.find(entity) != entity_index_map.end());

				next_index--;

				unsigned int index_removed = entity_index_map[entity];
				Entity last_entity = index_entity_map[next_index];
				
				entity_index_map[last_entity] = index_removed;
				index_entity_map[index_removed] = last_entity;

				componentarray[index_removed] = componentarray[next_index];

				componentarray.pop_back();
				entity_index_map.erase(entity);
				index_entity_map.erase(next_index);
			}
		};

		struct System {
			ID whitelist;
			ID blacklist;
		};

		class Registry {
			std::vector<Entity> available_entities;
			Entity next_entity = 0;
			ComponentType next_component_type = 0;

			std::unordered_map<std::size_t, std::shared_ptr<ComponentArrayT>> componentarray_map;
			std::unordered_map<std::size_t, ComponentType> componenttype_map;
			std::unordered_map<Entity, ID> entityid_map;
			std::unordered_map<ID, std::set<Entity>> system_map;
			std::unordered_map<std::size_t, Entity> store_map;

		public:
			Entity Create() {
				Entity entity;

				if (available_entities.size()) {
					entity = available_entities.back();
					available_entities.pop_back();
				} else {
					entity = next_entity++;
				}

				entityid_map[entity] = 0;

				return entity;
			}

			template <typename T>
			bool Contains(Entity entity) {
				if (componentarray_map.find(typeid(T).hash_code()) != componentarray_map.end())
					return ((entityid_map[entity] & (1 << GetComponentArray<T>()->type)) == entityid_map[entity]);
				
				return false;
			}

			template <typename T>
			void Remove(Entity entity) {
				assert(componentarray_map.find(typeid(T).hash_code()) != componentarray_map.end());

				GetComponentArray<T>()->Destroy(entity);
			}

			void Destroy(Entity entity) {
				available_entities.push_back(entity);

				for (auto& pair: componentarray_map) {
					pair.second->Destroy(entity);
				}
			}

			template <typename T, typename ...Args>
			void Add(Entity entity, Args ...args) {
				GetComponentArray<T>()->Add(entity, T{args...});

				entityid_map[entity] |= (1 << GetComponentArray<T>()->type);

				EntityChanged(entity);
			}

			void EntityChanged(Entity entity) {
				ID id = entityid_map[entity];

				for (auto& pair: system_map) {
					if ((pair.first & id) == pair.first) {
						pair.second.insert(entity);
					} else {
						pair.second.erase(entity);
					}
				}
			}

			template <typename T>
			T& Get(Entity entity) {
				assert(componentarray_map.find(typeid(T).hash_code()) != componentarray_map.end());

				return GetComponentArray<T>()->Get(entity);
			}

			template <typename ...Tc>
			std::set<Entity>& View() {
				ID system = 0;
				for (auto type_id: {typeid(Tc).hash_code()...}) {
					system |= (1 << RegisterComponentType(type_id));
				}

				if (system_map.find(system) == system_map.end()) {
					system_map[system] = std::set<Entity>();
					for (auto& pair: entityid_map) {
						EntityChanged(pair.first);
					}
				}

				return system_map[system];
			}

			template <typename T>
			T& Store() {
				if (t)
			}

		private:
			ComponentType RegisterComponentType(std::size_t type_id) {
				if (componenttype_map.find(type_id) == componenttype_map.end()) {
					assert(next_component_type < 32);

					componenttype_map[type_id] = next_component_type++;
				}

				return componenttype_map[type_id];
			}

			template <typename T>
			std::shared_ptr<ComponentArray<T>> GetComponentArray() {
				if (componentarray_map.find(typeid(T).hash_code()) == componentarray_map.end()) {
					auto ptr = std::make_shared<ComponentArray<T>>();
					ptr->type = RegisterComponentType(typeid(T).hash_code());
					componentarray_map[typeid(T).hash_code()] = std::static_pointer_cast<ComponentArrayT>(ptr);
				}

				return std::static_pointer_cast<ComponentArray<T>>(componentarray_map[typeid(T).hash_code()]);
			}
		};
	}
}