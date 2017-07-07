/*
 *  PugiXmlTest.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: igor
 */

#include <iostream>
#include <sstream>
#include <pugixml.hpp>
#include <algorithm>
#include <functional>

using namespace std;

void readFileConfig(pugi::xml_node& Section);

int main(int argc, char* argv[])
{
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file("test.xml");
    if(result.status != pugi::status_ok)
    {
        cout << "Error" << endl;
        return 1;
    }

    pugi::xml_node Config = doc.child("Configuration");
    if(Config.empty())
    {
        cout << "empty" << endl;
    }
    else
    {
        cout << "not empty" << endl;
    }
    pugi::xml_node Element = Config.child("Test");
    if(!Element.empty())
    {
        pugi::xml_attribute Uni = Element.attribute("unsigned_int");
        cout << Uni.as_uint() << endl;
    }
    Element = Config.child("File");
    if(!Element.empty())
    {
        readFileConfig(Element);
    }
    Element = Config.child("DoCopy");
    if(!Element.empty())
    {
        cout << Element.attribute("bool").as_bool() << endl;
    }

    Element = Config.child("Buffer");
    if(!Element.empty())
    {
        unsigned int Size = 0;
        string Value = Element.attribute("size").as_string();
        stringstream Convert;
        Convert << Value << endl;
        Convert >> Size;
        if(!Convert.eof())
        {
            Convert >> Value;
            transform(Value.begin(), Value.end(), Value.begin(), [](char c) { return toupper(c); });
            if("K" == Value)
                Size *= 1024;
            else if("M" == Value)
                Size *= 1024 * 1024;
            else if("G" == Value)
                Size *= 1024 * 1024 * 1024;
        }
        cout << Size << endl;
    }
    return 0;
}

void readFileConfig(pugi::xml_node& Section)
{
    pugi::xml_node Element = Section.child("FileName");
    if(!Element.empty())
    {
        cout << "FileName: " << Element.attribute("text").as_string() << endl;
    }
    Element = Section.child("FilePath");
    if(!Element.empty())
    {
        cout << "FilePath: " << Element.attribute("text").as_string() << endl;
    }
    Element = Section.child("Number");
    if(!Element.empty())
    {
        cout << "Number: " << Element.first_child().value() << endl;
    }
}
