#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<CommCtrl.h>
#include<cstdio>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,(DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hSpin = GetDlgItem(hwnd, IDC_SPIN_PREFIX);
		SendMessage(hSpin, UDM_SETRANGE32, 0, 32);		
		SetFocus(GetDlgItem(hwnd, IDC_IPADDRESS));
	}
		break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 5;
		CHAR sz_prefix[SIZE]{};
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			/*if (HIWORD(wParam) == EN_CHANGE)
			{
				DWORD dwaddress = 0;
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwaddress);
				BYTE first = FIRST_IPADDRESS(dwaddress);
				BYTE dw_prefix = 0;
				if (first < 128)dw_prefix = 8;
				else if (first < 192)dw_prefix = 16;
				else if (first < 224)dw_prefix = 24;
				CHAR sz_prefix[5]{};
				sprintf(sz_prefix, "%i", dw_prefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}*/
		}
		break;
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				//MessageBox(hwnd, "Prefix changed", "Notification", MB_OK);
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE, (LPARAM)sz_prefix);
				DWORD dw_prefix = atoi(sz_prefix);
				DWORD dw_mask = UINT_MAX;
				//dw_mask >>= (32 - dw_prefix);
				dw_mask <<= (32 - dw_prefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask);
			}
		}
		break;
		}
		
		
	}
	break;
	case WM_NOTIFY:
	{
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		switch (wParam)
		{
		case IDC_IPADDRESS:
		{
			NMIPADDRESS* pAddress = ((NMIPADDRESS *)(lParam));
			if (pAddress->iField == 0)
			{
				DWORD dw_prefix = 0;
				if (pAddress->iValue < 128)dw_prefix = 8;
				else if (pAddress->iValue < 192)dw_prefix = 16;
				else if (pAddress->iValue < 224)dw_prefix = 24;
				CHAR sz_prefix[5]{};
				sprintf(sz_prefix, "%i", dw_prefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}
		}
		break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
