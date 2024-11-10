#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> // for pretty printing
#include <string>
//Public includes
#include "logger.hpp"

//Private includes
//#include "core/socketServer.cpp"
//#include "core/async.cpp"



// Function definition
void timer_expired(std::string &id) {
    debug_custom::log(debug_custom::LogLevel::ERROR, id);
}

int main() {
    // log(LogLevel::ERROR,"Error Test",6,2031.2);
    // debug_custom::log(debug_custom::LogLevel::SUCCESS,"Success Test");
    // debug_custom::log(debug_custom::LogLevel::INFO,"Info Test");
    
    std::string str_id = "109928345";
    timer_expired(str_id);
    
    return 0;
}