#ifndef JSON_TOKENIZER
#define JSON_TOKENIZER

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <iostream>

class JsonTokenizerError : public std::runtime_error {
public:
    explicit JsonTokenizerError(const std::string& message)
        : std::runtime_error(message) {}
};

enum class TokenType {
    Number,
    String,
    Boolean,
    Null,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Comma,
    Colon,
    EndOfInput
};

struct Token {
    TokenType type;
    std::optional<std::string> value;

    explicit Token(TokenType type) : type(type), value(std::nullopt) {}

    Token(TokenType type, std::string value)
        : type(type), value(std::move(value)) {}
};

class JsonTokenizer {
public:
    explicit JsonTokenizer(std::string_view input)
        : input_(input), position_(0) {}

    Token nextToken();

private:
    std::string_view input_; 
    size_t position_;

    void skipWhitespace();

    Token parseString();

    Token parseNumber();

    Token parseBooleanOrNull();

    bool isEndOfInput() const { return position_ >= input_.size(); }

    std::optional<char> currentChar() const {
        return isEndOfInput() ? std::nullopt
                              : std::make_optional(input_[position_]);
    }

    std::optional<char> advance() {
        if (isEndOfInput()) return std::nullopt;
        return input_[position_++];
    }
};

#endif