#include "WorkingTheDataBase.h"
using namespace jsoncons;


//Initializes the connection to the database
void WorkingTheDataBase::Connect()
{
	try
	{
		connectionObject = new connection(connectionString.c_str());

		worker = new work(*connectionObject);

	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}

//Collects the string in the desired formatand connects it to the Database
WorkingTheDataBase::WorkingTheDataBase(string HostBD, string PortBD, string NameBD, string UserNameBD, string PasswordBD)
{
	 connectionString = "host=" + HostBD + " port=" + PortBD + " dbname=" + NameBD + " user=" + UserNameBD +
		" password =" + PasswordBD;
	Connect();
}

//Checks for tables in the database, 
//if there is a table, then the true attribute is set in the isCreateTable array
void WorkingTheDataBase::СheckAvailabilityTable()
{

	try
	{
		/*result response;
		response = worker->exec("SELECT table_name FROM information_schema.tables WHERE table_schema = 'public' ORDER BY table_name");
		*/
		for (auto [table_name] : worker->stream<string>(
			"SELECT table_name FROM information_schema.tables WHERE table_schema = 'public' ORDER BY table_name"))
		{
			std::cout << table_name << ".\n";
			if(isCreateTable.count(table_name))
			{
				isCreateTable[table_name] = true;
			}
		}

	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}

//Retrieves and returns a complete list of all products from the database
json WorkingTheDataBase::GetAllProduct()
{
	Connect();
	json jsonResult(json_array_arg);
	try
	{
		int numberEntries = worker->query_value<int>(
			R"(SELECT COUNT(*) FROM public."Product")");
		jsonResult.reserve(numberEntries);

		for (auto [Product_id,Title, Description, Price] : worker->stream<int,string,string,int>(
			     R"(SELECT "Product"."Product_id", "Product"."Title","Product"."Description", "Product"."Price" FROM public."Product")"))
		{
			
			json dataProduct;
			dataProduct["Product_id"] = Product_id;
			dataProduct["Title"] = Title;
			dataProduct["Description"] = Description;
			dataProduct["Price"] = std::to_string(Price);
			jsonResult.add(std::move(dataProduct));
		}
		worker->commit();
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
	return jsonResult;
}

//Retrieves and returns a complete list of all orders from the database
json WorkingTheDataBase::GetAllOrder()
{
	Connect();
	json jsonResult(json_array_arg);
	try
	{
		int numberEntries = worker->query_value<int>(
			"SELECT COUNT(*) FROM public.\"Order\"");
		jsonResult.reserve(numberEntries);

		for (auto [Order_id,Title, Date, Sum] : worker->stream<int,string, string, int>(
			"SELECT \"Order\".\"Order_id\",\"Order\".\"Title\",\"Order\".\"Date\", \"Order\".\"Sum\" FROM public.\"Order\""))
		{

			json dataProduct;
			dataProduct["Order_id"] = std::to_string(Order_id);
			dataProduct["Title"] = Title;
			dataProduct["Date"] = Date;
			dataProduct["Sum"] = std::to_string(Sum);
			jsonResult.add(std::move(dataProduct));
		}
		
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
	worker->commit();
	return jsonResult;
}

//Receives a specific order by id.
//Format of the parameter data = { {"idOrder", int} }
json WorkingTheDataBase::GetOrder(json data)
{
	Connect();
	json jsonResult(jsoncons::json_array_arg);
	try
	{
		jsonResult.reserve(1);
		string SQL =
			R"(SELECT "Order"."Order_id","Order"."Title","Order"."Date", "Order"."Sum" FROM public."Order" WHERE "Order"."Order_id"=)";
		string idOrder = data["Order_id"].as_string();
		SQL += idOrder;
		json dataOrder;
		for (auto [Order_id, Title, Date, Sum] : worker->stream<int, string, string, int>(SQL))
		{

			
			dataOrder["Order_id"] = std::to_string(Order_id);
			dataOrder["Title"] = Title;
			dataOrder["Date"] = Date;
			dataOrder["Sum"] = std::to_string(Sum);
		}
		json Product_Order(jsoncons::json_array_arg);//SELECT COUNT(*) from "Order", "Product_Order", "Product" WHERE "Product_Order"."Order_id" = 2 and "Product_Order"."Product_id" = "Product"."Product_id"
		string SQLCount = R"(SELECT COUNT(*) from "Order", "Product_Order", "Product" WHERE "Product_Order"."Product_id" = "Product"."Product_id" and "Product_Order"."Order_id" =)";
		SQLCount += idOrder + R"( and "Order"."Order_id" = )" + idOrder;
		try
		{
			int numberEntries = worker->query_value<int>(SQLCount);
			Product_Order.reserve(numberEntries);
			string SQLProduct = R"(SELECT "Product_Order"."CountProduct","Product"."Product_id", "Product"."Title","Product"."Description","Product"."Price" from "Order", "Product_Order", "Product" WHERE "Product_Order"."Product_id" = "Product"."Product_id" and "Product_Order"."Order_id" =)";
			SQLProduct += idOrder + R"( and "Order"."Order_id" = )" + idOrder;
			for (auto [CountProduct,Product_id, Title, Description, Price] : worker->stream<int,int, string, string, int>(SQLProduct))
			{
				json dataProduct;
				dataProduct["CountProduct"] = std::to_string(CountProduct);
				dataProduct["Product_id"] = std::to_string(Product_id);
				dataProduct["Title"] = Title;
				dataProduct["Description"] = Description;
				dataProduct["Price"] = std::to_string(Price);
				Product_Order.add(std::move(dataProduct));
			}
		}
		catch (const exception& e)
		{
			cout << e.what() << endl;
			
		}
		if(dataOrder.to_string() == "{}")
		{
			dataOrder["isNotOrder"] = true;
		}
		dataOrder["Product_Order"] = Product_Order;
		jsonResult.add(std::move(dataOrder));
}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
	worker->commit();
	return jsonResult;
}

//Updates the order in the database
//with the data parameter format :
//{ 
// "Order_id":int,
// "Product_Order":[
//    {
//      "CountProduct":int,
//      "Price":int,
//      "Product_id":int }]} 
void WorkingTheDataBase::UpdateOrder(json data)
{
	result response;
	Connect();
	try
	{
		string Order_id = std::to_string(data["Order_id"].as_int());
		try
		{
			string SQLOldOrder = R"(DELETE FROM "Product_Order" WHERE "Product_Order"."Order_id" = )" + Order_id;
			response = worker->exec(SQLOldOrder);
			response.clear();
		}
		catch (const exception& e)
		{
			cout << e.what() << endl;
		}
		const json& Product_Order = data["Product_Order"];
		int sum = 0;
		for (const auto& item : Product_Order.array_range())
		{
			// Access rated as string and rating as double
			string SQLOldOrder = R"(INSERT INTO "Product_Order"("Order_id", "Product_id" , "CountProduct" ) VALUES()";
			int CountProduct = item["CountProduct"].as_int();
			int Price = item["Price"].as_int();
			sum += CountProduct * Price;
			SQLOldOrder += "" + Order_id + ", " + std::to_string(item["Product_id"].as_int()) + ", " + std::to_string(CountProduct) + ")";
			response = worker->exec(SQLOldOrder);
			response.clear();
		}
		string SQLNewOrder = R"(UPDATE "Order" SET "Sum" = )";
		SQLNewOrder += std::to_string(sum) + R"( WHERE "Order"."Order_id" =)" + Order_id;
		response = worker->exec(SQLNewOrder);
		response.clear();
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
	worker->commit();
}

//Creates a product in the database 
//with the data parameter format :
//{ 
//      "Title":string,
//      "Description":string,
//      "Price":int } 
void WorkingTheDataBase::CreateProduct(json data)
{
	Connect();
	try
	{
		result response;
		string Title = data["Title"].to_string();
		string Description = data["Description"].to_string();
		string Price = std::to_string(data["Price"].as_int());
		response = worker->exec("INSERT INTO \"Product\"(\"Title\", \"Description\", \"Price\") VALUES('" + Title + "', '" + Description + "', " + Price + ");");
		response.clear();
		worker->commit();
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
}


//Creates an order in the database
//with the data parameter format :
//{ "Title":string}}
void WorkingTheDataBase::CreateOrder(json data)
{
	Connect();
	try
	{
		time_t now = time(0);



		tm* ltm = localtime(&now);
		string Date = std::to_string(ltm->tm_year + 1900) + "-" + std::to_string(ltm->tm_mon + 1) + "-" + std::to_string(ltm->tm_mday);
		cout << "Number of sec since January 1,1970:" << Date << endl;
		result response;
		string Title = data["Title"].as_string();
		response = worker->exec("INSERT INTO \"Order\"(\"Title\", \"Date\", \"Sum\") VALUES('" + Title + "', '" + Date + "', 0);");
		response.clear();
		worker->commit();

	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
}

//Disconnects from the database
void WorkingTheDataBase::Disconnect()
{
	worker->commit();
	connectionObject->close();
}

void WorkingTheDataBase::insertDataToProduct()
{


}

//Creates tables based on the attribute in the isCreateTable array
void WorkingTheDataBase::CreateTable()
{
	Connect();
	result response;
	try
	{
		for (auto value : isCreateTable)
		{
			if (!value.second)
			{

				if (value.first == "Order")
				{
					response = worker->exec("CREATE TABLE public.\"Order\"\
						(\"Order_id\" SERIAL PRIMARY KEY,\
						\"Title\" TEXT NOT NULL,\
						\"Date\" DATE NOT NULL,\
						\"Sum\"  FLOAT NOT NULL)");
					response.clear();
					continue;
				}
				if (value.first == "Product")
				{
					response = worker->exec("CREATE TABLE public.\"Product\"\
						(\"Product_id\" SERIAL PRIMARY KEY,\
						\"Title\" TEXT NOT NULL,\
						\"Description\" TEXT NOT NULL,\
						\"Price\"  FLOAT NOT NULL)");
					response.clear();

					continue;
				}
				if (value.first == "Product_Order")
				{
					response = worker->exec("CREATE TABLE public.\"Product_Order\"\
						(\"OrderProduct_id\" SERIAL PRIMARY KEY,\
						\"Order_id\" integer NOT NULL REFERENCES  public.\"Order\" ( \"Order_id\" ), \
						\"Product_id\" integer NOT NULL REFERENCES public.\"Product\" ( \"Product_id\" ),\
						\"CountProduct\" integer NOT NULL)");
					response.clear();
					continue;
				}
			}
		}
		worker->commit();
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
		worker->abort();
	}
}




//CREATE TABLE public."Order"
//("Order_id"  SERIAL PRIMARY KEY,
//	"Title" TEXT NOT NULL,
//	"Date" DATE NOT NULL,
//	"Sum"  FLOAT NOT NULL);
//CREATE TABLE public."Product"
//("Product_id"  SERIAL PRIMARY KEY,
//	"Title" TEXT NOT NULL,
//	"Description" TEXT NOT NULL,
//	"Price"  FLOAT NOT NULL);
//CREATE TABLE public."OrderProduct"
//("OrderProduct_id"  SERIAL PRIMARY KEY,
//	"Order_id" integer NOT NULL REFERENCES  "Order" ("Order_id"),
//	"Product_id" integer NOT NULL REFERENCES "Product"("Product_id"))


//DROP TABLE public."Product_Order";
//DROP TABLE public."Order";
//DROP TABLE public."Product"

//INSERT INTO "Order"("Date", "Sum", "Title")
//VALUES('2004-10-20', 123, 'fg131313');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('fg131313', 123, 'fg131313');
//INSERT INTO "OrderProduct"("Order_id", "Product_id")
//VALUES(3, 4)

//SELECT* from "Order", "OrderProduct", "Product" WHERE "OrderProduct"."Order_id" = "Order"."Order_id" and "OrderProduct"."Product_id" = "Product"."Product_id"

//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Толщина корпуса ноутбука Honor MagicBook X14 i5/8/512, сделанного из алюминиевого сплава, составляет 15,9 мм. В нем установлены четырехъядерный процессор Intel Core i5 10210U, 8 Гб оперативной памяти и аккумулятор, обеспечивающий до семи часов работы без подзарядки.', 49999, 'Ноутбук Honor MagicBook X14 i5/8/512 Silver (NBR-WAH9)');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('', 109999, 'Ноутбук Apple MacBook Air 13 M1/8/256 Gold');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Ноутбук Acer Swift 3 SF314-43 оснащен шестиядерным процессором AMD Ryzen 5 5500U. Он работает при поддержке 16 Гб оперативной памяти. Для установки приложения и хранения файлов есть SSD-накопитель 512 Гб. Вес мобильного компьютера — 1,2 кг.', 99999, 'Ноутбук Acer Swift 3 SF314-43 (NX.AB1ER.00Y)');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Ноутбук Asus M3500QA-L1066T – это сочетание стильного дизайна и современных технологий, обеспечивающих высокую производительность. Благодаря восьмиядерному процессору AMD Ryzen 7 5800H и 8 Гб оперативной памяти он позволяет продуктивно работать в многозадачном режиме.', 79999, 'Ноутбук ASUS M3500QA-L1066T');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Hisense 55A6BG — телевизор с экраном формата 16:9, разрешение которого составляет 3840 х 2160 пикселей. Он поддерживает решения HDR10 Dolby Vision и благодаря этому реалистично воспроизводит основные цвета и тончайшие оттенки, а детали в наиболее светлых и самых темных участках изображения хорошо различимы. ', 34999, 'Телевизор Hisense 55A6BG');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Philips The One 58PUS8506/60 — телевизор с процессором P5 Perfect Picture, поддержкой HDR 10+ и подсветкой Ambilight. В нем реализованы технологии Dolby Vision и Dolby Atmos.', 69999, 'Телевизор Philips The One 58PUS8506/60');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Стиральная машина Haier HW70-BP12959A – это сочетание компактности и широких возможностей. Глубина этой модели всего 37,3 см, что позволяет установить её даже в условиях ограниченного пространства, при этом она вмещает до 7 кг белья.', 39999, 'Стиральная машина узкая Haier HW70-BP12959A');
//INSERT INTO "Product"("Description", "Price", "Title")
//VALUES('Haier HWD80-BP14959A – сушильно-стиральная машина, с которой вы будете тратить в разы меньше времени на бытовые хлопоты. Плюс модель предельно компактна, поэтому займёт минимум места в вашей ванной или на кухне. С ЗАБОТОЙ Рельефная поверхность барабана Pillow Drum максимально бережно воздействует даже на волокна тонкой ткани.', 54999, 'Стиральная машина с сушкой Haier HWD80-BP14959A');
