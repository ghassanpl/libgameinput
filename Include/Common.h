#pragma once

/// TODO: Make these C++17/C++20 compatible
#include <ghassanpl/cpp98/enum_flags.h>
#include <ghassanpl/cpp11/named.h>
#include <ghassanpl/rec2.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/scalar_common.hpp>
//#include <glm/gtx/extended_min_max.hpp> 

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

	using DeviceComponentID = uintptr_t;
	static constexpr inline DeviceComponentID InvalidDeviceComponentID = std::numeric_limits<DeviceComponentID>::max();
	
	using DeviceInputID = DeviceComponentID;
	static constexpr inline DeviceInputID InvalidDeviceInputID = std::numeric_limits<DeviceInputID>::max();

	using DeviceOutputID = DeviceComponentID;
	static constexpr inline DeviceOutputID InvalidDeviceOutputID = std::numeric_limits<DeviceOutputID>::max();

	using SubDeviceID = uintptr_t;
	static constexpr inline SubDeviceID InvalidSubDeviceID = std::numeric_limits<SubDeviceID>::max();

	struct IInputSystem;

	using PlayerID = ghassanpl::named<uintptr_t, struct PlayerIDTag>;
	using Seconds = std::chrono::duration<double>;
	using TimePoint = std::chrono::high_resolution_clock::time_point;
	using ghassanpl::enum_flags;
	using rec2 = ghassanpl::trec2<double>;
	using vec2 = glm::dvec2;
	using vec3 = glm::dvec3;
	using vec4 = glm::dvec4;
	using quat = glm::qua<double>;

	struct ViewRay
	{
		vec3 Position{};
		vec3 Direction{}; 
	};

	vec3 ClosestPointBetween(ViewRay const& ray1, ViewRay const& ray2);

	using uint128_t = std::array<uint64_t, 2>;
	auto Vec3ToU128(vec3 const& v) -> uint128_t;
	auto U128ToVec3(uint128_t v) -> vec3;
	auto Vec2ToU64(vec2 const& v) -> uint64_t;
	auto U64ToVec2(uint64_t v) -> vec3;
	auto DoubleToU32(double v) -> uint32_t;
	auto U32ToDouble(uint32_t v) -> double;
}