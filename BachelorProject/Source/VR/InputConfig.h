#pragma once

namespace VR {
	constexpr char ACTIONS_PATH[] = "_asset/vr_actions.json";

	constexpr unsigned int ACTIONS_NUMBER = 5;
	constexpr char ACTIONS_PREFIX[] = "/actions/sph";
	const std::array<std::string, ACTIONS_NUMBER> ActionNameArray{
		"showmenu",
		"createcontainer",
		"createfluid",
		"click",
		"hold"
	};

	constexpr unsigned int ACTION_INPUT_SOURCES_NUMBER = 2;
	constexpr char ACTIONS_INPUT_SOURCE_PREFIX[] = "/user/hand";
	const std::array<std::string, ACTION_INPUT_SOURCES_NUMBER> ActionInputSourceNameArray{
		"left",
		"right"
	};
}
