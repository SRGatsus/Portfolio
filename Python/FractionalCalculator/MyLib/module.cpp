#include <Windows.h>
#include <cmath>
#include <Python.h>
#include <string>
using namespace std;

string summation(string a, string  b) {
	int transfer = 0;
	string result = "";
	if (a.length() < b.length()) {
		string temp = a;
		a = b;
		b = temp;
		b = string((a.length() - b.length()), '0') + b;
	}
	else {
		b = string((a.length() - b.length()), '0') + b;
	}
	int iA = a.length() - 1;
	int iB = b.length() - 1;
	while (iA > -1) {
		int intA = a[iA] - '0';
		int intB = b[iB] - '0';
		int sum = intA + intB + transfer;
		int intC = sum % 10;
		transfer = sum/10;
		result += to_string(intC);
		iA -= 1;
		iB -= 1;
	}
	result += to_string(transfer);
	int iResult = 0;
	int i = 0;
	reverse(result.begin(), result.end());
	while (result[iResult] == '0' && iResult < result.length() - 1) {
		i += 1;
		iResult += 1;
	}
	return result.substr(i, result.length());
}


string multiplication(string a1, string b1) {
	string result = "0";
	int transfer = 0;
	string a = a1.substr(1, a1.length()-2);
	string b = b1.substr(1, b1.length()-2);
	int iA = a.length() - 1;
	int iB = b.length() - 1;
	while (iA > -1) {
		int intA = a[iA] - '0';
		iB = b.length() - 1;
		string interimResult = string(((a.length() - 1) - iA), '0');

		transfer = 0;
		while (iB > -1) {
			int intB = b[iB] - '0';
			int sum = intA * intB + transfer;
			int intC = sum % 10;
			transfer = sum / 10;
			interimResult += to_string(intC);
			iB -= 1;
			
		}
		interimResult += to_string(transfer);
		reverse(interimResult.begin(), interimResult.end());
		result = summation(result, interimResult);
		iA -= 1;
	}
	int iResult = 0;
	int i = 0;
	while (result[iResult] == '0' && iResult < result.length() - 1) {
		i += 1;
		iResult += 1;
	}
	return result.substr(i, result.length());
}

PyObject* tanh_impl(PyObject* /* unused module reference */, PyObject* args) {
	PyObject* repr = PyObject_Repr(args);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	string bytesA = string(PyBytes_AS_STRING(str));
	string bytesB = string(PyBytes_AS_STRING(str));
	string result = multiplication(bytesA, bytesB);
	//printf("counted:");
	//printf(result.c_str());
	//printf("\n");
	return PyFloat_FromDouble(0.0f);
}
static PyMethodDef superfastcode_methods[] = {
	// The first property is the name exposed to Python, fast_tanh
	// The second is the C++ function with the implementation
	// METH_O means it takes a single PyObject argument
	{ "fast_tanh", (PyCFunction)tanh_impl, METH_O, nullptr },

	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};
static PyModuleDef superfastcode_module = {
	PyModuleDef_HEAD_INIT,
	"LibPython",                        // Module name to use with Python import statements
	"Provides some functions, but faster",  // Module description
	0,
	superfastcode_methods                   // Structure that defines the methods of the module
};
PyMODINIT_FUNC PyInit_LibPython() {
	return PyModule_Create(&superfastcode_module);
}