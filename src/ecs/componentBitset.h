#pragma once
#include <bitset>
#include "components.h"







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