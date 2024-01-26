/* --- wlusbapi.cpp --------------------------------------------------------------------------------------- **
 *
 * 	wlusbapi - "�������" ��������  dll, ��� ������ � ������� �� ���������������
 *	���������(MSVB, NI LabView, ect)
 *
 * 	�������������� ������:
 * 		* E-154
 * 		* E14-140
 * 		* E140-140M
 * 		* E14-440
 * 		* E20-10
 *
 * 	(C) Lcard, 2009.
 * -------------------------------------------------------------------------------------------------------- */

#include <windows.h>
#include <conio.h>                                     
#include "lusbapi.h"
#include "lusbbase.h"
#include "wlusbapi.h"
#include "circularBuf.h"
#pragma argsused

/************************************************************************************************
 *																                                *								
 *                                CONSTANT DEFINITIONS       			    				    *
 *																								*																							
 ***********************************************************************************************/
#define E140_MAX_DAC_FREQ      	 	200.0
#define E440_MAX_DAC_FREQ      	 	125.0
#define E140_M_MAX_DAC_CODE			32767
#define RD_REQ_QNT					0x08

/* ������������ �������� �������� �������� ������ DAC */
static const BYTE dacMaxAmplitudes[ ] =
{
	5, 		/* E154 */
	5, 		/* E140 */
	5, 		/* E440 */
	5		/* E2010 */
};

/* ������������ �������� ���� DAC */
static const WORD dacMaxCodes[ ] =
{
	127, 	/* E154 */
	2047, 	/* E140 */
	2047, 	/* E440 */
	2047  	/* E2010 */
};

/* �������������� �������������� ����������� ��������� */
enum
{
	E154_ID,
	E14_140_ID,
	E14_440_ID,
	E2010_ID,
	SUPPORTED_DEVICES_QUANTITY
};

/* ����� ������� ���, ��� ��� �������� � ������� */
static const CHAR moduleNames[][7] =
{
	"E154",
	"E140",
	"E440",
	"E2010"
};

/* ������� ������ */
static CHAR moduleRev;

/************************************************************************************************
 *																                                *								
 *                                 FUNCTION DECLARATIONS       		    	    	    	    *
 *																								*																							
 ***********************************************************************************************/

/* ����� ������ ������ �� ������ */
DWORD WINAPI ServiceRealTimeReadThread(PVOID /*Context*/);

/* ����� ������ ������ � ������ */
DWORD WINAPI ServiceRealTimeWriteThread(PVOID /*Context*/);

/* ���� � ����������� ������ ��� ������ ������ */
void f_RdEnterCritSect(void);

/* ����� �� ����������� ������ ��� ������ ������ */
void f_RdLeaveCritSect(void);

/************************************************************************************************
 *																                                *								
 *                                VARIABLE DEFINITIONS       			    				    *
 *																								*																							
 ***********************************************************************************************/

/***************************************
 *       	 COMMON variables          *
 **************************************/

/* ���������� ������ ������ */
static HANDLE hReadThread;

/* ���������� ������ ������ */
static HANDLE hWriteThread;

/* ������������� ������ ������ ������ */
static DWORD readThreadId;

/* ������������� ������ ������ ������ */
static DWORD writeThreadId;

/* ����������� ������ */
static CRITICAL_SECTION rdCritSection;
static CRITICAL_SECTION wrCritSection;

/* ������������� ������ */
static WORD moduleId;

/* ��������� �� ��������� ������ */
static LPVOID pModule = NULL;

/* ���������� ���������� */
static HANDLE moduleHandle;

/* C������� ������ ���� USB */
static BYTE usbSpeed;

/* ������ ������ ������ � ��� */
static DWORD dacPacketSize;

/* ��������� �� ����� ��� */
static SHORT *pDacBuf = NULL;

/* ��������� �� ��������� ���������� ���������� ������ ��� */
static CIRBUF_Param *pDacBufParam;

/* ��������� �� ����� ������ ��� */
static SHORT *pDacPacket = NULL;

/* ������� ����������� ������ ��� */
static BOOL dacBufUnderflow;

/* ������� ��������� ��� */
static BOOL dacStopped;

/* ������ ������ ������ � ��� */
static DWORD adcPacketSize;

/* ��������� �� ����� ��� */
static SHORT *pAdcBuf = NULL;

/* ��������� �� ��������� ���������� ���������� ������ ��� */
static CIRBUF_Param *pAdcBufParam;

/* ��������� �� ����� ������ ��� */
static SHORT *pAdcPacket = NULL;

/* ������� ������������ ������ ��� */
static volatile BOOL adcBufOverflow;

/* ������� ��������� ��� */
static volatile BOOL adcStopped;

/* ������� ������ ��� ����� ������ � ��� */
static volatile BOOL readThreadErrDetect;

/* ������� ������ ��� �������� ������ � ��� */
static volatile BOOL writeThreadErrDetect;

/* ������� ���������� ����� ������ */
static volatile BOOL isReadThreadComplete = FALSE;

/* ������� ���������� ������ ������ */
static volatile BOOL isWriteThreadComplete = FALSE;

/* ���� ���������� �������� ����� �������� ��������� ������� */
static BOOL ttlOutEnabled = 0;

/***************************************
 *      MODULE E154 variables          *
 **************************************/

/* ������ ���������� � ������ */
static MODULE_DESCRIPTION_E154 e154Desc;

/* ��������� ������ ��� */
static ADC_PARS_E154 e154AdcParam;

/***************************************
 *      MODULE E140 variables          *
 **************************************/

/* ������ ���������� � ������ */
static MODULE_DESCRIPTION_E140 e140Desc;

/* ��������� ������ ��� */
static ADC_PARS_E140 e140AdcParam;


/***************************************
 *      MODULE E440 variables          *
 **************************************/

/* ������ ���������� � ������ */
static MODULE_DESCRIPTION_E440 e440Desc;

/* C�������� ���������� DAC ������ */
static DAC_PARS_E440 e440DacParam;

/* ��������� ������ ��� */
static ADC_PARS_E440 e440AdcParam;


/***************************************
 *      MODULE E2010 variables         *
 **************************************/

/* ������ ���������� � ������ */
static MODULE_DESCRIPTION_E2010 e2010Desc;

/* ��������� ������ ��� */
static ADC_PARS_E2010 e2010AdcParam;


