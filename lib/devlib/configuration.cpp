#include "configuration.hpp"
#include "string.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include "../buildconf.h"

//%%%%%%%%%%%%
//  Constructor. Takes in configuration file location. If a file does not exist, it will
//      be created!
//%%%%%%%%%%%%
dev::ConfigurationParser::ConfigurationParser(std::string location) : _location(location)
{
    refresh();
}

//%%%%%%%%%%%%
//  Reads the configuration file into the map
//%%%%%%%%%%%%
void dev::ConfigurationParser::refresh()
{
    //Erase the contents of _cfdata
    _cfmtx.lock();
    _cfdata.clear();
    _cfmtx.unlock();

    //Reads the configuration file
    std::ifstream fin(_location.c_str(), std::ios::in);

    //If the file does not exist, create a new file!
    if(!fin.is_open()) std::ofstream(_location.c_str(), std::ios::out).close();

    std::string buffer;
    //Extract each line!
    while(std::getline(fin, buffer, '\n'))
    {
        //Check if the index of the hash does exist! If it exists, get it out of the way!
        if(dev::charPos(buffer, '#') != -1) buffer = buffer.substr(0, dev::charPos(buffer, '#') - 1);
        dev::trim(buffer);
        //Check to make sure that it isn't a line with only comments!
        if(buffer.size() != 0)
        {
            std::string key, value;
            if(dev::charPos(buffer, '=') == -1) key = buffer;
            else
            {
                size_t eqloc = dev::charPos(buffer, '=');
                key = buffer.substr(0, eqloc);
                value = buffer.substr(eqloc + 1, buffer.size());
            }
            dev::trim(key);
            dev::trim(value);
            _cfmtx.lock();
            _cfdata[key] = value;
            _cfmtx.unlock();
        }
    }

    fin.close();
}

void dev::ConfigurationParser::flush()
{
    std::ofstream fout(_location.c_str(), std::ios::out);
    if(fout.is_open())
    {
        _cfmtx.lock();
        for(std::map<std::string, std::string>::const_iterator it = _cfdata.begin(); it != _cfdata.end(); ++it)
        {
            fout << it->first << "=" << it->second << std::endl;
        }
        _cfmtx.unlock();
    }

    fout.close();
}

std::string dev::ConfigurationParser::get(std::string key)
{
    _cfmtx.lock();
    std::string x = _cfdata[key];
    _cfmtx.unlock();
    return x;
}
void dev::ConfigurationParser::add(std::string key, std::string value)
{
    _cfmtx.lock();
    _cfdata[key] = value;
    _cfmtx.unlock();
}
void dev::ConfigurationParser::remove(std::string key)
{
    _cfmtx.lock();
    _cfdata.erase(key);
    _cfmtx.unlock();
}
std::string& dev::ConfigurationParser::operator[](std::string key) { return _cfdata[key]; }

bool dev::ConfigurationParser::tryBool(std::string key)
{
    _cfmtx.lock();
    std::string b = dev::itrim(dev::tolower(_cfdata[key]));
    _cfmtx.unlock();
    return b == "yes" || b == "1" || b == "true" || b == "on";
}

const std::string dev::ConfigurationParser::on_request(const dlib::incoming_things& incoming, dlib::outgoing_things& outgoing)
{
    if(incoming.path == "/")
    {
        std::stringstream z;
#define x z <<
        x "<html>";
            x "<head>";
                x "<title>Configuration Utility Frontend</title>";
            x "</head>";

            x "<body>";
                x "<h1>Configuration: <span style=\"color:SpringGreen;\">" << _location << "</span></h1>";
                x "<div style=\"position: fixed; top: 36px; left: 50px; right: 50px; bottom: 16px; border: 2px solid #404040; overflow-y: auto;\">";

                    x "<form style=\"margin: 0px;\" action=\"/setval\" method=\"POST\">";
                        x "<table style=\"width: 100%; background-color: #404040; color: #C6C6C6;\">";
                            x "<tr>";
                                x "<td style=\"width: 20%;\">Key</td>";
                                x "<td>Value</td>";
                                x "<td style=\"width: 27px;\">&rarr;</td>";
                                x "<td style=\"width: 27px;\">X</td>";
                            x "</tr>";
                        x "</table>";
                    x "</form>";

                    _cfmtx.lock();
                    for(std::map<std::string, std::string>::const_iterator it = _cfdata.begin(); it != _cfdata.end(); ++it)
                    {
                        x "<form style=\"margin: 0px;\" action=\"/setval\" method=\"POST\" id=\"" << (*it).first << "\">";
                            x "<table style=\"width: 100%; background-color: #C6C6C6;\">";
                                x "<tr>";
                                    x "<td style=\"width: 20%;\">" << (*it).first;
                                        x "<input style=\"display:none\" readonly name=\"name\" placeholder=\"Key\" type=\"text\">";
                                    x "</td>";
                                    x "<td><input style=\"width: 100%; height: 19px; padding: 0px;\" type=\"text\" value=\"" << (*it).second << "\" name=\"value\" placeholder=\"Value\"></td>";
                                    x "<td style=\"width: 27px;\"><input type=\"submit\" value=\"&rarr;\"></td>";
                                    x "<td style=\"width: 27px;\"><input type=\"submit\" value=\"X\" onmouseover=\"javascript:document.getElementById('" << (*it).first << "').action = '/delval';\" onmouseout=\"javascript:document.getElementById('" << (*it).first << "').action = '/setval';\"></td>";
                            x "</table>";
                        x "</form>";
                    }
                    _cfmtx.unlock();

                    x "<form style=\"margin: 0px;\" action=\"/setval\" method=\"POST\">";
                        x "<table style=\"width: 100%; background-color: #C6C6C6\">";
                            x "<tr>";
                                x "<td style=\"width: 20%;\"><input type=\"text\" style=\"width: 100%; height: 19px; padding: 0px;\" type=\"text\" name=\"key\" placeholder=\"Key\"></td>";
                                x "<td><input type=\"text\" style=\"width: 100%; height: 19px; padding: 0px;\" type=\"text\" name=\"value\" placeholder=\"Value\"></td>";
                                x "<td style=\"width: 27px;\"><input type=\"submit\" value=\"&rarr;\"></td>";
                                x "<td style=\"width: 27px;\">--</td>";
                            x "</tr>";
                        x "</table>";
                    x "</form>";

                x "</div>";
                x "<p style=\"position: fixed; bottom: 0px; left: 50px;>&copy; Devyash Lodha</p>";
            x "</body>";

        x "</html>";
#undef x
        return z.str();
    }
    else if(incoming.path == "/setval")
    {
        _cfmtx.lock();
        _cfdata[incoming.queries["key"]] = incoming.queries["value"];
        _cfmtx.unlock();
        outgoing.http_return = 301;
        outgoing.headers["Location"] = "/";
        return "";
    }
    else if(incoming.path == "/delval")
    {
//        _cfmtx.lock();
//        _cfdata.(incoming.queries["key"]);
//        _cfmtx.unlock();
        outgoing.http_return = 301;
        outgoing.headers["Location"] = "/";
        return "";
    }
    return "";
}

#ifndef CONFIGURATION_LOCATION
#define CONFIGURATION_LOCATION "app.cfg"
#endif

dev::ConfigurationParser parser(CONFIGURATION_LOCATION);
dev::ConfigurationParser& dev::getCFG() { return parser; }
