# Generating Lexical Translation Dictionary Candidates from Greek-English Parallel Texts
JHU Fall '24 Information Retrieval Project 5

## Overview
A simple experiment processing a bunch of unicode text in c++ into bags-of-words to find likely candidates for translation dictionaries between English and Greek texts. Ranks candidate translations based off Pointwise Mutual Information (https://en.wikipedia.org/wiki/Pointwise_mutual_information).

## Technical Approach
- Docker dev container using cpp:1-debian-11.
- ICU's BreakIterator for proper word boundary detection across different scripts and locales
- Smart pointers for RAII-based resource management
- Template-based logging system with type-safe formatting via {fmt}
- CMake-based build system with proper dependency management
- Position-independent code support for better library integration

## Dependencies
- C++20 compiler
- CMake 3.16 or higher
- Boost (system, locale components)
- ICU (uc, i18n components)
- {fmt} library

## Building

```bash
mkdir build
cd build
cmake ..
make
```


## Implementation Details

### Tokenization
The tokenizer implements:
- UTF-8 text handling
- Case normalization
- Filtering of punctuation and numbers
- Locale-aware word boundary detection

### Logging
The logging system provides:
- Timestamp-based logging
- Multiple severity levels (SUCCESS, INFO, ERROR)
- Type-safe message formatting
- Thread-safe output

## Best Practices Used
- RAII for resource management
- Exception-safe code
- Modern CMake practices
- Comprehensive error handling
- Clear separation of interface and implementation

## Performance Considerations
- Uses string_view for efficient string passing (in places...)
- Smart pointer management for ICU resources
- Templated implementations to avoid runtime overhead
- Position-independent code for better linking flexibility

## License
MIT

## Contributing
JHU Whiting School of Engineering's Computer Science Department
