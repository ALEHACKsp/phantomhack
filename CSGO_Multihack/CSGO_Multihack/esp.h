#pragma once

/*

Type: Namespace
Name: ESP
Full name: Extra Sensory Perception
Intended for: Rendering an extra information about entities

*/

class C_BasePlayer;
class C_BaseEntity;
class C_BaseCombatWeapon;
class C_PlantedC4;
class Color;
class ClientClass;
typedef unsigned long DWORD;

namespace ESP
{
	namespace Player
	{
		bool Begin(C_BasePlayer* pl);

		void RenderBox(bool headOnly);
		void RenderBones();
		void RenderDefusing();
		void RenderName();
		void RenderDistance();
		void RenderHealth(bool headOnly);
		void RenderArmour(bool headOnly);
		void RenderWeapon();
		void RenderSnapline();
		void NotifyWhenSpotted();
		void RenderWall();
	}

	namespace Misc
	{
		void RenderCrosshair();
		void RenderWeapon(C_BaseCombatWeapon* ent);
		void RenderDefuseKit(C_BaseEntity* ent);
		void RenderPlantedC4(C_BaseEntity* ent);
		void deprecated_GrenadePrediction(Color color);
	}

	namespace Sound
	{
		void __fastcall BeepAsync(DWORD freq, DWORD dur, int channel, int holdTime = 0);
		void IsAimingEnemy();
		void EnemyIsNear();
	}

	namespace Aimbot
	{
		void DisplayInfo();
	}

	bool CreateFonts(int fontSize);
	void DestroyFonts();
}
