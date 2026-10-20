#pragma once

#include <string_view>
#include <filesystem>
#include <iostream>
#include <fstream>

#pragma once
#include <array>
#include <string_view>
#include <vector>

struct TimeMetricDescriptor {
	std::string_view Name;
	double TimeReport::* Member;
};

struct CountMetricDescriptor {
	std::string_view Name;
	int64_t TimeReport::* Member;
};

inline constexpr std::array<TimeMetricDescriptor, 8> TIME_METRICS{ {
	{ "CreateEntities", &TimeReport::CreateEntities },
	{ "PopulateComponents", &TimeReport::PopulateComponents },
	{ "QuerySingle", &TimeReport::QuerySingle },
	{ "QueryPair", &TimeReport::QueryPair },
	{ "QueryComplex", &TimeReport::QueryComplex },
	{ "QuerySingleMessy", &TimeReport::QuerySingleMessy },
	{ "QueryPairMessy", &TimeReport::QueryPairMessy },
	{ "QueryComplexMessy", &TimeReport::QueryComplexMessy },
} };

inline constexpr std::array<CountMetricDescriptor, 6> COUNT_METRICS{ {
	{ "QuerySingle", &TimeReport::QuerySingleResultsFound },
	{ "QueryPair", &TimeReport::QueryPairResultsFound },
	{ "QueryComplex", &TimeReport::QueryComplexResultsFound },
	{ "QuerySingle", &TimeReport::QuerySingleResultsFoundMessy },
	{ "QueryPair", &TimeReport::QueryPairResultsFoundMessy },
	{ "QueryComplex", &TimeReport::QueryComplexResultsFoundMessy },
} };

struct CsvHeader {
	std::string WorldNameHeader = "WorldName";
	std::vector<int> EntityAmounts{};
	std::string ToString() {
		std::string outputString{ WorldNameHeader };
		for (int number : EntityAmounts) {
			outputString += ", " + std::to_string(number);
		}
		return outputString;
	}
};

struct TimeCsvLine {
	std::string_view WorldName{};
	std::vector<double> Times{};
	std::string ToString() {
		std::string outputString{ WorldName };
		for (double number : Times) {
			outputString += ", " + std::to_string(number);
		}
		return outputString;
	}
};

struct CountCsvLine {
	std::string_view WorldName{};
	std::vector<int64_t> ResultsFound{};
	std::string ToString() {
		std::string outputString{ WorldName };
		for (int64_t number : ResultsFound) {
			outputString += ", " + std::to_string(number);
		}
		return outputString;
	}
};

/*
worldNames should be the same size and order as the outer vector of timeReports,
entityAmountSet should be the same size and order as the inner vectors of timeReports
parent folder of folderPath should already exist, will create "leaf" folder if needed
*/
void PrintResultsToFolder(const std::vector<std::string_view> worldNames, const std::vector<int>& entityAmountSet, const std::vector<std::vector<TimeReport>> timeReports, std::filesystem::path folderPath) {

	CsvHeader sharedHeader{ "WorldName", entityAmountSet };

	// each Time metric will be its own file, each file will have a line for each world
	std::array<std::vector<TimeCsvLine>, TIME_METRICS.size()> timeCsvFiles{};
	std::array<std::vector<CountCsvLine>, COUNT_METRICS.size()> countCsvFiles{};

	// For each world
	for (size_t w = 0; w < worldNames.size(); w++) {
		TimeCsvLine timeLineBase{ worldNames[w] };
		CountCsvLine countLineBase{ worldNames[w] };
		// For this world, make a TimeCsvLine for each metric
		std::array<TimeCsvLine, TIME_METRICS.size()> worldTimeMetrics{};
		worldTimeMetrics.fill(timeLineBase);
		std::array<CountCsvLine, COUNT_METRICS.size()> worldCountMetrics{};
		worldCountMetrics.fill(countLineBase);

		auto& worldTimeReports = timeReports[w];

		// For each time report
		for (TimeReport timeReportForEntityAmount : worldTimeReports) {
			// Go through each metric
			for (size_t m = 0; m < TIME_METRICS.size(); m++) {
				TimeMetricDescriptor metric = TIME_METRICS[m];
				// And put it in the world list
				worldTimeMetrics[m].Times.push_back(timeReportForEntityAmount.*(metric.Member));
			}
			for (size_t m = 0; m < COUNT_METRICS.size(); m++) {
				CountMetricDescriptor metric = COUNT_METRICS[m];
				// And put it in the world list
				worldCountMetrics[m].ResultsFound.push_back(timeReportForEntityAmount.*(metric.Member));
			}
		}
		// Move world list into metric -> world format
		for (size_t m = 0; m < worldTimeMetrics.size(); m++) {
			timeCsvFiles[m].push_back(worldTimeMetrics[m]);
		}
		for (size_t m = 0; m < worldCountMetrics.size(); m++) {
			countCsvFiles[m].push_back(worldCountMetrics[m]);
		}
	}

	std::filesystem::create_directory(folderPath);

	for (size_t m = 0; m < TIME_METRICS.size(); m++) {
		auto metric = TIME_METRICS[m];
		auto worldList = timeCsvFiles[m];
		std::filesystem::path filePath = folderPath / (std::string(metric.Name) + ".Time.csv");
		std::ofstream file{ filePath, std::ios::trunc };
		file << sharedHeader.ToString() << "\n";
		for (auto worldLine : worldList) {
			file << worldLine.ToString() << "\n";
		}
	}
	for (size_t m = 0; m < COUNT_METRICS.size(); m++) {
		auto metric = COUNT_METRICS[m];
		auto worldList = countCsvFiles[m];
		std::filesystem::path filePath = folderPath / (std::string(metric.Name) + ".Count.csv");
		std::ofstream file{ filePath, std::ios::trunc };
		file << sharedHeader.ToString() << "\n";
		for (auto worldLine : worldList) {
			file << worldLine.ToString() << "\n";
		}
	}
}