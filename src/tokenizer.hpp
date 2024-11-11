#include <unicode/brkiter.h>
#include <unicode/unistr.h>
#include <unicode/utf8.h>
#include <string>
#include <vector>
#include <set>

/**
 * @class Tokenizer
 * @brief A class for breaking text into tokens using ICU's word boundary analysis
 *
 * This class provides functionality to break text into individual tokens using
 * ICU's BreakIterator with locale-aware word boundary detection. It handles
 * UTF-8 encoded text and performs case normalization.
 */
class Tokenizer {
public:

    /**
     * @brief Constructs a Tokenizer with the specified locale
     *
     * @param locale_string The locale identifier (e.g., "en_US") to use for word breaking rules
     * @throw std::runtime_error if the ICU BreakIterator creation fails
     *
     * Creates a word-breaking iterator for the specified locale using ICU's
     * BreakIterator. The iterator is configured to detect word boundaries
     * according to the rules of the specified locale.
     */
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

    /**
     * @brief Tokenizes the input text into a set of unique tokens
     *
     * @param text The UTF-8 encoded input text to tokenize
     * @return std::set<std::string> A set of unique tokens extracted from the text
     *
     * This method performs the following operations:
     * - Converts the input UTF-8 text to ICU's UnicodeString
     * - Converts the text to lowercase for normalization
     * - Breaks the text into tokens using word boundaries
     * - Filters out tokens that are:
     *   - Empty strings
     *   - Pure whitespace or punctuation
     *   - Pure numeric digits
     *
     * @note The returned tokens are normalized to lowercase and UTF-8 encoded
     */
    std::set<std::string> tokenize(std::string_view text) const {
        std::set<std::string> tokens;
        UErrorCode status = U_ZERO_ERROR;
        
        // Explicitly specify UTF-8 encoding when creating UnicodeString
        icu::UnicodeString unitext = icu::UnicodeString::fromUTF8(
            icu::StringPiece(text.data(), text.length())
        );

        icu::UnicodeString lower_case = unitext.toLower();
        // Set tolower
        m_break_iterator->setText(lower_case);
        
        int32_t start = m_break_iterator->first();
        int32_t end = m_break_iterator->next();
        
        while (end != icu::BreakIterator::DONE) {
            std::string token;
            // Explicitly convert back to UTF-8
            unitext.tempSubStringBetween(start, end).toUTF8String(token);
            
            // Add the token if it's not empty, not punctuation, and not a digit

            if (!token.empty() && !std::all_of(token.begin(), token.end(), 
                [](unsigned char c) { return std::isspace(c) || std::ispunct(c); }) &&
                !std::all_of(token.begin(), token.end(), 
                [](unsigned char c) { return std::isdigit(c); })) 
            { 
                tokens.emplace(token);
            }
            
            start = end;
            end = m_break_iterator->next();
        }
        
        return tokens;
    }

private:
    /** @brief Smart pointer to the ICU BreakIterator instance */
    std::unique_ptr<icu::BreakIterator> m_break_iterator;
};