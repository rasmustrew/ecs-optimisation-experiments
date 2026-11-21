
inline constexpr float CHANCE_OF_GETTING_COMPONENT_TYPE = 0.5f;
inline constexpr float CHANCE_OF_REMOVAL_VS_ADDITION = 0.5f;

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
std::vector<TimeReport> RunExperiments(int iterationsWarmup, int iterationsReal, int entityAmountWarmup, const std::vector<int> entityAmountSet) {
	std::vector<TimeReport> timeReportSet{};
	for (const int& numberOfEntities : entityAmountSet) {
		ReseedRandomGenerator();
		RunWarmup<TWorld>(iterationsWarmup, entityAmountWarmup);

		std::cout << "ENTITIES: " << numberOfEntities << "\n";
		ReseedRandomGenerator();
		const auto startTime = std::chrono::steady_clock::now();
		TimeReport aggregatedTimeReport{};

		for (int i = 0; i < iterationsReal; i++) {
			auto world = TWorld{};
			aggregatedTimeReport += RunExperimentRound(world, numberOfEntities);
		}

		const auto endTime = std::chrono::steady_clock::now();
		const std::chrono::duration<double, std::micro> elapsedTime = endTime - startTime;
		std::cout << "TOTAL: \n";
		aggregatedTimeReport.Print();

		aggregatedTimeReport /= iterationsReal;
		std::cout << "PER RUN: \n";
		aggregatedTimeReport.Print();

		timeReportSet.push_back(aggregatedTimeReport);

		std::cout << "Elapsed wall-clock (Full): " << elapsedTime.count() << " us \n";
	}
	return timeReportSet;
}

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
void RunWarmup(int iterations, int entityAmount) {
	for (int i = 0; i < iterations; i++) {
		auto world = TWorld{};
		RunExperimentRound(world, entityAmount);
	}
}

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
void AddRandomComponents(TWorld& world, Entity entity) {
	const float randomChanceInt = RandomFloat(0, 1);
	if (randomChanceInt >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		IntComponent intComponent = CreateRandomIntComponent();
		world.AddComponent(entity, intComponent);
	}

	const float randomChanceFloat = RandomFloat(0, 1);
	if (randomChanceFloat >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		FloatComponent floatComponent = CreateRandomFloatComponent();
		world.AddComponent(entity, floatComponent);
	}

	const float randomChanceDouble = RandomFloat(0, 1);
	if (randomChanceDouble >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		DoubleComponent doubleComponent = CreateRandomDoubleComponent();
		world.AddComponent(entity, doubleComponent);
	}

	const float randomChanceText = RandomFloat(0, 1);
	if (randomChanceText >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		TextComponent textComponent = CreateRandomTextComponent();
		world.AddComponent(entity, textComponent);
	}

	const float randomChanceBoolean = RandomFloat(0, 1);
	if (randomChanceBoolean >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		BooleanComponent booleanComponent = CreateRandomBooleanComponent();
		world.AddComponent(entity, booleanComponent);
	}

	const float randomChanceMixedA = RandomFloat(0, 1);
	if (randomChanceMixedA >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		MixedComponentA mixedComponent = CreateRandomMixedComponentA();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedB = RandomFloat(0, 1);
	if (randomChanceMixedB >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		MixedComponentB mixedComponent = CreateRandomMixedComponentB();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedC = RandomFloat(0, 1);
	if (randomChanceMixedC >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		MixedComponentC mixedComponent = CreateRandomMixedComponentC();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedD = RandomFloat(0, 1);
	if (randomChanceMixedD >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		MixedComponentD mixedComponent = CreateRandomMixedComponentD();
		world.AddComponent(entity, mixedComponent);
	}

	const float randomChanceMixedE = RandomFloat(0, 1);
	if (randomChanceMixedE >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		MixedComponentE mixedComponent = CreateRandomMixedComponentE();
		world.AddComponent(entity, mixedComponent);
	}
}

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
void AddRandomLargeComponents(TWorld& world, Entity entity) {
	const float randomChanceLargeNumeric = RandomFloat(0, 1);
	if (randomChanceLargeNumeric >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		LargeNumericComponent component{
			CreateRandomIntComponent(),
			CreateRandomFloatComponent(),
			CreateRandomDoubleComponent(),
			CreateRandomIntArrayComponent(),
			CreateRandomFloatArrayComponent(),
			CreateRandomInt64Array<128>(),
			CreateRandomFloatArray<128>(),
			CreateRandomDoubleArray<128>()
		};
		world.AddComponent(entity, component);
	}

	const float randomChanceLargeMixedA = RandomFloat(0, 1);
	if (randomChanceLargeMixedA >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		LargeMixedComponentA component{
			CreateRandomMixedComponentA(),
			CreateRandomMixedComponentB(),
			CreateRandomMixedComponentC(),
			CreateRandomTextComponent(),
			CreateRandomBooleanComponent(),
			RandomString<96>(),
			RandomString<128>(),
			CreateRandomIntArray<64>(),
			CreateRandomFloatArray<64>(),
			CreateRandomDoubleArray<64>()
		};
		world.AddComponent(entity, component);
	}

	const float randomChanceLargeMixedB = RandomFloat(0, 1);
	if (randomChanceLargeMixedB >= CHANCE_OF_GETTING_COMPONENT_TYPE) {
		LargeMixedComponentB component{
			CreateRandomMixedComponentD(),
			CreateRandomMixedComponentE(),
			CreateRandomIntArrayComponent(),
			CreateRandomFloatArrayComponent(),
			CreateRandomIntComponent(),
			CreateRandomFloatComponent(),
			CreateRandomDoubleComponent(),
			RandomString<72>(),
			RandomString<104>(),
			CreateRandomByteArray<128>()
		};
		world.AddComponent(entity, component);
	}
}

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
const Entity GetRandomEntity(TWorld& world) {
	const auto entities = world.GetEntities();
	const size_t entityLength = static_cast<size_t>(std::ranges::distance(entities));
	const size_t randomIndex = RandomIndex(0, entityLength - 1);
	return entities[randomIndex];
}

template <typename TWorld>
	requires ExperimentWorldLike<TWorld>
TimeReport RunExperimentRound(TWorld& world, const int numberOfEntities) {

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
		if (randomChanceRemoveEntity >= CHANCE_OF_REMOVAL_VS_ADDITION) {
			const auto randomEntity = GetRandomEntity(world);
			world.RemoveEntity(randomEntity);
		}
		else {
			const auto newEntity = world.CreateEntity();
			AddRandomComponents(world, newEntity);
			AddRandomLargeComponents(world, newEntity);
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