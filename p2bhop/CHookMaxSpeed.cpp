/*
 * CHookMaxSpeed.cpp
 *
 *  Created on: 26 Aug 2023
 *      Author: Jackson Smackson
 */

#include "pch.h"
#include "CHookMaxSpeed.h"
#include "HookCode.h"
#include <windows.h>
#include <cstdint>



CHookMaxSpeed::CHookMaxSpeed() {
	// TODO Auto-generated constructor stub
	HookCreated = false;
}

CHookMaxSpeed::~CHookMaxSpeed() {
	// TODO Auto-generated destructor stub
}

/**
 * Hooks the function that clamps all pawns' max velocity
 * LocalPlayer gets special treatment
 *
 * Creates a  jump to the HookCode. After HookCode is done, the flow returns
 * and fixes the stack pointer
 */
void CHookMaxSpeed::CreateHook() {
	EngineDLLBase = (uint32_t) GetModuleHandleA("Engine.dll");


	DWORD oldprotection;

	if (EngineDLLBase != 0) {
		if (VirtualProtect((LPVOID)(EngineDLLBase + MaxSpeedOffset), 8,
			PAGE_EXECUTE_READWRITE, &oldprotection)) {

			WriteJump(EngineDLLBase + MaxSpeedOffset,
				reinterpret_cast<uint32_t>(&HookCode), true);

			/*
			*(uint8_t*) (EngineDLLBase + MaxSpeedOffset + 5) = 0x83; //sub esp, 4
			*(uint8_t*) (EngineDLLBase + MaxSpeedOffset + 6) = 0xEC; //sub esp, 4
			*(uint8_t*) (EngineDLLBase + MaxSpeedOffset + 7) = 0x04; //sub esp, 4
			*/

			*(uint8_t*)(EngineDLLBase + MaxSpeedOffset + 5) = 0x90; //NOP
			*(uint8_t*)(EngineDLLBase + MaxSpeedOffset + 6) = 0x90; //NOP
			*(uint8_t*)(EngineDLLBase + MaxSpeedOffset + 7) = 0x90; //NOP
		}
		else {
			MessageBoxA(0, "changing page protection of MaxSpeed failed", "Error",
				0);
		}
	}
	else {
		MessageBoxA(0, "EngineDLL is nil of MaxSpeed failed", "Error",
			0);
	}
}


/**
 *	Writes a jump or a call as way to create detours
 *	@param from Location where the jump/call is written
 *	@param to 	the destination (used to calculate the jump offset)
 *	@param j0c1 Jump 0 and Call 1 is used to create either a jump or a call
 */
void CHookMaxSpeed::WriteJump(uint32_t from, uint32_t to, bool j0c1) {
	uint32_t JCode;
	uint8_t FinalCode[4] = { 0, 0, 0, 0 };
	uint8_t *PWriter;
	uint32_t i;

	JCode = to - from - 0x5;
	unsigned char *finalCodePtr = reinterpret_cast<unsigned char*>(&JCode);

	for (i = 0; i < 4; ++i) {
		FinalCode[i] = finalCodePtr[i];
	}

	PWriter = (uint8_t*) from;
	if (j0c1)
		*PWriter = 0xE8; //CALL
	else
		*PWriter = 0xE9; //JMP

	for (i = 0; i < 4; i++) {
		PWriter = (uint8_t*) (from + 1 + i);
		*PWriter = FinalCode[i];
	}
}



