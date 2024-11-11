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
    // log(debug_custom::LogLevel::ERROR,"Error Test");
    
    std::string greek_filepath{"greek.tsv"};
    std::ifstream stream{greek_filepath, std::ios::in};

    std::string greek_locale{"el_GR.UTF-8"};
    std::string eng_locale{"en_US.UTF-8"};
    Tokenizer tok_grk(greek_locale);
    Tokenizer tok_eng(eng_locale);

    // 
    std::unordered_map<std::string, std::unordered_map<std::string, WordPairCounts>> greek_to_english;
    std::unordered_map<std::string, std::unordered_map<std::string, WordPairCounts>> english_to_greek;
    auto text_pairs = ParallelTextParser::parse_file(greek_filepath);

    size_t line_id{0};
    for (const auto& [greek, english] : text_pairs) {
        if (greek.empty() || english.empty()) continue;
        
        //For each line
        line_id++;
        auto eng_tokens = tok_eng.tokenize(english);
        auto grk_tokens = tok_grk.tokenize(greek);
        
        print_coll(grk_tokens);
        print_coll(eng_tokens);

        //iterate through the vectors
        

        //break;  
        }

    return 0;
}