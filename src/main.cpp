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

    // Nested map[english][greek] = count
    std::unordered_map<std::string, std::unordered_map<std::string, uint32_t>> eng_to_grk_words;
    // Map to store count of docs with english words
    std::unordered_map<std::string, uint32_t> eng_words;
    // Map to store count of docs with greek words
    std::unordered_map<std::string, uint32_t> grk_words;   
    auto text_pairs = ParallelTextParser::parse_file(greek_filepath);

    size_t line_id{0};
    for (const auto& [greek, english] : text_pairs) {
        if (greek.empty() || english.empty()) continue;
        
        //For each line
        line_id++;
        //generate sets of tokens (unique vals only)
        auto eng_tokens = tok_eng.tokenize(english);
        auto grk_tokens = tok_grk.tokenize(greek);
        
        bool first_grk_iteration = true;
        for (auto& eng_token : eng_tokens) {
            eng_words.emplace(eng_token,0);
            eng_words[eng_token]++;
            for (auto& grk_token : grk_tokens) {
                eng_to_grk_words[eng_token].emplace(grk_token, 0);
                eng_to_grk_words[eng_token][grk_token]++;
                
                //if it's the first iteration of greek words, emplace them (only do it once)
                if (first_grk_iteration) {
                    grk_words.emplace(grk_token,0);
                    grk_words[grk_token]++;
                }
            }
            first_grk_iteration = false;
        }
        if (line_id % 1000 == 0) {
            debug_custom::log(debug_custom::LogLevel::INFO, fmt::format("processed line: {}",line_id));
        }
    }
        // for (const auto& grk_token : grk_tokens) {
        //     std::cout << grk_token << ": " << grk_tokens[grk_token] << std::endl;
        // }
    for (const auto& [grk_word, count] : grk_words) {
        std::cout << grk_word << ": " << count << std::endl;
    }

        // print_coll(grk_tokens);
        // std::cout << std::endl;
        // print_coll(eng_tokens);

        //iterate through the vectors

    return 0;
}