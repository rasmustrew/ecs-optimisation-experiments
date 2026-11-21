#pragma once
#include "../ecsBase.h"
#include "../components.h"

#include <unordered_map>
#include <typeindex>
#include <any>
#include <tuple>
#include <vector>
#include <string>

#if USE_AMD_UPROF
#include <AMDProfileController.h>
#endif


using ComponentsMap = std::unordered_map<std::type_index, std::any>;

/*
Assumption: Only 1 of any Component Type is added to an Entity
n = number of entities
t = number of component types
*/
class World1 {
private:
	std::vector<std::tuple<Entity, ComponentsMap>> Entities{};
	EntityId CurrentId{};

public:
	/*
	Expected Runtime: O(1) (Amortized)
	Worst Case Runtime: O(n) (if resizing Entities)
	*/
	Entity CreateEntity() {
		Entity entity{ CurrentId++ };
		Entities.push_back(std::tuple{ entity, ComponentsMap{} });
		return entity;
	}

	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n)
	*/
	void RemoveEntity(Entity entity) {
		std::erase_if(Entities, [entity](auto& tuple) {
			return std::get<Entity>(tuple) == entity;
		});
	}


	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n + t) (if resizing componentsMap)
	*/
	template<ComponentType T>
	void AddComponent(Entity entity, T component) {
		for (auto& [innerEntity, componentsMap] : Entities) {
			if (innerEntity == entity) {
				componentsMap.emplace(std::type_index(typeid(T)), component);
				return;
			}
		}
	}

	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n + t) (if resizing componentsMap)
	*/
	template<ComponentType T>
	void RemoveComponent(Entity entity) {
		for (auto& [innerEntity, componentsMap] : Entities) {
			if (innerEntity == entity) {
				componentsMap.erase(std::type_index(typeid(T)));
				return;
			}
		}
	}

	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n + t) (if hash collisions)
	*/
	template<ComponentType T>
	T& GetComponent(Entity entity) {
		for (auto& [innerEntity, componentsMap] : Entities) {
			if (innerEntity == entity) {
				auto resultComponents = componentsMap.find(std::type_index(typeid(T)));
				std::any& component = resultComponents->second;
				return std::any_cast<T&>(component);
			}
		}
	}

	/*
	Runtime: O(n)
	*/
	std::vector<Entity> GetEntities() {
		std::vector<Entity> entities{};
		for (auto& [entity, componentsMap] : Entities) {
			entities.push_back(entity);
		}
		return entities;
	}

	/*
	Expected Runtime: O(n * t)
	Worst Case Runtime: O(n * t^2) (if hash collisions)
	*/
	template<ComponentType... Ts>
	std::vector<Entity> GetEntitiesWithComponents() {
		std::vector<Entity> entities{};

		for (auto& [entity, componentsMap] : Entities) {
			if ((componentsMap.contains(std::type_index(typeid(Ts))) && ...)) {
				entities.push_back(entity);
			}
		}
		return entities;
	}

	/*
	Expected Runtime: O(n * t)
	Worst Case Runtime: O(n * t^2) (if hash collisions)
	*/
	template<ComponentType... Ts>
	std::vector<std::tuple<Entity, Ts&...>> Query() {

#if USE_AMD_UPROF
		amdProfileStrictResume();
#endif

		std::vector<std::tuple<Entity, Ts&...>> results{};

		for (auto& [entity, componentsMap] : Entities) {

			const bool hasAll =
				(componentsMap.contains(std::type_index(typeid(Ts))) && ...);

			if (!hasAll) {
				continue;
			}

			std::tuple<Entity, Ts&...> row(entity, std::any_cast<Ts&>(componentsMap.at(std::type_index(typeid(Ts))))...);
			results.emplace_back(row);
		}

#if USE_AMD_UPROF
		amdProfileStrictPause();
#endif

		return results;
	}
};




