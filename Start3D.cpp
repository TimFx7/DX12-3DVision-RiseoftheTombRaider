// Activates 3D by manipulating memory for Rise of the Tomb Raider
//

#include "Start3D.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>

int main()
{
	HWND GameWindow;
	int kez = 0;
	int Stereoscopic;
	DWORD procId;
	uintptr_t moduleBase;
	HANDLE hProcess = 0;
	uintptr_t dynamicPtrBaseAddr;
	std::vector<unsigned int> StereoscopicOffsets;
	uintptr_t StereoscopicAddr;


	GameWindow = ::FindWindowExA(0, 0, "TR2NxApp", 0); // Tomb Raider Window Class Name 
	while ((GameWindow == NULL) & (kez <= 60))
	{
		Sleep(1000);
		GameWindow = ::FindWindowExA(0, 0, "TR2NxApp", 0);
		kez++;
		if (kez == 60)
		{
			MessageBox(nullptr, TEXT("Rise of the Tomb Raider uninitialized. \nPlease start the game first !!!"), TEXT("WARNING : Enabling 3D Vision"), MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}

	}

	HWND LauncherMenu = FindWindowA("TR2NxApp", "Rise of the Tomb Raider"); // Rise of the Tomb Raider: Launcher Menu
	if (LauncherMenu != NULL)
	{
		//Get ProcId of the target process
		procId = GetProcId(L"ROTTR.exe");  //Rise of the Tomb Raider 
		moduleBase = GetModuleBaseAddress(procId, L"ROTTR.exe");
		//Get Handle to Process
		hProcess = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		// Rise of the Tomb Raider: Launcher Menu //Options//Stereoscopic 3D: ON
		dynamicPtrBaseAddr = moduleBase + 0x1066B80;
		StereoscopicOffsets = { 0x148, 0x20, 0x90 };
		StereoscopicAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, StereoscopicOffsets);

		Stereoscopic = 2;
		WriteProcessMemory(hProcess, (BYTE*)StereoscopicAddr, &Stereoscopic, sizeof(Stereoscopic), nullptr);

		return 0;
	}

	

	Sleep(5000); //  if ROTTR.exe start parameters "-nolouncher" 

	//Get ProcId of the target process
        procId = GetProcId(L"ROTTR.exe");  //Tomb Raider 
	moduleBase = GetModuleBaseAddress(procId, L"ROTTR.exe");

	hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	// Rise of the Tomb Raider: INGAME menu//Options//DISPLAY//Stereoscopic 3D: ON
	dynamicPtrBaseAddr = moduleBase + 0xFA7DC0;
	StereoscopicOffsets = { 0xD44 }; 
	StereoscopicAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, StereoscopicOffsets);


	// The game may crash when activated in-game options as // Stereoscopic = (2) 3DVision.
	// When SBS = (1) is activated first and then switched to 3DVision, the crash probability decreases by 50%.
	// In this process, minimizing the game window and bringing it back to the foreground further reduces the possibility of crashes.

	Stereoscopic = 1;
	WriteProcessMemory(hProcess, (BYTE*)StereoscopicAddr, &Stereoscopic, sizeof(Stereoscopic), nullptr);
	ShowWindow(GameWindow, SW_MINIMIZE);
	Sleep(5000); 
	ShowWindow(GameWindow, SW_SHOWNORMAL);
	Sleep(500);
	Stereoscopic = 2;
	WriteProcessMemory(hProcess, (BYTE*)StereoscopicAddr, &Stereoscopic, sizeof(Stereoscopic), nullptr);
	ShowWindow(GameWindow, SW_SHOWNORMAL);


	return 0;
}
