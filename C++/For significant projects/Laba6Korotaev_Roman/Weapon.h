#include <string>
using namespace std;
#include <vector>
#include <time.h>

class Car {
private:
	string Name;
	double MaxSpeed=0;//максимальная скорость-задется при создании объетка,не может меняться.
	double Speed=0;//текущая скорость-задается в началае/считается при Upload,не больше максимальной
	double Massa=0;//задается при создании, учитывает объем бака.
	int CountPeople=0;//Количество людей в машине, добавляет массу к общиму обЪему
	int MaxPeople=0;//Макс людей, задается при создании объекта
	double MaxFuel=0;//Максимальный обЪем бака, задается при создании/не меняется в последующим;
	double Fuel=0;//Текущий обЪем.
	double PowerReserve=0;//Запас хода.
	double FuelConsum=6.9;//Расход топлива. Задается.

public:
	Car(string Name="Lada granta",int MaxSpeed =200, int Massa =1400,int MaxPeople =20, int MaxFuel =100) {
		this->MaxSpeed = MaxSpeed;
		this->Name = Name;
		this->Massa = Massa;
		this->MaxPeople = MaxPeople;
		this->MaxFuel = MaxFuel;
	}
	~Car() {}
	//set/get
	void SetSpeed(double Speed) {
		this->Speed = Speed;
	}
	void SetMassa(double  Massa) {
		this->Massa = Massa;
	}
	void SetCountPeople(int  CountPeople) {
		this->CountPeople = CountPeople;
	}
	void SetFuel(double Fuel) {
		this->Fuel = Fuel;
	}
	void SetMaxPeople(int MaxPeople) {
		this->MaxPeople = MaxPeople;
	}
	void SetPowerReserve(double PowerReserve) {
		this->PowerReserve = PowerReserve;
	}
	void SetFuelConsum(double FuelConsum) {
		this->FuelConsum = FuelConsum;
	}
	void SetName(string Name) {
		this->Name = Name;
	}
	string GetName() {
		return Name;
	}
	double GetMaxSpeed() {
		return MaxSpeed;
	}
	double GetMaxFuel() {
		return MaxFuel;
	}	
	double GetSpeed() {
		return Speed;
	}	
	double GetMassa() {
		return Massa;
	}	
	double GetFuel() {
		return Fuel;
	}	
	double GetPowerReserve() {
		return PowerReserve;
	}
	double GetFuelConsum() {
		return FuelConsum;
	}
	int GetCountPeople() {
		return CountPeople;
	}
	int GetMaxPeople() {
		return MaxPeople;
	}
	//виртуальные функции
	virtual double CalPowerReserve() {
		PowerReserve = Fuel / FuelConsum; 
		return PowerReserve;
	}
	virtual int CalPeople(int People) {
		CountPeople += People;
		if (CountPeople > MaxPeople) {
			int Raznos = CountPeople -MaxPeople;
			CountPeople = MaxPeople;
			return Raznos;
		}
		else if (CountPeople < 0) {
			CountPeople = 0;
		}
		return 0;
	}
	virtual void Info() {
		cout << "Информация о машине:" << endl;
		cout << "Название: "+Name << endl;
		cout << "Пассажировместимость: "+ to_string(MaxPeople) << endl;
		cout << "Текущие количество людей в машине: "+ to_string(CountPeople) << endl;
		cout << "Полная масса: "+ to_string(Massa) << endl;
		cout << "Расход топлива, л/100 км: ездовой цикл: "+ to_string(FuelConsum) << endl;
		cout << "Максимальная скорость, км/ч: "+ to_string(MaxSpeed) << endl;
		cout << "Объем бака: "+ to_string(MaxFuel) << endl;
		cout << "Запас хода: "+ to_string(PowerReserve) << endl;


	}
};


