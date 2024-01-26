#include "e140.h"
#include "../../Include/Lusbapi.h"

// ������������� ������������ ������
static HINSTANCE hInstance;

//------------------------------------------------------------------------
// ����� ����� � ����������
//------------------------------------------------------------------------
#if defined (__BORLANDC__)
BOOL WINAPI DllEntryPoint(HINSTANCE hInst, DWORD fdwReason, LPVOID /*lpvReserved*/)
#else
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID /*lpvReserved*/)
#endif
{
	if(fdwReason == DLL_PROCESS_ATTACH)
		// �������� ������������� ������������ ������
		hInstance = hInst;
	return TRUE;
}

//------------------------------------------------------------------------
// ���������� ������� ������ ����������
//------------------------------------------------------------------------
DWORD WINAPI GetDllVersion(void)
{
	return CURRENT_VERSION_LUSBAPI;
}

//------------------------------------------------------------------------
// �������� ���������� ������������ ������
//------------------------------------------------------------------------

LPVOID WINAPI CreateLInstance(PCHAR const DeviceName)
{
	LPVOID pDevice;

	// �������� �������� ����������
	if(!DeviceName) return NULL;

	// ������ ��������� �������� ��������� �� ������
	if(!strcmpi(DeviceName, "LUSBBASE")) pDevice = static_cast<ILUSBBASE *>(new TLUSBBASE(hInstance));
	else if(!strcmpi(DeviceName, "E140")) pDevice = static_cast<ILE140 *>(new TLE140(hInstance));
	else pDevice = NULL;
	// ���������� ��������� �� ��, ��� ����������
	return pDevice;
}

