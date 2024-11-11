#pragma once

#include <string>
#include <utf8.h>
#include <vector>
#include <tuple>

/**
* @brief Utility function to print any collection to stdout
* @tparam T Collection type that supports range-based for loop
* @param coll Collection to print
* @details Prints each element separated by spaces, followed by newline
*/
template <typename T>
void print_coll(T& coll)
{
    for (const auto& elem : coll) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

/**
* @brief Iterator class for reading valid UTF-8 lines from a file stream
* @details Provides input iterator interface for reading lines while
*          validating UTF-8 encoding. Invalid UTF-8 lines are skipped.
*/
class LineReader {
private:
    std::ifstream& stream;
    std::string current_line;

public:
    /**
    * @brief Constructs reader for given file stream
    * @param s Reference to input file stream
    */
    explicit LineReader(std::ifstream& s) : stream(s) {}
    
    /**
    * @brief Check if more lines can be read
    * @return false if end of file reached
    */
    explicit operator bool() const { return !stream.eof(); }
    
    /**
    * @brief Access current line
    * @return string_view of current line
    */
    std::string_view operator*() const { return current_line; }
    
    /**
    * @brief Advance to next valid UTF-8 line
    * @return Reference to this reader
    * @details Skips lines with invalid UTF-8 encoding
    */    
    LineReader& operator++() {
        if (std::getline(stream, current_line)) {
            if (!utf8::is_valid(current_line.begin(), current_line.end())) {
                current_line.clear();
            }
        }
        return *this;
    }
};

/**
* @brief Parser for parallel text files with tab-separated pairs
* @details Reads files containing pairs of texts separated by tabs,
*          typically used for parallel language corpora
*/
class ParallelTextParser {
public:
    using TextPair = std::pair<std::string, std::string>;
    
    /**
    * @brief Parse file containing tab-separated text pairs
    * @param filename Path to file to parse
    * @return Vector of text pairs
    * @details Uses classic locale for UTF-8 compatibility
    *          Each line should contain two texts separated by a tab
    *          Lines without tabs are skipped
    */
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


