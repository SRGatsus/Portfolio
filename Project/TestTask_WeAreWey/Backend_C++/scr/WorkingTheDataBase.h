#pragma once
#pragma  warning(disable: 4996)
#include <pqxx/pqxx>
#include <jsoncons/json.hpp>

using jsoncons::json;
using namespace pqxx;
using namespace std;


class WorkingTheDataBase
{
protected:

	connection* connectionObject = nullptr;
	work* worker;
	string connectionString;
	map <string, bool> isCreateTable = { {"Order", false},
										{"Product", false},
										{"Product_Order", false} };
public:
	
	WorkingTheDataBase(string HostBD, string PortBD, string NameBD, string UserNameBD, string PasswordBD);
	WorkingTheDataBase();

	void Connect();
	void ÑheckAvailabilityTable();
	void CreateTable();
	void insertDataToProduct();
	void UpdateOrder(json data);
	void CreateOrder(json data);
	void CreateProduct(json data);
	void Disconnect();

	json GetAllProduct();
	json GetAllOrder();
	json GetOrder(json data);
};