/************************************************************************************************
 *																                                *								
 *                               	 COMMON FUNCTIONS DEFINITIONS       	    			    *
 *																								*																							
 ***********************************************************************************************/

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	����� ����� � ����������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	switch (reason)
		{
		/* ������������� � ��������� ������������ �������� �������� */
		case DLL_PROCESS_ATTACH:
			break;
		/* ���������� �������� ���� ����� FreeLibrary */
		case DLL_PROCESS_DETACH:
			//LV_StopRealTimeAdcSampling();
			//LV_CloseModule();
			break;
		/* ������� ������� ����� ����� */
		case DLL_THREAD_ATTACH:
			break;
		/* ���������� ������ */
		case DLL_THREAD_DETACH:
			//LV_StopRealTimeAdcSampling();
			//LV_CloseModule();
			break;
		}
    return (1);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	���������� ������� ������ ����������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) DWORD WINAPI LV_GetDllVersion(VOID)
{
    return (GetDllVersion());
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	�������� ������.
 *
 * 	��������:
 * 	����������� ��������� � �������� ������� ���������� ������.
 * 	���� ������ ������ ����������� ��� ���. ���� ��� ������������� ����� ������ ��
 * 	�������������� ���������, ������������ ������������ � ���������� ���������� ������.
 * 	���������� TRUE, ���� ������ ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_OpenModule(PCHAR _moduleName, BYTE* _moduleIdx)
{
    WORD slotIdx;
	CHAR moduleName[16];

	if (_moduleIdx == NULL)
		return FALSE;
		
	*_moduleIdx = 0x00;

	/* �������� ��������� �� �������� �������� ����� ��������� */
    LV_CloseModule();

	/* ����������� ���� ������������� ������.
	 * ��� ����� ��������� ��������� �� ������� ���������,
	 * � �������� ������� ���������� ������. ���� ������
	 * ���������, �������� ��� ���, ������� ����������
	 * �������������� ��� ���.
	 */

	/* ��������� ��������� �� ������� ��������� */
    pModule = CreateLInstance("Lusbbase");
    if (!pModule)
    {
        LV_CloseModule();
		return FALSE;
    }

	/* ������� ���������� ������ E14-1140 � ������
	 * WORD MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI ����������� ������
	 */
    for (slotIdx = 0x0; slotIdx < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; slotIdx++)
    {
        if (((ILUSBBASE*)pModule)->OpenLDevice(slotIdx))
            break;
    }
    // ���-������ ����������?
    if (slotIdx == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI)
    {
        LV_CloseModule();
        return FALSE;
    }

	/* ������ �������� ������ � ������������ ����������� ����� */
    if (!((ILUSBBASE*)pModule)->GetModuleName(moduleName))
    {
        LV_CloseModule();
		return FALSE;
    }
	
	/* �������� ��������� �� ������� ��������� */
	LV_CloseModule();

	/* ��������� �������������� ������ �� ����� */
	if(!strcmpi(moduleName, "E140"))
		moduleId = E14_140_ID;
	else if(!strcmpi(moduleName, "E154"))
		moduleId = E154_ID;
	else if(!strcmpi(moduleName, "E440"))
		moduleId = E14_440_ID;
	else if(!strcmpi(moduleName, "E20-10"))
		moduleId = E2010_ID;
	else
	{	
		*_moduleIdx = 0xff;
		return (FALSE);
	}

	*_moduleIdx = moduleId;

    /* ��������� ��������� �� ��������� ���������� ������ */
    pModule = CreateLInstance((CHAR*)moduleNames[moduleId]);
    if (!pModule)
    {
        LV_CloseModule();
		return FALSE;
    }

	/* �������� ���������� */
	if (!((ILUSBBASE*)pModule)->OpenLDevice(slotIdx))
    {
        LV_CloseModule();
        return FALSE;
    }

    // ��������� ��������� ���������� ����������
    moduleHandle = ((ILUSBBASE*)pModule)->GetModuleHandle();
    if (moduleHandle == INVALID_HANDLE_VALUE)
    {
        LV_CloseModule();
        return FALSE;
    }

    // ��������� �������� �������� ������ ���� USB
    if (!((ILUSBBASE*)pModule)->GetUsbSpeed(&usbSpeed))
    {
        LV_CloseModule();
        return FALSE;
    }

    /* ��� ������� � DSP ���������� ������� ������� �������� */
	switch (moduleId)
	{
		case (E154_ID):
		case (E14_140_ID):
			/* Do nothing */
			break;

		case (E14_440_ID):
			if (!((ILE440*)pModule)->TEST_MODULE())
			{
				// ��� LBIOS'� ������ �� ���������������� ������� ������� DLL ����������
				if (!((ILE440*)pModule)->LOAD_MODULE())
				{
					LV_CloseModule();
					return FALSE;
				}
		
				// �������� �������� ������
				if (!((ILE440*)pModule)->TEST_MODULE())
				{
					LV_CloseModule();
					return FALSE;
				}
			}
			break;

		case (E2010_ID):
			if (!((ILE2010*)pModule)->TEST_MODULE())
			{
				// ��� LBIOS'� ������ �� ���������������� ������� ������� DLL ����������
				if (!((ILE2010*)pModule)->LOAD_MODULE())
				{
					LV_CloseModule();
					return FALSE;
				}

				// �������� �������� ������
				if (!((ILE2010*)pModule)->TEST_MODULE())
				{
					LV_CloseModule();
					return FALSE;
				}
			}
			break;
	}

    /* ��������� ���������� �� ���� ������ */
	switch (moduleId)
	{
		case (E154_ID):
			if (!((ILE154*)pModule)->GET_MODULE_DESCRIPTION(&e154Desc))
			{
				LV_CloseModule();
				return FALSE;
			}
			strcpy(_moduleName, (char*)e154Desc.Module.DeviceName);
			moduleRev = e154Desc.Module.Revision;
			strncat(_moduleName, (char*)(&moduleRev), 1);
			break;

		case (E14_140_ID):
			if (!((ILE140*)pModule)->GET_MODULE_DESCRIPTION(&e140Desc))
			{
				LV_CloseModule();
				return FALSE;
			}
			strcpy(_moduleName, (char*)e140Desc.Module.DeviceName);
			moduleRev = e140Desc.Module.Revision;
			strncat(_moduleName, (char*)(&moduleRev), 1);
			break;

		case (E14_440_ID):
			if (!((ILE440*)pModule)->GET_MODULE_DESCRIPTION(&e440Desc))
			{
				LV_CloseModule();
				return FALSE;
			}
			strcpy(_moduleName, (char*)e440Desc.Module.DeviceName);
			moduleRev = e440Desc.Module.Revision;
			strncat(_moduleName, (char*)(&moduleRev), 1);			
			break;

		case (E2010_ID):
			if (!((ILE2010*)pModule)->GET_MODULE_DESCRIPTION(&e2010Desc))
			{
				LV_CloseModule();
				return FALSE;
			}
			strcpy(_moduleName, (char*)e2010Desc.Module.DeviceName);
			moduleRev = e2010Desc.Module.Revision;
			strncat(_moduleName, (char*)(&moduleRev), 1);
			break;
	}

    return TRUE;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	�������� ������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) VOID WINAPI LV_CloseModule(VOID)
{
    if (pModule != NULL)
    {
		while (hReadThread != NULL || hWriteThread != NULL)
			;
        ((ILUSBBASE*)pModule)->ReleaseLInstance();
        pModule = NULL;
		ttlOutEnabled = 0;
    }
}


/************************************************************************************************
 *																                                *								
 *                               	 ADC FUNCTIONS DEFINITIONS       	    				    *
 *																								*																							
 ***********************************************************************************************/

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ������� ���.
 *
 * 	��������:
 * 		�������������� ���� ��� � ������(���� ������) � ���������� �����������
 * 		��������(���� ������).
 * 		���������� TRUE, ���� �������������� ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
static BOOL WINAPI E154_ProcessOnePoint(const SHORT _sampleCode,
										const WORD _chNum,
										const BYTE _needCalibr,
										const BYTE _transToVolt,
										double* _result
										)
{
    double offset;
	double scale;
    WORD gainIdx;
    const double scaleGain[4] =
    {
        5.0 / 2000., 1.6 / 2000., 0.5 / 2000., 0.16 / 2000.
    };

	if (_result == NULL)
		return FALSE;
	
    // ���������� ������������
    gainIdx = (_chNum >> 6) & 3;
    if (_needCalibr == FALSE)
    {
        // ��� ����������
        if (_transToVolt == FALSE)
        {
            // ���� ���, ��� ����������
            offset = 0;
            scale = 1.0;
        }
        else
        {
            // ������, ��� ����������
            offset = 0;
            scale = scaleGain[gainIdx];
        }
    }
    else
    {
        // � �����������
        if (_transToVolt == FALSE)
        {
            // ���� ���, � �����������
            offset = e154Desc.Adc.OffsetCalibration[gainIdx];
            scale = e154Desc.Adc.ScaleCalibration[gainIdx];
        }
        else
        {
            // ������, � �����������
            offset = e154Desc.Adc.OffsetCalibration[gainIdx];
            scale = e154Desc.Adc.ScaleCalibration[gainIdx] * scaleGain[gainIdx];
        }
    }

    *_result = (_sampleCode + offset) * scale;

    return TRUE;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ������� ���.
 *
 * 	��������:
 * 		�������������� ���� ��� � ������(���� ������) � ���������� �����������
 * 		��������(���� ������).
 * 		���������� TRUE, ���� �������������� ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
static BOOL WINAPI E140_ProcessOnePoint(const SHORT _sampleCode,
										const WORD _chNum,
										const BYTE _needCalibr,
										const BYTE _transToVolt,
										double* _result
									   )
{
	double offset, scale;
	WORD gainIdx;
	double scaleGain[4] = {10.0/8000., 2.5/8000., 0.625/8000., 0.15625/8000.};

	if (_result == NULL)
		return FALSE;
		
    // ���������� ������������
	gainIdx = (_chNum >> 6) & 3;
	if (_needCalibr == FALSE)
	{
		// ��� ����������
		if (_transToVolt == FALSE)
		{
			// ���� ���, ��� ����������
			offset=0;
			scale=1.0;
		}
	    else
		{
			// ������, ��� ����������
			offset=0;
			scale=scaleGain[gainIdx];
		}
	}
	else
	{
		// � �����������
		if (_transToVolt == FALSE)
		{
			// ���� ���, � �����������
			offset=e140Desc.Adc.OffsetCalibration[gainIdx];
			scale=e140Desc.Adc.ScaleCalibration[gainIdx];
		}
		else
		{
			// ������, � �����������
			offset=e140Desc.Adc.OffsetCalibration[gainIdx];
			scale=e140Desc.Adc.ScaleCalibration[gainIdx]*scaleGain[gainIdx];
		}
	}

	*_result = (_sampleCode + offset) * scale;

	return TRUE;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ������� ���.
 *
 * 	��������:
 * 		�������������� ���� ��� � ������(���� ������) � ���������� �����������
 * 		��������(���� ������).
 * 		���������� TRUE, ���� �������������� ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
static BOOL WINAPI E440_ProcessOnePoint(const SHORT _sampleCode,
										const WORD _chNum,
										const BYTE _needCalibr,
										const BYTE _transToVolt,
										double* _result
										)
{
    double offset, scale;
    WORD gainIdx;
    const double scaleGain[4] =
    {
        10.0 / 8000., 2.5 / 8000., 0.625 / 8000., 0.15625 / 8000.
    };

	if (_result == NULL)
		return FALSE;
		
    // ���������� ������������
    gainIdx = (_chNum >> 6) & 3;
    if (_needCalibr == FALSE)
    {
        // ��� ����������
        if (_transToVolt == FALSE)
        {
            // ���� ���, ��� ����������
            offset = 0;
            scale = 1.0;
        }
        else
        {
            // ������, ��� ����������
            offset = 0;
            scale = scaleGain[gainIdx];
        }
    }
    else
    {
        // � �����������
        if (_transToVolt == FALSE)
        {
            // ���� ���, � �����������
            offset = e440Desc.Adc.OffsetCalibration[gainIdx];
            scale = e440Desc.Adc.ScaleCalibration[gainIdx];
        }
        else
        {
            // ������, � �����������
            offset = e440Desc.Adc.OffsetCalibration[gainIdx];
            scale = e440Desc.Adc.ScaleCalibration[gainIdx] * scaleGain[gainIdx];
        }
    }

    *_result = (_sampleCode + offset) * scale;

    return TRUE;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ������� ���.
 *
 * 	��������:
 * 		�������������� ���� ��� � ������(���� ������) � ���������� �����������
 * 		��������(���� ������).
 * 		���������� TRUE, ���� �������������� ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
static BOOL WINAPI E2010_ProcessOnePoint(const SHORT _adcSample,
										 const WORD _chNum,
										 const BYTE _needCalibr,
										 const BYTE _transToVolt,
										 double* _result
										)
{
    double offset, scale;
    WORD gainIdx;
    double scaleGain[3] =
    {
        3.0 / 8000., 1.0 / 8000., 0.3 / 8000.
    };
	
	if (_result == NULL)
		return FALSE;	

    // ���������� ������������
    gainIdx = e2010AdcParam.InputRange[_chNum];
    if (_needCalibr == FALSE)
    {
        // ��� ����������
        if (_transToVolt == FALSE)
        {
            // ���� ���, ��� ����������
            offset = 0;
            scale = 1.0;
        }
        else
        {
            // ������, ��� ����������
            offset = 0;
            scale = scaleGain[gainIdx];
        }
    }
    else
    {
        // � �����������
        if (_transToVolt == FALSE)
        {
            // ���� ���, � �����������
            offset = e2010Desc.Adc.OffsetCalibration[gainIdx];
            scale = e2010Desc.Adc.ScaleCalibration[gainIdx];
        }
        else
        {
            // ������, � �����������
            offset = e2010Desc.Adc.OffsetCalibration[gainIdx];
            scale = e2010Desc.Adc.ScaleCalibration[gainIdx] * scaleGain[gainIdx];
        }
    }

    *_result = (_adcSample + offset) * scale;

    return TRUE;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ������� ���.
 *
 * 	��������:
 * 		�������������� ���� ��� � ������(���� ������) � ���������� �����������
 * 		��������(���� ������).
 * 		���������� TRUE, ���� �������������� ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_ProcessSingleAdcSample(const SHORT _sampleCode,
															const WORD _chNum,
															const BYTE _needCalibr,
															const BYTE _transToVolt,
															double* _result
															)
{
	if (_result == NULL)
		return FALSE;
		
	switch (moduleId)
	{
		case (E154_ID):
			return E154_ProcessOnePoint(_sampleCode, _chNum, _needCalibr, _transToVolt, _result);

		case (E14_140_ID):
			return E140_ProcessOnePoint(_sampleCode, _chNum, _needCalibr, _transToVolt, _result);

		case (E14_440_ID):
			return E440_ProcessOnePoint(_sampleCode, _chNum, _needCalibr, _transToVolt, _result);

		case (E2010_ID):
			return E2010_ProcessOnePoint(_sampleCode, _chNum, _needCalibr, _transToVolt, _result);

      default:
      	return FALSE;
	}
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ������� �������� ���.
 *
 * 	��������:
 * 		�������������� ����� ��� � ������(���� ������) � ���������� �����������
 * 		��������(���� ������).
 * 		���������� TRUE, ���� �������������� ������ �������, ����� FALSE.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_ProcessAdcArray(const SHORT* _pSampleData,
													 const DWORD _size,
													 const BYTE _needCalibr,
													 const BYTE _transToVolt,
													 double* _pResultData
												    )
{

	DWORD i;
	WORD j;
	WORD chQnt = 0;
	WORD chNum;
	const SHORT *pin = _pSampleData;
	double *pout = _pResultData;

	if ((_pSampleData == NULL) || (_pResultData == NULL))
        return FALSE;
	
	switch (moduleId)
	{
		case (E154_ID):
			chQnt = e154AdcParam.ChannelsQuantity;
            break;
		
		case (E14_140_ID):
			chQnt = e140AdcParam.ChannelsQuantity;
            break;

		case (E14_440_ID):
			chQnt = e440AdcParam.ChannelsQuantity;
            break;
		
		case (E2010_ID):
			chQnt = e2010AdcParam.ChannelsQuantity;
            break;
        default:
            return FALSE;  
	}
	if (!chQnt)
		return FALSE;

	for (i = j = 0; i < _size; i++)
	{
		switch (moduleId)
		{
			case (E154_ID):
				chNum = e154AdcParam.ControlTable[j];
				break;
			
			case (E14_140_ID):
				chNum = e140AdcParam.ControlTable[j];
				break;
			
			case (E14_440_ID):
				chNum = e440AdcParam.ControlTable[j];
				break;
			
			case (E2010_ID):
				chNum = e2010AdcParam.ControlTable[j];
				break;
            default:
                return FALSE;
		}

		LV_ProcessSingleAdcSample(*pin,  chNum, _needCalibr, _transToVolt, pout);
		pout++;
		pin++;
		if (++j >= chQnt)
			j = 0;
	}

	return TRUE;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	�������� ����������� ������ ������.
 *
 * 	��������:
 * 		���������� ����� ������ - ��� ��������, ����������� ���������� � ���������� ������
 *		������, �������� � ������ ������ ������� ����������� ������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) WORD WINAPI LV_CreateAdcChannel(const WORD _chNum,
													   const WORD _gain,
													   const WORD _diffMode
													  )
{
    return (_chNum & 0x1F) | ((_gain & 3) << 6) | ((_diffMode & 1) << 5);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ���������� ����������� �����.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI E154_SetAdcSynchroParameters
	(
	const WORD 		_clkSource,						
	const WORD 		_enableClkOutput,				
	const WORD 		_inputMode,							
	const WORD 		_synchroAdType,						
	const WORD 		_synchroAdMode, 						
	const WORD 		_synchroAdChannel,  				
	const SHORT 	_synchroAdPorog, 					
	const WORD 		_channelsQuantity,					
	const WORD* 	_controlTable,					
	const double 	_adcRate,	  			  			
	const double 	_interKadrDelay,		  			
	double* 		_kadrRate,
	double*			_actualAdcRate,
	double*			_actualInterKadrDelay					
	)
{
    int i;

    if (!_channelsQuantity)
        return false;
		
	if ((pModule == NULL) || (_controlTable == NULL))
		return FALSE;

    // ������� ������� ��������� ������ ���
    if (!((ILE154 *)pModule)->GET_ADC_PARS(&e154AdcParam))
        return false;

    // ��������� �������� ��������� ���
    e154AdcParam.ClkSource = _clkSource;
    e154AdcParam.EnableClkOutput = _enableClkOutput;
    e154AdcParam.InputMode = _inputMode;
    e154AdcParam.SynchroAdType = _synchroAdType;
    e154AdcParam.SynchroAdMode = _synchroAdMode;
    e154AdcParam.SynchroAdChannel = _synchroAdChannel;
    e154AdcParam.SynchroAdPorog = _synchroAdPorog;
    e154AdcParam.ChannelsQuantity = _channelsQuantity;

    for (i = 0x0; i < _channelsQuantity; i++)
        e154AdcParam.ControlTable[i] = _controlTable[i];

	e154AdcParam.AdcRate = _adcRate;
    e154AdcParam.InterKadrDelay = _interKadrDelay;

    // ��������� ��������� ��������� ������ ��� � ������
    if (!((ILE154 *)pModule)->SET_ADC_PARS(&e154AdcParam))
        return false;

    if (!((ILE154 *)pModule)->GET_ADC_PARS(&e154AdcParam))
        return false;

	if (_kadrRate != NULL)
		*_kadrRate = e154AdcParam.KadrRate;
	if (_actualAdcRate != NULL)
		*_actualAdcRate = e154AdcParam.AdcRate;
	if (_actualInterKadrDelay != NULL)
		*_actualInterKadrDelay = e154AdcParam.InterKadrDelay;

    return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ���������� ����������� �����.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI E140_SetAdcSynchroParameters
	(
	const WORD _clkSource,
	const WORD _enableClkOutput,
	const WORD _inputMode,
	const WORD _synchroAdType,
	const WORD _synchroAdMode,
	const WORD _synchroAdChannel,
	const SHORT _synchroAdPorog,
	const WORD  _channelsQuantity,
	const WORD* _controlTable,
	double 		_adcRate,						
	double 		_interKadrDelay,
	double* 	_kadrRate,									
	double* 	_actualAdcRate,
	double* 	_actualInterKadrDelay
	)
{
    int i;

    if (!_channelsQuantity)
        return false;
		
	if ((pModule == NULL) || (_controlTable == NULL))
		return FALSE;		

    // ������� ������� ��������� ������ ���
    if (!((ILE140 *)pModule)->GET_ADC_PARS(&e140AdcParam))
        return false;

    // ��������� �������� ��������� ���
	e140AdcParam.ClkSource = _clkSource;
	e140AdcParam.EnableClkOutput = _enableClkOutput;
    e140AdcParam.InputMode = _inputMode;
    e140AdcParam.SynchroAdType = _synchroAdType;
    e140AdcParam.SynchroAdMode = _synchroAdMode;
    e140AdcParam.SynchroAdChannel = _synchroAdChannel;
    e140AdcParam.SynchroAdPorog = _synchroAdPorog;
    e140AdcParam.ChannelsQuantity = _channelsQuantity;

    for (i = 0x0; i < _channelsQuantity; i++)
        e140AdcParam.ControlTable[i] = _controlTable[i];

    e140AdcParam.AdcRate = _adcRate;
	e140AdcParam.InterKadrDelay = _interKadrDelay;

    // ��������� ��������� ��������� ������ ��� � ������
    if (!((ILE140 *)pModule)->SET_ADC_PARS(&e140AdcParam))
        return false;

    if (!((ILE140 *)pModule)->GET_ADC_PARS(&e140AdcParam))
        return false;

	if (_kadrRate != NULL)
		*_kadrRate = e140AdcParam.KadrRate;
	if (_actualAdcRate != NULL)
		*_actualAdcRate = e140AdcParam.AdcRate;
	if (_actualInterKadrDelay != NULL)
		*_actualInterKadrDelay = e140AdcParam.InterKadrDelay;

    return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ���������� ����������� �����.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI E440_SetAdcSynchroParameters
	(
	const WORD 		_isCorrectionEnabled,		
	const WORD 		_adcClockSource,					
	const WORD 		_inputMode,						
	const WORD 		_synchroAdType,				
	const WORD 		_synchroAdMode, 					
	const WORD 		_synchroAdChannel,  			
	const SHORT 	_synchroAdPorog, 				
	const WORD 		_channelsQuantity,				
	const WORD* 	_controlTable,	
	const double 	_adcRate,	  			  		
	const double 	_interKadrDelay,		  					
	const WORD 		_adcFifoBaseAddress,			
	const WORD 		_adcFifoLength,					
	const double* 	_adcOffsetCoefs,	
	const double* 	_adcScaleCoefs,
	double* 		_kadrRate,
	double* 		_actualAdcRate,
	double* 		_actualInterKadrDelay	
	)
{
	int i;
	
	if (!_channelsQuantity)
		return false;
		
	if ((pModule == NULL) || (_controlTable == NULL))
		return FALSE;			
	
	// ������� ������� ��������� ������ ���
	if(!((ILE440 *)pModule)->GET_ADC_PARS(&e440AdcParam))
		return false;
	
	// ��������� �������� ��������� ���
	e440AdcParam.IsCorrectionEnabled = (_isCorrectionEnabled > 0) ? (TRUE) : (FALSE);
	e440AdcParam.AdcClockSource = _adcClockSource;
	e440AdcParam.InputMode = _inputMode;
	e440AdcParam.SynchroAdType = _synchroAdType;
	e440AdcParam.SynchroAdMode = _synchroAdMode;
	e440AdcParam.SynchroAdChannel = _synchroAdChannel;
	e440AdcParam.SynchroAdPorog = _synchroAdPorog;
	e440AdcParam.ChannelsQuantity = _channelsQuantity;

	for (i = 0x0; i < _channelsQuantity; i++)
		e440AdcParam.ControlTable[i] = _controlTable[i];

   if (_isCorrectionEnabled)
   {
	if (_adcOffsetCoefs != NULL && _adcScaleCoefs != NULL)
	{
		for (i = 0x0; i < _channelsQuantity; i++)
		{
			e440AdcParam.AdcOffsetCoefs[i] = _adcOffsetCoefs[i];
			e440AdcParam.AdcScaleCoefs[i] = _adcScaleCoefs[i];
		}
	}
	else
	{
		for (i = 0x0; i < _channelsQuantity; i++)
		{
			e440AdcParam.AdcOffsetCoefs[i] = e440Desc.Adc.OffsetCalibration[i];
			e440AdcParam.AdcScaleCoefs[i] = e440Desc.Adc.ScaleCalibration[i];
		}
	}
   }
	
	e440AdcParam.AdcRate = _adcRate;
	e440AdcParam.InterKadrDelay = _interKadrDelay;
	e440AdcParam.AdcFifoBaseAddress = _adcFifoBaseAddress;
	e440AdcParam.AdcFifoLength = _adcFifoLength;

	// ��������� ��������� ��������� ������ ��� � ������
	if(!((ILE440 *)pModule)->SET_ADC_PARS(&e440AdcParam)) return false;
	
	if(!((ILE440 *)pModule)->GET_ADC_PARS(&e440AdcParam)) return false;
	
	if (_kadrRate)
		*_kadrRate = e440AdcParam.KadrRate;
	if (_actualAdcRate)
		*_actualAdcRate = e440AdcParam.AdcRate;
	if (_actualInterKadrDelay)
		*_actualInterKadrDelay = e440AdcParam.InterKadrDelay;
	
	return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	��������� ���������� ����������� �����.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI E2010_SetAdcSynchroParameters
	(
	const WORD	 	_isAdcCorrectionEnabled,
	const WORD 	 	_overloadMode,
	const WORD 	 	_inputCurrentControl,
	const WORD	 	_startSource,				
	const DWORD  	_startDelay, 					
	const WORD	 	_synchroSource,				
	const DWORD  	_stopAfterNKadrs,				
	const WORD	 	_synchroAdMode,   				
	const WORD	 	_synchroAdChannel,			
	const SHORT  	_synchroAdPorog,  				
	const BYTE	 	_isBlockDataMarkerEnabled,	
	const WORD 	 	_channelsQuantity,
	const WORD*	 	_controlTable,
	const WORD*	 	_inputRange,
	const WORD*	 	_inputSwitch,
	const double 	_adcRate,
	const double 	_interKadrDelay,
	const double*	_adcOffsetCoefs,
	const double*	_adcScaleCoefs,
	double*			_kadrRate,
	double* 	 	_actualAdcRate,
	double*		 	_actualInterKadrDelay
    )
{
	WORD i, j;
	
	if (!_channelsQuantity)
		return false;
	
	if ((pModule == NULL) || (_controlTable == NULL) || (_inputRange == NULL) || (_inputSwitch == NULL))
		return FALSE;	
		
	// ������� ������� ��������� ������ ���
	if(!((ILE2010 *)pModule)->GET_ADC_PARS(&e2010AdcParam))
		return false;
	
	/* ��������� ���������� ���.
	 */
	e2010AdcParam.IsAdcCorrectionEnabled = (_isAdcCorrectionEnabled > 0) ? (TRUE) : (FALSE);		
	e2010AdcParam.OverloadMode = _overloadMode;
	e2010AdcParam.InputCurrentControl = _inputCurrentControl;
	e2010AdcParam.SynchroPars.StartSource = _startSource;
	e2010AdcParam.SynchroPars.StartDelay = _startDelay;
	e2010AdcParam.SynchroPars.SynhroSource = _synchroSource;
	e2010AdcParam.SynchroPars.StopAfterNKadrs = _stopAfterNKadrs;
	e2010AdcParam.SynchroPars.SynchroAdMode = _synchroAdMode;
	e2010AdcParam.SynchroPars.SynchroAdChannel = _synchroAdChannel;
	e2010AdcParam.SynchroPars.SynchroAdPorog = _synchroAdPorog;
	e2010AdcParam.ChannelsQuantity = _channelsQuantity; 	
	
	/* ������������ ����������� ������� */	
	for (i = 0x0; i < _channelsQuantity; i++)
		e2010AdcParam.ControlTable[i] = _controlTable[i];

	/* �������� �������� ���������� � ��� ����������� �������� ������� */
	for (i = 0x0; i < _channelsQuantity; i++)
	{
		e2010AdcParam.InputRange[i] = _inputRange[i];  		// ������� ���������
		e2010AdcParam.InputSwitch[i] = _inputSwitch[i];		// �������� ����� - ������
	}

	// ������� � ��������� ���������� ������ ��� ���������������� ������������ ���
	for (i = 0x0; i < ADC_INPUT_RANGES_QUANTITY_E2010; i++)
	{	
		if (_adcOffsetCoefs != NULL && _adcScaleCoefs != NULL)
		{
			// ������������� ��������
			e2010AdcParam.AdcOffsetCoefs[i][j] = _adcOffsetCoefs[j + i*ADC_CHANNELS_QUANTITY_E2010];
			// ������������� ��������
			e2010AdcParam.AdcScaleCoefs[i][j] = _adcScaleCoefs[j + i*ADC_CHANNELS_QUANTITY_E2010];
		}
		else
		{
			for (j = 0x0; j < _channelsQuantity; j++)
			{
				// ������������� ��������
				e2010AdcParam.AdcOffsetCoefs[i][j] = e2010Desc.Adc.OffsetCalibration[j + i*ADC_CHANNELS_QUANTITY_E2010];
				// ������������� ��������
				e2010AdcParam.AdcScaleCoefs[i][j] = e2010Desc.Adc.ScaleCalibration[j + i*ADC_CHANNELS_QUANTITY_E2010];
			}
		}
	}
		
	/* ������� ������������� */
	e2010AdcParam.AdcRate = _adcRate;

	/* ����������� �������� */
	e2010AdcParam.InterKadrDelay = _interKadrDelay;
	
	// ��������� ��������� ��������� ������ ��� � ������
	if(!((ILE2010 *)pModule)->SET_ADC_PARS(&e2010AdcParam)) return false;
	
	if(!((ILE2010 *)pModule)->GET_ADC_PARS(&e2010AdcParam)) return false;
	
	if (_kadrRate)
		*_kadrRate = e2010AdcParam.KadrRate;
	if (_actualInterKadrDelay)
		*_actualInterKadrDelay = e2010AdcParam.InterKadrDelay;
	if (_actualAdcRate)
		*_actualAdcRate = e2010AdcParam.AdcRate;
	
	return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	������ ����� ������ � ��� � �������� �������.
 *
 * 	��������:
 * 		_bufSize - ������ ������ � ����������� ������
 * 		_packetSize - ������ ������.
 *
 * 		��������! ������ ������ ������ ���� ������ ������� ������. ���� ������ ������ ��������
 * 		������������� �� ������������� ����� �������, ������ ������ ����� ���������� � ����������
 * 		�������� ��������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_StartRealTimeAdcSampling(const DWORD _bufSize, const DWORD _packetSize)
{
	DWORD bufSize = 1;
	DWORD packetSize = _packetSize;

	switch (moduleId)
	{
		case (E154_ID):
			if (!e154AdcParam.ChannelsQuantity)
				return false;
			break;

		case (E14_140_ID):
			if (!e140AdcParam.ChannelsQuantity)
				return false;
			break;

		case (E14_440_ID):
			if (!e440AdcParam.ChannelsQuantity)
				return false;
			break;

		case (E2010_ID):
			if (!e2010AdcParam.ChannelsQuantity)
				return false;
			break;
	}

	if (_bufSize < 1024 || (_bufSize & 1))
        return false;

	while (1)
	{
		bufSize *= 2;
		if (bufSize >= _bufSize)
			break;
	}

    if (packetSize > bufSize / 2)
        packetSize = bufSize / 2;

    if (packetSize < 32)
        packetSize = 32;

    if (packetSize % 32)
    {
        packetSize = (packetSize / 32);
        packetSize = (packetSize + 1)*32;
    }

	adcStopped = 0;
    isReadThreadComplete = FALSE;
	readThreadErrDetect = FALSE;
	hReadThread = NULL;

    /* ��������� ������ ��� ����� */
    pAdcBuf = new SHORT[bufSize];
    if (NULL == pAdcBuf)
    {
        return (false);
    }

    /* ��������� ������ ��� ����� */
    pAdcPacket = new SHORT[RD_REQ_QNT*packetSize];
    if (NULL == pAdcPacket)
    {
        free(pAdcBuf);
        return (false);
    }

    /* ��������� ������ ��� ��������� ������ */
    pAdcBufParam = new CIRBUF_Param;
    if (NULL == pAdcBufParam)
    {
        free(pAdcBuf);
        free(pAdcPacket);
        return (false);
    }

    /* ������������� ������ */
    CIRBUF_Init(bufSize, pAdcBufParam);

	/* ����������� ������ */
	InitializeCriticalSection(&rdCritSection);
	pAdcBufParam->EnterCriticalSection = f_RdEnterCritSect;
	pAdcBufParam->LeaveCriticalSection = f_RdLeaveCritSect;

	/* �������� � ������ ������ ������ ������ */
    hReadThread = CreateThread(0, 0x2000, ServiceRealTimeReadThread, 0, 0, &readThreadId);
    if (!hReadThread)
	{	
        free(pAdcBuf);
        free(pAdcPacket);
        free(pAdcBufParam);
        return (false);
	}
	adcPacketSize = packetSize;

    if (readThreadErrDetect)
        return false;

    return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		��������� ������ � �������� ������� ������ � ���.
 *
 * 	��������:
 * 		������������� ������ ������� ��������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_StopRealTimeAdcSampling(void)
{
    adcStopped = 1;

    for (; !isReadThreadComplete; )
    {
        Sleep(20);
    }

    // ��� ��������� ������ ������ ����� ������
    WaitForSingleObject(hReadThread, INFINITE);
    //TerminateThread(hReadThread, 0);
    DeleteCriticalSection(&rdCritSection);

    CloseHandle(hReadThread);
    hReadThread = NULL;
    free(pAdcBuf);
    free(pAdcPacket);
    free(pAdcBufParam);
    pAdcBuf = NULL;
    pAdcPacket = NULL;
	pAdcBufParam = NULL;

    if (readThreadErrDetect)
        return false;

    return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		��������� ������ � ��� � ���������� ������.
 *
 * 	��������:
 * 		_pData - ��������� �� ����� ���� ����� �������� ������.
 * 		_sampleQnt - ���������� ������� ������� ���� ������� �� ������.
 * 		_timeoutMs - ����-��� �������� ������, ��.
 * 		_waitComplete - ���� �������� ������ ����, ��������� ��������� �������� ������ ��
 * 						��������� ����-����.
 * 		_pProcessedSampleQnt - ���������� ���������� � ����� �������.
 * 		_restSampleQnt - ���������� ���������� �������.
 * 		_overflow - ���� ������������ ������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_GetRealTimeAdcData(SHORT* _pData, 																												
														const DWORD _sampleQnt,
														const DWORD _timeoutMs,
														const BYTE _waitToComplete,
                                                        DWORD* _pProcessedSampleQnt,
														DWORD* _restSampleQnt,
                                                        BYTE* _overflow
													    )
{
    DWORD n_ready, n_to_send;
    DWORD time_waited;

	if (pAdcBufParam == NULL || _pData == NULL)
		return FALSE;
	
	if (_pProcessedSampleQnt != NULL)
		*_pProcessedSampleQnt = 0;
		
    time_waited = 0;

	/* �������� ����������� ���������� ���������� ������
	 * � ����� ��� ��������� ����-����.
	 */
    for (; ;)
    {
		n_ready = CIRBUF_GetDataSize(pAdcBufParam);
		if (_restSampleQnt != NULL)
			*_restSampleQnt = n_ready;

        if (n_ready >= _sampleQnt)
        {
            n_to_send = _sampleQnt;
            break;
        }

        if (!_waitToComplete)
        {
            n_to_send = n_ready;
            break;
        }

        if (readThreadErrDetect)
        {
			if (_overflow != NULL)
				*_overflow = adcBufOverflow;
            return false;
        }

        Sleep(10);
        time_waited += 10;

        if (_timeoutMs > 0)
        {
        		if (time_waited > _timeoutMs)
        		{
				if (_overflow != NULL)
					*_overflow = adcBufOverflow;
            	return false;
        		}
        }
    }


    if (!n_to_send)
    {
		if (_restSampleQnt != NULL)
        *_restSampleQnt = n_ready;
		if (_overflow != NULL)
			*_overflow = adcBufOverflow;
        return true;
    }

	/* ���������� ������ �� ������ */
	CIRBUF_POP_MULTIPLE_FRONT(pAdcBuf, (*pAdcBufParam), _pData, n_to_send);

	if (_restSampleQnt != NULL)
		*_restSampleQnt = n_ready - n_to_send;
	if (_pProcessedSampleQnt)
		*_pProcessedSampleQnt = n_to_send;
	if (_overflow != NULL)
		*_overflow = adcBufOverflow;

    return readThreadErrDetect ? false : true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		����������� ��������� ������ � ���.
 *
 * 	��������:
 * 		_chNum - ����� ������ ���.
 * 		_needCalibr - ������� ������������� ���������� ������������� �������������.
 * 		_error - ������� ������.
 *
 * 		���������� ���������� ��������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) double WINAPI LV_DoSingleAdcSample(const WORD _chNum, const BYTE _needCalibr, BYTE* _error)
{
    double f = 0.0;
    SHORT adc;
    BOOL result;

	if (pModule == NULL)
		{
		*_error = 0;
		return 0.0;
		}
	
	/* ��������� ������ � ��� */
	switch (moduleId)
	{
		case (E154_ID):
			result = ((ILE154*)pModule)->ADC_SAMPLE(&adc, _chNum);
			break;

		case (E14_140_ID):
			result = ((ILE140*)pModule)->ADC_SAMPLE(&adc, _chNum);
			break;

		case (E14_440_ID):
			result = ((ILE440*)pModule)->ADC_SAMPLE(&adc, _chNum);
			break;

		case (E2010_ID):
			result = FALSE;
			break;
	}
	if (_error != NULL)
		*_error = (result == TRUE) ? 1 : 0;
    if (result == FALSE)
        return f;

	/* ��������� ���������� ������ */
	switch (moduleId)
	{
		case (E154_ID):
			result = E154_ProcessOnePoint(adc, _chNum, _needCalibr, TRUE, &f);
			break;

		case (E14_140_ID):
			result = E140_ProcessOnePoint(adc, _chNum, _needCalibr, TRUE, &f);
			break;

		case (E14_440_ID):
			result = E440_ProcessOnePoint(adc, _chNum, _needCalibr, TRUE, &f);
			break;

		case (E2010_ID):
			result = E2010_ProcessOnePoint(adc, _chNum, _needCalibr, TRUE, &f);
			break;
	}
	if (_error != NULL)
		*_error = (result == TRUE) ? 1 : 0;

    return f;
}


/************************************************************************************************
 *																                                *								
 *                               	 DAC FUNCTIONS DEFINITIONS       	    				    *
 *																								*																							
 ***********************************************************************************************/

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		��������� ���������� ���.
 *
 * 	��������:
 * 		_rate - ������� ������� �� �����.
 * 		_syncWithAdc - ���������������� ������ �  ������ ���.
 * 		_zeroOnStop - ��� ��������� ���������� ������ ���. �� ������ ��� ������� ��������.
 * 		_actualRate - ������� ������������� ������� ������� �� �����.
 *
 * 		���������� true, ���� ������������ ������ �������, ����� false.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_E140_SetDacSyncParameters(const double _rate,
                                                               const BYTE _syncWithAdc,
                                                               const BYTE _zeroOnStop,
                                                               double* _actualRate
                                                              )
{
    DAC_PARS_E140 dp;
    double 		  rate;

	if (pModule == NULL)
		return FALSE;
	if (_actualRate != NULL)
		*_actualRate = 0;

	/* �������� ������� ��� */
	if (DAC_INACCESSIBLED_E140 == e140Desc.Dac.Active)
		return (FALSE);

    /* ��������� ������� ���������� ������ ��� */
    if (!((ILE140 *)pModule)->GET_DAC_PARS(&dp))
        return false;

    /* �������� ������������ ���������� */
    rate = (_rate > E140_MAX_DAC_FREQ) ? (E140_MAX_DAC_FREQ) : (_rate);

    /* ��������� ��������� ���������� ��� */
    dp.DacRate = rate;
    dp.SyncWithADC = _syncWithAdc;
    dp.SetZeroOnStop = _zeroOnStop;

    /* �������� ���������� � ����� */
    if (!((ILE140 *)pModule)->SET_DAC_PARS(&dp))
        return false;

    if (!((ILE140 *)pModule)->GET_DAC_PARS(&dp))
        return false;

	if (_actualRate != NULL)
		*_actualRate = dp.DacRate;
    return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		��������� ���������� ���.
 *
 * 	��������:
 * 		_rate - ������� ������� �� �����.
 * 		_fifoLength - ����� ������ FIFO.
 * 		_actualRate - ������� ������������� ������� ������� �� �����.
 *
 * 		���������� true, ���� ������������ ������ �������, ����� false.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_E440_SetDacSyncParameters(const double _rate,
                                                               const WORD _fifoLength,
                                                               double* _actualRate
                                                              )
{
    DAC_PARS_E440 dp;
    double 		  rate;

	if (pModule == NULL)
		return FALSE;
	if (_actualRate != NULL)	
	*_actualRate = 0;

	/* �������� ������� ��� */
	if (DAC_INACCESSIBLED_E440 == e440Desc.Dac.Active)
		return (FALSE);

    /* ��������� ������� ���������� ������ ��� */
    if (!((ILE440 *)pModule)->GET_DAC_PARS(&dp))
        return false;

    /* �������� ������������ ���������� */
    rate = (_rate > E440_MAX_DAC_FREQ) ? (E440_MAX_DAC_FREQ) : (_rate);

    /* ��������� ��������� ���������� ��� */
    dp.DacRate = rate;
	dp.DacFifoBaseAddress = 0x3000;
    dp.DacFifoLength = _fifoLength;
    dp.DacEnabled = 1;
	if (_actualRate != NULL)
		*_actualRate = rate;

    /* �������� ���������� � ����� */
    if (!((ILE440 *)pModule)->SET_DAC_PARS(&dp))
        return false;

    if (!((ILE440 *)pModule)->GET_DAC_PARS(&dp))
        return false;

	if (_actualRate != NULL)	
		*_actualRate = dp.DacRate;
    return true;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *  	������ �������� ������ ��� � �������� �������.
 *
 * 	��������:
 * 		_bufSize - ������ ������ � ����������� ������
 * 		_packetSize - ������ ������.
 *
 * 		��������! ������ ������ ������ ���� ������ ������� ������. ���� ������ ������ ��������
 * 		������������� �� �������������� ����� �������, ������ ������ ����� ���������� � ����������
 * 		�������� ��������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_StartRealTimeDacSampling(const DWORD _bufSize, const DWORD _packetSize)
{
	DWORD bufSize = 1;
	DWORD packetSize = _packetSize;

	if (_bufSize < 1024 || (_bufSize & 1))
        return false;

	while (1)
	{
		bufSize *= 2;
		if (bufSize >= _bufSize)
			break;
	}

    if (packetSize > bufSize / 2)
        packetSize = bufSize / 2;

    if (packetSize < 32)
        packetSize = 32;

    if (packetSize % 32)
    {
        packetSize = (packetSize / 32);
        packetSize = (packetSize + 1) * 32;
    }

	dacStopped = 0;
    isWriteThreadComplete = FALSE;
	writeThreadErrDetect = FALSE;
	hWriteThread = NULL;

    /* ��������� ������ ��� ����� */
    pDacBuf = new SHORT[bufSize];
    if (NULL == pDacBuf)
    {
        return (false);
    }

    /* ��������� ������ ��� ����� */
    pDacPacket = new SHORT[2 * packetSize];
    if (NULL == pDacPacket)
    {
        free(pDacBuf);
        return (false);
    }

    /* ��������� ������ ��� ��������� ������ */
    pDacBufParam = new CIRBUF_Param;
    if (NULL == pDacBufParam)
    {
        free(pDacBuf);
        free(pDacPacket);
        return (false);
    }

    /* ������������� ������ */
    CIRBUF_Init(bufSize, pDacBufParam);

	/* ����������� ������ */
	InitializeCriticalSection(&wrCritSection);
	pDacBufParam->EnterCriticalSection = NULL;//f_WrEnterCritSect;
	pDacBufParam->LeaveCriticalSection = NULL;//f_WrLeaveCritSect;

    /* �������� � ������ ������ �������� ������ */
    hWriteThread = CreateThread(0, 0x2000, ServiceRealTimeWriteThread, 0, 0, &writeThreadId);
    //SetThreadPriority(hWriteThread, THREAD_PRIORITY_ABOVE_NORMAL);
    if (!hWriteThread)
    {
        free(pDacBuf);
        free(pDacPacket);
        free(pDacBufParam);
        return (false);
    }

    dacPacketSize = packetSize;

    if (writeThreadErrDetect)
        return (false);

    return (true);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		��������� �������� ������ � �������� �������.
 *
 * 	��������:
 * 		������������� ������ ������� ��������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_StopRealTimeDacSampling(void)
{
    dacStopped = 1;

    /* �������� ���������� �������� ������ */
    for (; !isWriteThreadComplete;)
    {
        Sleep(20);
    }

    /* �������� ���������� ������ ������ ������ */
    WaitForSingleObject(hWriteThread, INFINITE);
    //TerminateThread(hWriteThread, 0);
    DeleteCriticalSection(&wrCritSection);

    /* ������������ ������� �������� */
    CloseHandle(hWriteThread);
    hWriteThread = NULL;
    free(pDacBuf);
    free(pDacPacket);
    free(pDacBufParam);
    pDacBuf = NULL;
    pDacPacket = NULL;

    if (writeThreadErrDetect)
        return (false);

    return (true);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		������ ������ � ��� � �������� �������.
 *
 * 	��������:
 * 		_pData ��������� �� ������:
 * 		_pointQnt - ���������� ������������ �����;
 * 		_pointProcessed - ���������� ���������� ���������� �����;
 * 		_pointFreeQnt - ���������� ����� � ������;
 * 		_timeout - ������� �������� ������, ��;
 * 		_underflow - ���� ����������� ������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_WriteRealTimeData(const SHORT *_pData,
                                                       const DWORD _pointQnt,
                                                       const DWORD _timeout,
                                                       DWORD *_pointProcessed,
                                                       DWORD *_pointFreeQnt,
                                                       BYTE *_underflow
                                                      )
{
    DWORD writeDataSize;
    DWORD restDataSize;
    DWORD timePassed = 0;

	if (_pData == NULL || _pointProcessed == NULL || _pointFreeQnt == NULL)
		return FALSE;
	
	if (_pointProcessed)
		*_pointProcessed = 0;
	if (_underflow)
		*_underflow = dacBufUnderflow;
    while (true)
    {
        /* ���������� ������� ���������� ����� */
        *_pointFreeQnt = CIRBUF_GetEmptySize(pDacBufParam);

        /* ���������� ������� ������������ ������ */
        restDataSize = _pointQnt - *_pointProcessed;

        /* ��������� ������ � ����� */
        if (restDataSize)
        {
            /* ���������� ������� ������������ ������ */
            writeDataSize = (restDataSize > *_pointFreeQnt) ? *_pointFreeQnt : restDataSize;

            /* ��������� ������ � ����� */
            CIRBUF_PUSH_MULTIPLE_BACK(pDacBuf, (*pDacBufParam), (&_pData[*_pointProcessed]), writeDataSize);

            /* ���������� ���������� ���������� ������ */
            *_pointProcessed += writeDataSize;

            /* ������������� ������� ���������� ����� */
            *_pointFreeQnt -= writeDataSize;

            /* E��� ��� ������ �������� � ����� ������� */
            if (*_pointProcessed == _pointQnt)
                return (true);

            /* �������� ����-���� */
            timePassed += 10;
            if (timePassed > _timeout)
            {
                return (false);
            }
            Sleep(10);
        }
        else
            break;
    }

    return (true);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		����������� ����� ���������� �� ���.
 *
 * 	��������:
 * 		_voltage - ��������������� ����������;
 * 		_dacNum - ����� ������ ���;
 * 		_needCalibr - ������������� ����������;
 *
 * 		���������� true, ���� �������� ������ �������, ����� false.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_SetDacVoltage(const double _voltage,
												   const WORD _dacNum,
												   const BYTE _needCalibr
												  )
{
	WORD maxDacCode;
	double maxDacAmp;
    SHORT val;

	if (pModule == NULL)
		return FALSE;
	
	maxDacCode = dacMaxCodes[moduleId];
	maxDacAmp = dacMaxAmplitudes[moduleId];

	/* ���������� � ����� ��� */
	val = (SHORT)(_voltage * (maxDacCode / maxDacAmp));

	switch (moduleId)
	{
		case (E154_ID):
			if (_needCalibr == TRUE)
				val = (val + e154Desc.Dac.OffsetCalibration[_dacNum]) *
					  e154Desc.Dac.ScaleCalibration[_dacNum];
			return (BOOL) ((ILE154 *)pModule)->DAC_SAMPLE(&val, _dacNum);

		case (E14_140_ID):
			if (_needCalibr == TRUE)
				val = (val + e140Desc.Dac.OffsetCalibration[_dacNum]) *
					  e140Desc.Dac.ScaleCalibration[_dacNum];
			return (BOOL) ((ILE140 *)pModule)->DAC_SAMPLE(&val, _dacNum);

		case (E14_440_ID):
			if (_needCalibr == TRUE)
				val = (val + e440Desc.Dac.OffsetCalibration[_dacNum]) *
					  e440Desc.Dac.ScaleCalibration[_dacNum];
			return (BOOL) ((ILE440 *)pModule)->DAC_SAMPLE(&val, _dacNum);

		case (E2010_ID):
			if (_needCalibr == TRUE)
				val = (val + e2010Desc.Dac.OffsetCalibration[_dacNum]) *
					  e2010Desc.Dac.ScaleCalibration[_dacNum];
			return (BOOL) ((ILE2010 *)pModule)->DAC_SAMPLE(&val, _dacNum);

		default:
			return FALSE;
	}
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		�������������� �������� �������� ��� � ������� � ���� � �� ����������.
 *
 * 	��������:
 * 		_data - ������ �������� ��� � �������;
 * 		_size - ���������� �����;
 * 		_needCalibr - ������������� ����������;
 * 		_result - ������ ����������.
 *
 * 		���������� true, ���� �������� ������ �������, ����� false.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_ProcessDacData(const double* _data,
													const DWORD _size,
													const BYTE _needCalibr,
													SHORT* _result
													)
{
    WORD dacNum;
    DWORD idx;
    double offset;
    double scale;
	WORD maxDacCode;
	double maxDacAmp;
	SHORT val;

	if (_data == NULL || _result == NULL)
		return FALSE;
	
	if (E14_140_ID == moduleId && moduleRev == 'B')
	{
		maxDacCode = E140_M_MAX_DAC_CODE;
	}
	else
		maxDacCode = dacMaxCodes[moduleId];
	maxDacAmp = dacMaxAmplitudes[moduleId];

    for (idx = 0; idx < _size; idx++)
    {
        /* ������� �������� � ������� � ���� */
		val = (SHORT)(_data[idx] * (maxDacCode / maxDacAmp));

        /* ���������� */
        if (_needCalibr)
        {
            dacNum = idx % 2;

            /* ���������� � ������ ������������� */
			switch (moduleId)
			{
				case (E154_ID):
					offset = e154Desc.Dac.OffsetCalibration[dacNum];
					scale = e154Desc.Dac.ScaleCalibration[dacNum];
					break;
		
				case (E14_140_ID):
					offset = e140Desc.Dac.OffsetCalibration[dacNum];
					scale = e140Desc.Dac.ScaleCalibration[dacNum];
					break;
		
				case (E14_440_ID):
					offset = e440Desc.Dac.OffsetCalibration[dacNum];
					scale = e440Desc.Dac.ScaleCalibration[dacNum];
					break;
		
				case (E2010_ID):
					offset = e2010Desc.Dac.OffsetCalibration[dacNum];
					scale = e2010Desc.Dac.ScaleCalibration[dacNum];
					break;
			}
            _result[idx] = (val + offset) * scale;
        }
		else
			_result[idx] = val;
    }

    return (TRUE);
}

/************************************************************************************************
 *																                                *								
 *                              DIGITAL I/O FUNCTIONS DEFINITIONS         					    *
 *																								*																							
 ***********************************************************************************************/

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		������ ������� ����� �������� ��������� �������.
 *
 * 	��������:
 * 		_error - ������� ������.
 *
 * 		���������� ��������� ������ 16 ������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) WORD WINAPI LV_GetInputState(BYTE* _error)
{
    BOOL result;
    WORD ttl;

	if (pModule == NULL)
		return FALSE;
		
	switch (moduleId)
	{
		case (E154_ID):
			result = ((ILE154*)pModule)->TTL_IN(&ttl);
			break;

		case (E14_140_ID):
			result = ((ILE140*)pModule)->TTL_IN(&ttl);
			break;

		case (E14_440_ID):
			result = ((ILE440*)pModule)->TTL_IN(&ttl);
			break;

		case (E2010_ID):
			result = ((ILE2010*)pModule)->TTL_IN(&ttl);
			break;
	}
    if (result == FALSE)
    {
		if (_error != NULL)
			*_error = 0;
        return 0;
    }

	if (_error != NULL)
		*_error = 1;
    return (WORD) ttl;
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		��������� ��������� ���������� �������.
 *
 * 	��������:
 *
 * 		���������� true, ���� �������� ������ �������, ����� ���������� 0.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) BOOL WINAPI LV_SetOutputState(const WORD _state)
{
	if (pModule == NULL)
		return FALSE;
		
    if (!ttlOutEnabled)
    {
		switch (moduleId)
		{
			case (E154_ID):
				if (!((ILE154*)pModule)->ENABLE_TTL_OUT(true))
					return 0;
				break;
	
			case (E14_140_ID):
				if (!((ILE140*)pModule)->ENABLE_TTL_OUT(true))
					return 0;
				break;
	
			case (E14_440_ID):
				if (!((ILE440*)pModule)->ENABLE_TTL_OUT(true))
					return 0;
				break;
	
			case (E2010_ID):
				if (!((ILE2010*)pModule)->ENABLE_TTL_OUT(true))
					return 0;
				break;
		}
        ttlOutEnabled = TRUE;
    }

	switch (moduleId)
	{
		case (E154_ID):
			return (BOOL) ((ILE154*)pModule)->TTL_OUT(_state);

		case (E14_140_ID):
			return (BOOL) ((ILE140*)pModule)->TTL_OUT(_state);

		case (E14_440_ID):
			return (BOOL) ((ILE440*)pModule)->TTL_OUT(_state);

		case (E2010_ID):
			return (BOOL) ((ILE2010*)pModule)->TTL_OUT(_state);

      default:
      	return FALSE;
	}
}

/************************************************************************************************
 *																                                *								
 *                              	OTHER FUNCTIONS DEFINITIONS         					    *
 *																								*																							
 ***********************************************************************************************/


/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		����� ����� � ������� ���������������� ������� ������������ ��������.
 *
 * 	��������:
 * 		_pData - ��������� �� ������ ������;
 * 		_dataSize - ���������� ��������� � �������;
 * 		_slope - ��� ������������� : 1 - �� ������, 0 - �� �����;
 * 		_trigLevel - ������� ��������, �;
 * 		_threshold - ����� ������������ ��������, �;
 * 		_detectVal - �������� ��� ������� �������� �������, �
 * 		_isDetect - ���� ���������� ��������.
 *
 * 		���������� ������ �������� ��� �������� �������� �������� �������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) DWORD WINAPI LV_DetectTrigPointIdx(const double* _pData,
														 const DWORD _dataSize,
														 const BYTE _slope,
														 const double _trigLevel,
														 const double _threshold,
														 double* _detectVal,
														 BYTE* _isDetect
														 )
{
    DWORD idx;
    double val;
    double prevVal;

    *_detectVal = 0;
    *_isDetect = 0;

    /* ����� �������� ����������������� ������� ������������ ��������.
     * ���� ����������� ������������� �� ������ �������, �������� ������������
     * �������� ���������� �������� ������ �������� �� �������� ������� ��� ������
     * ������ ������������, ���� ������������� �� �����, �������� �������� ���������
     * ������ ������� �� �������� ������� ��� ������ �������� ������.
     */
    prevVal = _pData[0];
    for (idx = 0; idx < _dataSize; idx++)
    {
        val = _pData[idx];

        /* ������������� �� ��������� ������ */
        if (_slope)
        {
            if (val >= (_trigLevel + _threshold) &&
				val > prevVal &&
				prevVal < _trigLevel
			   )
            {
                *_isDetect = 1;
                break;
            }
        }
        /* ������������� �� ������� ������ */
        else
        {
            if (val <= (_trigLevel - _threshold) &&
				val < prevVal &&
				prevVal > _trigLevel
			   )
            {
                *_isDetect = 1;
                break;
            }
        }

        prevVal = val;
    }

    *_detectVal = val;

    if (*_isDetect)
    {
        return (idx);
    }
    else
    {
        return (0);
    }
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		�������� ������������ �������.
 *
 * 	��������:
 * 		_pData - ��������� �� ������ ������;
 * 		_dataSize - ���������� ��������� � �������;
 * 		_decimationFactor - ����������� ������������;
 * 		_pDecimatedData - ������ ����������.
 *-----------------------------------------------------------------------------------------------*/
__declspec(dllexport) VOID WINAPI LV_DecimateArray(const double* _pData,          	
												   const DWORD _dataSize,         	
												   const DWORD _decimationFactor,
												   double* _pDecimatedData
												   )
{
	DWORD idx;
	DWORD ctr;
	if (_decimationFactor < 1)
	{
		memcpy(_pDecimatedData, _pData, _dataSize * sizeof(double));	
		return;
	}

	idx = 0;
	ctr = 0;
	while (idx < _dataSize)
	{
		_pDecimatedData[ctr] = _pData[idx];
		idx += _decimationFactor;
		++ctr;
	}

	return;
}

/************************************************************************************************
 *																                                *								
 *                              	THREADS DEFINITIONS         							    *
 *																								*																							
 ***********************************************************************************************/

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		�����, � ������� �������������� ���� ������ �� ���.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
DWORD WINAPI ServiceRealTimeReadThread(PVOID)
{
    WORD RequestNumber;
    DWORD i;

    // ������������� ������� �� ���� �������
    HANDLE ReadEvent[RD_REQ_QNT];

    // ������ OVERLAPPED �������� �� ���� ���������
    OVERLAPPED ReadOv[RD_REQ_QNT];

    IO_REQUEST_LUSBAPI IoReq[RD_REQ_QNT];
	BOOL ok;
	double	adcRate;

    // ��������� ������ ��� � ������������ ������� USB-����� ������ ������
	switch (moduleId)
	{
		case (E154_ID):
			ok = ((ILE154*)pModule)->STOP_ADC();
			adcRate = e154AdcParam.AdcRate;
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->STOP_ADC();
			adcRate = e140AdcParam.AdcRate;
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->STOP_ADC();
			adcRate = e440AdcParam.AdcRate;
			break;

		case (E2010_ID):
			ok = ((ILE2010*)pModule)->STOP_ADC();
			adcRate = e2010AdcParam.AdcRate;
			break;
	}
    if (!ok)
    {
        readThreadErrDetect = TRUE;
        isReadThreadComplete = true;
        return 0x0;
    }

    // ��������� ����������� ��� ����� ������ ���������
    for (i = 0x0; i < RD_REQ_QNT; i++)
    {
        // ������ �������
        ReadEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

        // ������������� ��������� ���� OVERLAPPED
        ZeroMemory(&ReadOv[i], sizeof(OVERLAPPED));
        ReadOv[i].hEvent = ReadEvent[i];

        // ��������� ��������� IoReq
        IoReq[i].Buffer = pAdcPacket + i * adcPacketSize;
        IoReq[i].NumberOfWordsToPass = adcPacketSize;
        IoReq[i].NumberOfWordsPassed = 0x0;
        IoReq[i].Overlapped = &ReadOv[i];
        IoReq[i].TimeOut = adcPacketSize / adcRate + 2000;
    }

    // ������ ��������������� ������ �� ���� ������
    RequestNumber = 0x0;
	for (i = 0; i < RD_REQ_QNT; i++)
	{
		switch (moduleId)
		{
			case (E154_ID):
				ok = ((ILE154*)pModule)->ReadData(&IoReq[i]);
				break;
	
			case (E14_140_ID):
				ok = ((ILE140*)pModule)->ReadData(&IoReq[i]);
				break;
	
			case (E14_440_ID):
				ok = ((ILE440*)pModule)->ReadData(&IoReq[i]);
				break;
	
			case (E2010_ID):
				ok = ((ILE2010*)pModule)->ReadData(&IoReq[i]);
				break;
		}
	}
    if (!ok)
    {
		for (i = 0x0; i < RD_REQ_QNT; i++)
			CloseHandle(ReadEvent[i]);
        readThreadErrDetect = TRUE;
        isReadThreadComplete = TRUE;
        return 0x0;
    }

    // �������� ���
	switch (moduleId)
	{
		case (E154_ID):
			ok = ((ILE154*)pModule)->START_ADC();
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->START_ADC();
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->START_ADC();
			break;

		case (E2010_ID):
			ok = ((ILE2010*)pModule)->START_ADC();
			break;
	}
    if (ok)
    {
        // ���� ����� ������
		for (;;)
        {
            // ��� ���������� �������� ����� ���������� ������ ������
			for (;;)
			{
				if (adcStopped)
					break;

				if (WaitForSingleObject(ReadEvent[RequestNumber], IoReq[RequestNumber].TimeOut) ==
					 WAIT_TIMEOUT)
				{
					continue;
				}
				else
					break;
			}

        if (adcStopped)
         	break;

			/* ��������� �������� ������ � �����. ���� ������ �������
			 * ������ ��� ���������� ����� � ������ ���������� ������ � ������ ������.
			 */
			CIRBUF_PUSH_MULTIPLE_BACK(pAdcBuf, (*pAdcBufParam), IoReq[RequestNumber].Buffer, adcPacketSize);

			/* �������� ������ �� ������������ */
			if (CIRBUF_DATA_LOST == CIRBUF_GetStatus(pAdcBufParam))
				adcBufOverflow = TRUE;

            if (readThreadErrDetect)
                break;
			else
			{
				// ������� ������ �� ��������� ������ ������
				switch (moduleId)
				{
					case (E154_ID):
						ok = ((ILE154*)pModule)->ReadData(&IoReq[RequestNumber]);
						break;
			
					case (E14_140_ID):
						ok = ((ILE140*)pModule)->ReadData(&IoReq[RequestNumber]);
						break;
			
					case (E14_440_ID):
						ok = ((ILE440*)pModule)->ReadData(&IoReq[RequestNumber]);
						break;
			
					case (E2010_ID):
						ok = ((ILE2010*)pModule)->ReadData(&IoReq[RequestNumber]);
						break;
				}
				if (!ok)
				{
					readThreadErrDetect = TRUE;
					break;
				}
				else
				{
					if (++RequestNumber >= RD_REQ_QNT)
						RequestNumber = 0;
				}
			}
        }
    }
    else
        readThreadErrDetect = TRUE;

    // ��������� ������ ���
	switch (moduleId)
	{
		case (E154_ID):
			ok = ((ILE154*)pModule)->STOP_ADC();
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->STOP_ADC();
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->STOP_ADC();
			break;

		case (E2010_ID):
			ok = ((ILE2010*)pModule)->STOP_ADC();
			break;
	}
    if (!ok)
        readThreadErrDetect = TRUE;

    // ������ �������� ������������� ����������� ������ �� ���� ������
    if (!CancelIo(moduleHandle))
        readThreadErrDetect = TRUE;

    // ��������� ��� �������������� �������
    for (i = 0x0; i < RD_REQ_QNT; i++)
    {
        CloseHandle(ReadEvent[i]);
    }
	
	//CloseHandle(moduleHandle);

    // ��������� ��������
    Sleep(20);

    // ��������� ������ ���������� ������ ������ ����� ������
    isReadThreadComplete = true;

    // ������ ����� �������� �������� �� ������
    ExitThread(0);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		�����, � ������� �������������� ����� ������ �� ���.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
DWORD WINAPI ServiceRealTimeWriteThread(PVOID)
{
    DWORD i;
    WORD requestNum;

    /* �������������� ������� */
    HANDLE writeEvent[2];

    /* OVERLAPPED ��������� */
    OVERLAPPED writeOv[2];

    /* ��������� ������� */
    IO_REQUEST_LUSBAPI ioReq[2];

	BOOL errNum;
	BOOL ok;

    /* ��������� ������ ��� */
	switch (moduleId)
	{
		case (E154_ID):
			ok = 0;
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->STOP_DAC();
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->STOP_DAC();
			break;

		case (E2010_ID):
			ok = 0;
			break;

      default:
      	ok = 0;
	}
    if (!ok)
    {
        isWriteThreadComplete = true;
        return (0);
    }

	/* ���������� FIFO ������ ��� � ������ E440 */
	if (E14_440_ID == moduleId)
	{
		errNum = ((ILE440*)pModule)->PUT_DM_ARRAY(e440DacParam.DacFifoBaseAddress,
												  e440DacParam.DacFifoLength,
												  pDacPacket
												 );
		if (!errNum)
		{
			writeThreadErrDetect = TRUE;
			isWriteThreadComplete = TRUE;
			return 1;
		}
	}

    /* ������������ ������ ��������� */
    for (i = 0x00; i < 0x2; i++)
    {
        writeEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        ZeroMemory(&writeOv[i], sizeof(OVERLAPPED));
        writeOv[i].hEvent = writeEvent[i];

        /* ���������� ��������� ���������� ������� */
        ioReq[i].Buffer = pDacPacket + i * dacPacketSize;
        ioReq[i].NumberOfWordsToPass = dacPacketSize;
        ioReq[i].NumberOfWordsPassed = 0x0;
        ioReq[i].Overlapped = &writeOv[i];
        ioReq[i].TimeOut = 2000;
    }

    /* ������������� ������ ������ */
    memset(pDacPacket, 0x00, dacPacketSize * sizeof(pDacPacket[0]));

    /* ��������� �������� ������ */
	switch (moduleId)
	{
		case (E154_ID):
			ok = 0;
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->WriteData(&ioReq[0]);
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->WriteData(&ioReq[0]);
			break;

		case (E2010_ID):
			ok = 0;
			break;

		default:
			ok = 0;
	}
    if (!ok)
    {
        CloseHandle(writeEvent[0]);
        CloseHandle(writeEvent[1]);
        writeThreadErrDetect = TRUE;
        isWriteThreadComplete = true;
        return (0);
    }

    /* ������ ��� � ��������� �������� ������ � ��� */
	switch (moduleId)
	{
		case (E154_ID):
			ok = 0;
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->START_DAC();
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->START_DAC();
			break;

		case (E2010_ID):
			ok = 0;
			break;

		default:
			ok = 0;
	}

    if (ok)
    {
        requestNum = 0;
        while (1)
        {
            /* ������ �� ������ ���������� ������ */
            requestNum ^= 0x1;
			switch (moduleId)
			{
				case (E154_ID):
					ok = 0;
					break;
		
				case (E14_140_ID):
					ok = ((ILE140*)pModule)->WriteData(&ioReq[requestNum]);
					break;
		
				case (E14_440_ID):
					/* ���������� ������ ������ � ������ ������ */
					for (i = 0; i < dacPacketSize; i++)
					{
						*(pDacPacket + requestNum*dacPacketSize + i) &= (WORD)(0xFFF);
						*(pDacPacket + requestNum*dacPacketSize + i) |= (WORD)((i % 2) << 12);
					}

					/* ������ ������ */
					ok = ((ILE440*)pModule)->WriteData(&ioReq[requestNum]);
					break;
		
				case (E2010_ID):
					ok = 0;
					break;

				default:
					ok = 0;
			}
            if (!ok)
            {
                writeThreadErrDetect = TRUE;
                break;
            }

            /* �������� ��������� ������ ���������� ������ */
            while (1)
            {
                if (dacStopped)
                    break;

                if (WaitForSingleObject(writeEvent[requestNum ^ 0x1], ioReq[requestNum ^ 0x1].TimeOut) ==
                    WAIT_TIMEOUT)
                    continue;
                else
                    break;
            }

            if (dacStopped)
            {
                break;
            }

            /* ������ ���������� ������ � ������.
             * ���� ����� ���� ��� ���������� ������ ������ ������� ������,
             * ��������������� ���� ����������� ������ � ��������� ����������� ������������
             * ���������� ������ � �����.
             * ����� �� ������ ����������� ����� ������ ������.
             */
            while (1)
            {
				DWORD currDataSize;

                currDataSize = CIRBUF_GetDataSize(pDacBufParam);
                if (currDataSize >= dacPacketSize)
                {
                    /* ���������� ����� ������ ������ �� ������ � ��������� �� � ����� */
                    CIRBUF_POP_MULTIPLE_FRONT(pDacBuf,
                                              (*pDacBufParam),
                                              (pDacPacket + dacPacketSize * (requestNum ^ 0x1)),
                                              dacPacketSize
                                             );
                    dacBufUnderflow = 0;
                    break;
                }
                else if (!currDataSize)
                {
                    dacBufUnderflow = 1;
                }
				
				if (dacStopped)
                    break;
            }

            if (writeThreadErrDetect || dacStopped)
                break;
        }
    }
    else
        writeThreadErrDetect = TRUE;

    /* ��������� ������ ��� */
	switch (moduleId)
	{
		case (E154_ID):
			ok = 0;
			break;

		case (E14_140_ID):
			ok = ((ILE140*)pModule)->STOP_DAC();
			break;

		case (E14_440_ID):
			ok = ((ILE440*)pModule)->STOP_DAC();
			break;

		case (E2010_ID):
			ok = 0;
			break;

		default:
			ok = 0;

	}
    if (!ok)
        writeThreadErrDetect = TRUE;

    /* ���������� �������� ������������� ����������� ������ �� ������ ������ */
    if (!CancelIo(((ILUSBBASE*)pModule)->GetModuleHandle()))
        writeThreadErrDetect = TRUE;

    /* �������� ������� */
    for (i = 0x0; i < 0x2; i++)
    {
        CloseHandle(writeEvent[i]);
    }

    /* ��������� �������� */
    Sleep(20);

    isWriteThreadComplete = true;
    return (0);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		���� � ����������� ������ ��� ������ ������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
void f_RdEnterCritSect(void)
{
	EnterCriticalSection(&rdCritSection);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		����� �� ����������� ������ ��� ������ ������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
void f_RdLeaveCritSect(void)
{
	LeaveCriticalSection(&rdCritSection);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		���� � ����������� ������ ��� ������ ������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
void f_WrEnterCritSect(void)
{
	EnterCriticalSection(&wrCritSection);
}

/*-----------------------------------------------------------------------------------------------**
 * 	����������:
 *		����� �� ����������� ������ ��� ������ ������.
 *
 * 	��������:
 *-----------------------------------------------------------------------------------------------*/
void f_WrLeaveCritSect(void)
{
	LeaveCriticalSection(&wrCritSection);
}

/*===============================================================================================*/










