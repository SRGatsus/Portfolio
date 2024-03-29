#ifndef __LusbapiH__
#define __LusbapiH__

	// --------------------------------------------------------------------------
	// ---------------------------- COMMON PART ---------------------------------
	// --------------------------------------------------------------------------
	#include <windows.h>
	#include "LusbapiTypes.h"

	// ������ ����������
	#define 	VERSION_MAJOR_LUSBAPI 			(0x3)		// ������ ���� �����
	#define 	VERSION_MINOR_LUSBAPI 			(0x4)		// ������ ���� �����
	#define 	CURRENT_VERSION_LUSBAPI			((VERSION_MAJOR_LUSBAPI << 0x10) | VERSION_MINOR_LUSBAPI)

	#define InitLDevice(Slot) OpenLDevice(Slot)

	// ��������������� �������
	extern "C" DWORD WINAPI GetDllVersion(void);
	extern "C" LPVOID WINAPI CreateLInstance(PCHAR const DeviceName);

	// ��������� ������� �������� ������ ������ �� ���� USB
	enum { USB11_LUSBAPI, USB20_LUSBAPI, INVALID_USB_SPEED_LUSBAPI };
	// ������ ���������� �����-���� ����������� ������
	enum { NO_MODULE_MODIFICATION_LUSBAPI = -1 };
	// ����������� ��������� ���-�� ������������ ����������� ������
	const WORD MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI = 127;


	// ==========================================================================
	// *************************** L-Card USB BASE ******************************
	// ==========================================================================
	struct ILUSBBASE
	{
		// ������� ������ ���������� ��� ������ � USB ������������
		virtual BOOL WINAPI OpenLDevice(WORD VirtualSlot) = 0;
		virtual BOOL WINAPI CloseLDevice(void) = 0;
		virtual BOOL WINAPI ReleaseLInstance(void) = 0;
		// ��������� ����������� ���������� USB
		virtual HANDLE WINAPI GetModuleHandle(void) = 0;
		// ��������� �������� ������������� ������
		virtual BOOL WINAPI GetModuleName(PCHAR const ModuleName) = 0;
		// ��������� ������� �������� ������ ���� USB
		virtual BOOL WINAPI GetUsbSpeed(BYTE * const UsbSpeed) = 0;
		// ���������� ������� ������� ������������������ ������
		virtual BOOL WINAPI LowPowerMode(BOOL LowPowerFlag) = 0;
		// ������� ������ ������ � ��������� �������
		virtual BOOL WINAPI GetLastErrorInfo(LAST_ERROR_INFO_LUSBAPI * const LastErrorInfo) = 0;
	};




	// ==========================================================================
	// *************************** ������ E14-140 *******************************
	// ==========================================================================
	// ��������� ������� ���������� �������� ���������� ������ E14-140
	enum {	ADC_INPUT_RANGE_10000mV_E140, ADC_INPUT_RANGE_2500mV_E140, ADC_INPUT_RANGE_625mV_E140, ADC_INPUT_RANGE_156mV_E140, INVALID_ADC_INPUT_RANGE_E140 };
	// ��������� ������� ��������� �������� ��������� ��� ���
	enum {	INT_ADC_CLOCK_E140, EXT_ADC_CLOCK_E140, INVALID_ADC_CLOCK_E140 };
	// ��������� ������� ���������� ����������� �������� ��������� ���
	// �� ����� SYN �������� ��������� ������� (������ ��� ����������
	// ��������� �������� ��������� ���)
	enum {	ADC_CLOCK_TRANS_DISABLED_E140, ADC_CLOCK_TRANS_ENABLED_E140, INVALID_ADC_CLOCK_TRANS_E140 };
	// ��������� ���� ������������� ������ E14-140
	enum { 	NO_SYNC_E140, TTL_START_SYNC_E140, TTL_KADR_SYNC_E140, ANALOG_SYNC_E140, INVALID_SYNC_E140 };
	// ��������� ����� ������� ���������� ���
	enum {	DAC_INACCESSIBLED_E140, DAC_ACCESSIBLED_E140, INVALID_DAC_OPTION_E140 };
	// ��������� ������� ������� ������ E14-140
	enum {	REVISION_A_E140, REVISION_B_E140, INVALID_REVISION_E140 };
	// ��������� ������� ������������� ��������� ������ ��� � ���
	enum {	DIS_ADC_DAC_SYNC_E140, ENA_ADC_DAC_SYNC_E140, INVALID_ADC_DAC_SYNC_E140 };
	// ��������� ������� ������� ��������� ���������� ���
	enum {	NORMAL_DAC_ON_STOP_E140, ZERO_DAC_ON_STOP_E140, INVALID_DAC_ON_STOP_E140 };

	// ��������� ��� ������ � �������
	enum 	{
				MAX_CONTROL_TABLE_LENGTH_E140 = 128,
				ADC_INPUT_RANGES_QUANTITY_E140 = INVALID_ADC_INPUT_RANGE_E140,
				ADC_CALIBR_COEFS_QUANTITY_E140 = ADC_INPUT_RANGES_QUANTITY_E140,
				DAC_CHANNELS_QUANTITY_E140 = 0x2, DAC_CALIBR_COEFS_QUANTITY_E140 = DAC_CHANNELS_QUANTITY_E140,
				TTL_LINES_QUANTITY_E140 = 0x10,	  		// ���-�� �������� �����
				USER_FLASH_SIZE_E140 = 0x200,   			// ������ ������� ����������������� ���� � ������
				REVISIONS_QUANTITY_E140 = INVALID_REVISION_E140,		// ���-�� ������� (�����������) ������
			};
	// ��������� �������� ���������� ��� � �
	const double ADC_INPUT_RANGES_E140[ADC_INPUT_RANGES_QUANTITY_E140] =
	{
		10.0, 10.0/4.0, 10.0/16.0, 10.0/64.0
	};
	// ��������� ��������� ���������� ��� � �
	const double DAC_OUTPUT_RANGE_E140 = 5.0;
	// ��������� ������� ������
	const BYTE REVISIONS_E140[REVISIONS_QUANTITY_E140] = { 'A', 'B' };

	#pragma pack(1)
	// ��������� � ����������� �� ������ E14-140
	struct MODULE_DESCRIPTION_E140
	{
		MODULE_INFO_LUSBAPI     Module;		// ����� ���������� � ������
		INTERFACE_INFO_LUSBAPI  Interface;	// ���������� �� ������������ ����������
		MCU_INFO_LUSBAPI<VERSION_INFO_LUSBAPI>		Mcu;	// ���������� � ����������������
		ADC_INFO_LUSBAPI        Adc;			// ���������� � ���
		DAC_INFO_LUSBAPI        Dac;			// ���������� � ���
		DIGITAL_IO_INFO_LUSBAPI DigitalIo;	// ���������� � �������� �����-������
	};
	// ��������� ����������������� ���� ������ E14-140
	struct USER_FLASH_E140
	{
		BYTE Buffer[USER_FLASH_SIZE_E140];
	};
	// ���������, �������� ����� ������ ��� ��� ������ E14-140
	struct ADC_PARS_E140
	{
		WORD ClkSource;							// �������� �������� ��������� ��� ������� ���
		WORD EnableClkOutput;					// ���������� ���������� �������� ��������� ������� ���
		WORD InputMode;							// ����� ����� ����� � ���
		WORD SynchroAdType;						// ��� ���������� �������������
		WORD SynchroAdMode; 						// ����� ���������� ������������
		WORD SynchroAdChannel;  				// ����� ��� ��� ���������� �������������
		SHORT SynchroAdPorog; 					// ����� ������������ ��� ��� ���������� �������������
		WORD ChannelsQuantity;					// ����� �������� �������
		WORD ControlTable[128];					// ����������� ������� � ��������� ��������
		double AdcRate;							// ������� ������ ��� � ���
		double InterKadrDelay;					// ����������� �������� � ��
		double KadrRate;							// ������� ����� � ���
	};
	// ���������, �������� ����� ��������� ������ ��� ��� ������ E14-140
	struct DAC_PARS_E140
	{
		BYTE SyncWithADC;							// 0 = ������� ���� ���; !0 = ���������������� � ������ ���
		BYTE SetZeroOnStop;						// !0 = ��� ��������� ���������� ������ ���������� �� ������ ��� 0 �
		double DacRate;							// ������� ������ ��� � ���
	};
	#pragma pack()

	//-----------------------------------------------------------------------------
	// ��������� ��� ������ E14-140
	//-----------------------------------------------------------------------------
	struct ILE140 : public ILUSBBASE
	{
		// ������� ��� ������ � ���
		virtual BOOL WINAPI GET_ADC_PARS(ADC_PARS_E140 * const AdcPars) = 0;
		virtual BOOL WINAPI SET_ADC_PARS(ADC_PARS_E140 * const AdcPars) = 0;
		virtual BOOL WINAPI START_ADC(void) = 0;
		virtual BOOL WINAPI STOP_ADC(void) = 0;
		virtual BOOL WINAPI ADC_KADR(SHORT * const Data) = 0;
		virtual BOOL WINAPI ADC_SAMPLE(SHORT * const AdcData, WORD AdcChannel) = 0;
		virtual BOOL WINAPI ReadData(IO_REQUEST_LUSBAPI * const ReadRequest) = 0;

		// ������� ��� ������ � ���
		virtual BOOL WINAPI GET_DAC_PARS(DAC_PARS_E140 * const DacPars) = 0;
		virtual BOOL WINAPI SET_DAC_PARS(DAC_PARS_E140 * const DacPars) = 0;
		virtual BOOL WINAPI START_DAC(void) = 0;
		virtual BOOL WINAPI STOP_DAC(void) = 0;
		virtual BOOL WINAPI WriteData(IO_REQUEST_LUSBAPI * const WriteRequest) = 0;
		virtual BOOL WINAPI DAC_SAMPLE(SHORT * const DacData, WORD DacChannel) = 0;
		virtual BOOL WINAPI DAC_SAMPLES(SHORT * const DacData1, SHORT * const DacData2) = 0;

		// ������� ��� ������ � ��� �������
		virtual BOOL WINAPI ENABLE_TTL_OUT(BOOL EnableTtlOut) = 0;
		virtual BOOL WINAPI TTL_IN(WORD * const TtlIn) = 0;
		virtual BOOL WINAPI TTL_OUT(WORD TtlOut) = 0;

		// ������� ��� ������ � ���������������� ����������� ����
		virtual BOOL WINAPI ENABLE_FLASH_WRITE(BOOL IsUserFlashWriteEnabled) = 0;
		virtual BOOL WINAPI READ_FLASH_ARRAY(USER_FLASH_E140 * const UserFlash) = 0;
		virtual BOOL WINAPI WRITE_FLASH_ARRAY(USER_FLASH_E140 * const UserFlash) = 0;

		// ������� ��� ������ �� ��������� ����������� ����
		virtual BOOL WINAPI GET_MODULE_DESCRIPTION(MODULE_DESCRIPTION_E140 * const ModuleDescription) = 0;
		virtual BOOL WINAPI SAVE_MODULE_DESCRIPTION(MODULE_DESCRIPTION_E140 * const ModuleDescription) = 0;

		// ������� ��� ������� ������ � ����������������
		virtual BOOL WINAPI GetArray(BYTE * const Buffer, WORD Size, WORD Address) = 0;
		virtual BOOL WINAPI PutArray(BYTE * const Buffer, WORD Size, WORD Address) = 0;
	};






	// ==========================================================================
	// *************************** ������ E14-440 *******************************
	// ==========================================================================
	// ��������� ��������� ������ ������ E14-440
	enum {	INIT_E440, RESET_E440, INVALID_RESET_TYPE_E440 };
	// ��������� ������� ��������� �������� ��������� ��� ���
	enum	{	INT_ADC_CLOCK_E440, INT_ADC_CLOCK_WITH_TRANS_E440, EXT_ADC_CLOCK_E440, INVALID_ADC_CLOCK_E440 };
	// ��������� ������� ���������� �������� ���������� ������ E14-440
	enum {	ADC_INPUT_RANGE_10000mV_E440, ADC_INPUT_RANGE_2500mV_E440, ADC_INPUT_RANGE_625mV_E440, ADC_INPUT_RANGE_156mV_E440, INVALID_ADC_INPUT_RANGE_E440 };
	// ��������� ���� ������������� ������ E14-440
	enum {	NO_SYNC_E440, TTL_START_SYNC_E440, TTL_KADR_SYNC_E440, ANALOG_SYNC_E440, INVALID_SYNC_E440 };
	// ��������� ����� ������� ���������� ���
	enum {	DAC_INACCESSIBLED_E440, DAC_ACCESSIBLED_E440, INVALID_DAC_OPTION_E440 };
	// ��������� ���� DSP (������ ������ ADSP-2185)
	enum {	ADSP2184_E440, ADSP2185_E440, ADSP2186_E440, INVALID_DSP_TYPE_E440 };
	// ��������� �������� ������� ������ (������ ������ 24000 ���)
	enum {	F14745_E440, F16667_E440, F20000_E440, F24000_E440, INVALID_QUARTZ_FREQ_E440 };
	// ��������� ������� ������� ������ E14-440
	enum	{	REVISION_A_E440, REVISION_B_E440, REVISION_C_E440, REVISION_D_E440, REVISION_E_E440, REVISION_F_E440, INVALID_REVISION_E440 };

	// ��������� ��� ������ � �������
	enum 	{
				MAX_CONTROL_TABLE_LENGTH_E440 = 128,
				ADC_INPUT_RANGES_QUANTITY_E440 = INVALID_ADC_INPUT_RANGE_E440,
				ADC_CALIBR_COEFS_QUANTITY_E440 = ADC_INPUT_RANGES_QUANTITY_E440,
				MAX_ADC_FIFO_SIZE_E440 = 0x3000,			// 12288
				DAC_CHANNELS_QUANTITY_E440 = 0x2, DAC_CALIBR_COEFS_QUANTITY_E440 = DAC_CHANNELS_QUANTITY_E440,
				MAX_DAC_FIFO_SIZE_E440 = 0x0FC0,			// 4032
				TTL_LINES_QUANTITY_E440 = 0x10, 			// ���-�� �������� �����
				REVISIONS_QUANTITY_E440 = INVALID_REVISION_E440,		// ���-�� ������� (�����������) ������
			};
	// ��������� �������� ���������� ��� � �
	const double ADC_INPUT_RANGES_E440[ADC_INPUT_RANGES_QUANTITY_E440] =
	{
		10.0, 10.0/4.0, 10.0/16.0, 10.0/64.0
	};
	// ��������� ��������� ���������� ��� � �
	const double DAC_OUTPUT_RANGE_E440 = 5.0;
	// ��������� ������� ������
	const BYTE REVISIONS_E440[REVISIONS_QUANTITY_E440] = { 'A', 'B', 'C', 'D', 'E', 'F' };

	#pragma pack(1)
	// ��������� � ����������� �� ������ E14-440
	struct MODULE_DESCRIPTION_E440
	{
		MODULE_INFO_LUSBAPI     Module;		// ����� ���������� � ������
		INTERFACE_INFO_LUSBAPI  Interface;	// ���������� �� ������������ ����������
		MCU_INFO_LUSBAPI<VERSION_INFO_LUSBAPI>		Mcu;	// ���������� � ����������������
		DSP_INFO_LUSBAPI        Dsp;			// ���������� � DSP
		ADC_INFO_LUSBAPI        Adc;			// ���������� � ���
		DAC_INFO_LUSBAPI        Dac;			// ���������� � ���
		DIGITAL_IO_INFO_LUSBAPI DigitalIo;	// ���������� � �������� �����-������
	};
	// ���������, �������� ����� ������ ��� ��� ������ E-440
	struct ADC_PARS_E440
	{
		BOOL IsAdcEnabled;		 			// ������ ������ ��� (������ ��� ������)
		BOOL IsCorrectionEnabled;			// ���������� ����������� �������������� ������ �� ������ �������� DSP
		WORD AdcClockSource;					// �������� �������� ��������� ������� ���: ���������� ��� �������
		WORD InputMode;						// ����� ����� ����� � ���
		WORD SynchroAdType;					// ��� ���������� �������������
		WORD SynchroAdMode; 					// ����� ���������� ������������
		WORD SynchroAdChannel;  			// ����� ��� ��� ���������� �������������
		SHORT SynchroAdPorog; 				// ����� ������������ ��� ��� ���������� �������������
		WORD ChannelsQuantity;				// ����� �������� �������
		WORD ControlTable[MAX_CONTROL_TABLE_LENGTH_E440];		// ����������� ������� � ��������� ��������
		double AdcRate;	  			  		// ������� ������ ��� � ���
		double InterKadrDelay;		  		// ����������� �������� � ��
		double KadrRate;					// ������� ����� � ���
		WORD AdcFifoBaseAddress;			// ������� ����� FIFO ������ ���
		WORD AdcFifoLength;					// ����� FIFO ������ ���
		double AdcOffsetCoefs[ADC_CALIBR_COEFS_QUANTITY_E440];	// ��������	���: 4���������
		double AdcScaleCoefs[ADC_CALIBR_COEFS_QUANTITY_E440];		// ������� ���	: 4���������
	};

	// ���������, �������� ����� ������ ��� ��� ������ E-440
	struct DAC_PARS_E440
	{
		BOOL DacEnabled;						// ����������/���������� ������ ���
		double DacRate;	  			  		// ������� ������ ��� � ���
		WORD DacFifoBaseAddress;			// ������� ����� FIFO ������ ���
		WORD DacFifoLength;					// ����� FIFO ������ ���
	};
	#pragma pack()

	// ����� ������ �������� ����� ������ � ������ �������� �������� DSP
	const WORD DataBaseAddress_E440 = 0x30;
	// ���������� �������� LBIOS ��� ������ E14-440 (������������ � ������ �������� DSP)
	#define 	  	L_PROGRAM_BASE_ADDRESS_E440				(DataBaseAddress_E440 + 0x0)
	#define 	  	L_READY_E440 									(DataBaseAddress_E440 + 0x1)
	#define	  	L_TMODE1_E440 									(DataBaseAddress_E440 + 0x2)
	#define	  	L_TMODE2_E440 									(DataBaseAddress_E440 + 0x3)
	#define	  	L_TEST_LOAD_E440	 							(DataBaseAddress_E440 + 0x4)
	#define	  	L_COMMAND_E440	 			      			(DataBaseAddress_E440 + 0x5)

	#define		L_DAC_SCLK_DIV_E440							(DataBaseAddress_E440 + 0x7)
	#define		L_DAC_RATE_E440								(DataBaseAddress_E440 + 0x8)
	#define	  	L_ADC_RATE_E440  			      			(DataBaseAddress_E440 + 0x9)
	#define		L_ADC_ENABLED_E440	 						(DataBaseAddress_E440 + 0xA)
	#define		L_ADC_FIFO_BASE_ADDRESS_E440				(DataBaseAddress_E440 + 0xB)
	#define		L_CUR_ADC_FIFO_LENGTH_E440					(DataBaseAddress_E440 + 0xC)
	#define		L_ADC_FIFO_LENGTH_E440						(DataBaseAddress_E440 + 0xE)
	#define		L_CORRECTION_ENABLED_E440					(DataBaseAddress_E440 + 0xF)
	#define		L_LBIOS_VERSION_E440							(DataBaseAddress_E440 + 0x10)
	#define		L_ADC_SAMPLE_E440								(DataBaseAddress_E440 + 0x11)
	#define		L_ADC_CHANNEL_E440	 						(DataBaseAddress_E440 + 0x12)
	#define		L_INPUT_MODE_E440								(DataBaseAddress_E440 + 0x13)
	#define		L_SYNCHRO_AD_CHANNEL_E440					(DataBaseAddress_E440 + 0x16)
	#define		L_SYNCHRO_AD_POROG_E440						(DataBaseAddress_E440 + 0x17)
	#define		L_SYNCHRO_AD_MODE_E440 						(DataBaseAddress_E440 + 0x18)
	#define		L_SYNCHRO_AD_TYPE_E440 						(DataBaseAddress_E440 + 0x19)

	#define		L_CONTROL_TABLE_LENGHT_E440				(DataBaseAddress_E440 + 0x1B)
	#define		L_FIRST_SAMPLE_DELAY_E440					(DataBaseAddress_E440 + 0x1C)
	#define		L_INTER_KADR_DELAY_E440						(DataBaseAddress_E440 + 0x1D)

	#define		L_DAC_SAMPLE_E440								(DataBaseAddress_E440 + 0x20)
	#define		L_DAC_ENABLED_E440					 		(DataBaseAddress_E440 + 0x21)
	#define		L_DAC_FIFO_BASE_ADDRESS_E440				(DataBaseAddress_E440 + 0x22)
	#define		L_CUR_DAC_FIFO_LENGTH_E440					(DataBaseAddress_E440 + 0x24)
	#define		L_DAC_FIFO_LENGTH_E440						(DataBaseAddress_E440 + 0x25)

	#define		L_FLASH_ENABLED_E440 						(DataBaseAddress_E440 + 0x26)
	#define		L_FLASH_ADDRESS_E440 						(DataBaseAddress_E440 + 0x27)
	#define		L_FLASH_DATA_E440 							(DataBaseAddress_E440 + 0x28)

	#define		L_ENABLE_TTL_OUT_E440						(DataBaseAddress_E440 + 0x29)
	#define		L_TTL_OUT_E440									(DataBaseAddress_E440 + 0x2A)
	#define		L_TTL_IN_E440									(DataBaseAddress_E440 + 0x2B)

	#define		L_ADC_CLOCK_SOURCE_E440						(DataBaseAddress_E440 + 0x2F)

	#define		L_SCALE_E440									(DataBaseAddress_E440 + 0x30)
	#define		L_ZERO_E440										(DataBaseAddress_E440 + 0x34)

	#define		L_CONTROL_TABLE_E440							(0x80)

	#define		L_DSP_INFO_STUCTURE_E440					(0x200)

	//-----------------------------------------------------------------------------
	// ��������� ������ E14-440
	//-----------------------------------------------------------------------------
	struct ILE440 : public ILUSBBASE
	{
		// ������� ������ � DSP
		virtual BOOL WINAPI RESET_MODULE(BYTE ResetFlag = INIT_E440) = 0;
		virtual BOOL WINAPI LOAD_MODULE(PCHAR const FileName = NULL) = 0;
		virtual BOOL WINAPI TEST_MODULE(void) = 0;
		virtual BOOL WINAPI SEND_COMMAND(WORD Command) = 0;

		// ������� ��� ������ � ���
		virtual BOOL WINAPI GET_ADC_PARS(ADC_PARS_E440 * const AdcPars) = 0;
		virtual BOOL WINAPI SET_ADC_PARS(ADC_PARS_E440 * const AdcPars) = 0;
		virtual BOOL WINAPI START_ADC(void) = 0;
		virtual BOOL WINAPI STOP_ADC(void) = 0;
		virtual BOOL WINAPI ADC_KADR(SHORT * const Data) = 0;
		virtual BOOL WINAPI ADC_SAMPLE(SHORT * const AdcData, WORD AdcChannel) = 0;
		virtual BOOL WINAPI ReadData(IO_REQUEST_LUSBAPI * const ReadRequest) = 0;

		// ������� ��� ������ � ���
		virtual BOOL WINAPI GET_DAC_PARS(DAC_PARS_E440 * const DacPars) = 0;
		virtual BOOL WINAPI SET_DAC_PARS(DAC_PARS_E440 * const DacPars) = 0;
		virtual BOOL WINAPI START_DAC(void) = 0;
		virtual BOOL WINAPI STOP_DAC(void) = 0;
		virtual BOOL WINAPI WriteData(IO_REQUEST_LUSBAPI * const WriteRequest) = 0;
		virtual BOOL WINAPI DAC_SAMPLE(SHORT * const DacData, WORD DacChannel) = 0;

		// ������� ��� ������ � ��������� �������
		virtual BOOL WINAPI ENABLE_TTL_OUT(BOOL EnableTtlOut) = 0;
		virtual BOOL WINAPI TTL_IN(WORD * const TtlIn) = 0;
		virtual BOOL WINAPI TTL_OUT(WORD TtlOut) = 0;

		// ������� ��� ������ ���������������� ���� ������
		virtual BOOL WINAPI ENABLE_FLASH_WRITE(BOOL EnableFlashWrite) = 0;
		virtual BOOL WINAPI READ_FLASH_WORD(WORD FlashAddress, SHORT * const FlashWord) = 0;
		virtual BOOL WINAPI WRITE_FLASH_WORD(WORD FlashAddress, SHORT FlashWord) = 0;

		// ������� ��� ������ �� ��������� ����������� �� ����
		virtual BOOL WINAPI GET_MODULE_DESCRIPTION(MODULE_DESCRIPTION_E440 * const ModuleDescription) = 0;
		virtual BOOL WINAPI SAVE_MODULE_DESCRIPTION(MODULE_DESCRIPTION_E440 * const ModuleDescription) = 0;

		// ������� ��� ������ � ������� DSP
		virtual BOOL WINAPI PUT_LBIOS_WORD(WORD Address, SHORT Data) = 0;
		virtual BOOL WINAPI GET_LBIOS_WORD(WORD Address, SHORT * const Data) = 0;
		virtual BOOL WINAPI PUT_DM_WORD(WORD Address, SHORT Data) = 0;
		virtual BOOL WINAPI GET_DM_WORD(WORD Address, SHORT * const Data) = 0;
		virtual BOOL WINAPI PUT_PM_WORD(WORD Address, long Data) = 0;
		virtual BOOL WINAPI GET_PM_WORD(WORD Address, long * const Data) = 0;
		virtual BOOL WINAPI PUT_DM_ARRAY(WORD BaseAddress, WORD NPoints, SHORT * const Data) = 0;
		virtual BOOL WINAPI GET_DM_ARRAY(WORD BaseAddress, WORD NPoints, SHORT * const Data) = 0;
		virtual BOOL WINAPI PUT_PM_ARRAY(WORD BaseAddress, WORD NPoints, long * const Data) = 0;
		virtual BOOL WINAPI GET_PM_ARRAY(WORD BaseAddress, WORD NPoints, long * const Data) = 0;

		// ������� ��� ������ � ����������� ���� ������
		virtual BOOL WINAPI ERASE_BOOT_FLASH(void) = 0;
		virtual BOOL WINAPI PUT_ARRAY_BOOT_FLASH(DWORD BaseAddress, DWORD NBytes, BYTE *Data) = 0;
		virtual BOOL WINAPI GET_ARRAY_BOOT_FLASH(DWORD BaseAddress, DWORD NBytes, BYTE *Data) = 0;
	};




	



#endif
