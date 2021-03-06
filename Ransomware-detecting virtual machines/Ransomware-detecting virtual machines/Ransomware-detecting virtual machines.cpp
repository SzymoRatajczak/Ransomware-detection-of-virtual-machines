// Ransomware-detecting virtual machines.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>

bool checkVms()
{
	//Win32 API value
	bool bretVal = false;
	
	PROCESSENTRY32 procEntry = { sizeof(PROCESSENTRY32) };

	//true if i have found  virtual machine
	bool bVmfound = false;

	HANDLE hProceSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	bretVal = Process32First(hProceSnap, &procEntry);


	//skip first process
	while (Process32Next(hProceSnap, &procEntry))
	{

		//get process executable way
		LPTSTR execName = procEntry.szExeFile;

		if (_wcsicmp(execName, L"VBoxService.exe") == 0 || _wcsicmp(execName, L"vmtoolsd.exe") == 0)
		{
			//found VMWare or Virtual Box service
			bVmfound = true;
			break;
		}

		//search in target process module
		MODULEENTRY32 dllEntry = { sizeof(MODULEENTRY32) };
		
		//current dll module  descriptor
		HANDLE hdllSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procEntry.th32ProcessID);

		bretVal = Module32First(hdllSnap, dllEntry);


		//skip first module
		while (Module32Next(hdllSnap, &dllEntry))
		{
			if (_wcsicmp(dllEntry.szModule, L"sbieDLL.dll") == 0)
			{
				//found sandbox dll
				bVmfound = true;
				break;
			}
		}

		if (hdllSnap != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hDllsSnap);

			//if i found VM process exit
			if (bVmfound)
				break;
			
		}
		return bVmfound;
	}




}
