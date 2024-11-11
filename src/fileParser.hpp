#pragma once

#include <fstream>
#include <string>
#include <string_view>

struct StringCouplet {
    std::string_view a;
    std::string_view b;
};

class FileParser {
public:
    // Constructor
    FileParser(const std::string &filepath) : 
        stream_(filepath, std::ifstream::in) {
        
    }

    // Rule of 5
    ~FileParser() {
        if (stream_.is_open()) {
            stream_.close();
        }
    }
    
    // Delete copy operations - file streams aren't copyable
    FileParser(const FileParser&) = delete;
    FileParser& operator=(const FileParser&) = delete;
    
    // Move operations
    FileParser(FileParser&& other) noexcept : 
        stream_(std::move(other.stream_)),
        current_line_(std::move(other.current_line_)) {}
    
    FileParser& operator=(FileParser&& other) noexcept {
        if (this != &other) {
            if (stream_.is_open()) {
                stream_.close();
            }
            stream_ = std::move(other.stream_);
            current_line_ = std::move(other.current_line_);
        }
        return *this;
    }

    // Original interface
    std::string_view read_line();
    static StringCouplet separate_line(const char separator);

    // Helper methods
    bool is_open() const { return stream_.is_open(); }
    bool good() const { return stream_.good(); }
    bool eof() const { return stream_.eof(); }

private:
    std::fstream stream_;
    std::string current_line_;
};