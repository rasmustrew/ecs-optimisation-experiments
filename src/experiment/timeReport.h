#pragma once
#include <iostream>


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