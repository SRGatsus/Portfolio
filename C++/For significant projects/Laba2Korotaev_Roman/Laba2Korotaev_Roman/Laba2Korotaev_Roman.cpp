
#include "stdafx.h"
#include <iostream>


using namespace std;

struct Test
{
	float num;
	string str;
};


//Составная часть дека
struct Node
{
	Test value;
	Node* prev;
	Node* next;
};

//Дек
struct Deque
{
	Node* start;
	Node* end;
	int size;
};

//Создание нового дека
int* DequeNew(Deque* deque)
{
	deque->start = NULL;
	deque->end = NULL;
	deque->size = 0;
	return 0;
}

//Полное удаление дека
int* DequeDelete(Deque* deque)
{
	while (deque->start != NULL)
	{
		Node* next = deque->start->next;
		delete deque->start;
		deque->start = next;
	}
	deque->start = deque->end = NULL;
	deque->size = 0;
	return 0;
}

//Добавление в конец
void DequePushBack(Deque* deque, Test value)
{
	Node* temp = new Node;
	temp->value = value;
	temp->next = temp->prev = NULL;
	if (deque->start == NULL) {
		deque->end = deque->start = temp;
	}
	else
	{
		deque->end->next = temp;
		temp->prev = deque->end;
		deque->end = temp;
	}
	++deque->size;
}

//Удаление из конца
int DequePopBack(Deque* deque)
{
	if (deque->size == 0)
	{
		cout << "Дек пуст\n";
		return -1;
	}
	else
	{
		Node* prev = deque->end->prev;
		delete deque->end;
		deque->end = prev;
		if (deque->end == NULL)
			deque->start = deque->end;
		else
			deque->end->next = NULL;
		--deque->size;

		return 0;
	}
}

//Размер дека
int DequeSize(Deque* deque)
{
	if (deque->size == 0)
	{
		cout << "Дек пуст\n";
		return -1;
	}
	else
	{
		int s = deque->size;
		cout << "Размер дека: " << s << endl;
		return s;
	}
}

//Добавление в начало
int DequePushHead(Deque* deque, Test value)
{
	Node* temp = new Node;
	temp->value = value;
	temp->next = temp->prev = NULL;
	if (deque->start == NULL) {
		deque->end = deque->start = temp;
	}
	else {
		deque->start->prev = temp;
		temp->next = deque->start;
		deque->start = temp;
	}
	++deque->size;
	return 0;
}

//Удаление из начала
int DequePopHead(Deque* deque)
{
	if (deque->size == 0)
	{
		cout << "Дек пуст\n";
		return -1;
	}
	else
	{
		Node* next = deque->start->next;
		delete deque->start;
		deque->start = next;
		if (deque->start == NULL)
			deque->end = deque->start;
		else
			deque->start->prev = NULL;
		--deque->size;
		return 0;
	}
}

//Печать дека
int* DequePrint(Deque* deque)
{
	Node* temp;
	int counter = 0;
	for (temp = deque->start; temp != NULL; temp = temp->next)
	{
		cout << ++counter << ". ";
		cout << "Число: " << temp->value.num << "  Символы: ";
		cout << temp->value.str << "  ";
		cout << endl;
	}
	cout << endl;
	return 0;
}

Test inputData()
{
	Test result;
	cout << "Введите число: ";
	cin >> result.num;
	cout << endl;
	cout << "Введите строку: ";
	cin >> result.str;
	cout << endl;
	return result;
}

int main()
{
	Test lev;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int variant;
	Deque deque;
	DequeNew(&deque);
	while (true)
	{
		system("cls");
		cout << "Добавить элемент в начало - 1\n";
		cout << "Добавить элемент в конец - 2\n";
		cout << "Удалить элемент из начала - 3\n";
		cout << "Удалить элемент из конца - 4\n";
		cout << "Очистить дек - 5\n";
		cout << "Размер дека - 6\n";
		cout << "Напечатать дек - 7\n";
		cout << "Выход - 8\n";
		cin >> variant;

		switch (variant)
		{
		case 1:
		{
			lev = inputData();
			DequePushHead(&deque, lev);
			system("pause");
			break;
		}
		case 2:
		{
			lev = inputData();
			DequePushBack(&deque, lev);
			system("pause");
			break;
		}
		case 3:
		{
			DequePopHead(&deque);
			system("pause");
			break;
		}
		case 4:
		{
			DequePopBack(&deque);
			system("pause");
			break;
		}
		case 5:
		{
			DequeDelete(&deque);
			system("pause");
			break;
		}
		case 6:
		{
			DequeSize(&deque);
			system("pause");
			break;
		}
		case 7:
		{
			DequePrint(&deque);
			system("pause");
			break;
		}
		case 8:
		{
			system("pause");
			exit(0);
		}
		default:
		{
			cout << "Данная команда не определена" << endl;
			system("pause");
			break;
		}
		}
	}

}