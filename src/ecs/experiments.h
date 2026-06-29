#pragma once

#include "ecsBase.h"
#include "ecsSimple.h"
#include "components.h"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <ranges>
#include <string>


const float chanceOfGettingComponentType = 0.5f;
const float chanceOfRemovalVsAddition = 0.5f;

struct TimeReport {
	double CreateEntities;
	double PopulateComponents;
	double QuerySingle;
	double QueryPair;
	double QueryComplex;
	double QuerySingleMessy;
	double QueryPairMessy;
	double QueryComplexMessy;
	int64_t QuerySingleResultsFound;
	int64_t QueryPairResultsFound;
	int64_t QueryComplexResultsFound;
	int64_t QuerySingleResultsFoundMessy;
	int64_t QueryPairResultsFoundMessy;
	int64_t QueryComplexResultsFoundMessy;


	TimeReport operator + (const TimeReport& other) const {
		return TimeReport{
			CreateEntities + other.CreateEntities,
			PopulateComponents + other.PopulateComponents,
			QuerySingle + other.QuerySingle,
			QueryPair + other.QueryPair,
			QueryComplex + other.QueryComplex,
			QuerySingleMessy + other.QuerySingleMessy,
			QueryPairMessy + other.QueryPairMessy,
			QueryComplexMessy + other.QueryComplexMessy,
			QuerySingleResultsFound + other.QuerySingleResultsFound,
			QueryPairResultsFound + other.QueryPairResultsFound,
			QueryComplexResultsFound + other.QueryComplexResultsFound,
			QuerySingleResultsFoundMessy + other.QuerySingleResultsFoundMessy,
			QueryPairResultsFoundMessy + other.QueryPairResultsFoundMessy,
			QueryComplexResultsFoundMessy + other.QueryComplexResultsFoundMessy
		};
	};

	TimeReport operator / (int denominator) const {
		return TimeReport{
			CreateEntities / denominator,
			PopulateComponents / denominator,
			QuerySingle / denominator,
			QueryPair / denominator,
			QueryComplex / denominator,
			QuerySingleMessy / denominator,
			QueryPairMessy / denominator,
			QueryComplexMessy / denominator,
			QuerySingleResultsFound / denominator,
			QueryPairResultsFound / denominator,
			QueryComplexResultsFound / denominator,
			QuerySingleResultsFoundMessy / denominator,
			QueryPairResultsFoundMessy / denominator,
			QueryComplexResultsFoundMessy / denominator
		};
	};

	TimeReport& operator += (const TimeReport& other) {
		*this = *this + other;
		return *this;
	};
	TimeReport& operator /= (int denominator) {
		*this = *this / denominator;
		return *this;
	};

	void Print() {
		std::cout << "Elapsed wall-clock (CreateEntities): " << this->CreateEntities << " us \n";
		std::cout << "Elapsed wall-clock (PopulateComponents): " << this->PopulateComponents << " us \n";
		std::cout << "Elapsed wall-clock (QuerySingle): " << this->QuerySingle << " us \n";
		std::cout << "Elapsed wall-clock (QueryPair): " << this->QueryPair << " us \n";
		std::cout << "Elapsed wall-clock (QueryComplex): " << this->QueryComplex << " us \n";
		std::cout << "Elapsed wall-clock (QuerySingleMessy): " << this->QuerySingleMessy << " us \n";
		std::cout << "Elapsed wall-clock (QueryPairMessy): " << this->QueryPairMessy << " us \n";
		std::cout << "Elapsed wall-clock (QueryComplexMessy): " << this->QueryComplexMessy << " us \n";
		std::cout << "Elapsed wall-clock (AggregatedSummed): " << this->CreateEntities + this->PopulateComponents + this->QuerySingle + this->QueryPair + this->QueryComplex + this->QuerySingleMessy + this->QueryPairMessy + this->QueryComplexMessy << " us \n";
		std::cout << "Number of QuerySingle Results found: " << this->QuerySingleResultsFound << " \n";
		std::cout << "Number of QueryPair Results found: " << this->QueryPairResultsFound << " \n";
		std::cout << "Number of QueryComplex Results found: " << this->QueryComplexResultsFound << " \n";
		std::cout << "Number of QuerySingleMessy Results found: " << this->QuerySingleResultsFoundMessy << " \n";
		std::cout << "Number of QueryPairMessy Results found: " << this->QueryPairResultsFoundMessy << " \n";
		std::cout << "Number of QueryComplexMessy Results found: " << this->QueryComplexResultsFoundMessy << " \n";
		std::cout << "------------------------------------------------------------------- \n";
	}

};



