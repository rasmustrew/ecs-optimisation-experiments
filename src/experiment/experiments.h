#pragma once

#include "../ecs/ecsBase.h"
#include "../ecs/ecsSimple.h"
#include "../ecs/components.h"
#include "timeReport.h"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <ranges>
#include <string>

template <typename TWorld>
concept ExperimentWorldLike =
WorldLike<
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
	MixedComponentE>&& std::default_initializable<TWorld>;



template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
std::vector<TimeReport> RunExperiments(int iterationsReal, int iterationsWarmup, int entityAmountWarmup, const std::vector<int> entityAmountSet);

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
void RunWarmup(int iterations, int entityAmount);

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
void AddRandomComponents(TWorld& world, Entity entity);

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
const Entity GetRandomEntity(TWorld& world);

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
TimeReport RunExperimentRound(TWorld& world, const int entityAmount);

#include "experiments.tpp"