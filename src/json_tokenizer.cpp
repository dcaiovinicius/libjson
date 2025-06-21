#include "json_tokenizer.h"
#include <cctype>
#include <cstddef>

Token JsonTokenizer::nextToken() {
    skipWhitespace();

    if (isEndOfInput()) {
        return Token(TokenType::EndOfInput);
    }

    auto current = currentChar();
    if (!current) {
        throw JsonTokenizerError("Unexpected end of input");
    }

    switch (*current) {
    case '{':
        advance();
        return Token(TokenType::LeftBrace);
    case '}':
        advance();
        return Token(TokenType::RightBrace);
    case '[':
        advance();
        return Token(TokenType::LeftBracket);
    case ']':
        advance();
        return Token(TokenType::RightBracket);
    case ',':
        advance();
        return Token(TokenType::Comma);
    case ':':
        advance();
        return Token(TokenType::Colon);
    case '"':
        return parseString();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return parseNumber();
    case 't':
    case 'f':
    case 'n':
        return parseBooleanOrNull();
    default:
        throw JsonTokenizerError("Unexpected character: " + std::string(1, *current));
    }
}

void JsonTokenizer::skipWhitespace() {
    while (!isEndOfInput() && std::isspace(input_[position_])) {
        ++position_;
    }
}

Token JsonTokenizer::parseString() {
    if (advance() != '"') {
        throw JsonTokenizerError("Expected opening quote");
    }

    std::string result;
    bool escaped = false;

    while (auto c = currentChar()) {
        if (escaped) {
            switch (*c) {
            case '"':
            case '\\':
            case '/':
                result += *c;
                break;
            case 'b':
                result += '\b';
                break;
            case 'f':
                result += '\f';
                break;
            case 'n':
                result += '\n';
                break;
            case 'r':
                result += '\r';
                break;
            case 't':
                result += '\t';
                break;
            default:
                throw JsonTokenizerError("Invalid escape sequence: \\" + std::string(1, *c));
            }
            escaped = false;
            advance();
            continue;
        }

        if (*c == '\\') {
            escaped = true;
            advance();
            continue;
        }

        if (*c == '"') {
            advance(); // Skip closing quote
            return Token(TokenType::String, std::move(result));
        }

        result += *c;
        advance();
    }

    throw JsonTokenizerError("Unterminated string literal");
}

Token JsonTokenizer::parseNumber() {
    std::string result;
    bool has_decimal = false;
    bool has_exponent = false;

    if (currentChar() == '-') {
        result += '-';
        advance();
    }

    if (currentChar() == '0') {
        result += '0';
        advance();

        if (currentChar() && std::isdigit(*currentChar())) {
            throw JsonTokenizerError("Invalid number format: leading zeros");
        }
    } else if (currentChar() && std::isdigit(*currentChar())) {
        while (currentChar() && std::isdigit(*currentChar())) {
            result += *currentChar();
            advance();
        }
    } else {
        throw JsonTokenizerError("Expected digit for number");
    }

    // Handle decimal point
    if (currentChar() && *currentChar() == '.') {
        has_decimal = true;
        result += '.';
        advance();
        if (!currentChar() || !std::isdigit(*currentChar())) {
            throw JsonTokenizerError("Expected digit after decimal point");
        }
        while (currentChar() && std::isdigit(*currentChar())) {
            result += *currentChar();
            advance();
        }
    }

    // Handle exponent
    if (currentChar() && (*currentChar() == 'e' || *currentChar() == 'E')) {
        has_exponent = true;
        result += *currentChar();
        advance();
        if (currentChar() && (*currentChar() == '+' || *currentChar() == '-')) {
            result += *currentChar();
            advance();
        }
        if (!currentChar() || !std::isdigit(*currentChar())) {
            throw JsonTokenizerError("Expected digit after exponent");
        }
        while (currentChar() && std::isdigit(*currentChar())) {
            result += *currentChar();
            advance();
        }
    }

    return Token(TokenType::Number, std::move(result));
}

Token JsonTokenizer::parseBooleanOrNull() {
    std::string_view prefixes[] = { "true", "false", "null" };
    TokenType types[] = { TokenType::Boolean, TokenType::Boolean, TokenType::Null };

    for (size_t i = 0; i < 3; ++i) {
        if (input_.substr(position_, prefixes[i].size()) == prefixes[i]) {
            position_ += prefixes[i].length();
            return Token(types[i], std::string(prefixes[i]));
        }
    }

    throw JsonTokenizerError("Expected 'true', 'false', or 'null'");
}