#pragma once

#include <concepts>
#include <cstdint>
#include <functional>
#include <ranges>
#include <type_traits>

/*
requires: specifying the shape of T, what fields must exist, what functions, return type etc.
traits: (std::is_aggregate_v) for specifying properties T must have, be copyable, be aggregate
concept: where you combine traits and requires to specify the full requirements for T
*/

using EntityId = std::uint64_t;

struct Entity {
	EntityId Id{};

	// Implement equality operator via id, needed for map to find exact match in hash bucket
	bool operator == (const Entity& other) const {
		return Id == other.Id;
	};
};


// Implement hash specialisation for Entity so it can be used in maps etc. 
template<>
struct std::hash<Entity> {
	std::size_t operator()(const Entity& entity) const noexcept {
		return std::hash<EntityId>{}(entity.Id);
	}
};

// For T to be a ComponentType it: Must be a standard data holding struct
template<typename T>
concept ComponentType = std::is_aggregate_v<T> && std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>;

// Must be a range over entities
template<typename TRange>
concept EntityRange =
std::ranges::input_range<TRange> &&
std::convertible_to<std::ranges::range_value_t<TRange>, Entity>;

// Type alias for QueryRow as a tuple over an entity and some components
template<ComponentType... Ts>
using QueryRow = std::tuple<Entity, Ts&...>;

// Must be a range over query rows
// When defining a concept, the template parameters must be unconstrained in <>
template<typename TRange, typename... Ts>
concept QueryRange =
(ComponentType<Ts> && ...) &&
std::ranges::input_range<TRange> &&
std::same_as<std::ranges::range_value_t<TRange>, QueryRow<Ts...>>;


// TComponent must be a ComponentType
// In the context of the && ... TComponents refers to a single component 
// as TComponents is unpacked by the ...
// Must implement certain functions
template<typename TWorld, typename... TComponents>
concept WorldLike = (ComponentType<TComponents> && ...)
&& requires(TWorld world, Entity entity) {
	{ world.CreateEntity() } -> std::same_as<Entity>;
	{ world.RemoveEntity(entity) } -> std::same_as<void>;
	{ world.GetEntities() } -> EntityRange;
	{ world.template GetEntitiesWithComponents<TComponents...>() } -> EntityRange;
	{ world.template Query<TComponents...>() } -> QueryRange<TComponents...>;

}
&& (requires(TWorld world, Entity entity, TComponents component) {
	{ world.AddComponent(entity, component) } -> std::same_as<void>;
	{ world.template RemoveComponent<TComponents>(entity) } -> std::same_as<void>;
	{ world.template GetComponent<TComponents>(entity) } -> std::same_as<TComponents&>;
} && ...);


