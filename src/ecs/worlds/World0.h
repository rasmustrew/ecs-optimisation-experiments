#pragma once
#include "../ecsBase.h"
#include "../components.h"

#include <unordered_map>
#include <typeindex>
#include <any>
#include <tuple>
#include <vector>
#include <string>
#include <variant>
#include <ranges>
#include <algorithm>

template<typename T>
bool VariantHoldsComponentType(Component component) {
	return std::holds_alternative<T>(component);
}

using ComponentsList = std::vector<Component>;

/*
Naive ECS implementation, used only to compare other implementations against.
Assumption: Only 1 of any Component Type is added to an Entity
n = number of entities
t = number of component types
*/
class World0 {
private:
	std::vector<std::tuple<Entity, ComponentsList>> Entities{};
	EntityId CurrentId{};

public:
	/*
	Expected Runtime: O(1) (Amortized)
	Worst Case Runtime: O(n) (if resizing Entities)
	*/
	Entity CreateEntity() {
		Entity entity{ CurrentId++ };
		Entities.push_back(std::tuple{ entity, ComponentsList{} });
		return entity;
	}

	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n) (if resizing Entities)
	*/
	void RemoveEntity(Entity entity) {
		std::erase_if(Entities, [entity](auto& tuple) {
			return std::get<Entity>(tuple) == entity;
		});
	}


	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n + t) (if resizing componentsList)
	*/
	template<ComponentType T>
	void AddComponent(Entity entity, T component) {
		for (auto& [innerEntity, componentsList] : Entities) {
			if (innerEntity == entity) {
				componentsList.push_back(component);
				return;
			}
		}
	}

	/*
	Expected Runtime: O(n)
	Worst Case Runtime: O(n + t) (if resizing componentsList)
	*/
	template<ComponentType T>
	void RemoveComponent(Entity entity) {
		for (auto& [innerEntity, componentsList] : Entities) {
			if (innerEntity == entity) {
				std::erase_if(componentsList, [](Component& component) {
					return std::holds_alternative<T>(component);
				});
				return;
			}
		}
		throw std::string("Could not find Component of type:") + std::to_string(typeid(T).name()) + std::string(" on Entity: ") + std::to_string(entity.Id);
	}

	/*
	Expected Runtime: O(n + t)
	Worst Case Runtime: O(n + t)
	*/
	template<ComponentType T>
	T& GetComponent(Entity entity) {
		for (auto& [innerEntity, componentsList] : Entities) {
			if (innerEntity == entity) {
				for (Component& component : componentsList) {
					if (std::holds_alternative<T>(component)) {
						return component;
					}
				}
			}
		}
		throw std::string("Could not find Component of type:") + std::to_string(typeid(T).name()) + std::string(" on Entity: ") + std::to_string(entity.Id);
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
	Expected Runtime: O(n * t^2)
	Worst Case Runtime: O(n * t^2)
	*/
	template<ComponentType... Ts>
	std::vector<Entity> GetEntitiesWithComponents() {
		std::vector<Entity> entities{};

		for (auto& [entity, componentList] : Entities) {
			if (((std::any_of(componentList.begin(), componentList.end(),
							  [](const Component& component) {
								  return std::holds_alternative<Ts>(component);
							  })) && ...)) {
				entities.push_back(entity);
			}

		}
		return entities;
	}

	/*
	Expected Runtime: O(n * t^2)
	Worst Case Runtime: O(n * t^2)
	*/
	template<ComponentType... Ts>
	std::vector<std::tuple<Entity, Ts&...>> Query() {
		std::vector<std::tuple<Entity, Ts&...>> results{};

		for (auto& [entity, componentList] : Entities) {


			const bool hasAll = (std::any_of(componentList.begin(), componentList.end(), VariantHoldsComponentType<Ts>) && ...);

			if (!hasAll) {
				continue;
			}

			std::tuple<Entity, Ts&...> row(entity, std::get<Ts>(std::find_if(componentList.begin(), componentList.end(), VariantHoldsComponentType<Ts>)[0])...);
			results.emplace_back(row);
		}
		return results;
	}
};




