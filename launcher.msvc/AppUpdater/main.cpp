#include <Windows.h>
#include <WinBase.h>
#include <stdio.h>

BOOL FileExists(LPCWSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(strlen(lpCmdLine) == 0)	
	{
		MessageBox(0, L"Arguments are missing", L"App updater", MB_OK);
		return 1;
	}

	WCHAR appName[1000];
	MultiByteToWideChar(CP_ACP, 0, lpCmdLine, -1, appName, 1000);		
	
	WCHAR newFileName[1000];
	wcscpy(newFileName, appName);
	wcscat(newFileName, L"_new");	

	if(FileExists(newFileName))
	{
		//Backup it
		WCHAR backupName[1000];
		wcscpy(backupName, appName);
		wcscat(backupName, L"_backup");	

		BOOL error = false;
		BOOL ret = CopyFileEx(appName, backupName, NULL, NULL, NULL, 0);
		if(!ret)
		{
			error = true;
			/*DWORD err = GetLastError();
			WCHAR errorMsg[20];
			wsprintf(errorMsg, L"%d", err);
			MessageBox(0, errorMsg, L"", MB_OK);*/
		}

		if(!error)
		{
			//Then copy the new file		
			ret = CopyFileEx(newFileName, appName, NULL, NULL, NULL, 0);
			if(!ret)
			{
				error = true;
			} else {
				DeleteFile(newFileName);
				MessageBox(0, L"Upgrade successfull !", L"Upgrade successfull", MB_OK);
				return 0;
			}
		}

		if(error)
		{
			MessageBox(0, L"An error occured while upgrading. Operation cancelled.", L"Upgrade error", MB_OK);
			return 1;
		}
	} 

	return 0;
}