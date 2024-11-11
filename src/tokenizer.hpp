#include <unicode/brkiter.h>
#include <unicode/unistr.h>
#include <unicode/utf8.h>
#include <string>
#include <vector>

class Tokenizer {
public:
    Tokenizer(const std::string& locale_string) {
        UErrorCode status = U_ZERO_ERROR;
        m_break_iterator.reset(
            icu::BreakIterator::createWordInstance(
                icu::Locale(locale_string.c_str()), 
                status
            )
        );
        if (U_FAILURE(status)) {
            throw std::runtime_error("Failed to create break iterator");
        }
    }

    std::vector<std::string> tokenize(std::string_view text) const {
        std::vector<std::string> tokens;
        UErrorCode status = U_ZERO_ERROR;
        
        // Explicitly specify UTF-8 encoding when creating UnicodeString
        icu::UnicodeString utext = icu::UnicodeString::fromUTF8(
            icu::StringPiece(text.data(), text.length())
        );

        icu::UnicodeString lower_case = utext.toLower();
        // Set tolower
        m_break_iterator->setText(lower_case);
        
        int32_t start = m_break_iterator->first();
        int32_t end = m_break_iterator->next();
        
        while (end != icu::BreakIterator::DONE) {
            std::string token;
            // Explicitly convert back to UTF-8
            utext.tempSubStringBetween(start, end).toUTF8String(token);
            
            // Add the token if it's not empty, not punctuation, and not a digit

            if (!token.empty() && !std::all_of(token.begin(), token.end(), 
                [](unsigned char c) { return std::isspace(c) || std::ispunct(c); }) &&
                !std::all_of(token.begin(), token.end(), 
                [](unsigned char c) { return std::isdigit(c); })) 
            { 
                tokens.push_back(token);
            }
            
            start = end;
            end = m_break_iterator->next();
        }
        
        return tokens;
    }

private:
    std::unique_ptr<icu::BreakIterator> m_break_iterator;
};