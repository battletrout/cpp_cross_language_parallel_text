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

    std::tuple<int,int,int,float> PmiVals(const std::string& eng_word, const std::string& grk_word);
    
    //D = this - (A + B + C) 
    uint32_t total_doc_count;

};