#pragma once

#include<unordered_map>
#include<string>

struct PmiAggregator {
    // Nested map[english][greek] = count (A in table)
    std::unordered_map<std::string, std::unordered_map<std::string, uint32_t>> eng_to_grk_words;
    // Map to store count of docs with english words (B = this - A in table)
    std::unordered_map<std::string, uint32_t> eng_words;
    // Map to store count of docs with greek words (C = this - A in table)
    std::unordered_map<std::string, uint32_t> grk_words;   

    /**
    * @brief Calculates Point-wise Mutual Information (PMI) between an English and Greek word pair
    * @details PMI is calculated using co-occurrence statistics where:
    *          A = Number of times eng_word and grk_word appear together
    *          B = Times eng_word appears without grk_word
    *          C = Times grk_word appears without eng_word
    *          N = Total document count
    *          PMI = log2((N * A) / ((A+B)*(A+C)))
    * 
    * @param eng_word English word to analyze
    * @param grk_word Greek word to analyze
    * @return std::tuple containing:
    *         - int A: Co-occurrence count
    *         - int B: English word occurrences without Greek word
    *         - int C: Greek word occurrences without English word
    *         - float pmi: Point-wise Mutual Information score
    */
    std::tuple<int,int,int,float> calc_PMI(const std::string& eng_word, const std::string& grk_word);
    
    /**
    * @brief Ranks possible Greek translations for an English word using PMI scores
    * @details Filters out rare word pairs (A ≤ 5), sorts by PMI score in descending order,
    *          and optionally prints the top results
    * 
    * @param eng_word_for_translation English word to find translations for
    * @param return_ranks Number of top rankings to return/print (default: 5)
    * @param print Whether to print the results (default: false)
    * 
    * @return Vector of tuples containing:
    *         - std::string: Greek word
    *         - int A: Co-occurrence count
    *         - int B: English word occurrences without Greek word
    *         - int C: Greek word occurrences without English word
    *         - float pmi: PMI score
    *         Sorted by PMI score in descending order, filtered for A > 5
    */
    std::vector<std::tuple<std::string, int, int, int, float>> rank_translations(
        const std::string& eng_word_for_translation, int return_ranks, bool print);   
    
    //D = this - (A + B + C) 
    uint32_t total_doc_count{0};

};