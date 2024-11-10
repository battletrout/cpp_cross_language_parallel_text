#include <ranges>
#include <string>
#include <string_view>
#include <boost/locale.hpp>

class Tokenizer {
public:
    // initialize tokenizer with "en_US.UTF-8" or "el_GR.UTF-8"
    Tokenizer(const std::string& locale_string) {
        static bool initialized = false;
        if (!initialized) {
            // Do this setup only once, not per tokenizer
            std::ios_base::sync_with_stdio(false);
            std::cout.imbue(std::locale(""));
            std::wcout.imbue(std::locale(""));
            initialized = true;
        };
        boost::locale::generator gen;
        m_locale = gen(locale_string); 
    }
    // tokenize method using instance's locale
    std::vector<std::string> tokenize(std::string_view text) const {
        auto lowered = boost::locale::to_lower(std::string{text}, m_locale);
        std::vector<std::string> tokens;
        
        std::string current_token;
        for (char c : lowered) {
            if (std::isspace(c)) {
                if (!current_token.empty()) {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
            } else if (!std::ispunct(c)) {
                current_token += c;
            }
        }
        if (!current_token.empty()) {
            tokens.push_back(current_token);
        }
        
        return tokens;
    }

private:
    std::locale m_locale;  // Store locale as member variable

    static std::string clean_token(std::string_view sv) {
        std::string cleaned;
        cleaned.reserve(sv.size());
        for (char c : sv) {
            if (!std::ispunct(static_cast<unsigned char>(c))) {
                cleaned += c;
            }
        }
        return cleaned;
    }
};