#pragma once

#include <string>
#include <utf8.h>
#include <vector>
#include <tuple>

template <typename T>
void print_coll(T& coll)
{
    for (const auto& elem : coll) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

class LineReader {
private:
    std::ifstream& stream;
    std::string current_line;

public:
    explicit LineReader(std::ifstream& s) : stream(s) {}
    
    explicit operator bool() const { return !stream.eof(); }
    
    std::string_view operator*() const { return current_line; }
    
    LineReader& operator++() {
        if (std::getline(stream, current_line)) {
            if (!utf8::is_valid(current_line.begin(), current_line.end())) {
                current_line.clear();
            }
        }
        return *this;
    }
};

class ParallelTextParser {
public:
    using TextPair = std::pair<std::string, std::string>;
    
    static std::vector<TextPair> parse_file(const std::string& filename) {
        std::vector<TextPair> pairs;
        std::ifstream file(filename);
        
        // Ensure UTF-8 handling
        file.imbue(std::locale::classic());
        
        std::string line;
        while (std::getline(file, line)) {
            auto tab_pos = line.find('\t');
            if (tab_pos != std::string::npos) {
                pairs.emplace_back(
                    line.substr(0, tab_pos),
                    line.substr(tab_pos + 1)
                );
            }
        }
        return pairs;
    }
};


