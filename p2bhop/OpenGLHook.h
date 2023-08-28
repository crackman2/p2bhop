#pragma once
#include <cstdint>

/*
	procedure HookSwapBuffers(); stdcall;
	procedure WriteJump(J_FROM: DWORD; J_TO: DWORD; J0C1: boolean); stdcall;
	procedure InitGlobalObjects(); stdcall;
	procedure dbgLog(txt: PChar); stdcall;
*/
void HookSwapBuffers();
void WriteJump(uint32_t from, uint32_t to, bool j1c0);
void InitGlobalObjects();
