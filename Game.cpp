#include "Game.h"

bool CGame::InitAddress()
{
	const char* clientModule = "client.dll";
	HMODULE clientModuleHandle = ProcessMgr.GetProcessModuleHandle(clientModule);

	if (clientModuleHandle == nullptr)
	{
		return false;
	}

	// Calculate the addresses using offsets.
	this->Address.ClientDLL = reinterpret_cast<DWORD64>(clientModuleHandle);
	this->Address.EntityList = this->Address.ClientDLL + Offset::EntityList;
	this->Address.Matrix = this->Address.ClientDLL + Offset::Matrix;
	this->Address.ViewAngle = this->Address.ClientDLL + Offset::ViewAngle;
	this->Address.LocalController = this->Address.ClientDLL + Offset::LocalPlayerController;
	this->Address.LocalPawn = GetClientDLLAddress() + Offset::LocalPlayerPawn;


	return true;
}

DWORD64 CGame::GetClientDLLAddress()
{ n
	return this->Address.ClientDLL;
}

DWORD64 CGame::GetEntityListAddress()
{
	return this->Address.EntityList;
}

DWORD64 CGame::GetMatrixAddress()
{
	return this->Address.Matrix;
}

DWORD64 CGame::GetViewAngleAddress() 
{
	return this->Address.ViewAngle;
}

DWORD64 CGame::GetEntityListEntry()
{
	return this->Address.EntityListEntry;
}

DWORD64 CGame::GetLocalControllerAddress()
{
	return this->Address.LocalController;
}

DWORD64 CGame::GetLocalPawnAddress()
{
	return this->Address.LocalPawn;
}

bool CGame::UpdateEntityListEntry()
{
	DWORD64 EntityListEntry = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), EntityListEntry))
		return false;
	if (!ProcessMgr.ReadMemory<DWORD64>(EntityListEntry + 0x10, EntityListEntry))
		return false;

	this->Address.EntityListEntry = EntityListEntry;

	return this->Address.EntityListEntry != 0;
}

bool CGame::SetViewAngle(float Yaw, float Pitch)
{
	Vec2 Angle{ Pitch,Yaw };

	if (!ProcessMgr.WriteMemory<Vec2>(this->Address.ViewAngle, Angle))
		return false;

	return true;
}