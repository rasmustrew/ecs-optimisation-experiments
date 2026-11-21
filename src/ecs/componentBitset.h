#pragma once
#include <bitset>


template <typename... Ts>
struct TypeList {};

using AllComponents = TypeList<
	IntComponent, FloatComponent, DoubleComponent,
	TextComponent, BooleanComponent,
	MixedComponentA, MixedComponentB, MixedComponentC, MixedComponentD, MixedComponentE,
	LargeNumericComponent, LargeMixedComponentA, LargeMixedComponentB
>;

// Compile time recursive definition of IndexOf
template <typename T, typename List>
struct IndexOf;

template <typename T, typename... Ts>
struct IndexOf<T, TypeList<T, Ts...>> : std::integral_constant<std::size_t, 0> {};

template <typename T, typename U, typename... Ts>
struct IndexOf<T, TypeList<U, Ts...>> : std::integral_constant<std::size_t, 1 + IndexOf<T, TypeList<Ts...>>::value> {};

template <typename T>
inline constexpr std::size_t COMPONENT_INDEX = IndexOf<T, AllComponents>::value;


class ComponentBitset {
	std::bitset<20> InternalBitset{};

public:
	template<ComponentType T>
	void AddComponent() {
		size_t componentIndex = COMPONENT_INDEX<T>;
		InternalBitset.set(componentIndex, true);
	}

	template<ComponentType T>
	void RemoveComponent() {
		size_t componentIndex = COMPONENT_INDEX<T>;
		InternalBitset.set(componentIndex, false);
	}

	template <ComponentType... Ts>
	bool Contains() {
		std::bitset<20> queryBitset{};
		(queryBitset.set(COMPONENT_INDEX<Ts>, true), ...);
		return queryBitset == (queryBitset & InternalBitset);
	}
};