class Airplane :public Car {
private:
	int MaxHeight;//Максимальная высота
	int NumberEngine ;//количество турбин
	int AirplaneType; //тип самолета.0-пассажирский;1-военный;2-грузовой;3-частный;
public:
	Airplane(string Name, int Massa, int MaxPeople, int MaxFuel, int MaxHeight=1200, int NumberEngine=4, int AirplaneType=0 ) : Car(Name, 0, Massa, MaxPeople, MaxFuel)
	{
		this->MaxHeight = MaxHeight;
		this->NumberEngine = NumberEngine;
		this->AirplaneType = AirplaneType;
		SetFuelConsum(6800);
	}
	~Airplane() {}
	//get/set
	int GetMaxHeight() {
		return MaxHeight;
	}
	int GetNumberEngine() {
		return NumberEngine;
	}
	string GetAirplaneType() {
		switch (AirplaneType)
		{
		case 0: {
			return  "пассажирский";
		}
		case 1: {
			return "военный";
		}
		case 2: {
			return  "грузовой";
		}
		case 3: {
			return  "частный";

		}
		default:
			break;
		}
	}
	void SetMaxHeight(int MaxHeight) {
		this->MaxHeight = MaxHeight;
	}
	void SetNumberEngine(int NumberEngine) {
		this->NumberEngine = NumberEngine;
	}
	void SetTrailerConnect(int AirplaneType) {
		this->AirplaneType = AirplaneType;
	}
	//Виртуальные функции
	 double CalPowerReserve() {
		double PowerReserveAirplane = GetFuel() / GetFuelConsum();
		switch (AirplaneType)
		{
		case 0: {
			PowerReserveAirplane *= 0.75;
		}
		case 1: {
			PowerReserveAirplane *= 0.6;
		}
		case 2: {
			PowerReserveAirplane *= 1.2;
		}
		case 3: {
			PowerReserveAirplane *= 1.8;
		}
		default:
			break;
		}
		SetPowerReserve(PowerReserveAirplane);
		return PowerReserveAirplane;
	}
	 void MaxPeopleAir() {
		 int Max = GetMaxPeople();
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
		 }
		 case 3: {
			 Max = 15;
		 }
		 default:
			 break;
		 }
		 SetMaxPeople(Max);
	 }
	 int CalPeople(int People) {
		int CountPeopleTrack = GetCountPeople();
		int CountPeopleTrack2 = GetCountPeople();
		int Max = GetMaxPeople();
		CountPeopleTrack += People;
			if (CountPeopleTrack > Max ) {
				CountPeopleTrack = Max;
			}else if(CountPeopleTrack < 0){
				CountPeopleTrack = 0;
			}
		SetCountPeople(CountPeopleTrack);
		int Paznos = (People - CountPeopleTrack - CountPeopleTrack2);
		return Paznos;
	}
	 void Info() {
		cout << "Информация о машине:" << endl;
		cout << "Название: " + GetName() << endl;
		cout << "Пассажировместимость: " + to_string(GetMaxPeople()) << endl;
		cout << "Текущие количество людей в машине: " + to_string(GetCountPeople()) << endl;
		cout << "Полная масса: " + to_string(GetMassa()) << endl;
		cout << "Расход топлива, л/100 км: ездовой цикл: " + to_string(GetFuelConsum()) << endl;
		cout << "Максимальная скорость, км/ч: " + MaxSpeedType() << endl;
		cout << "Объем бака: " + to_string(GetMaxFuel()) << endl;
		cout << "Запас хода: " + to_string(GetPowerReserve()) << endl;
		cout << "Количество двигателей: " + to_string(NumberEngine) << endl;
		cout << "Максимальная высота: " + to_string(MaxHeight) << endl;
		cout << "Вид самолета: " + GetAirplaneType() << endl;
		cout << "Минимальная длина полосы м: " + BandWidth() << endl;
		cout << "Тип груза: " + TypeThings() << endl;
	}
	//свои функции(3 штуки)
	//1)расчет максимальной скорости, изходя из количества турбин.
	//2)расчет минимальной длина полосы, изходя из количества турбин.
	//3)тип груза перевозимой самолетом, изходя из количества турбин и типа.
	string MaxSpeedType() {
		
			switch (NumberEngine)
			{
			case 2: {
				return  "800";
			}
			case 3: {
				return "1000";
			}
			case 4: {
				return  "1200";
			}
			case 6: {
				return  "1500";
			}
			default: return  "3000";
				break;
			}
	}
	string BandWidth() {
		switch (NumberEngine)
		{
		case 2: {
			return  "1850";
		}
		case 3: {
			return "2000";
		}
		case 4: {
			return  "2300";
		}
		case 6: {
			return  "2580";
		}
		default: return  "3000";
			break;
		}
	}
	string TypeThings() {
		switch (AirplaneType)
		{
		case 0: {
			return  "Людей и груз";
		}
		case 1: {
			return "военный груз и технику";
		}
		case 2: {
			return  "Груз любого типа";
		}
		case 3: {
			return  "Людей";
		}
		default:
			break;
		}
	}
};
class Truck :public Car {
private:
	bool TrailerConnect = false;//присоединет ли прицеп к грузовику
	int NumberAxes = 4;//количество осей на камазе
	int TrailerType ;//0-Трайлер тент;1-Трайлер контейнеровоз;2-Трайлер открытая бортовая платформа;3-Трайлер автовоз;
public:
	Truck(string Name, int MaxSpeed, int Massa, int MaxPeople , int MaxFuel,bool TrailerConnect, int NumberAxes, int TrailerType=0) : Car(Name, MaxSpeed, Massa, MaxPeople, MaxFuel)
	 {
		this->TrailerConnect = TrailerConnect;
		this->NumberAxes = NumberAxes;
		this->TrailerType = TrailerType;
		SetFuelConsum(36);
	}
	~Truck() {}
	//get/set
	bool GetTrailerConnect() {
		return TrailerConnect;
	}
	int GetNumberAxes() {
		return NumberAxes;
	}
	string GetTrailerTyp() {
		switch (TrailerType)
		{
		case 0: {
			return  "тент";
		}
		case 1: {
			return "контейнеровоз";
		}
		case 2: {
			return  "открытая бортовая платформа";
		}
		case 3: {
			return  "Трайлер автовоз";
		}
		default:
			break;
		}
	}
	void SetTrailerConnect() {
		TrailerConnect = !TrailerConnect;
	}
	void SetNumberAxes(int NumberAxes) {
		this->NumberAxes = NumberAxes;
	}
	void SetTrailerType(int TrailerType) {
		this->TrailerType = TrailerType;
	}
	//виртуальные функции
	 double CalPowerReserve() {
		double PowerReserveTrack = GetFuel() / GetFuelConsum();
		if (TrailerConnect) {
			switch (TrailerType)
			{
			case 0: {
				PowerReserveTrack *= 0.9;
			}
			case 1: {
				PowerReserveTrack *= 0.8;
			}
			case 2: {
				PowerReserveTrack *= 1.2;
			}
			case 3: {
				PowerReserveTrack *= 1.5;
			}
			default:
				break;
			}
		}
		SetPowerReserve(PowerReserveTrack);
		return PowerReserveTrack;
	}
	 void Max() {
		 int Max = GetMaxPeople();
		 switch (TrailerType)
		 {
		 case 0: {
			 Max += 60;
		 }
		 case 1: {
			 Max += 2;
		 }
		 case 2: {
			 Max += 15;
		 }
		 case 3: {
			 Max += 20;
		 }
		 default:
			 break;
		 }
		 SetMaxPeople(Max);
	 }
	 int CalPeople(int People) {
		int CountPeopleTrack = GetCountPeople();
		CountPeopleTrack += People;
		int CountPeopleTrack2 = GetCountPeople();
		int Max = GetMaxPeople();
		if (TrailerConnect) {
			switch (TrailerType)
			{
			case 0: {
				Max += 60;
			}
			case 1: {
				Max += 2;
			}
			case 2: {
				Max += 15;
			}
			case 3: {
				Max += 20;
			}
			default:
				break;
			}

		}
			if (CountPeopleTrack > Max) {
					CountPeopleTrack = Max;
		}
			else if (CountPeopleTrack < 0) {
				CountPeopleTrack = 0;
			}
		SetCountPeople(CountPeopleTrack);
		int Paznos = (People - CountPeopleTrack - CountPeopleTrack2);
		return Paznos;
		return 0;
	}
	void Info() {
		cout << "Информация о машине:" << endl;
		cout << "Название: " + GetName() << endl;
		cout << "Пассажировместимость: " + to_string(GetMaxPeople()) << endl;
		cout << "Текущие количество людей в машине: " + to_string(GetCountPeople()) << endl;
		cout << "Полная масса: " + to_string(GetMassa()) << endl;
		cout << "Расход топлива, л/100 км: ездовой цикл: " + to_string(GetFuelConsum()) << endl;
		cout << "Максимальная скорость, км/ч: " + to_string(GetMaxSpeed()) << endl;
		cout << "Объем бака: " + to_string(GetMaxFuel()) << endl;
		cout << "Запас хода: " + to_string(GetPowerReserve()) << endl;
		cout << "Количество осей: " + to_string(NumberAxes) << endl;
		string Flag = TrailerConnect ? "Да" : "нет";
		cout << "Присоединен прицеп: " + Flag << endl;
		if (TrailerConnect) {
			cout << "Вид прицепа: " + GetTrailerTyp() << endl;
		}
		cout << "Коэффициента сцепления в сухую погоду при скорости 10 км/ч: " + CoefAdhesion() << endl;
		cout << "Минимальная длинна полосы для маневра: " + Minlength() << endl;
		cout << "Тип вещей,которые вы можете перевозить: " + TypeThings() << endl;
	}
	//свои функции (3 штуки)
	//1)расчет коэффициента сцепления, изходя из количества осей.
	//2)расчет минимальной длинна полосы, изходя из типа тенда.
	//3)какие типы веще может перевозить грузовик, изходя из типа тенда.
	string CoefAdhesion() {
		if (TrailerConnect) {
			switch (TrailerType)
			{
			case 0: {
				return  "0.74";
			}
			case 1: {
				return "0.73";
			}
			case 2: {
				return  "0.72";
			}
			case 3: {
				return  "0.76";
			}
			default:
				break;
			}
		}
		else {
			return  "0.75";
		}
	}
	string Minlength() {
		if (TrailerConnect) {
			switch (TrailerType)
			{
			case 0: {
				return  "21 170см";
			}
			case 1: {
				return "19 910см";
			}
			case 2: {
				return  "21 170см";
			}
			case 3: {
				return  "24 297см";
			}
			default:
				break;
			}
		}
		else {
			return  "7 270см";
		}
	}
	string TypeThings() {
		if (TrailerConnect) {
			switch (TrailerType)
			{
			case 0: {
				return  "Все";
			}
			case 1: {
				return "Контейнеры";
			}
			case 2: {
				return  "Коробки,платформы";
			}
			case 3: {
				return  "Технику,машину";
			}
			default:
				break;
			}
		}
		else {
			return  "ничего,ток себя";
		}
	}

};
