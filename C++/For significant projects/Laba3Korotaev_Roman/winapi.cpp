#include "stdafx.h"
#include "winapi.h"
#include <string>
#include <iomanip>
//���������
#define MAX_LOADSTRING 100

typedef HWND **PPHWND;

//���������� ����������
WCHAR szTitle[MAX_LOADSTRING] = _T("WinApi");
WCHAR szTitleMul[MAX_LOADSTRING] = _T("�������");
WCHAR szWindowClass[MAX_LOADSTRING] = _T("main");
WCHAR szWindowClassMul[MAX_LOADSTRING] = _T("mul");

HINSTANCE hInst;

HWND hwnd;
HWND mainWnd;
HWND mulWnd;

ATOM MainRegisterClass(HINSTANCE);
ATOM MulRegisterClass(HINSTANCE);

BOOL InitMainInstance(HINSTANCE, int);
BOOL InitMulInstance(HINSTANCE, int);

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MulWndProc(HWND, UINT, WPARAM, LPARAM);

void CreateMatrix(HWND);
void CreateResultMatrix(HWND);
void ClearMatrix(PPHWND*, int);
bool VerifyString(WCHAR *str);
bool VerifyMatrixElement(WCHAR *str);
bool isNumber(WCHAR *s);
bool AlgrtmDekstr(int A, int B, int FGH);
void saveMatrix();
bool flag = false;
bool flagClosw = false;
int MassMatrix[7][7];
void CreateMatrix2(HWND hWnd);

HWND lMatrixSize, lMultiplier, lMultiplier2, lMatrix, lMatrix2, lResultMatrix, lAnswer;
//Edit texts
HWND etMatrixRows, etMatrixCols, etMultiplier, etMultiplier2, etReslt, IDE_Edit1;
//Buttons
HWND btnCreate, btnStart;
//Matrix size
int n = 0, m = 0;

//Matrix edit texts
PPHWND etMatrix, etMatrix2;
PPHWND etResultMatrix;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)//������� ������� (��� � ������� ���������� main)
{
	hInst = hInstance;

	if (!MainRegisterClass(hInstance)) return FALSE;

	if (!InitMainInstance(hInstance, nCmdShow)) return FALSE;

	MSG msg;
	/*
	HWND hwnd,     ���������� ����
	UINT message,  ������������� ���������
	WPARAM wParam, ���. ��������
	LPARAM lParam, ���. ��������
	DWORD time,    ����� ��������� � �������
	POINT pt,      ������������ ��������� ����, ��� �� ��� � ������ �������� ��������� (����� ��� ����������� ������)
	*/
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MainRegisterClass(HINSTANCE hInstance)//������� ����������� ������� ������
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

ATOM MulRegisterClass(HINSTANCE hInstance)//������� ����������� ������� ������
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MulWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClassMul;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}



BOOL InitMainInstance(HINSTANCE hInstance, int nCmdShow)//������ ������� ����
{
	
	/*
	LPCTSTR lpClassName,    ��������� �� ������������������ ��� ������
	LPCTSTR lpWindowName,   ��������� �� ��� ����
	DWORD dwStyle,          ����� ���� (����� ��������� ����� ��������� ����� �������� ��� (|))
	int x,                  �������������� ������� ����
	int y,                  ������������ ������� ����
	int nWidth,             ������ ����
	int nHeight,            ������ ����
	HWND hWndParent,        ���������� ������������� ��� ���� ���������
	HMENU hMenu,            ���������� ���� ��� ID ��������� ����
	HANDLE hInstance,       ���������� ���������� ����������
	LPVOID lpParam          ��������� �� ������ �������� ����
	*/
	//��������� �������� �� ������ ������� ����, ��� ��� �������� hWndParent = 0 (0 = NULL)
	mainWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		100, 100,600, 600, nullptr, nullptr, hInstance, nullptr);



	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu = CreatePopupMenu();
	HMENU hPopupMenu2 = CreatePopupMenu();
	HMENU hPopupMenu3 = CreatePopupMenu();

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&����");
	{
		AppendMenu(hPopupMenu, MF_STRING, 1, L"�����");
	}

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu2, L"�������"); 
	{
		AppendMenu(hPopupMenu2, MF_STRING, 2, L"������� ���������");
	}
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu3, L"&������");
	{
		AppendMenu(hPopupMenu3, MF_STRING, 3, L"�� ������ � � B?");
	}

	SetMenu(mainWnd, MainMenu);

	//SetMenu(mainWnd, hmenu1);
	if (!mainWnd) return FALSE;

	//���������� � ���������
	ShowWindow(mainWnd, nCmdShow);
	UpdateWindow(mainWnd);

	return TRUE;
}

