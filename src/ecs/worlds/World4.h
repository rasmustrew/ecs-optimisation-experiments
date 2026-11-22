#pragma once
#include "../ecsBase.h"
#include "../components.h"
#include "../componentBitset.h"
#include "../componentVector.h"

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <any>
#include <tuple>
#include <vector>
#include <string>

#if USE_AMD_UPROF
#include <AMDProfileController.h>
#endif


/*
Assumption: Only 1 of any Component Type is added to an Entity
n = number of entities
t = number of component types
*/
class World4 {
private:
	std::vector <std::tuple<Entity, ComponentBitset>> Entities{};
	ComponentStorage<AllComponents> Storage;
	EntityId CurrentId{};

public:
	/*
	Expected Runtime: O(1) (Amortized)
	Worst Case Runtime: O(n) (if resizing Entities)
	*/
	Entity CreateEntity() {
		Entity entity{ CurrentId++ };
		Entities.push_back(std::tuple{ entity, ComponentBitset{} });
		return entity;
	}

	/*
	* Deliberately not removing entity from each ComponentVector,
	as it will make no difference to results or query perfomance
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
		for (auto& [innerEntity, componentTypes] : Entities) {
			if (innerEntity == entity) {
				componentTypes.AddComponent<T>();
				ComponentVector<T>& components = Storage.GetComponentVector<T>();
				components.AddComponent(entity, component);
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
		for (auto& [innerEntity, componentTypes] : Entities) {
			if (innerEntity == entity) {
				componentTypes.RemoveComponent<T>();
				ComponentVector<T> components = Storage.GetComponentVector<T>();
				components.RemoveComponent(entity);
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
		ComponentVector<T> components = Storage.GetComponentVector<T>();
		components.GetComponent(entity);
	}

	/*
	Runtime: O(n)
	*/
	std::vector<Entity> GetEntities() {
		std::vector<Entity> entities{};
		for (auto& [entity, componentTypes] : Entities) {
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

		for (auto& [entity, componentTypes] : Entities) {
			if (componentTypes.Contains<Ts...>()) {
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

		std::vector<Entity> entities = this->GetEntitiesWithComponents<Ts...>();

		std::vector<std::tuple<Entity, Ts&...>> results{};

		for (Entity entity : entities) {

			std::tuple<Entity, Ts&...> row(entity, Storage.GetComponentVector<Ts>().GetComponent(entity)...);
			results.push_back(row);
		}

		return results;

#if USE_AMD_UPROF
		amdProfileStrictPause();
#endif


	}
};




