#pragma once
#include <string_view>

#include "../ecs/worlds/World0.h"
#include "../ecs/worlds/World1.h"
#include "../ecs/worlds/World2.h"
#include "../ecs/worlds/World3.h"

template <typename T>
constexpr std::string_view WORLD_NAME = "UnknownWorld";

template <>
constexpr std::string_view WORLD_NAME<World0> = "World0";

template <>
constexpr std::string_view WORLD_NAME<World1> = "World1";

template <>
constexpr std::string_view WORLD_NAME<World2> = "World2";

template <>
constexpr std::string_view WORLD_NAME<World3> = "World3";