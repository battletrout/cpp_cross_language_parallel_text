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
#include "pmiAggregator.hpp"
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

    PmiAggregator aggregator;
    // Nested map[english][greek] = count
    // std::unordered_map<std::string, std::unordered_map<std::string, uint32_t>> eng_to_grk_words;
    // // Map to store count of docs with english words
    // std::unordered_map<std::string, uint32_t> eng_words;
    // // Map to store count of docs with greek words
    // std::unordered_map<std::string, uint32_t> grk_words;   
    auto text_pairs = ParallelTextParser::parse_file(greek_filepath);

    size_t line_id{0};
    for (const auto& [greek, english] : text_pairs) {
        if (greek.empty() || english.empty()) continue;
        
        //For each line
        aggregator.total_doc_count++;
        //generate sets of tokens (unique vals only)
        auto eng_tokens = tok_eng.tokenize(english);
        auto grk_tokens = tok_grk.tokenize(greek);
        
        bool first_grk_iteration = true;
        for (auto& eng_token : eng_tokens) {
            aggregator.eng_words.emplace(eng_token,0);
            aggregator.eng_words[eng_token]++;
            for (auto& grk_token : grk_tokens) {
                aggregator.eng_to_grk_words[eng_token].emplace(grk_token, 0);
                aggregator.eng_to_grk_words[eng_token][grk_token]++;
                
                //if it's the first iteration of greek words, emplace them (only do it once)
                if (first_grk_iteration) {
                    aggregator.grk_words.emplace(grk_token,0);
                    aggregator.grk_words[grk_token]++;
                }
            }
            first_grk_iteration = false;
        }
        if (aggregator.total_doc_count % 1000 == 0) {
            debug_custom::log(debug_custom::LogLevel::INFO, fmt::format("processed line: {}",aggregator.total_doc_count));
        }
    }
        
    // for (const auto& [grk_word, count] : eng_to_grk_words["the"]) {
    //     std::cout << grk_word << ": " << count << std::endl;
    // }
    std::cout << aggregator.eng_to_grk_words["airplane"]["αεροπλάνο"] <<std::endl;
    std::cout << aggregator.grk_words["αεροπλάνο"] <<std::endl;
    std::cout << aggregator.eng_words["airplane"] <<std::endl;
    //PART C: Calculate for Airplane
    auto [A, B, C, pmi] = aggregator.calc_PMI("airplane","αεροπλάνο");
    fmt::print("For airplane to αεροπλάνο\nA: {}, B: {}, C: {}, PMI: {}\n\n", A, B, C, pmi);

    //PART D: Calculate candidates for english words 
    for (const auto& eng_word : {"telephone","swimming","helicopter","washington","bulgarian"}) {
        std::cout << eng_word << " candidates: \n";
        auto _ = aggregator.rank_translations(eng_word, 5, true);
        std::cout << "\n\n";
    }

    //PART E: Calculate top-1 for English words that occur in 90 - 100 sentences
    
    return 0;
}