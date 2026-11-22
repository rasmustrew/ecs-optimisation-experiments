#include <ctime>
#include <variant>
#include <iostream>
#include <chrono>
#include <filesystem>
#include "experiment/experiments.h"
#include "ecs/worlds/world0.h"
#include "ecs/worlds/world1.h"
#include "ecs/worlds/world2.h"
#include "ecs/worlds/world3.h"
#include "ecs/worlds/world4.h"
#include "experiment/timeReport.h"
#include "experiment/worldNames.h"

#include "experiment/outputFormatting.h"

#if USE_AMD_UPROF
#include <AMDProfileController.h>
#endif
#include <keepAwake.h>


using World = std::variant<World0, World1, World2, World3, World4>;

const int iterationsWarmup = 100;
const int iterationsReal = 100;
const int numberOfEntitiesWarmup = 1000;
const std::vector<int> entitiesSet = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
const std::vector<World> worlds = { World3{}, World4{} };

int main(int, char**) {
	KeepAwakeScope keepAwake;

#if USE_AMD_UPROF
	amdProfileStrictPause();
#endif

	std::vector<std::vector<TimeReport>> timeReportSets{};
	std::vector<std::string_view> worldNames{};

	for (auto& w : worlds) {
		std::visit([&](auto& world) { // Get underlying type T from std::variant<T>
			using TWorld = std::remove_cvref_t<decltype(world)>; // Remove &
			auto worldName = WORLD_NAME<TWorld>;
			std::cout << "Running: " << worldName << "\n";
			worldNames.push_back(worldName);
			auto timeReportSet = RunExperiments<TWorld>(iterationsWarmup, iterationsReal, numberOfEntitiesWarmup, entitiesSet);
			timeReportSets.push_back(timeReportSet);
		}, w);

	}
	PrintResultsToFolder(worldNames, entitiesSet, timeReportSets, std::filesystem::path{ "./output" });
}