template <typename TWorld>
	requires WorldLike<
		TWorld,
			IntComponent,
			FloatComponent,
			DoubleComponent,
			TextComponent,
			BooleanComponent,
			MixedComponentA,
			MixedComponentB,
			MixedComponentC,
			MixedComponentD,
			MixedComponentE>
void AddRandomComponents(TWorld& world, Entity entity) {
	const float randomChanceInt = RandomFloat(0, 1);
	if (randomChanceInt >= chanceOfGettingComponentType) {
		IntComponent intComponent = CreateRandomIntComponent();
		world.AddComponent(entity, intComponent);
	}

	const float randomChanceFloat = RandomFloat(0, 1);
	if (randomChanceFloat >= chanceOfGettingComponentType) {
		FloatComponent floatComponent = CreateRandomFloatComponent();
		world.AddComponent(entity, floatComponent);
	}

	const float randomChanceDouble = RandomFloat(0, 1);
	if (randomChanceDouble >= chanceOfGettingComponentType) {
		DoubleComponent doubleComponent = CreateRandomDoubleComponent();
		world.AddComponent(entity, doubleComponent);
	}

	const float randomChanceText = RandomFloat(0, 1);
	if (randomChanceText >= chanceOfGettingComponentType) {
		TextComponent textComponent = CreateRandomTextComponent();
		world.AddComponent(entity, textComponent);
	}

	const float randomChanceBoolean = RandomFloat(0, 1);
	if (randomChanceBoolean >= chanceOfGettingComponentType) {
		BooleanComponent booleanComponent = CreateRandomBooleanComponent();
		world.AddComponent(entity, booleanComponent);
	}

	const float randomChanceMixedA = RandomFloat(0, 1);
	if (randomChanceMixedA >= chanceOfGettingComponentType) {
		MixedComponentA mixedComponent = CreateRandomMixedComponentA();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedB = RandomFloat(0, 1);
	if (randomChanceMixedB >= chanceOfGettingComponentType) {
		MixedComponentB mixedComponent = CreateRandomMixedComponentB();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedC = RandomFloat(0, 1);
	if (randomChanceMixedC >= chanceOfGettingComponentType) {
		MixedComponentC mixedComponent = CreateRandomMixedComponentC();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedD = RandomFloat(0, 1);
	if (randomChanceMixedD >= chanceOfGettingComponentType) {
		MixedComponentD mixedComponent = CreateRandomMixedComponentD();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedE = RandomFloat(0, 1);
	if (randomChanceMixedE >= chanceOfGettingComponentType) {
		MixedComponentE mixedComponent = CreateRandomMixedComponentE();
		world.AddComponent(entity, mixedComponent);
	}
}

template <typename TWorld>
	requires WorldLike<TWorld>
const Entity GetRandomEntity(TWorld& world) {
	const auto entities = world.GetEntities();
	const size_t entityLength = static_cast<size_t>(std::ranges::distance(entities));
	const size_t randomIndex = RandomIndex(0, entityLength - 1);
	return entities[randomIndex];
}

template <typename TWorld>
	requires WorldLike<
		TWorld,
			IntComponent,
			FloatComponent,
			DoubleComponent,
			TextComponent,
			BooleanComponent,
			MixedComponentA,
			MixedComponentB,
			MixedComponentC,
			MixedComponentD,
			MixedComponentE>
TimeReport RunExperiments(TWorld& world, const int numberOfEntities) {

	//Create Entities

	const auto startTimeCreateEntities = std::chrono::steady_clock::now();

	for (int64_t i = 0; i < numberOfEntities; i++) {
		Entity e = world.CreateEntity();
	}

	const auto endTimeCreateEntities = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffCreateEntities = endTimeCreateEntities - startTimeCreateEntities;

	// Populate Components

	auto entityList = world.GetEntities();
	std::shuffle(entityList.begin(), entityList.end(), GetRandomGenerator());

	const auto startTimePopulateComponents = std::chrono::steady_clock::now();

	for (Entity& entity : entityList) {
		AddRandomComponents(world, entity);
	}

	const auto endTimePopulateComponents = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffPopulateComponents = endTimePopulateComponents - startTimePopulateComponents;

	// Query: single component
	const auto startTimeQuerySingle = std::chrono::steady_clock::now();
	auto resultsSingle = world.template Query<IntComponent>();
	const auto endTimeQuerySingle = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffQuerySingle = endTimeQuerySingle - startTimeQuerySingle;

	// Query: pair
	const auto startTimeQueryPair = std::chrono::steady_clock::now();
	auto resultsPair = world.template Query<IntComponent, FloatComponent>();
	const auto endTimeQueryPair = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffQueryPair = endTimeQueryPair - startTimeQueryPair;

	// Query: complex
	const auto startTimeQueryComplex = std::chrono::steady_clock::now();
	auto resultsComplex = world.template Query<IntComponent, DoubleComponent, MixedComponentA, MixedComponentE, BooleanComponent>();
	const auto endTimeQueryComplex = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffQueryComplex = endTimeQueryComplex - startTimeQueryComplex;

	const int numberOfRemovalsOrAdditions = numberOfEntities;
	for (int i = 0; i < numberOfRemovalsOrAdditions; i++) {
		const float randomChanceRemoveEntity = RandomFloat(0, 1);
		if (randomChanceRemoveEntity >= chanceOfRemovalVsAddition) {
			const auto randomEntity = GetRandomEntity(world);
			world.RemoveEntity(randomEntity);
		}
		else {
			const auto newEntity = world.CreateEntity();
			AddRandomComponents(world, newEntity);
		}
	}

	// Query: single component
	const auto startTimeQuerySingleMessy = std::chrono::steady_clock::now();
	auto resultsSingleMessy = world.template Query<IntComponent>();
	const auto endTimeQuerySingleMessy = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffQuerySingleMessy = endTimeQuerySingleMessy - startTimeQuerySingleMessy;

	// Query: pair
	const auto startTimeQueryPairMessy = std::chrono::steady_clock::now();
	auto resultsPairMessy = world.template Query<IntComponent, FloatComponent>();
	const auto endTimeQueryPairMessy = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffQueryPairMessy = endTimeQueryPairMessy - startTimeQueryPairMessy;

	// Query: complex
	const auto startTimeQueryComplexMessy = std::chrono::steady_clock::now();
	auto resultsComplexMessy = world.template Query<IntComponent, DoubleComponent, MixedComponentA, MixedComponentE, BooleanComponent>();
	const auto endTimeQueryComplexMessy = std::chrono::steady_clock::now();
	const std::chrono::duration<double, std::micro> diffQueryComplexMessy = endTimeQueryComplexMessy - startTimeQueryComplexMessy;


	return TimeReport{
		diffCreateEntities.count(),
		diffPopulateComponents.count(),
		diffQuerySingle.count(),
		diffQueryPair.count(),
		diffQueryComplex.count(),
		diffQuerySingleMessy.count(),
		diffQueryPairMessy.count(),
		diffQueryComplexMessy.count(),
		std::ranges::distance(resultsSingle),
		std::ranges::distance(resultsPair),
		std::ranges::distance(resultsComplex),
		std::ranges::distance(resultsSingleMessy),
		std::ranges::distance(resultsPairMessy),
		std::ranges::distance(resultsComplexMessy)
	};

}