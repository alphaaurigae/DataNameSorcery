#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H


#include <string>
#include <iostream>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>


namespace json_output {

inline void outputJsonToConsole(const std::string& ip, const std::string& result) {
    Poco::JSON::Object jsonResult;
    jsonResult.set("ip", ip);
    jsonResult.set("result", result);
    Poco::JSON::Stringifier::stringify(jsonResult, std::cout);
    std::cout << std::endl;
}

}
#endif