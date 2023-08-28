/*
 * CP2Player.cpp
 *
 *  Created on: 26 Aug 2023
 *      Author: Jackson Smackson
 */
#include "pch.h"
#include "CP2Player.h"
#include <windows.h>
#include <cmath>
#include <locale>
#include <codecvt>
#include <sstream>

CP2Player::CP2Player() {
	// TODO Auto-generated constructor stub
	pos = { 0,0,0 };
	vel = { 0,0,0 };
	cam = { 0,0 };

	FirstTimeInit = true;

	InitLocalPlayerAddresses();
}

CP2Player::~CP2Player() {
	// TODO Auto-generated destructor stub
}

void CP2Player::GetPosition() {
	if (!IsBadReadPtr((void*)LocalPlayerBase, 4) and ((void*)(LocalPlayerBase) != nullptr)) {
		pos.x = *(float*)(LocalPlayerBase + offset_posx);
		pos.y = *(float*)(LocalPlayerBase + offset_posy);
		pos.z = *(float*)(LocalPlayerBase + offset_posz);
	}
}

void CP2Player::GetVelocity() {
	if (!IsBadReadPtr((void*)LocalPlayerBase, 4) and ((void*)(LocalPlayerBase) != nullptr)) {
		vel.x = *(float*)(LocalPlayerBase + offset_velx);
		vel.y = *(float*)(LocalPlayerBase + offset_vely);
		vel.z = *(float*)(LocalPlayerBase + offset_velz);
	}
}


void CP2Player::GetOnGround() {
	if (!IsBadReadPtr((void*)LocalPlayerBase, 1) and ((void*)(LocalPlayerBase) != nullptr)) {
		onground = *(uint8_t*)(LocalPlayerBase + offset_onground);
	}
}

/*
 * camx is a 16 bit unsigned integer from 0 to 65535
 * it is first converted into degrees and then into radians
 */
void CP2Player::GetAngles() {
	if (!IsBadReadPtr((void*)LocalPlayerBase, 4) and ((void*)(LocalPlayerBase) != nullptr)) {
		camx = *(uint16_t*)(LocalPlayerBase + offset_camx);
		cam.x = camx / 182.04444444444;
		cam.x = cam.x * (3.1415926535 / 180);
	}
}

void CP2Player::GetKeys()
{
	if (!IsBadReadPtr((void*)WASDKeysBase, 1)) 
		wasdkeystate = *(uint8_t*)(WASDKeysBase);

	if (!IsBadReadPtr((void*)SpaceKeyBase, 1))
		spacekeystate = *(uint8_t*)(SpaceKeyBase);
}

void CP2Player::PointVelocityVectorInCameraDirection() {
	float velocityMagnitude = 0;
	float normalizedVX, normalizedVY = 0;
	float dotProduct = 0;
	float dirAngleX, dirAngleY = 0;


	dirAngleX = cosf(cam.x);
	dirAngleY = sinf(cam.x);


	velocityMagnitude = sqrtf((vel.x * vel.x) + (vel.y * vel.y));


	if(velocityMagnitude != 0){
		normalizedVX = vel.x / velocityMagnitude;
		normalizedVY = vel.y / velocityMagnitude;

		dotProduct = dirAngleX * normalizedVX + dirAngleY * normalizedVY;

		velocityMagnitude = (velocityMagnitude > maxvelocity) ? maxvelocity : velocityMagnitude;

		vel.x = dotProduct * dirAngleX * velocityMagnitude;
		vel.y = dotProduct * dirAngleY * velocityMagnitude;
	}
}

void CP2Player::SetVelocity(uint8_t dir) {
	const uint8_t mask_x = 0x01;
	const uint8_t mask_y = 0x02;
	const uint8_t mask_z = 0x04;

	if (!IsBadReadPtr((void*)LocalPlayerBase, 4) and ((void*)(LocalPlayerBase) != nullptr)) {
		if ((dir bitand mask_x) != 0) *(float*)(LocalPlayerBase + offset_velx) = vel.x;
		if ((dir bitand mask_y) != 0) *(float*)(LocalPlayerBase + offset_vely) = vel.y;
		if ((dir bitand mask_z) != 0) *(float*)(LocalPlayerBase + offset_velz) = vel.z;
	}
}

