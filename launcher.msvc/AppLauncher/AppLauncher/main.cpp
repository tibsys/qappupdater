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
		MessageBox(0, L"Arguments are missing", L"App launcher", MB_OK);
		return 1;
	}

	WCHAR appName[FILENAME_MAX];
	MultiByteToWideChar(CP_ACP, 0, lpCmdLine, -1, appName, 1000);		
		
	WCHAR newFileName[FILENAME_MAX];
	wcscpy(newFileName, appName);
	wcscat(newFileName, L"_new");	

	LPWSTR currentPath = new WCHAR[FILENAME_MAX];
	::GetModuleFileName(NULL, currentPath, _MAX_PATH);

	WCHAR appPath[FILENAME_MAX];
	memset(appPath, 0, FILENAME_MAX);
	wchar_t* end = wcsrchr(currentPath, L'\\');
	if(end)
	{						
		DWORD len = end - currentPath;
		
		wcsncpy(appPath, currentPath, len);		
		wcscat(appPath, L"\\");
		wcscat(appPath, appName);			
	}

	if(!FileExists(appName))
	{
		MessageBox(0, L"File not found !", L"", MB_OK);
	} else {	
		if(FileExists(newFileName))
		{ 
			//We have an update			    			
			SHELLEXECUTEINFO shInfo = {0};
			shInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			shInfo.fMask = NULL;
			shInfo.hwnd = NULL;
			shInfo.lpVerb = L"runas";
			shInfo.lpFile = L"AppUpdater.exe";
			shInfo.lpParameters = appName;
			shInfo.nShow = SW_SHOWNORMAL;
			shInfo.hInstApp = NULL;
			BOOL status = ShellExecuteEx(&shInfo); //Execute the updater
			if(status) {
		        WaitForSingleObject(shInfo.hProcess, INFINITE);

			} else
				MessageBox(0, L"Error while updating", L"", MB_OK);	    
		} else
			//Execute the main app
			ShellExecute(NULL, L"open", appPath, NULL, currentPath , SW_SHOWNORMAL);
	}

	delete[] currentPath;	

	return 0;
}