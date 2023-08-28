/*
 * CP2Player.h
 *
 *  Created on: 26 Aug 2023
 *      Author: Jackson Smackson
 */

#ifndef CP2PLAYER_H_
#define CP2PLAYER_H_

#include <cstdint>
#include "customtypes.h"
#include "CHookMaxSpeed.h"
#include "GlobalVars.h"

class CP2Player {
public:
	Vec3f pos;
	Vec3f vel;
	Vec2f cam;
	Vec2f camalt;

	uint8_t onground;
	uint8_t wasdkeystate;
	uint8_t spacekeystate;

	uint32_t LocalPlayerBase;
	CHookMaxSpeed HookMaxSpeed;

	CP2Player();
	virtual ~CP2Player();
	void GetPosition();
	void GetVelocity();
	void GetOnGround();
	void GetAngles();
	void GetKeys();

	void PointVelocityVectorInCameraDirection();
	void SetVelocity(uint8_t dir);
	void SetOnGround();
	void GetAll();

	void InitLocalPlayerAddresses();
	void MaxSpeed_Restore();
	void MaxSpeed_Overwrite();

	bool IsMenuOpen();
	bool IsKeyPressed(char Key);

private:
	uint32_t EngineDLLBase;
	uint32_t FPSGameDLLBase;

	uint32_t MaxSpeedBase;

	uint32_t WASDKeysBase;

	uint32_t SpaceKeyBase;

	uint16_t camx;
	bool FirstTimeInit;

	const float maxvelocity = 3000.0f;

	const uint8_t keyA = 16;
	const uint8_t keyD = 8;
	const uint8_t keyS = 4;
	const uint8_t keyW = 2;

	const uint32_t offset_Engine = 0x4CFF28;
	const uint32_t offset_LocalPlayer [6] = {0x2C, 0x58, 0x10, 0x144, 0x8C, 0x0};

	const uint32_t offset_posx = 0xE0;
	const uint32_t offset_posy = 0xE4;
	const uint32_t offset_posz = 0xE8;

	const uint32_t offset_velx = 0xF8;
	const uint32_t offset_vely = 0xFC;
	const uint32_t offset_velz = 0x100;

	const uint32_t offset_camx = 0x450;

	const uint32_t offset_onground = 0x3C;

	const uint32_t offset_menuopen = 0x4477FC;

	const uint32_t offset_WASDKeysBase = 0x63C680;
	const uint32_t offset_WASDKeys[4] = { 0x80, 0x30, 0x61C, 0x28 };

	const uint32_t offset_SpaceKeyBase = 0x4CFF28;//offset from Engine.dll
	const uint32_t offset_SpaceKey[3] = { 0x2C ,0x68, 0xED0 };

	const uint32_t MaxSpeedOffset = 0x22ABD0; //offset from Engine.dll
	const uint8_t bytes_MaxSpeed[2][8] = {
			{ 0xF3, 0x0F, 0x10, 0x83, 0xC8, 0x02, 0x00, 0x00 }, //ori
			{ 0xF3, 0x0F, 0x10, 0x83, 0xD8, 0x02, 0x00, 0x00 }  //mod
	};

};

#endif /* CP2PLAYER_H_ */
