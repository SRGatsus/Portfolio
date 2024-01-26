#pragma  warning(disable: 4996)
#include <cstdlib>
#include <locale>
#include <iostream>
#include <memory>
#include <restbed>
#include <vector>
#include <pqxx/pqxx>
#include "WorkingTheDataBase.h"
#include "ParseSettingsXML.h"
#include <jsoncons/json.hpp>
#include "regex"

using jsoncons::json;
using namespace pqxx;
using namespace std;
using namespace restbed;



int PORT = 8082;
string UserNameBD = "postgres";
string PasswordBD = "admin";
string PortBD = "5432";
string HostBD = "localhost";
string NameBD = "TestTask";

WorkingTheDataBase* workDataBase = nullptr;
ParseSettingsXML* parseSettingsXML = nullptr;

void ErrorHandler(const int status_code, const exception error, const shared_ptr< Session > session)
{
    if (session not_eq nullptr and session->is_open())
    {
        string message = error.what();
        message.push_back('\n');

        //we must leave the socket intact on error,
        //otherwise follow up messages will be lost.
        session->yield(status_code, message, { { "Content-Length", ::to_string(message.length()) }, { "Connection", "keep-alive" } });
    }
}

map<string, string> ParseString(const string& query)
{
    map<string, string> data;
    regex pattern("([\\w+%]+)=([^&]*)");
    auto words_begin = sregex_iterator(query.begin(), query.end(), pattern);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; i++)
    {
        string key = (*i)[1].str();
        string value = (*i)[2].str();
        data[key] = value;
    }

    return data;
}

