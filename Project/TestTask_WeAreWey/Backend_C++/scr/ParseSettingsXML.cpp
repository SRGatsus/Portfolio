#include "ParseSettingsXML.h"

#include <iostream>

//Loads an XML file named fileName. The file must lie next to the executable file(exe)
ParseSettingsXML::ParseSettingsXML(string fileName)
{
	if (doc.load_file(fileName.c_str()))
	{
		cout << "The file has been uploaded successfully\n";
		isFile = true;
	}else
	{
		cout << "The settings file is missing\n";
	}
}

//Tries to reach the child element in the XML file. The XML path parameter is an array of strings (["server","settings"])
void ParseSettingsXML::PathToChild(string *pathXML)
{
	if (!isFile) return;
	int i = 0;
	//TODO: понять почему работает именно так 17.09.22
	while (pathXML[i].size() != 1 && !pathXML[i].empty() && pathXML[i].size() <2000000000 )
	{
		if (i == 0)
		{
			child = doc.child(pathXML[i].c_str());
		}
		else
		{
			child = child.child(pathXML[i].c_str());
		}
		i++;
	}
	if(!child)
	{
		cout << "Couldn 't get child\n";
	}
}
//Gets the name Attribute attribute from the node element
xml_attribute* ParseSettingsXML::GetAttribute(xml_node node, string nameAttribute)
{
	if (!node) return nullptr;
	xml_attribute attributeValue = node.attribute(nameAttribute.c_str());
	if (!attributeValue) return nullptr;
	return  &attributeValue;
}
//Returns the value of an attribute named attributeName for a child element named nameChild. 
//If the child element could not be found or the attribute is not set, defaultValue is returned;
int ParseSettingsXML::ParseChildAttribut(string nameChild,string attributeName, int defaultValue)
{
	if(child)
	{
		xml_attribute* valueAttribite = GetAttribute(child.child(nameChild.c_str()), attributeName);
		return valueAttribite ? valueAttribite->as_int(-1) != -1 ? valueAttribite->as_int() : defaultValue : defaultValue;
	}
	return defaultValue;
	
}
//Returns the value of an attribute named attributeName for a child element named nameChild. 
//If the child element could not be found or the attribute is not set, defaultValue is returned;
string ParseSettingsXML::ParseChildAttribut(string nameChild, string attributeName, string defaultValue)
{
	if(child)
	{
		xml_attribute* valueAttribite = GetAttribute(child.child(nameChild.c_str()), attributeName);
		return valueAttribite ? valueAttribite->as_string() != "" ? valueAttribite->as_string() : defaultValue : defaultValue;
	}
	return defaultValue;

}

