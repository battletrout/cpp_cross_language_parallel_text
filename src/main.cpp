#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "logger.hpp"
#include "tokenizer.hpp"
#include "textUtils.hpp"
#include "pmiAggregator.hpp"

int main(int argc,      // Number of strings in array argv
          char *argv[],   // Array of command-line argument strings
          char *envp[] )  // Array of environment variable strings) 
          {
    log(debug_custom::LogLevel::INFO,"Start...");
    
    int count;
    // Display each command-line argument.
    std::cout << "\nCommand-line arguments:\n";
    for( count = 0; count < argc; count++ )
         std::cout << "  argv[" << count << "]   "
                << argv[count] << "\n";

    // take greek filepath as arg if there, otherwise default to greek.tsv
    std::string greek_filepath{};
    if (argc > 1) {
        greek_filepath = argv[1];
    } else {
        greek_filepath = "greek.tsv";
    }
    log(debug_custom::LogLevel::INFO,greek_filepath);

    //Initialize tokenizers with english and greek localities
    std::string greek_locale{"el_GR.UTF-8"};
    std::string eng_locale{"en_US.UTF-8"};
    Tokenizer tok_grk(greek_locale);
    Tokenizer tok_eng(eng_locale);

    //Initialize pmi aggregator
    PmiAggregator aggregator;

    auto text_pairs = ParallelTextParser::parse_file(greek_filepath);

    size_t line_id{0};
    for (const auto& [greek, english] : text_pairs) {
        //for each line: skip if either grk or eng is empty. Otherwise...
        if (greek.empty() || english.empty()) continue;
        
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
        
    //DEBUG -- verify that values are being stored correctly -- fixed 11/10/24
    // std::cout << aggregator.eng_to_grk_words["airplane"]["αεροπλάνο"] <<std::endl;
    // std::cout << aggregator.grk_words["αεροπλάνο"] <<std::endl;
    // std::cout << aggregator.eng_words["airplane"] <<std::endl;
    //PART C: Calculate for Airplane
    auto [A, B, C, pmi] = aggregator.calc_PMI("airplane","αεροπλάνο");
    fmt::print("For airplane to αεροπλάνο\nA: {}, B: {}, C: {}, PMI: {}\n\n", A, B, C, pmi);

    //PART D: Calculate candidates for english words 
    for (const auto& eng_word : {"telephone","swimming","helicopter","washington","bulgarian"}) {
        std::cout << eng_word << " candidates: \n";
        //rank translations, but print from inside the function
        auto _ = aggregator.rank_translations(eng_word, 5, true);
        std::cout << "\n\n";
    }

    //PART E: Calculate top-1 for English words that occur in 90 - 100 sentences
    //Loop over all english words, and calculate PMI for the ones with {90 =< count < 100} 
    //Output format for csv
    fmt::print("English,Greek,A,B,C,PMI\n");
    for (const auto& [eng_word, count] : aggregator.eng_words) {
        if (count >= 90 && count < 100) {
            //rank top 1 translation, don't print from inside function
            auto top_rank_vec = aggregator.rank_translations(eng_word, 1, false);
            auto [grk_word,A,B,C,pmi] = top_rank_vec[0];
            fmt::print("{},{},{},{},{},{:.4f}\n", eng_word, grk_word, A,B,C,pmi);
        }
    }
    log(debug_custom::LogLevel::INFO,"Done!");
    return 0;
}