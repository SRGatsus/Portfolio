// Laba6Korotaev.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Weapon.h"
#include <stdio.h>
#include <tchar.h>
#include <wtypes.h>
//Изменить 

//функции
void EnterNum(int& Number, int MaxValue, int MinValue);

//переменные
HINSTANCE hDll = LoadLibrary(L"Dll2.dll");
typedef bool (*PMatrixMul)(WCHAR* s);
PMatrixMul Mul = (PMatrixMul)GetProcAddress(hDll, "isNumber");
int Action = 0;



int main()
{   //car
    string Name;
    int MaxSpeed = 0;//максимальная скорость-задется при создании обЪетка,не может меняться.
    double Speed = 0;//текущая скорость-задается в началае/считается при Upload,не больше максимальной
    int Massa = 0;//задается при создании, учитывает обЪем бака. Не может быть больше 1/70 массы
    int CountPeople = 0;//Количество людей в машине, добавляет массу к общиму обЪему
    int MaxPeople = 0;//Макс людей, задается при создании обЪекта
    int MaxFuel = 0;//Максимальный обЪем бака, задается при создании/не меняется в последующим;
    int Fuel = 0;//Текущий обЪем, обновляется в Upload
    double PowerReserve = 0;//Запас хода. Считает Upload
    double FuelConsum = 6.9;//Расход топлива. Задается.
    //airplane
    int MaxHeight;//Максимальная высота
    int NumberEngine;//количество турбин
    int AirplaneType; //тип самолета.0-пассажирский;1-военный;2-грузовой;3-частный;
    //Track
    bool TrailerConnect = false;//присоединет ли прицеп к грузовику
    int NumberAxes = 4;//количество осей на камазе
    int TrailerType;//0-Трайлер тент;1-Трайлер контейнеровоз;2-Трайлер открытая бортовая платформа;3-Трайлер автовоз;
    setlocale(LC_ALL, "Russian_Russia.1251");
    if (hDll == NULL) {
        Action = 100;
        cout << " Ошибка, нет DLL" << endl;
    }
    Car* BMW;
    while (Action < 100) {
        system("cls");
     cout << "Выберети какой тип тарнспорта хотите создать" << endl;
     cout << "0)Машина" << endl;
     cout << "1)Самолет" << endl;
     cout << "2)Тягач" << endl;
     cout << "3)Выход " << endl;
     cout << "Команда: ";
     EnterNum(Action,4,0);
     switch (Action)
     {
     case 0: {
         cout << "Введите название машины" << endl;
         cin >> Name;
         system("cls");
         cout << "Введите массу машины(мин 50кг,макс 15000кг):" << endl;
         EnterNum(Massa, 15001, 50);
         system("cls");
         cout << "Введите максимальное количество людей в машине(мин 1,макс 12):" << endl;
         EnterNum(MaxPeople, 13, 1);
         system("cls");
         cout << "Введите максимальное количество топлива(кг)(мин 10кг,макс 200кг) *Изменить нельзя будет" << endl;
         EnterNum(MaxFuel, 201, 10);
         system("cls");
         cout << "Введите максимальную скорость(км/ч)(мин 10,макс 300) *Изменить нельзя будет" << endl;
         EnterNum(MaxSpeed, 301, 10);
         system("cls");
         BMW = new Car(Name, MaxSpeed,Massa, MaxPeople, MaxFuel);
         cout << "Введите текущие количество людей в машине" << endl;
         EnterNum(CountPeople, MaxPeople+1,0);
         system("cls");
         BMW->SetCountPeople(CountPeople);
         cout << "Введите текущий запас горючего(макс. " + to_string(BMW->GetMaxFuel()) + ")" << endl;
         EnterNum(Fuel,MaxFuel,0);
         BMW->SetFuel(Fuel);
         system("cls");
         int AirPlaneMenu = 0;
         while (AirPlaneMenu < 1) {
             cout << "Текущий запас хода(км): " + to_string(BMW->CalPowerReserve()*100) << endl;
             cout << "Изменить?" << endl;
             cout << "0)Да" << endl;
             cout << "1)Нет" << endl;
             EnterNum(AirPlaneMenu, 2, 0);
             if (AirPlaneMenu == 0) {
                 cout << "Сколько хотите оставить?" << endl;
                 cout << "Текущие количество(кг): " + to_string(BMW->GetFuel()) << endl;
                 cout << "Макс значение(кг): " + to_string(BMW->GetMaxFuel()) << endl;
                 EnterNum(Fuel, MaxFuel, 0);
                 BMW->SetFuel(Fuel);
             }
             system("cls");
         }
         system("cls");
         AirPlaneMenu = 0;
         while (AirPlaneMenu < 1) {
             cout << "Текущий количесвто людей в машине: " + to_string(BMW->GetCountPeople()) << endl;
             cout << "Изменить?" << endl;
             cout << "0)Да" << endl;
             cout << "1)Нет" << endl;
             EnterNum(AirPlaneMenu, 2, 0);
             if (AirPlaneMenu == 0) {
                 cout << "Сколько хотите добавить?" << endl;
                 cout << "Текущие количество: " + to_string(BMW->GetCountPeople()) << endl;
                 cout << "Макс значение: " + to_string(BMW->GetMaxPeople()) << endl;
                 EnterNum(CountPeople, MaxPeople+1, -MaxPeople);
                 int Raxnos = BMW->CalPeople(CountPeople);
                 if (Raxnos <= 0) {
                     cout << "Все влезли" << endl;
                 }
                 else {
                     cout << "Не влезло: " + Raxnos << endl;
                 }
             }
             system("cls");
         }
         system("cls");
         BMW->Info();
         system("pause");
         break;
     }
    //самолет
     case 1: {
         cout << "Введите название самолета" << endl;
         cin >> Name;
         system("cls");
         cout << "Введите массу самолета(мин 752кг,макс 28500000кг)" << endl;
         EnterNum(Massa, 28500000, 752);
         system("cls");
         cout << "Введите максимальное количество людей на борту(есть бонус от типа самолета)(мин 10,макс 14000):" << endl;
         EnterNum(MaxPeople, 14000, 10);
         system("cls");
         cout << "Введите максимальное количество топлива(кг)(мин 1200,макс 200000) *Изменить нельзя будет" << endl;
         EnterNum(MaxFuel, 200000, 1200);
         system("cls");
         cout << "Введите максимальную высоту полета(м)(мин 10,макс 100000)" << endl;
         EnterNum(MaxHeight, 10000, 10);
         system("cls");
         cout << "Введите количество турбин(мин. 1,макс 7)" << endl;
         EnterNum(NumberEngine, 8, 1);
         system("cls");
         cout << "Выберете тип самолета" << endl;
         cout << "0)Пассажирский" << endl;
         cout << "1)Военный" << endl;
         cout << "2)Грузовой" << endl;
         cout << "3)Частный" << endl;
         EnterNum(AirplaneType, 4, 0);
         system("cls");
         BMW = new Airplane(Name, Massa, MaxPeople, MaxFuel, MaxHeight, NumberEngine, AirplaneType);
         string Type = ((Airplane*)BMW)->TypeThings();
         cout << "Теперь вы можете перевозить " + Type << endl;
         system("pause");
         system("cls");
         int Max = BMW->GetMaxPeople();
         switch (AirplaneType)
         {
         case 0: {
             Max += 120;
             break;
         }
         case 1: {
             Max = 20;
             break;
         }
         case 2: {
             Max = 10;
             break;
         }
         case 3: {
             Max = 15;
             break;
         }
         default:
             break;
         }
         BMW->SetMaxPeople(Max);
         MaxPeople = Max;
         cout << "Максимальное значение человек на борту изменино на "+to_string(BMW->GetMaxPeople()) << endl;
         system("pause");
         system("cls");
         cout << "Введите текущие количество людей в самолете(макс. " + to_string(BMW->GetMaxPeople()) + ")" << endl;
         EnterNum(CountPeople, MaxPeople+1,0);
         system("cls");
         BMW->SetCountPeople(CountPeople);
         cout << "Введите текущий запас горючего(макс. "+ to_string(BMW->GetMaxFuel()*100) +")" << endl;
         EnterNum(Fuel, MaxFuel+1,0);
         BMW->SetFuel(Fuel);
         system("cls");
         int AirPlaneMenu = 0;
         while (AirPlaneMenu < 1) {
             cout << "Текущий запас хода: "+ to_string(BMW->CalPowerReserve()) << endl;
             cout << "Изменить?" << endl;
             cout << "0)Да" << endl;
             cout << "1)Нет" << endl;
             EnterNum(AirPlaneMenu, 2, 0);
             if (AirPlaneMenu == 0) {
                 cout << "Сколько хотите оставить?" << endl;
                 cout << "Текущие количество(кг): "+ to_string(BMW->GetFuel()) << endl;
                 cout << "Макс значение(кг): " + to_string(BMW->GetMaxFuel()) << endl;
                 EnterNum(Fuel,MaxFuel+1,0);
                 BMW->SetFuel(Fuel);
             }
             system("cls");
         }
         system("cls");
         AirPlaneMenu = 0;
         while (AirPlaneMenu < 1) {
             cout << "Текущий количесвто людей на борту: " + to_string(BMW->GetCountPeople()) << endl;
             cout << "Изменить?" << endl;
             cout << "0)Да" << endl;
             cout << "1)Нет" << endl;
             EnterNum(AirPlaneMenu, 2,0);
             if (AirPlaneMenu == 0) {
                 cout << "Сколько хотите добавить?" << endl;
                 cout << "Текущие количество: " + to_string(BMW->GetCountPeople()) << endl;
                 cout << "Макс значение: " + to_string(BMW->GetMaxPeople()) << endl;
                 EnterNum(CountPeople, MaxPeople+1, -MaxPeople);
                 BMW->SetFuel(CountPeople);
                 int Raxnos = BMW->CalPeople(CountPeople);
                 if (Raxnos <= 0) {
                     cout << "Все влезли" << endl;
                 }
                 else {
                     cout << "Не влезло: " + Raxnos << endl;
                 }
             }
             system("cls");
         }
         system("cls");
         BMW->Info();
         system("pause");
         break;
     }
     //трак
     case 2: {
         cout << "Введите название тягоча" << endl;
         cin >> Name;
         system("cls");
         cout << "Введите массу тягоча(мин 1 000 000,макс 10 000 000)" << endl;
         EnterNum(Massa, 10000000, 1000000);
         system("cls");
         cout << "Введите максимальное количество людей в тягоча(мин 10,макс 200)(есть бонус от типа прицепа):" << endl;
         EnterNum(MaxPeople, 201, 10);
         system("cls");
         cout << "Введите максимальную скорость(км/ч)(мин 10,макс 200) *Изменить нельзя будет" << endl;
         EnterNum(MaxSpeed, 201, 10);
         system("cls");
         cout << "Введите максимальное количество топлива(кг)(мин 50,макс 400 *Изменить нельзя будет" << endl;
         EnterNum(MaxFuel, 401, 50);
         system("cls");
         cout << "Введите количества осей(м)" << endl;
         EnterNum(NumberAxes,8, 2);
         system("cls");
         cout << "Хотите подлкючить трейлер ?(1-да,0-нет)(мин 2,макс 8)" << endl;
         int flagTrackConnect=0;
         EnterNum(flagTrackConnect,2, 0);
         flagTrackConnect == 0 ? TrailerConnect = false : TrailerConnect = true;
         system("cls");
         cout << "Выберете тип трейлера" << endl;
         cout << "0)Тент" << endl;
         cout << "1)Контейнеровоз" << endl;
         cout << "2)Открытая бортовая платформа" << endl;
         cout << "3)Автовоз" << endl;
         EnterNum(TrailerType, 4, 0);
         system("cls");
         BMW = new Truck(Name, MaxSpeed,Massa, MaxPeople, MaxFuel,TrailerConnect, NumberAxes, TrailerType);
         int Max = BMW->GetMaxPeople();
         switch (TrailerType)
         {
         case 0: {
             Max += 120;
             break;
         }
         case 1: {
             Max = 20;
             break;
         }
         case 2: {
             Max = 10;
             break;
         }
         case 3: {
             Max = 15;
             break;
         }
         default:
             break;
         }
         BMW->SetMaxPeople(Max);
         MaxPeople = Max;
         cout << "Максимальное значение человек в тягоче  изменино на " + to_string(BMW->GetMaxPeople()) << endl;
         system("pause");
         system("cls");
         cout << "Введите текущие количество людей в Тягоче(макс. "+to_string(BMW->GetMaxPeople())+")" << endl;
         EnterNum(CountPeople, MaxPeople+1,0);
         system("cls");
         int Raxnos = BMW->CalPeople(CountPeople);
         if (Raxnos <= 0) {
             cout << "Все влезли" << endl;
         }
         else {
             cout << "Не влезло: " + Raxnos << endl;
         }
         cout << "Введите текущий запас горючего(макс. " + to_string(BMW->GetMaxFuel()) + ")" << endl;
         EnterNum(Fuel, MaxFuel,0);
         BMW->SetFuel(Fuel);
         system("cls");
         int AirPlaneMenu = 0;
         while (AirPlaneMenu < 1) {
             cout << "Текущий запас хода(км): " + to_string(BMW->CalPowerReserve()*100) << endl;
             cout << "Изменить?" << endl;
             cout << "0)Да" << endl;
             cout << "1)Нет" << endl;
             EnterNum(AirPlaneMenu, 0, 2);
             if (AirPlaneMenu == 0) {
                 cout << "Сколько хотите оставить?" << endl;
                 cout << "Текущие количество(кг): " + to_string(BMW->GetFuel()) << endl;
                 cout << "Макс значение(кг): " + to_string(BMW->GetMaxFuel()) << endl;
                 EnterNum(Fuel, MaxFuel+1,0);
                 BMW->SetFuel(Fuel);
             }
             system("cls");
         }
         system("cls");
         BMW->Info();
         system("pause");
         break;
     }
     case 3: {
         Action = 100;
         break;
     }
     default:
         break;
     }
    };
}
void EnterNum(int& Number,int MaxValue,int MinValue) {
    bool flagEnterNum = false;
    while (flagEnterNum == false) {
        //ввод последовательности символов, которые нужно привести к int
        char Inp[255];
        cin >> Inp;
        typedef bool (*PMatrixMul)(char* s);
        PMatrixMul Mul = (PMatrixMul)GetProcAddress(hDll, "isNumber");
        bool flag = (*Mul)(Inp);

        if (flag) {
            cout << "Неверный ввод. Попробуйте снова.\n";
            continue;
        }
        else {
            if (atol(Inp) < MaxValue && atol(Inp) >=MinValue) {
                Number = atoi(Inp);
                flagEnterNum = true;
            }
            else {
                cout << "Вы ввели слишком большое число. Попробуйте снова.\n";
                flagEnterNum = false;
            }
        }
    }
}

