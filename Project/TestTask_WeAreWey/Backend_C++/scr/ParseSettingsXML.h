#pragma once
#include <pugixml.hpp>
#include <string>

using namespace std;
using namespace pugi;

class ParseSettingsXML
{
private:
	xml_document doc;
	xml_node child;
	bool isFile = false;

	xml_attribute* GetAttribute(xml_node node, string nameAttribute);
public:
	ParseSettingsXML(string fileName);
	void PathToChild(string pathXML[]);
	int ParseChildAttribut(string nameChild, string attributeName,int defaultValue);
	string ParseChildAttribut(string nameChild, string attributeName,string defaultValue);
};

