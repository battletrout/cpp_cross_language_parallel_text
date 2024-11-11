#pragma once

#include <iostream>
#include <string>
#include <utf8.h>
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>




// template<typename... Args>
// void print_coll(Args&&... args) {
//     std::cout << args << print_iterator(args)
// }


struct StringCouplet {
    std::string_view grk_string;
    std::string_view eng_string;
};

template <typename T>
void print_coll(T& coll)
{
    for (const auto& elem : coll) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

StringCouplet separate_line(std::string_view current_line, const char separator) {
    // Find first tab
    size_t tab_pos = current_line.find(separator);
    
    if (tab_pos == std::string::npos) {
        // Handle no separator - could throw or return empty views
        return StringCouplet{std::string_view(), std::string_view()};
    }

    return StringCouplet{
        std::string_view(current_line.data(), tab_pos),
        std::string_view(current_line.data() + tab_pos + 1, 
                        current_line.length() - tab_pos - 1)
    };
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

std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Debug: print first few bytes
    std::cout << "First bytes: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (int)(unsigned char)content[i] << " ";
    }
    std::cout << std::dec << "\n";
    
    return content;
}

// Line separation
std::vector<std::string> split_lines(const std::string& content) {
    std::vector<std::string> lines;
    boost::split(lines, content, boost::is_any_of("\n"));
    return lines;
}

// TSV splitting using boost
StringCouplet separate_line(const std::string& line) {
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of("\t"));
    return {parts[0], parts.size() > 1 ? parts[1] : ""};
}

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


