#include <windows.h>
#include "resource.h"

// g (Global optimization), s (Favor small code), y (No frame pointers).
#pragma optimize("gsy", on)
#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK Main(HWND, UINT, WPARAM, LPARAM);

HINSTANCE	hInst;
HWND		hDlg;

int WINAPI WinMain(HINSTANCE hinstCurrent,HINSTANCE hinstPrevious,LPSTR lpszCmdLine,int nCmdShow) {
	hInst = hinstCurrent;
	DialogBox(hinstCurrent,MAKEINTRESOURCE(IDD_MAIN),NULL,(DLGPROC)Main);	
	return 0;
}

LRESULT CALLBACK Main(HWND hDlgMain,UINT uMsg,WPARAM wParam,LPARAM lParam) {	
	PAINTSTRUCT	ps;
	HDC			hdc;
	HMODULE		wInet;
	DWORD		dwError;
	DWORD		dwFormatFlags= FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;
	DWORD		dwSystemLocale= MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);
	TCHAR		*szBuffer;

	hDlg = hDlgMain;

	switch(uMsg){
		case WM_INITDIALOG:
			SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM) LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON)));
			SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM) LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON)));
		break;
		case WM_COMMAND:
			if( LOWORD(wParam)==IDC_CHECK ){
				if( HIWORD(wParam)==BN_CLICKED ){
					szBuffer = (TCHAR*)calloc(1024,sizeof(TCHAR));
					wInet = LoadLibraryEx(TEXT("wininet.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
					dwError = GetDlgItemInt(hDlg, IDC_CODES, NULL, FALSE);
					
					if( !FormatMessage(dwFormatFlags, wInet, dwError, dwSystemLocale, (LPWSTR)&szBuffer, 0, NULL) ){				
						SetDlgItemText(hDlg, IDC_INFO, TEXT("Error not found."));
					}else{
						SetDlgItemText( hDlg, IDC_INFO, szBuffer);
					}
					if( wInet!=NULL ){
						FreeLibrary(wInet);
					}
				}
			}
		break;
		case WM_PAINT:
			hdc = BeginPaint(hDlg, &ps);
			InvalidateRect(hDlg, NULL, TRUE);
			EndPaint (hDlg, &ps);
		break;
		case WM_CLOSE:
			EndDialog(hDlg,wParam);
			DestroyWindow(hDlg);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}
	return 0;
}