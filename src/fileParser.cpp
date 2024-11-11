#include <fstream>

#include "utf8.h"

#include "fileParser.hpp"


std::string_view FileParser::read_line() {
    // Store line in a member string to maintain lifetime
    if (std::getline(stream_, current_line_)) {
        // Validate UTF-8
        if (!utf8::is_valid(current_line_.begin(), current_line_.end())) {
            // Handle invalid UTF-8 - could throw or return empty view
            return std::string_view();
        }
        return std::string_view(current_line_);
    }
    return std::string_view();
}

// StringCouplet FileParser::separate_line(const char separator) {
//     // Find first tab
//     size_t tab_pos = current_line_.find(separator);
    
//     if (tab_pos == std::string::npos) {
//         // Handle no separator - could throw or return empty views
//         return StringCouplet{std::string_view(), std::string_view()};
//     }

//     return StringCouplet{
//         std::string_view(current_line_.data(), tab_pos),
//         std::string_view(current_line_.data() + tab_pos + 1, 
//                         current_line_.length() - tab_pos - 1)
//     };
// }