#include <unordered_map>
#include <string>
#include <math.h>

#include "pmiAggregator.hpp"

std::tuple<int,int,int,float> PmiAggregator::PmiVals(const std::string& eng_word, const std::string& grk_word) {
    int A = eng_to_grk_words[eng_word][grk_word];
    int B = eng_words[eng_word] - A;
    int C = grk_words[grk_word] - A;
    //fmt::print("For airplane to αεροπλάνο\nA: {}, B: {}, C: {}", A, B, C); 
    int N = total_doc_count;

    float pmi = std::log2((N * A) /((A+B)*(A+C)));
    return std::tuple{A,B,C,pmi};

}
    