BOOL InitMulInstance(HINSTANCE hInstance, int nCmdShow)//������ �������� ���� 1
{
	mulWnd = CreateWindowW(szWindowClassMul, szTitleMul, WS_OVERLAPPEDWINDOW | WS_HSCROLL,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, mainWnd, nullptr, hInstance, nullptr);

	if (!mulWnd) return FALSE;

	ShowWindow(mulWnd, nCmdShow);
	UpdateWindow(mulWnd);
	//�����p��� ���� � ���� � � ��������p� � ���� ��� � �p��� ��p�������
	EnableWindow(mainWnd, FALSE);

	return TRUE;
}


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)//������� ��������� ��������� ������� ��������� ����
{
	switch (message)
	{
	case WM_CREATE:
		if (!MulRegisterClass(hInst)) return FALSE;
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{

		case 2: {
			flag = false;
			InitMulInstance(hInst, SW_SHOWNORMAL);

			break;
		}
		case 1: {
			  DestroyWindow(hWnd);
			  break;
		}
		case 3: {
			flag = true;
			InitMulInstance(hInst, SW_SHOWNORMAL);
			break;
		}
		default: {

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_SYSKEYDOWN:
		if ((wParam == VK_F4) && (lParam & (1 << 29))) {
			ClearMatrix(&etMatrix, n);
			etMatrix = NULL;
			DestroyWindow(mainWnd);
		}
		break;
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}




LRESULT CALLBACK MulWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)//������� ��������� ��������� ������� ��������� ����
{

	switch (message)
	{
	case WM_CREATE:
	{
		if (!flag){
			ShowWindow(CreateWindowW(_T("static"), _T("����������� �������:"), WS_CHILD | WS_VISIBLE,
				20, 20, 160, 20, hWnd, nullptr, nullptr, nullptr), SW_SHOWNORMAL);
			etMatrixRows = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
				180, 20, 30, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(etMatrixRows, SW_SHOWNORMAL);
			btnCreate = CreateWindowW(_T("button"), _T("�������"), WS_CHILD | WS_VISIBLE | WS_BORDER,
				220, 20, 100, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(btnCreate, SW_SHOWNORMAL);
			ShowWindow(CreateWindowW(_T("static"), _T("���� ������ 0 ��� 1:"), WS_CHILD | WS_VISIBLE,
				20, 50, 160, 20, hWnd, nullptr, nullptr, nullptr), SW_SHOWNORMAL);
			if (n > 0) {
				CreateMatrix(hWnd);
				for (int i = 1; i < n; i++) {
					for (int j = 1; j < n; j++) {
						WCHAR value[3];
						_itow(MassMatrix[i][j], value, 11);
						SetWindowTextW(etMatrix[i][j], value);
					}
				}
			}
			return 0;
		}else{
			lMultiplier = CreateWindowW(_T("static"), _T("��:"), WS_CHILD | WS_VISIBLE,
				20, 20, 30, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(lMultiplier, SW_SHOWNORMAL);

			etMultiplier = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
				50, 20, 30, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(etMultiplier, SW_SHOWNORMAL);

			lMultiplier2 = CreateWindowW(_T("static"), _T("�:"), WS_CHILD | WS_VISIBLE,
				90, 20, 30, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(lMultiplier2, SW_SHOWNORMAL);

			etMultiplier2 = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
				120, 20, 30, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(etMultiplier2, SW_SHOWNORMAL);

			btnStart = CreateWindowW(_T("button"), _T("������"), WS_CHILD | WS_VISIBLE | WS_BORDER,
				170, 20, 100, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(btnStart, SW_SHOWNORMAL);
			ShowWindow(CreateWindowW(_T("static"), _T("���������:"), WS_CHILD | WS_VISIBLE,
				20, 50, 80, 20, hWnd, nullptr, nullptr, nullptr), SW_SHOWNORMAL);

			etReslt = CreateWindowW(_T("static"), _T(""), WS_CHILD | WS_VISIBLE ,
				110, 50, 30, 20, hWnd, nullptr, nullptr, nullptr);
			ShowWindow(etReslt, SW_SHOWNORMAL);
			return 0;
		}

		break;
	}

	case WM_COMMAND: {
		
		if (lParam == (LPARAM)btnCreate) {
			//Clear prev data
			ClearMatrix(&etMatrix, n);
			etMatrix = NULL;
			if (lResultMatrix != NULL && IsWindow(lResultMatrix)) DestroyWindow(lResultMatrix);
			//Get matrix rows size
			WCHAR value[3];
			GetWindowText(etMatrixRows, value, 3);
			n = _wtoi(value);
			if (n <= 0 || !VerifyString(value)) {
				n = 0;
				MessageBox(nullptr, _T("������ ��� ����� ���������� ����� �������"), _T("������"), MB_OK | MB_TASKMODAL);
				return 0;
			}
			else if (n >7) {
				n = 0;
				MessageBox(nullptr, _T("������������ ������ �������: 10"), _T("������"), MB_OK | MB_TASKMODAL);
				return 0;
			}
			n++;
			CreateMatrix(hWnd);
			UpdateWindow(mulWnd);
			return 0;
		}
		else if (lParam == (LPARAM)btnStart) {
			if (n == 0) {
				MessageBox(nullptr, _T("������,������� ������� �������"), _T("������"), MB_OK | MB_TASKMODAL);
				return 0;
			}
			WCHAR value2[3];
			GetWindowText(etMultiplier, value2, 3);
			WCHAR value3[3];
			GetWindowText(etMultiplier2, value3, 3);
			if (!isNumber(value2) || !wcslen(value2) > 0 || !isNumber(value3) || !wcslen(value3) > 0) {
				MessageBox(nullptr, _T("������,�� �� ����� ���� �� �����"), _T("������"), MB_OK | MB_TASKMODAL);
				return 0;
			}
			int fromA = _wtoi(value2);
			int toB = _wtoi(value3);
			if (fromA<=0 || fromA >= n || toB <=0|| toB >= n) {
				MessageBox(nullptr, _T("������,�� ����� �� ������������ ����"), _T("������"), MB_OK | MB_TASKMODAL);
				return 0;
			}
			bool FalgGO = AlgrtmDekstr(fromA,toB,n);
			if (FalgGO){
				LPCWSTR somefunc = L"��";
				SetWindowTextW(etReslt, somefunc);
			}
			else {
				LPCWSTR somefunc = L"���";
				SetWindowTextW(etReslt, somefunc);
			}
			return 0;
		}else  {
			int wmId = LOWORD(wParam);
			if (wmId>=4 && wmId<=104 ){
				HWND item = GetDlgItem(hWnd, wmId);
				WCHAR value[11];
				GetWindowText(item, value, 3);
				if (isNumber(value) && wcslen(value) > 0) {
					int NumInMatrixIJ = _wtoi(value);
					if (!(NumInMatrixIJ == 0 || NumInMatrixIJ == 1)) {
						Beep(500, 300);
						LPCWSTR somefunc = L"";
						SetWindowTextW(item, somefunc);
						return 0;
					}
					else {
					}
					return 0;
				}
				else if (wcslen(value) > 0) {
					int af = rand() % (750 - 400 + 1) + 400;
					Beep(af, 300);
					LPCWSTR somefunc = L"";
					SetWindowTextW(item, somefunc);
					return 0;
				}
				return 0;
			}
		}
		UpdateWindow(mulWnd);
		break;
	}

	case WM_SYSCOMMAND:
		if (wParam == SC_MINIMIZE)
			ShowWindow(mulWnd, SW_SHOWMINNOACTIVE);
		else
			DefWindowProc(hWnd, message, wParam, lParam);
		break;

	case WM_CLOSE:
		if (!flag) { saveMatrix(); }
		ClearMatrix(&etMatrix, n);
		etMatrix = NULL;
		EnableWindow(mainWnd, true);
		ShowWindow(mainWnd, SW_SHOW);
		DestroyWindow(mulWnd);
		SetFocus(mainWnd);
		CreateMatrix2(mainWnd);
		for (int i = 1; i < n; i++) {
			for (int j = 1; j < n; j++) {
				WCHAR value[3];
				_itow(MassMatrix[i][j], value, 11);
				SetWindowTextW(etMatrix2[i][j], value);
			}
		}
		break;

	case WM_SYSKEYDOWN:
		if ((wParam == VK_F4) && (lParam&(1 << 29))){
			if (!flag) {saveMatrix();}
			ClearMatrix(&etMatrix, n);
			etMatrix = NULL;
			DestroyWindow(mulWnd);
			DestroyWindow(mainWnd);
			CreateMatrix2(mainWnd);
			for (int i = 1; i < n; i++) {
				for (int j = 1; j < n; j++) {
					WCHAR value[3];
					_itow(MassMatrix[i][j], value, 11);
					SetWindowTextW(etMatrix2[i][j], value);
				}
			}
		}
		break;
			
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void CreateMatrix(HWND hWnd) { //������� �������� �������
	etMatrix = new HWND*[n];
	for (int i = 0; i < n; i++) {
		etMatrix[i] = new HWND[n];
	}
	int x = 4;
	WCHAR value[6];
	lMatrix = CreateWindowW(_T("static"), _T("�������:"), WS_CHILD | WS_VISIBLE,
		20, 80, 65, 20, hWnd, nullptr, nullptr, nullptr);
	ShowWindow(lMatrix, SW_SHOWNORMAL);
	for (int i = 0; i < n ; i++) {
		for (int j = 0; j < n ; j++) {
			if (i==0 && j >=0) {
				etMatrix[i][j] = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED,
					20 + 40 * j, 110 + 30 * i, 30, 20, hWnd, nullptr, nullptr, nullptr);
				_itow(j, value, 11);
				SetWindowTextW(etMatrix[i][j],value);
			}
			else if(i >0 && j == 0){
				etMatrix[i][j] = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED,
					20 + 40 * j, 110 + 30 * i, 30, 20, hWnd, nullptr, nullptr, nullptr);
				_itow(i, value,11);
				SetWindowTextW(etMatrix[i][j], value);
			}
			else {
				etMatrix[i][j] = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
					20 + 40 * j, 110 + 30 * i, 30, 20, hWnd, (HMENU)x, nullptr, nullptr);
			}
			x++;
			
		}
	}
}

void ClearMatrix(PPHWND *matrix, int rows) { //�������� �������
	if (*matrix != NULL) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < rows; j++) {
				if (IsWindow((*matrix)[i][j])) {
					DestroyWindow((*matrix)[i][j]);
				}
			}
			delete[](*matrix)[i];
		}
		delete[](*matrix);
	}
}

bool VerifyString(WCHAR *str) {
	if (!iswdigit(str[0]) && str[0] != '-') return FALSE;
	for (int i = 1; i < wcslen(str); i++) {
		if (!iswdigit(str[i])) {
			return FALSE;
		}
	}

	return TRUE;
}

bool VerifyMatrixElement(WCHAR *str) {
	if (!VerifyString(str)) return FALSE;
	if (wcslen(str) > 2 && str[0]!='-') return FALSE;
	return TRUE;
}
bool isNumber(WCHAR *s) {

	for (int a = 0; a < wcslen(s); a++) {
		// ���� � ������ ���� �� �������� �������, �� ��� �� �����
		if (s[a] != 45 || s[a] != 43) { // �������� �� ���� �����
			if ((s[a] < 48) || (s[a] > 57))  return false;
		}

	}
	// ���� � ������ ������ �������� ������� ������ ��� �����
	return true;
}







/////////////////////////////////////////////////
//����������� ��� �������
bool AlgrtmDekstr(int AFG, int B,int FGH) {
	int big_num(10000);

	int  minMas(0), index_min(0);
	int* pos = new int[FGH];
	int* node = new int[FGH];
	int* min = new int[FGH];
	for (int i = 0;i < FGH;++i) {     // ��������� ���� � ������� �������� �������, ���������� ����� ���_��� ��� ������, �� � ��� ��.
		pos[i] = big_num;       // � ��� ������� �������� ��� "������������"
		node[i] = 0;
	}
	pos[AFG] = 0;                // ��������� �����-�� ������� ������� ���������, ����� ( ��� ��� �� �������, ��)
	for (int i = 0;i < FGH-1;++i) {    // �������� ����
		minMas = big_num;
		for (int j = 0;j < FGH;++j) {     // ������� ������� � ����������� � ��� �����������, �� ������ ���� ��� ����� ����
			if (!node[j] && pos[j] < minMas) {
				minMas = pos[j];
				index_min = j;
			}
		}
		node[index_min] = true;   // �������� ��������� ������� ��� ����������
		for (int j = 0;j < FGH;++j) {   // ����, � ������� �� ���� ���� ��������, ������� � ��������� ��� ���� � ���
			if (!node[j] && MassMatrix[index_min][j] > 0 && pos[index_min] != big_num && pos[index_min] + MassMatrix[index_min][j] < pos[j]) {
				pos[j] = pos[index_min] + MassMatrix[index_min][j];
			} // ������� �����, ���� ��� ������� �� ���������� � ��� ������ � ��������� � ���� ����� ���� ��������� ������� � ����� � ������� ����� ������, ���
		}     // ��� ������� �� ������ ������, ��  - ������ �������� ���� ������� �������.
	}
	//cout << pos[0] << "\n"; // ������ � ��� � pos ����������� ���������� �� ���������� ���� � ����� ������ �����
	if (pos[B] == big_num) {
		return false;
	}
	else {
		return true;
	}
}
/////////////////////////////////////////////////











void saveMatrix() {
	for (int i = 1; i < n; i++) {
		for (int j = 1; j < n; j++) {
			WCHAR value[3];
			GetWindowText(etMatrix[i][j], value, 11);
			int fg = _wtoi(value);
			if (i == j) 
			{
				MassMatrix[i][j] = 1;
			}
			else {
				MassMatrix[i][j] = fg;
			}
		}
	}
}

void CreateMatrix2(HWND hWnd) { //������� �������� �������
	etMatrix2 = new HWND * [n];
	for (int i = 0; i < n; i++) {
		etMatrix2[i] = new HWND[n];
	}
	int x = 4;
	WCHAR value[6];
	lMatrix2 = CreateWindowW(_T("static"), _T("�������:"), WS_CHILD | WS_VISIBLE,
		20, 20, 65, 20, hWnd, nullptr, nullptr, nullptr);
	ShowWindow(lMatrix, SW_SHOWNORMAL);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == 0 && j >= 0) {
				etMatrix2[i][j] = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED,
					20 + 40 * j, 55 + 30 * i, 30, 20, hWnd, nullptr, nullptr, nullptr);
				_itow(j, value, 11);
				SetWindowTextW(etMatrix2[i][j], value);
			}
			else if (i > 0 && j == 0) {
				etMatrix2[i][j] = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED,
					20 + 40 * j, 55 + 30 * i, 30, 20, hWnd, nullptr, nullptr, nullptr);
				_itow(i, value, 11);
				SetWindowTextW(etMatrix2[i][j], value);
			}
			else {
				etMatrix2[i][j] = CreateWindowW(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED,
					20 + 40 * j, 55 + 30 * i, 30, 20, hWnd, (HMENU)x, nullptr, nullptr);
			}
			x++;

		}
	}
}