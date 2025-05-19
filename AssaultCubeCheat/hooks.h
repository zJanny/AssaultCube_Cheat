#pragma once

namespace hooks
{
	void enableHooks();

	inline bool weaponChams = false;
	extern "C" void* o_rendermodel;
	extern "C" void applyChams(const char*);

}