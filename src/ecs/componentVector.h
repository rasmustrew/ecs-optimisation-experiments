#pragma once

#include <vector>
#include <tuple>
#include "components.h"

template<ComponentType T>
class ComponentVector {
	std::vector<T> Components{};
	std::vector<size_t> FreeList{};
	std::vector<size_t> EntityToIndex{};
public:

	ComponentVector() {
		EntityToIndex.resize(100);
	}

	void AddComponent(Entity entity, T component) {
		if (FreeList.size() != 0) {
			size_t freeIndex = FreeList.back();
			FreeList.pop_back();
			Components[freeIndex] = component;
			EntityToIndex[entity.Id] = freeIndex;
		}
		else {
			Components.push_back(component);
			if (EntityToIndex.size() <= entity.Id) {
				EntityToIndex.resize(entity.Id * 2);
			}
			EntityToIndex[entity.Id] = Components.size() - 1;
		}
	};

	// We dont need to remove the component itself, or edit the entities list,
	// we just need to make the space available for the next user. 
	void RemoveComponent(Entity entity) {
		size_t index = EntityToIndex[entity.Id];
		FreeList.push_back(index);
	}

	T& GetComponent(Entity entity) {
		size_t index = EntityToIndex[entity.Id];
		return Components[index];
	}
};

template<typename List>
class ComponentStorage;

template<typename... Ts>
class ComponentStorage<TypeList<Ts...>> {
private:
	std::tuple<ComponentVector<Ts>...> Storage;
public:
	template <ComponentType T>
	ComponentVector<T>& GetComponentVector() {
		return std::get<ComponentVector<T>>(Storage);
	}
};