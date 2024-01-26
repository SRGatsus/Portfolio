/* --- circularBuf.c -------------------------------------------------------------------------------------- **
 *  ���������� ���������� ������.
 *  ��������! ���������� ������ ������� ��������� ������� ������� 2!
 * 
 * -------------------------------------------------------------------------------------------------------- */

#include "circularBuf.h"

/************************************************************************************************
 *																                                *								
 *                                    FUNCTIONS DEFINITIONS      		    				    *
 *																								*																							
 ***********************************************************************************************/

/* ������������� ������.
 *
 */
void CIRBUF_Init(const CIRBUF_IndexType _dataSize, CIRBUF_Param* _pParam)

{
    _pParam->head = _pParam->tail = 0;
    _pParam->sizeMask = _dataSize - 1;
    _pParam->overflowMask = ~_pParam->sizeMask;
}

/* ������� ������
 *
 */
void CIRBUF_Flush(CIRBUF_Param* _pParam)
{
    CIRBUF_ENTER_CRITICAL_SECTION(_pParam);
    _pParam->head = _pParam->tail = 0;
    CIRBUF_LEAVE_CRITICAL_SECTION(_pParam);
}

/* ��������� ���������� ��������� ���������.
 * ����������� ������� �� ����������� ����� ��������
 * ������ � ������ ��������� �� ����/����������� ������ ������.
 * ���� ����������� �������������� � ������ ������ >= �������
 * ������ - ������ ���������� ����� ����� 0.
 */
CIRBUF_IndexType CIRBUF_GetEmptySize(CIRBUF_Param* _pParam)
{
    CIRBUF_IndexType diff; 

    CIRBUF_ENTER_CRITICAL_SECTION(_pParam);
    diff = CIRBUF_CALCULATE_HEAD_TO_TAIL_DIFF(_pParam);
    CIRBUF_LEAVE_CRITICAL_SECTION(_pParam);

    if (diff & _pParam->overflowMask)
        return (0);
    else
        return (_pParam->sizeMask - diff + 1);
}

/* ��������� ���������� ��������� � ������.
 *
 */
CIRBUF_IndexType CIRBUF_GetDataSize(CIRBUF_Param* _pParam)
{
    CIRBUF_IndexType diff;

    CIRBUF_ENTER_CRITICAL_SECTION(_pParam);
    diff = CIRBUF_CALCULATE_HEAD_TO_TAIL_DIFF(_pParam);
    CIRBUF_LEAVE_CRITICAL_SECTION(_pParam);

    if (diff & _pParam->overflowMask)
        return (_pParam->sizeMask + 1);
    else 
        return (diff);
}

/* ��������� ������� ������.
 * ����������:
 * - CIRBUF_BUFFER_EMPTY - ���� ����� ����;
 * - CIRBUF_DATA_LOST - ���� ���������� ������� �� ����� ��������������;
 * - CIRBUF_BUFFER_FULL - ���� ����� �����;
 * - CIRBUF_NO_ERROR - �� ���� ��������� �������.
 */
CIRBUF_Error CIRBUF_GetStatus(CIRBUF_Param* _pParam)
{
    CIRBUF_IndexType diff; 

    CIRBUF_ENTER_CRITICAL_SECTION(_pParam); 
    diff = CIRBUF_CALCULATE_HEAD_TO_TAIL_DIFF(_pParam);
    CIRBUF_LEAVE_CRITICAL_SECTION(_pParam);
        
    if (0 == diff)
        return (CIRBUF_BUFFER_EMPTY);
    else if ((diff - 1) & _pParam->overflowMask)
        return (CIRBUF_DATA_LOST);
    else if (diff & _pParam->sizeMask)
        return (CIRBUF_NO_ERROR);
    else
        return (CIRBUF_BUFFER_FULL);
}

/* ��������� ���������� ���������� ���������.
 *
 */
CIRBUF_IndexType CIRBUF_GetInDataTotal(CIRBUF_Param* _pParam)
{
    return (_pParam->tail);
}

/* ��������� ���������� ����������� ���������.
 *
 */
CIRBUF_IndexType CIRBUF_GetOutDataTotal(CIRBUF_Param* _pParam)
{
    return (_pParam->head);
}



