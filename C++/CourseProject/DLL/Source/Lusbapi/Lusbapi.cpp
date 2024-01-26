#include "e140.h"
#include "../../Include/Lusbapi.h"

// идентификатор программного модул€
static HINSTANCE hInstance;

//------------------------------------------------------------------------
// точка входа в библиотеку
//------------------------------------------------------------------------
#if defined (__BORLANDC__)
BOOL WINAPI DllEntryPoint(HINSTANCE hInst, DWORD fdwReason, LPVOID /*lpvReserved*/)
#else
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID /*lpvReserved*/)
#endif
{
	if(fdwReason == DLL_PROCESS_ATTACH)
		// сохраним идентификатор программного модул€
		hInstance = hInst;
	return TRUE;
}

//------------------------------------------------------------------------
// возвращает текущую версию библиотеки
//------------------------------------------------------------------------
DWORD WINAPI GetDllVersion(void)
{
	return CURRENT_VERSION_LUSBAPI;
}

//------------------------------------------------------------------------
// создание экземпл€ра необходимого класса
//------------------------------------------------------------------------

LPVOID WINAPI CreateLInstance(PCHAR const DeviceName)
{
	LPVOID pDevice;

	// проверим название устройства
	if(!DeviceName) return NULL;

	// теперь попробуем выделить интерфейс на модуль
	if(!strcmpi(DeviceName, "LUSBBASE")) pDevice = static_cast<ILUSBBASE *>(new TLUSBBASE(hInstance));
	else if(!strcmpi(DeviceName, "E140")) pDevice = static_cast<ILE140 *>(new TLE140(hInstance));
	else pDevice = NULL;
	// возвращаем указатель на то, что получилось
	return pDevice;
}

