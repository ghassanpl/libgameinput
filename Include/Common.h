#pragma once

/// TODO: Make these C++17/C++20 compatible
#include <ghassanpl/cpp98/enum_flags.h>
#include <ghassanpl/cpp11/named.h>
#include <ghassanpl/rec2.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <chrono>
#include <span>
#include <functional>
#include <array>
#include <compare>
#include <map>
#include <any>
#include <concepts>

namespace libgameinput
{
	using nlohmann::json;

	using InputID = std::string;
	inline static const InputID InvalidInput = {};

	using DeviceInputID = uintptr_t;
	static constexpr inline DeviceInputID InvalidDeviceInputID = std::numeric_limits<DeviceInputID>::max();
	using DeviceOutputID = uintptr_t;
	static constexpr inline DeviceOutputID InvalidDeviceOutputID = std::numeric_limits<DeviceOutputID>::max();

	struct IInputSystem;

	using PlayerID = ghassanpl::named<uintptr_t, struct PlayerIDTag>;
	using Seconds = std::chrono::duration<double>;
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using ghassanpl::enum_flags;
	using rec2 = ghassanpl::trec2<double>;
	using vec2 = glm::dvec2;
	using vec3 = glm::dvec3;
	using vec4 = glm::dvec4;
}