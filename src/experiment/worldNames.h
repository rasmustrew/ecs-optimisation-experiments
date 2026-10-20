#pragma once
#include <string_view>

template <typename T>
constexpr std::string_view WORLD_NAME = "UnknownWorld";

template <>
constexpr std::string_view WORLD_NAME<WorldSimple> = "WorldSimple";