std::vector<std::string> SplitString(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string UrlDecodeToString(string& SRC) {
    string ret;
    char ch;
    int i, ii;
    for (i = 0; i < SRC.length(); i++) {
        if (SRC[i] == '%') {
            sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
        else {
            ret += SRC[i];
        }
    }
    return (ret);
}

void OptionsAnswer(const shared_ptr< Session > session) {
    const auto request = session->get_request();
    int contentLength = request->get_header("Content-Length", 0);

    if (request->get_method() == "OPTIONS") {
        session->close(OK, "", { { "Content-Length", "0" } });
    }
}

void GetOrders(const shared_ptr< Session > session)
{
    const auto request = session->get_request();
    int contentLength = request->get_header("Content-Length", 0);

    session->fetch(contentLength, [](const shared_ptr< Session > session, const Bytes& body)
        {
            string allOrder = workDataBase->GetAllOrder().to_string();
            const multimap< string, string > headers
            {

                { "Content-Length", std::to_string(allOrder.size()) },
                { "Content-Type", "application/json" }
            };
            session->close(OK, allOrder, headers);
        });
}

void GetOrder(const shared_ptr< Session > session)
{
    const auto request = session->get_request();
    int contentLength = request->get_header("Content-Length", 0);

    session->fetch(contentLength, [request](const shared_ptr< Session > session, const Bytes& body)
        {
            string stringBody = (new std::string((char*)body.data(), body.size()))->c_str();
            json jsonBody = json::parse(regex_replace(UrlDecodeToString(stringBody), std::regex("="), ":"));
            string order = workDataBase->GetOrder(jsonBody).to_string();
            const multimap< string, string > headers
            {

                { "Content-Length", std::to_string(order.size()) },
                { "Content-Type", "application/json" }
            };
            session->close(OK, order, headers);
        });
}

void GetProducts(const shared_ptr< Session > session)
{
    const auto request = session->get_request();
    int contentLength = request->get_header("Content-Length", 0);

    session->fetch(contentLength, [](const shared_ptr< Session > session, const Bytes& body)
        {
            string allProduct = workDataBase->GetAllProduct().to_string();
            const multimap< string, string > headers
            {

                { "Content-Length", std::to_string(allProduct.size()) },
                { "Content-Type", "application/json" }
            };
            session->close(OK, allProduct, headers);
        });
}

void CreateOrder(const shared_ptr< Session > session)
{

    const auto request = session->get_request();
    int contentLength = request->get_header("Content-Length", 0);

    session->fetch(contentLength, [request](const shared_ptr< Session > session, const Bytes& body)
        {
            string stringBody = (new std::string((char*)body.data(), body.size()))->c_str();
            json jsonBody = json::parse(regex_replace(UrlDecodeToString(stringBody), std::regex("="), ":"));
			workDataBase->CreateOrder(jsonBody);
            string allOrder = workDataBase->GetAllOrder().to_string();
            const multimap< string, string > headers
            {

                { "Content-Length", std::to_string(allOrder.size()) },
                { "Content-Type", "application/json" }
            };
            session->close(OK, allOrder, headers);
        });
}

void UpdateOrder(const shared_ptr< Session > session)
{
    const auto request = session->get_request();
    int contentLength = request->get_header("Content-Length", 0);

    session->fetch(contentLength, [](const shared_ptr< Session > session, const Bytes& body)
        {
            string stringBody = (new std::string((char*)body.data(), body.size()))->c_str();
            json jsonBody = json::parse(regex_replace(UrlDecodeToString(stringBody), std::regex("="), ":"));
    		workDataBase->UpdateOrder(jsonBody);
            string result = "{ result:true }";
            const multimap< string, string > headers
            {

                { "Content-Length", std::to_string(result.size()) },
                { "Content-Type", "application/json" }
            };
            session->close(ACCEPTED, result, headers);
        });
}

void CreateProduct(const shared_ptr< Session > session)
{
    const auto request = session->get_request();

    int content_length = request->get_header("Content-Length", 0);

    session->fetch(content_length, [](const shared_ptr< Session > session, const Bytes& body)
        {
            string stringBody = (new std::string((char*)stringBody.data(), stringBody.size()))->c_str();
            json jsonBody = json::parse(regex_replace(UrlDecodeToString(stringBody), std::regex("="), ":"));
            workDataBase->CreateProduct(jsonBody);
            session->close(OK, "{result:true}", { { "Content-Length", "13" },{"Content-Type","application/json"} });
        });
}

void LoadSettings()
{
    string *pathToChild = new string[]{"Server","Settings"};
    parseSettingsXML = new ParseSettingsXML("settings.xml");
    parseSettingsXML->PathToChild(pathToChild);

	PORT = parseSettingsXML->ParseChildAttribut("Port","Value",PORT);
	UserNameBD = parseSettingsXML->ParseChildAttribut("UserNameBD", "Value", UserNameBD);
	PasswordBD = parseSettingsXML->ParseChildAttribut("PasswordBD", "Value", PasswordBD);
	PortBD = parseSettingsXML->ParseChildAttribut("PortBD", "Value", PortBD);
	HostBD = parseSettingsXML->ParseChildAttribut("HostBD", "Value", HostBD);
	NameBD = parseSettingsXML->ParseChildAttribut("NameBD", "Value", NameBD);

    cout << "\nPORT = " + to_string(PORT);
    cout << "\nUserNameBD = " + UserNameBD;
    cout << "\nPasswordBD = " + PasswordBD;
    cout << "\nPortBD = " + PortBD;
    cout << "\nHostBD = " + HostBD;
    cout << "\nNameBD = " + NameBD+"\n";
}


int main(const int, const char**)
{
    //Load Setting from the file settings.xml
    LoadSettings();

    workDataBase = new WorkingTheDataBase(HostBD,PortBD,NameBD, UserNameBD,PasswordBD);
    workDataBase->СheckAvailabilityTable();
    workDataBase->CreateTable();
    //workDataBase->insertDataToProduct();

    auto orders = make_shared< Resource >();
    orders->set_path("/orders");
    orders->set_method_handler("GET", GetOrders);

    auto orderById = make_shared< Resource >();
    orderById->set_path("/order");
    orderById->set_method_handler("POST", GetOrder); 
    orderById->set_method_handler("OPTIONS", OptionsAnswer);

	auto orderUpdate = make_shared< Resource >();
    orderUpdate->set_path("/orderer");
    orderUpdate->set_method_handler("POST", UpdateOrder);
    orderUpdate->set_method_handler("OPTIONS", OptionsAnswer);

    auto products = make_shared< Resource >();
    products->set_path("/products");
    products->set_method_handler("GET", GetProducts);

    auto newOrder = make_shared< Resource >();
    newOrder->set_path("/newOrder");
    newOrder->set_method_handler("POST", CreateOrder);
    newOrder->set_method_handler("OPTIONS", OptionsAnswer);

    auto newProduct = make_shared< Resource >();
    newProduct->set_path("/newProduct");
    newProduct->set_method_handler("POST", CreateProduct);
    newOrder->set_method_handler("OPTIONS", OptionsAnswer);

   
    auto settings = make_shared< Settings >();
    settings->set_default_headers({
                {"Access-Control-Allow-Origin", "*" },
                {"Access-Control-Allow-Methods", "GET, POST, PATCH, PUT, DELETE, OPTIONS" },
                {"Access-Control-Allow-Headers", "Origin, Content-Type, X-Auth-Token" }
        });
    settings->set_port(PORT);
    settings->set_default_header("Connection", "close");

    Service service;
    service.publish(orders);
    service.publish(products);
    service.publish(newOrder);
    service.publish(newProduct);
    service.publish(orderUpdate);
    service.publish(orderById);
    service.set_error_handler(ErrorHandler);
    service.start(settings);
    service.stop();

    workDataBase->Disconnect();

    return EXIT_SUCCESS;
}