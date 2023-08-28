#include "pch.h"
#include "OpenGLHook.h"
#include "GlobalObjects.h"
#include "CP2Player.h"
#include "MainF.h"

void HookSwapBuffers()
{
	uint8_t OriginalSwapbuffersCode[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };
	
	uint8_t* CodeCave = new uint8_t[1024];
	uint32_t SBuffers = (uint32_t)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
	DWORD old;

	if (SBuffers != 0) {
		if (VirtualProtect((LPVOID)CodeCave, 128, PAGE_EXECUTE_READWRITE, &old)) {
			if (VirtualProtect((LPVOID)SBuffers, 5, PAGE_EXECUTE_READWRITE, &old)) {
				WriteJump(SBuffers, (uint32_t)CodeCave, false);
				*(uint8_t*)(CodeCave +  0) = 0x60; //pushad
				*(uint8_t*)(CodeCave +  1) = 0x9C; //pushad
				WriteJump((uint32_t)CodeCave +  2, (uint32_t)MainF, true);
				*(uint8_t*)(CodeCave +  7) = 0x9D; //pushad
				*(uint8_t*)(CodeCave +  8) = 0x61; //pushad

				for (int i = 0; i < sizeof(OriginalSwapbuffersCode); i++) {
					*(uint8_t*)(CodeCave + 9 + i) = OriginalSwapbuffersCode[i];
				}

				WriteJump((uint32_t)CodeCave + 14, SBuffers + 5, false);
				InitGlobalObjects();

			}
			else {
				MessageBoxA(0, "Could not VirtualProtect wglSwapBuffers(OpenGL)", "Errors", 0);
			}
		}
		else {
			MessageBoxA(0, "Could not VirtualProtect the CodeCave(OpenGL)", "Errors", 0);
		}
	}
	else {
		MessageBoxA(0, "Could not find wglSwapbuffers", "Errors", 0);
	}
}

void WriteJump(uint32_t from, uint32_t to, bool j0c1) {
	uint32_t JCode;
	uint8_t FinalCode[4] = { 0, 0, 0, 0 };
	uint8_t* PWriter;
	uint32_t i;

	JCode = to - from - 0x5;
	unsigned char* finalCodePtr = reinterpret_cast<unsigned char*>(&JCode);

	for (i = 0; i < 4; ++i) {
		FinalCode[i] = finalCodePtr[i];
	}

	PWriter = (uint8_t*)from;
	if (j0c1)
		*PWriter = 0xE8; //CALL
	else
		*PWriter = 0xE9; //JMP

	for (i = 0; i < 4; i++) {
		PWriter = (uint8_t*)(from + 1 + i);
		*PWriter = FinalCode[i];
	}
}

void InitGlobalObjects()
{
//	MessageBoxA(0, "OpenGL hook created", "InitGlobalObjects", 0);
}
