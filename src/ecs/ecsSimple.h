#pragma once
#include "ecsBase.h"
#include "components.h"

#include <unordered_map>
#include <typeindex>
#include <any>
#include <tuple>
#include <vector>
#include <string>

using ComponentsMap = std::unordered_map<std::type_index, std::any>;

/*
Naive ECS implementation, used only to compare other implementations against
Assumption: Only 1 of any Component Type is added to an Entity
n = number of entities
t = number of component types
*/
class WorldSimple {
private:
	std::unordered_map<Entity, ComponentsMap> Entities;
	EntityId CurrentId{};

public:
	/*
	Expected Runtime: O(1) (Amortized)
	Worst Case Runtime: O(n)
	*/
	Entity CreateEntity() {
		Entity entity{ CurrentId++ };
		Entities.emplace(entity, ComponentsMap{});
		return entity;
	}

	/*
	Expected Runtime: O(1)
	Worst Case Runtime: O(n)
	*/
	void RemoveEntity(Entity entity) {
		Entities.erase(entity);
	}


	/*
	Expected Runtime: O(1) + O(1)
	Worst Case Runtime: O(n) + O(t)
	*/
	template<ComponentType T>
	void AddComponent(Entity entity, T component) {
		auto result = Entities.find(entity);
		//if (result == Entities.end()) {
		//	throw std::string("Could not find Entity: ") + std::to_string(entity.Id);
		//}

		auto& componentsMap = result->second;
		componentsMap.emplace(std::type_index(typeid(T)), component);
	}

	/*
	Expected Runtime: O(1) + O(1)
	Worst Case Runtime: O(n) + O(t)
	*/
	template<ComponentType T>
	void RemoveComponent(Entity entity) {
		auto result = Entities.find(entity);
		//if (result == Entities.end()) {
		//	throw std::string("Could not find Entity: ") + std::to_string(entity.Id);
		//}

		auto& componentsMap = result->second;
		componentsMap.erase(std::type_index(typeid(T)));
	}

	/*
	Expected Runtime: O(1) + O(1)
	Worst Case Runtime: O(n) + O(t)
	*/
	template<ComponentType T>
	T& GetComponent(Entity entity) {
		auto result = Entities.find(entity);
		//if (result == Entities.end()) {
		//	throw std::string("Could not find Entity: ") + std::to_string(entity.Id);
		//}

		auto& componentsMap = result->second;
		auto resultComponents = componentsMap.find(std::type_index(typeid(T)));
		std::any& component = resultComponents->second;
		return std::any_cast<T&>(component);
	}

	/*
	Runtime: O(n)
	*/
	std::vector<Entity> GetEntities() {
		std::vector<Entity> entities{};
		for (auto& [entity, componentList] : Entities) {
			entities.push_back(entity);
		}
		return entities;
	}

	/*
	Expected Runtime: O(n) + O(t)
	Worst Case Runtime: O(n) + O(t^2)
	*/
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

	/*
	Expected Runtime: O(n) + O(t)
	Worst Case Runtime: O(n) + O(t^2)
	*/
	template<ComponentType... Ts>
	std::vector<std::tuple<Entity, Ts&...>> Query() {
		std::vector<std::tuple<Entity, Ts&...>> results{};

		for (auto& [entity, componentList] : Entities) {

			const bool hasAll =
				(componentList.contains(std::type_index(typeid(Ts))) && ...);

			if (!hasAll) {
				continue;
			}

			std::tuple<Entity, Ts&...> row(entity, std::any_cast<Ts&>(componentList.at(std::type_index(typeid(Ts))))...);
			results.emplace_back(row);
		}
		return results;
	}
};




