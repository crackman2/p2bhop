#include "pch.h"
#include "MainF.h"
#include "GlobalVars.h"
#include "GlobalObjects.h"
#include <chrono>
#include <cmath>
#include <string>

void MainF()
{
	float camxdiff = 0;
	float camxabs = 0;
	float deltatime = 0;
	float velanglediff = 0;
	float velangletmp = 0;
	bool  reversehopping = false;

	

	if (!g_Player.IsMenuOpen()) {

		g_Player.InitLocalPlayerAddresses();
		g_LocalPlayerBase = g_Player.LocalPlayerBase;
		g_Player.GetAll();
		g_Player.GetKeys();



	  //if ((GetAsyncKeyState(VK_SPACE) != 0) and (g_Player.onground == 1)) {
		if (g_Player.IsKeyPressed(VK_SPACE) and (g_Player.onground == 1)) {
			g_Player.onground = 2;
			g_Player.vel.z = 550.0f;   //jump velocity
			g_Player.SetOnGround();
			g_Player.SetVelocity(0x4); //up only
		}

		//if (GetAsyncKeyState(VK_SPACE) != 0) {
		if (g_Player.IsKeyPressed(VK_SPACE)) {
			if (!IsBadReadPtr((void*)(g_Player.LocalPlayerBase), 4) and ((void*)(g_Player.LocalPlayerBase) != nullptr)) {
				g_Player.MaxSpeed_Overwrite();
			}
			else {
				g_Player.MaxSpeed_Restore();
			}
		}
		else {
			g_Player.MaxSpeed_Restore();
		}

		deltatime = GetCurrentTime_() - g_oldtime;
		g_oldtime = GetCurrentTime_();

		camxdiff = AngleDifference(g_Player.camalt.x, g_Player.cam.x); //flipped
		camxabs = fabsf(camxdiff) * 10.0f * deltatime;

		/*
		velanglediff = atan2f(g_Player.vel.y, g_Player.vel.x);// -(PI / 2);
		velanglediff = (velanglediff < 0) ? (velanglediff + (2 * PI)) : velanglediff;
		velanglediff = fabsf(velanglediff - g_Player.cam.x);
		reversehopping = (velanglediff > (PI / 2));
		*/

		/*
		if (GetAsyncKeyState('Q') != 0) {
			MessageBox(0, (LPCWSTR)std::to_wstring(velanglediff).c_str(), L"velanglediff", 0);
			MessageBox(0, (LPCWSTR)std::to_wstring(g_Player.cam.x).c_str(), L"g_Player.cam.x", 0);
			MessageBox(0, (LPCWSTR)std::to_wstring(reversehopping).c_str(), L"reversehopping", 0);
		}
		*/


		//if ((GetAsyncKeyState('A') != 0) and (camxdiff < 0) and not(GetAsyncKeyState('D') != 0) and (g_Player.onground == 2) and !reversehopping) {
		if (g_Player.IsKeyPressed('A') and (camxdiff < 0) and not(g_Player.IsKeyPressed('D')) and (g_Player.onground == 2) and !reversehopping) {
			g_Player.vel.x += cosf(g_Player.cam.x) * camxabs;
			g_Player.vel.y += sinf(g_Player.cam.x) * camxabs;
			g_Player.PointVelocityVectorInCameraDirection();
			g_Player.SetVelocity(0x3); //only horizontal
		}


		//if ((GetAsyncKeyState('D') != 0) and (camxdiff > 0) and not(GetAsyncKeyState('A') != 0) and (g_Player.onground == 2) and !reversehopping) {
		if (g_Player.IsKeyPressed('D') and (camxdiff > 0) and not(g_Player.IsKeyPressed('A')) and (g_Player.onground == 2) and !reversehopping) {
			g_Player.vel.x += cosf(g_Player.cam.x) * camxabs;
			g_Player.vel.y += sinf(g_Player.cam.x) * camxabs;
			g_Player.PointVelocityVectorInCameraDirection();
			g_Player.SetVelocity(0x3); //only horizontal
		}

		/*
		if ((GetAsyncKeyState('D') != 0) and (camxdiff < 0) and not(GetAsyncKeyState('A') != 0) and (g_Player.onground == 2) and reversehopping) {
			g_Player.vel.x += cosf(g_Player.cam.x) * -camxabs;
			g_Player.vel.y += sinf(g_Player.cam.x) * -camxabs;
			g_Player.PointVelocityVectorInCameraDirection();
			g_Player.SetVelocity(0x3); //only horizontal
		}


		if ((GetAsyncKeyState('A') != 0) and (camxdiff > 0) and not(GetAsyncKeyState('D') != 0) and (g_Player.onground == 2) and reversehopping) {
			g_Player.vel.x += cosf(g_Player.cam.x) * -camxabs;
			g_Player.vel.y += sinf(g_Player.cam.x) * -camxabs;
			g_Player.PointVelocityVectorInCameraDirection();
			g_Player.SetVelocity(0x3); //only horizontal
		}
		*/



		g_Player.camalt = g_Player.cam;
	}
}



float GetCurrentTime_() {
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}


/**
*	AngleDifference calculates the difference between two angles
*	while respecting the issue that with angles that go from
*	0 to 360 deg (or similarly with radians) that there is a massive
*	difference going from 359 to 1 for example. the shortest path is 
*	taken instead
* 
*	@param	oldAngle first angle
*	@param	newAngle second angle
*	@return the difference
*/
float AngleDifference(float oldAngle, float newAngle)
{
	float diff = newAngle - oldAngle;
	float M_PI = 3.1415296535f;

	while (diff > M_PI) {
		diff -= 2 * M_PI;
	}
	while (diff <= -M_PI) {
		diff += 2 * M_PI;
	}

	return diff;
}

 