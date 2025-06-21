#include "json_parser.h"
#include "json_tokenizer.h"
#include <iostream>
#include <stdexcept>

JsonValue JsonParser::parse() {
    nextToken();
    JsonValue output = parseValue();

    if (currentToken_.type != TokenType::EndOfInput) {
        throw JsonTokenizerError("Unexpected token after JSON value");
    }

    return output;
}

JsonValue JsonParser::parseValue() {
    switch (currentToken_.type) {
        case TokenType::String:
            return parseString();
        case TokenType::Number:
            return parseNumber();
        case TokenType::Boolean:
            return parseBoolean();
        case TokenType::Null:
            return parseNull();
        case TokenType::LeftBrace:
            return parseObject();
        case TokenType::LeftBracket:
            return parseArray();
        default:
            throw JsonTokenizerError("Unexpected token type in value");
    }
}

JsonValue JsonParser::parseString() {
    if (currentToken_.type != TokenType::String || !currentToken_.value.has_value()) {
        throw JsonTokenizerError("Expected string token with value");
    }

    std::string str = currentToken_.value.value();
    nextToken();
    return JsonValue(std::move(str));
}

JsonValue JsonParser::parseNumber() {
    if (currentToken_.type != TokenType::Number || !currentToken_.value.has_value()) {
        throw JsonTokenizerError("Expected number token with value");
    }

    double num = std::stod(currentToken_.value.value());
    nextToken();
    return JsonValue(num);
}

JsonValue JsonParser::parseBoolean() {
    if (currentToken_.type != TokenType::Boolean || !currentToken_.value.has_value()) {
        throw JsonTokenizerError("Expected boolean token with value");
    }

    bool value = (currentToken_.value.value() == "true");
    nextToken();
    return JsonValue(value);
}

JsonValue JsonParser::parseNull() {
    if (currentToken_.type != TokenType::Null) {
        throw JsonTokenizerError("Expected null token");
    }

    nextToken();
    return JsonValue();
}

JsonValue JsonParser::parseArray() {
    if (currentToken_.type != TokenType::LeftBracket) {
        throw JsonTokenizerError("Expected '[' to start array");
    }

    nextToken();
    std::vector<JsonValue> array;

    if (currentToken_.type == TokenType::RightBracket) {
        nextToken();
        return JsonValue(std::move(array));
    }

    while (true) {
        array.push_back(parseValue());

        if (currentToken_.type == TokenType::Comma) {
            nextToken();
        } else if (currentToken_.type == TokenType::RightBracket) {
            nextToken();
            break;
        } else {
            throw JsonTokenizerError("Expected ',' or ']' in array");
        }
    }

    return JsonValue(std::move(array));
}

JsonValue JsonParser::parseObject() {
    if (currentToken_.type != TokenType::LeftBrace) {
        throw JsonTokenizerError("Expected '{' to start object");
    }

    nextToken();  // skip '{'
    std::map<std::string, JsonValue> object;

    if (currentToken_.type == TokenType::RightBrace) {
        nextToken();
        return JsonValue(std::move(object));  // empty object
    }

    while (true) {
        if (currentToken_.type != TokenType::String || !currentToken_.value.has_value()) {
            throw JsonTokenizerError("Expected string as key in object");
        }

        std::string key = currentToken_.value.value();
        nextToken();

        if (currentToken_.type != TokenType::Colon) {
            throw JsonTokenizerError("Expected ':' after key in object");
        }
        nextToken();

        JsonValue val = parseValue();
        object.emplace(std::move(key), std::move(val));

        if (currentToken_.type == TokenType::Comma) {
            nextToken();
        } else if (currentToken_.type == TokenType::RightBrace) {
            nextToken();
            break;
        } else {
            throw JsonTokenizerError("Expected ',' or '}' in object");
        }
    }

    return JsonValue(std::move(object));
}
