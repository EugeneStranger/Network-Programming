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
CHAR* ParseAddress(CONST CHAR sz_description[], CHAR sz_address[], DWORD dw_address)
{
	sprintf
	(
		sz_address,
		"%s%i.%i.%i.%i\n", sz_description,
		FIRST_IPADDRESS(dw_address),
		SECOND_IPADDRESS(dw_address),
		THIRD_IPADDRESS(dw_address),
		FOURTH_IPADDRESS(dw_address)
	);
	return sz_address;
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
		CONST INT SIZE = 256;
		CONST INT SIZE_INFO = 10240;
		CONST INT SIZE_PREFIX = 5;
		CHAR sz_prefix[SIZE_PREFIX]{};
		CHAR sz_info[SIZE_INFO]{};
		CHAR sz_network_address[SIZE]{};
		CHAR sz_broadcast_address[SIZE]{};
		CHAR sz_number_of_hosts[SIZE]{};
		CHAR sz_first_host_address[SIZE]{};
		CHAR sz_first_last_address[SIZE]{};

		DWORD dw_address = 0;
		DWORD dw_mask = UINT_MAX;
		DWORD dw_prefix = 0;

		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);

		
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			if (HIWORD(wParam) ==EN_CHANGE)
			{
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				if (FIRST_IPADDRESS(dw_address) < 128)dw_prefix = 8;
				else if (FIRST_IPADDRESS(dw_address) < 192)dw_prefix = 16;
				else if (FIRST_IPADDRESS(dw_address) < 224)dw_prefix = 24;
				CHAR sz_prefix[5]{};
				sprintf(sz_prefix, "%i", dw_prefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}
		}
		break;
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				//MessageBox(hwnd, "Prefix changed", "Notification", MB_OK);
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE, (LPARAM)sz_prefix);
				//DWORD dw_adress = 0;
				//DWORD dw_mask = UINT_MAX;
				dw_prefix = atoi(sz_prefix);
				dw_mask <<= (32 - dw_prefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask);
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				DWORD dw_network_address = dw_address & dw_mask;
				DWORD dw_broadcast_address = ~dw_mask | dw_address;
				//sprintf
				//(
				//	sz_network_address,
				//	"Адрес сети:\t\t\t%i.%i.%i.%i",
				//	FIRST_IPADDRESS(dw_network_address),
				//	SECOND_IPADDRESS(dw_network_address),
				//	THIRD_IPADDRESS(dw_network_address),
				//	FOURTH_IPADDRESS(dw_network_address)
				//);
				sprintf(sz_number_of_hosts, "Количество узлов:\t\t%i\n", dw_broadcast_address - dw_network_address - 1);
				sprintf
				(
					sz_info,
					"Info:\n%s%s%s%s%s",
					ParseAddress("Адрес сети:\t\t\t", sz_network_address, dw_network_address),
					ParseAddress("Широковещательный адрес:\t", sz_broadcast_address, dw_broadcast_address),
					sz_number_of_hosts,
					ParseAddress("Начальный IP-адрес:\t\t", sz_first_host_address, dw_network_address + 1),
					ParseAddress("Конечный IP-адрес:\t\t", sz_first_last_address, dw_broadcast_address -1)
				);
				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);
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
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);

		switch (wParam)
		{
		case IDC_IPADDRESS:
		{
			//NMIPADDRESS* pAddress = ((NMIPADDRESS *)(lParam));
			//if (pAddress->iField == 0)
			//{
			//	DWORD dw_prefix = 0;
			//	if (pAddress->iValue < 128)dw_prefix = 8;
			//	else if (pAddress->iValue < 192)dw_prefix = 16;
			//	else if (pAddress->iValue < 224)dw_prefix = 24;
			//	CHAR sz_prefix[5]{};
			//	sprintf(sz_prefix, "%i", dw_prefix);
			//	SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			//}
		}
		break;
		case IDC_IPMASK:
		{
			//NMIPADDRESS* pMask = ((NMIPADDRESS*)(lParam));
			DWORD dw_mask = 0;
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM) & dw_mask);
			DWORD dw_prefix = 32;
			for (; dw_mask & 1 ^ 1;dw_mask>>=1)dw_prefix--;
			CONST INT SIZE = 5;
			CHAR sz_prefix[SIZE]{};
			sprintf(sz_prefix, "%i", dw_prefix);
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
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
