#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> // for pretty printing
#include <string>

// #include 
//Public includes
#include "logger.hpp"
// #include "fileParser.hpp"
#include "tokenizer.hpp"
#include "textUtils.hpp"
//Private includes
//#include "core/socketServer.cpp"
//#include "core/async.cpp"



// Function definition
void timer_expired(const std::string_view &id) {
    debug_custom::log(debug_custom::LogLevel::ERROR, id);
}


int main() {
    log(debug_custom::LogLevel::ERROR,"Error Test",6,2031.2);
    // debug_custom::log(debug_custom::LogLevel::SUCCESS,"Success Test");
    // debug_custom::log(debug_custom::LogLevel::INFO,"Info Test");
    
    std::string greek_filepath{"greek.tsv"};

    std::ifstream stream{greek_filepath, std::ios::in};

    //FileParser parser(greek_filepath);
    //std::cout << parser.read_line() << "\n";
    //"en_US.UTF-8" or "el_GR.UTF-8"
    std::string greek_locale{"el_GR.UTF-8"};
    std::string eng_locale{"en_US.UTF-8"};
    Tokenizer tok_grk(greek_locale);
    Tokenizer tok_eng(eng_locale);

    char tab_sep ='\t';

auto text_pairs = ParallelTextParser::parse_file(greek_filepath);

for (const auto& [greek, english] : text_pairs) {
    if (greek.empty() || english.empty()) continue;
    
    auto eng_tokens = tok_eng.tokenize(english);
    auto grk_tokens = tok_grk.tokenize(greek);
    
    print_coll(grk_tokens);
    print_coll(eng_tokens);
    //break;  // Remove this break when you're ready to process all lines
}

    // // Use for either language
    // auto tokens1 = tok_eng.tokenize("Hello, World!");

    // // or
    // auto tokens = tok_greek.tokenize("Γεια σας, κόσμε!");
    // std::cout << tokens[2];


    return 0;
}