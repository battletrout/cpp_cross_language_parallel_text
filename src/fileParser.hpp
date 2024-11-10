#pragma once

#include <fstream>
#include <string>

struct StringCouplet {
    std::string_view a;
    std::string_view b;
};

class FileParser {

public:
    //Take a filepath as a string for the initializer
    FileParser(const std::string &filepath, bool log_enabled) {};

    std::string_view readLine();
    StringCouplet separateLine(const char separator);
     

private:
    std::fstream _stream;

};