void CP2Player::SetOnGround() {
	if (!IsBadReadPtr((void*)LocalPlayerBase, 1) and ((void*)(LocalPlayerBase) != nullptr)) {
		*(uint8_t*)(LocalPlayerBase + offset_onground) = onground;
		g_OnGround = onground;
	}
}

void CP2Player::GetAll() {
	GetPosition();
	GetVelocity();
	GetOnGround();
	GetAngles();
}

void CP2Player::InitLocalPlayerAddresses() {
	uint32_t i = 0;
	DWORD old = 0;

	if(FirstTimeInit){
		EngineDLLBase = (uint32_t)GetModuleHandleA("Engine.dll");

		FPSGameDLLBase = (uint32_t)GetModuleHandleA("FPSGame.DLL");

		//check if this is required
		VirtualProtect((LPVOID)(EngineDLLBase + bytes_MaxSpeed[0][0]),12,PAGE_EXECUTE_READWRITE,&old);

		FirstTimeInit = false;
	}

	if ((EngineDLLBase != 0) and ((void*)(EngineDLLBase) != nullptr) and !IsBadReadPtr((void*)EngineDLLBase, 4)){
		LocalPlayerBase = EngineDLLBase + offset_Engine;
		for (int i = 0; i < (sizeof(offset_LocalPlayer) / sizeof(offset_LocalPlayer[0]));i++) {
			if (((void*)LocalPlayerBase != nullptr) and !IsBadReadPtr((void*)LocalPlayerBase,4)) {
				LocalPlayerBase = *(uint32_t*)(LocalPlayerBase);
				LocalPlayerBase += offset_LocalPlayer[i];
			}
			else {
				LocalPlayerBase = 0;
				break;
			};
		}

		if (((void*)LocalPlayerBase != nullptr) and !IsBadReadPtr((void*)LocalPlayerBase, 4)) {
			g_LocalPlayerBase = LocalPlayerBase;
			HookMaxSpeed.CreateHook();
		}

		WASDKeysBase = EngineDLLBase + offset_WASDKeysBase;
		for (int i = 0; i < (sizeof(offset_WASDKeys)/sizeof(offset_WASDKeys[0])); i++) {
			WASDKeysBase = *(uint32_t*)(WASDKeysBase);
			WASDKeysBase += offset_WASDKeys[i];
		}

		SpaceKeyBase = EngineDLLBase + offset_SpaceKeyBase;
		for (int i = 0; i < (sizeof(offset_SpaceKey) / sizeof(offset_SpaceKey[0])); i++) {
			SpaceKeyBase = *(uint32_t*)(SpaceKeyBase);
			SpaceKeyBase += offset_SpaceKey[i];
		}
	}
	else {
		FirstTimeInit = true;
	}

	
}

void CP2Player::MaxSpeed_Restore() {
	if ((EngineDLLBase != 0) and ((void*)(EngineDLLBase) != nullptr) and !IsBadReadPtr((void*)EngineDLLBase, 4)) {
		for (int i = 0; i < sizeof(bytes_MaxSpeed[0]); i++) {
			*(uint8_t*)(EngineDLLBase + MaxSpeedOffset + i) = (uint8_t)(bytes_MaxSpeed[0][i]);
		}
	}
}


void CP2Player::MaxSpeed_Overwrite() {
	HookMaxSpeed.CreateHook();
}

bool CP2Player::IsMenuOpen()
{
	if (!IsBadReadPtr((void*)EngineDLLBase, 4) and ((void*)EngineDLLBase != nullptr)) {
		if (*(uint8_t*)(EngineDLLBase + offset_menuopen) == 1)
			return true;
	}
	return false;
}

bool CP2Player::IsKeyPressed(char Key)
{
	switch (Key) {
		case 'W':
			return (wasdkeystate bitand keyW);
			break;
		case 'A':
			return (wasdkeystate bitand keyA);
			break;
		case 'S':
			return (wasdkeystate bitand keyS);
			break;
		case 'D':
			return (wasdkeystate bitand keyD);
			break;
		case 0x20:
			return (bool)spacekeystate;
			break;
	}

	return false;
}
