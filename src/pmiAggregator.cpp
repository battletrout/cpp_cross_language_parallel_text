#include <unordered_map>
#include <string>
#include <math.h>
#include <fmt/core.h>

#include "pmiAggregator.hpp"

std::tuple<int,int,int,float> PmiAggregator::calc_PMI(const std::string& eng_word, const std::string& grk_word) {
    int A = eng_to_grk_words[eng_word][grk_word];
    int B = eng_words[eng_word] - A;
    int C = grk_words[grk_word] - A;
    //fmt::print("For airplane to αεροπλάνο\nA: {}, B: {}, C: {}", A, B, C); 
    int N = total_doc_count;

    float pmi = std::log2((N * A) /((A+B)*(A+C)));
    //fmt::print("For {} to {}\nA: {}, B: {}, C: {}, PMI: {}\n",eng_word,grk_word, A, B, C, pmi);
    return std::tuple{A,B,C,pmi};
}

std::vector<std::tuple<std::string, int, int, int, float>> PmiAggregator::rank_translations(
    const std::string& eng_word_for_translation, int return_ranks, bool print) {
        std::vector<std::tuple<std::string, int, int, int, float>> rankings;
        //for each greek word that has appeared opposite the english word
       for (const auto& [grk_word, count] : eng_to_grk_words[eng_word_for_translation]) {
            auto [a,b,c,pmi] = calc_PMI(eng_word_for_translation, grk_word);
            //remove a <= 5
            if (a > 5) {
                rankings.push_back({grk_word, a, b, c, pmi});
            }
       }

       // Sort by PMI (last element of tuple) in descending order
       std::sort(rankings.begin(), rankings.end(),
            [](const auto& a, const auto& b) {
                return std::get<4>(a) > std::get<4>(b); // > for descending order on pmi
            });

        // Print top 5 (or less if vector is smaller)
        const size_t n = std::min(size_t(return_ranks), rankings.size());
        for(size_t i = 0; i < n; i++) {
            const auto& [word, a, b, c, pmi] = rankings[i];
            if (print) {
                fmt::print("#{}: {}, A: {}, B: {}, C: {}, PMI: {:.4f})\n", i+1, word, a, b, c, pmi);
            }
        }

       return rankings;
}

    
