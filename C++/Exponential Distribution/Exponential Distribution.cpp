#include <iostream>
#include <string>
#include <random>
#include <limits>
#include <numbers>
#include <vector>
#include <sstream>
#include <iterator>
#include <float.h>

using namespace std;


constexpr int MIN_POINT_MENU = 1;
constexpr int MAX_POINT_MENU = 2;
constexpr float FLOAT_MIN = 0;
constexpr float FLOAT_MAX = 1000;
vector<vector<float>> chunks(0, vector<float>(0));



template<typename T> T getNumberByConsol(string defaultText, bool is_limit = false, T minLimit = 0, T maxLimit = 0) {
	string inputUser = "";
	T number = 0.0f;
	while (true)
	{
		cout << defaultText;
		cin >> inputUser;
		try {
			number = stof(inputUser);
			if (is_limit) {
				if (number < minLimit || maxLimit < number) {
					cout << "Число должно быть в пределах между " + to_string(minLimit) + " и " + to_string(maxLimit) << endl;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		catch (invalid_argument const& e) {
			cout << "Введеная строка не содержит числа, попробуйте еще раз" << std::endl;
		}
		catch (out_of_range const& e) {
			cout << "Введеное число слишком большое, попробуйте еще раз" << std::endl;
		}
	}
	return number;

};

int factorial(int i)
{
	if (i <= 0) return 1;
	else return i * factorial(i - 1);
}

vector<vector<float>> exponential(float lamda, int countChunk, int countElementChunk) {
	vector<vector<float>> chunks(countChunk, vector<float>(countElementChunk));

	for (int indexChunk = 0; indexChunk < countChunk; indexChunk++) {
		random_device rd;
		default_random_engine gen(rd());
		uniform_real_distribution<float> distr(FLOAT_MIN, lamda *3);
		
		for (int indexElementChunk = 0; indexElementChunk < countElementChunk; indexElementChunk++) {
			float randomNamber = distr(gen);
			std::scientific;
			float k = (float)lamda * (float)exp((-1*lamda) * randomNamber);
			chunks[indexChunk][indexElementChunk] = k;
			std::cout.precision(4);
		}
		ostringstream oss;
		copy(chunks[indexChunk].begin(), chunks[indexChunk].end() - 1,
			ostream_iterator<float>(oss, ", "));
		oss << chunks[indexChunk].back();
		std::scientific;
		std::cout.precision(2);
		cout << "Чанк:" << indexChunk + 1 << " Результат: " << oss.str() << endl;
	}
	return chunks;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	std::scientific;
	float lamda = 10.25546;
	int countElementChunk = 10, countChunk = 10;
	while (true)
	{
		int pointMenu = -1;
		string emptyString = "";
		system("cls");
		{
			cout << "Практическая работа №1" << endl;
			cout << "Цель создание генератора экспоненциального распределения. Используя формулу: F(x) = lamda*e^(-lamda*x); x>=0" << endl;
			cout << "Алгоритм:" << endl;
			cout << "1) Генерация случайной велечины от 0 до lamda*10(ограничение, что бы меньше 0 было)" << endl;
			cout << "2) Получение числа по формуле " << endl;
			cout << "2) Добавление числа в чанк(массив)" << endl;
			cout << "* кол-во мачанков и элементов в чанк вводит пользователь" << endl << endl;
			cout << "Меню" << endl;
			cout << "1) Ввод/изменения параметров" << endl;
			cout << "2) Генерация данных" << endl;
			//cout << "3) Сохранение данных в файл" << endl << endl;
		}
		pointMenu = getNumberByConsol<int>("Введите пункт меню: ", true, MIN_POINT_MENU, MAX_POINT_MENU);
		float percentageWork = 0;
		float stepPercentageWork = 0;

		switch (pointMenu)
		{
		case 1:
			lamda = getNumberByConsol<float>("Введите параметр lamda(пример: 4,23): ", true, 0, FLT_MAX);
			countChunk = getNumberByConsol<int>("Введите кол-во чанков: ", true, 0, INT32_MAX);
			countElementChunk = getNumberByConsol<int>("Введите элементов в чанке: ", true, 0, INT32_MAX);
			break;
		case 2:
			if (countElementChunk <= 0 || countChunk <= 0) {

				cout << "Вы не инецелизировали данные" << endl;
				system("pause");
				continue;
			}
			percentageWork = 0;
			stepPercentageWork = (float)((float)100 / (countChunk * countElementChunk));
			cout << "Процесс начался" << endl;
			cout << "lamda: " << lamda << endl;
			chunks = exponential(lamda, countChunk, countElementChunk);
			system("pause");
			break;
		default:
			break;
		}
	}
}
