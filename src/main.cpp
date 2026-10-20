#include <ctime>
#include <variant>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <ecs/experiments.h>
#include <ecs/ecsSimple.h>
#include <ecs/timeReport.h>
#include <ecs/worldNames.h>

#include <outputFormatting.h>


using World = std::variant<WorldSimple>;

const int iterationsWarmup = 100;
const int iterationsReal = 10000;
const int numberOfEntitiesWarmup = 1000;
const std::vector<int> entitiesSet = { 100, 1000, 10000 };
const std::vector<World> worlds = { WorldSimple{}, WorldSimple{} };

int main(int, char**) {

	std::vector<std::vector<TimeReport>> timeReportSets{};
	std::vector<std::string_view> worldNames{};

	for (auto& w : worlds) {
		std::visit([&](auto& world) { // Get underlying type T from std::variant<T>
			using TWorld = std::remove_cvref_t<decltype(world)>; // Remove &
			auto worldName = WORLD_NAME<TWorld>;
			std::cout << "Running: " << worldName << "\n";
			worldNames.push_back(worldName);
			auto timeReportSet = RunExperiments<TWorld>(10, 10, 10, entitiesSet);
			timeReportSets.push_back(timeReportSet);
		}, w);

	}



	TimeReport aggregatedTimeReportWarmup{};



	PrintResultsToFolder(worldNames, entitiesSet, timeReportSets, std::filesystem::path{ "./output" });
}