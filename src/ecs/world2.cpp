#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <typeindex>
#include <cstdint>
#include <memory>
#include <string>

#include "ecsBase.h"

class IStore {
};


template<ComponentType T>
class Store : IStore {
private:
	std::vector<T> Components;
	// map from the entity to the index into Components
	std::unordered_map<Entity, size_t> EntityToIndex;

public:
	void AddComponentToEntity(Entity entity, T component) {
		if (EntityToIndex.contains(entity)) {
			return; // Todo: return an indicator this happened? 
		}

		Components.push_back(component);
		EntityToIndex.emplace(entity, Components.size() - 1);
	}

	T GetComponentForEntity(Entity entity) {
		auto result = EntityToIndex.find(entity);
		if (result == EntityToIndex.end()) {
			throw std::string("Could not find Component: ") + typeid(T).name();
		}

		return Components[result->second];
	}

};

class World2 {
private:
	std::unordered_map<std::type_index, std::unique_ptr<IStore>> Stores;
	std::unordered_map<Entity, std::unordered_set<std::type_index>> Entities;
	uint64_t IdCounter{};
public:

	template<ComponentType T>
	Store<T>* GetStore() {
		std::type_index key = std::type_index(typeid(T));
		auto result = Stores.find(key);
		if (result == Stores.end()) {
			result = Stores.emplace(key, std::make_unique<Store<T>>());
		}

		return *static_cast<Store<T>*>(result->second.get());
	}

	Entity CreateEntity() {
		return Entity{ IdCounter++ };
	}

	template<ComponentType... Ts>
	std::vector<Entity> GetEntitiesWithComponents() {
		std::vector<Entity> entities{};

		for (auto& [entity, componentList] : Entities) {
			if ((componentList.contains(std::type_index(typeid(Ts))) && ...)) {
				entities.push_back(entity);
			}
		}
		return entities;
	}

	template<ComponentType T>
	void AddComponent(Entity entity, T component) {
		auto store = GetStore<T>();
		store->AddComponentToEntity(component);
	}

};


