#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <string>
#include <map>
#include "../dlib/server.h"
#include "../dlib/threads.h"
namespace dev
{
    class ConfigurationParser : public dlib::server_http
    {
    private:
        std::map<std::string, std::string> _cfdata;
        std::string _location;
        dlib::mutex _cfmtx;
    public:
        ConfigurationParser(std::string location);
        std::string& operator[](std::string key);
        void remove(std::string key);
        std::string get(std::string key);
        void add(std::string key, std::string value);
        void refresh();
        void flush();
        std::map<std::string, std::string> getMap() { return _cfdata; }
        bool tryBool(std::string key);
        std::string getStorageLocation() { return _location; }
        const std::string on_request(const dlib::incoming_things& incoming, dlib::outgoing_things& outgoing);
    };
    ConfigurationParser& getCFG();
}
#endif // CONFIGURATION_HPP
