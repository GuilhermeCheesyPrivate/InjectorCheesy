#include <Windows.h>
#include <iostream>
#include "xorstr.h"

using namespace std;
HWND hwnd= nullptr;
int Gameid = 0;
DWORD pid = NULL;
DWORD tid = NULL;
HMODULE dll = NULL;

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	wchar_t* dllPath = argv[1];
	//FindWindowA(
	//	_In_opt_ LPCSTR lpClassName,
	//	_In_opt_ LPCSTR lpWindowName);

	if(!hwnd){
	hwnd = FindWindowA(_("DayZ"), 0); //Game window clas sname
	Gameid = 1;
	}

	if (hwnd == NULL) {
		cout << "[ Open Game First ]" << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	// Getting the thread of the window and the PID
	
	 tid = GetWindowThreadProcessId(hwnd, &pid);
	if (tid == NULL) {
	 	cout << "[ FAILED ] Could not get thread ID of the target window." << endl;
	 	system("pause");
		return EXIT_FAILURE;
	}

	// Loading DLL
	if(Gameid == 1)
	 dll = LoadLibraryExA(_("C:\\Users\\Guilherme\\google.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES); //Loading dll from params


	if (dll == NULL) {
		cout << "[ FAILED ] The DLL could not be found." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	// Getting exported function address
	HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, _("DayZ")); //export see dllmain.cpp "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam)
	if (addr == NULL) {
	 	cout << "[ FAILED ] The function was not found." << endl;
	 	system("pause");
		//return EXIT_FAILURE;
	}

	// Setting the hook in the hook chain
	HHOOK handle = SetWindowsHookEx(WH_GETMESSAGE, addr, dll, tid); // Or WH_KEYBOARD if you prefer to trigger the hook manually
	if (handle == NULL) {
		cout << "[ FAILED ] Couldn't set the hook with SetWindowsHookEx." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	// Triggering the hook
	PostThreadMessage(tid, WM_NULL, NULL, NULL);

	// Waiting for user input to remove the hook
	//cout << "[ OK ] Hook set and triggered." << endl;
	//cout << "[ >> ] Press any key to unhook (This will unload the DLL)." << endl;

	system("cls");
	system("del C:\\Users\\Guilherme\\google.dll");
	system("cls");
	cout << "[ Dont Close This CMD ] \n" << endl;
	cout << "[ OK ] \n" << endl;
	cout << "[ Will Can Play now ] \n" << endl;
	system("pause > nul");
	system("pause > nul");

	// Unhooking
	BOOL unhook = UnhookWindowsHookEx(handle);
	if (unhook == FALSE) {
		//cout << "[ FAILED ] Could not remove the hook." << endl;
	//	system("pause");
		return EXIT_FAILURE;
	}

	
	Sleep(5000);

	return EXIT_SUCCESS;
}
