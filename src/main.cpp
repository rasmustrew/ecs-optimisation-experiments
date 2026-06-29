#include <ctime>
#include <iostream>
#include "ecs/experiments.h"
#include <ecs/ecsSimple.h>

#include <chrono>

const int iterationsWarmup = 100;
const int iterationsReal = 10000;
const int numberOfEntitiesWarmup = 1000;
const std::array<int, 3> entitiesSet{ 100, 1000, 10000 };




void RunWarmup() {
	for (int i = 0; i < iterationsWarmup; i++) {
		auto world = WorldSimple{};
		RunExperiments(world, numberOfEntitiesWarmup);
	}
}

int main(int, char**) {
	TimeReport aggregatedTimeReportWarmup{};

	for (const int& numberOfEntities : entitiesSet) {
		ReseedRandomGenerator();
		RunWarmup();

		std::cout << "ENTITIES: " << numberOfEntities << "\n";
		ReseedRandomGenerator();
		const auto startTime = std::chrono::steady_clock::now();
		TimeReport aggregatedTimeReport{};

		for (int i = 0; i < iterationsReal; i++) {
			auto world = WorldSimple{};
			aggregatedTimeReport += RunExperiments(world, numberOfEntities);
		}

		const auto endTime = std::chrono::steady_clock::now();
		const std::chrono::duration<double, std::micro> elapsedTime = endTime - startTime;
		std::cout << "TOTAL: \n";
		aggregatedTimeReport.Print();

		aggregatedTimeReport /= iterationsReal;
		std::cout << "PER RUN: \n";
		aggregatedTimeReport.Print();

		std::cout << "Elapsed wall-clock (Full): " << elapsedTime.count() << " us \n";
	}

}