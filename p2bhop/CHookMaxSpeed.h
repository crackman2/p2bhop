/*
 * CHookMaxSpeed.h
 *
 *  Created on: 26 Aug 2023
 *      Author: Jackson Smackson
 */

#ifndef CHOOKMAXSPEED_H_
#define CHOOKMAXSPEED_H_

#include <cstdint>

class CHookMaxSpeed {
public:
	uint32_t EngineDLLBase;
	bool HookCreated;

	CHookMaxSpeed();
	virtual ~CHookMaxSpeed();

	void CreateHook();

private:
	const uint32_t MaxSpeedOffset = 0x22ABD0; //offset from Engine.dll
	void WriteJump(uint32_t from, uint32_t to, bool j1c0);
};

#endif /* CHOOKMAXSPEED_H_ */
