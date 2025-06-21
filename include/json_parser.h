#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "json_tokenizer.h"
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <optional>
#include <stdexcept>

enum class JsonType {
    Null,
    String,
    Number,
    Boolean,
    Object,
    Array
};

struct JsonValue {
    JsonType type;
    std::variant<
        std::nullptr_t,
        std::string,
        double,
        bool,
        std::map<std::string, JsonValue>,
        std::vector<JsonValue>
    > value;

    JsonValue() : type(JsonType::Null), value(nullptr) {}
    explicit JsonValue(std::string str) : type(JsonType::String), value(std::move(str)) {}
    explicit JsonValue(double num) : type(JsonType::Number), value(num) {}
    explicit JsonValue(bool boolean) : type(JsonType::Boolean), value(boolean) {}
    explicit JsonValue(std::map<std::string, JsonValue> obj) : type(JsonType::Object), value(std::move(obj)) {}
    explicit JsonValue(std::vector<JsonValue> arr) : type(JsonType::Array), value(std::move(arr)) {}

    // Access by key for objects
    const JsonValue& operator[](const std::string& key) const {
        if (type != JsonType::Object) throw std::runtime_error("JsonValue is not an object");
        const auto& obj = std::get<std::map<std::string, JsonValue>>(value);
        auto it = obj.find(key);
        if (it == obj.end()) throw std::runtime_error("Key not found: " + key);
        return it->second;
    }

    // Access by index for arrays
    const JsonValue& operator[](size_t index) const {
        if (type != JsonType::Array) throw std::runtime_error("JsonValue is not an array");
        const auto& arr = std::get<std::vector<JsonValue>>(value);
        if (index >= arr.size()) throw std::runtime_error("Index out of bounds");
        return arr[index];
    }

    // Getters with type validation
    const std::string& asString() const {
        if (type != JsonType::String) throw std::runtime_error("Not a string");
        return std::get<std::string>(value);
    }

    double asDouble() const {
        if (type != JsonType::Number) throw std::runtime_error("Not a number");
        return std::get<double>(value);
    }

    int asInt() const {
        return static_cast<int>(asDouble());
    }

    bool asBool() const {
        if (type != JsonType::Boolean) throw std::runtime_error("Not a boolean");
        return std::get<bool>(value);
    }

    const std::map<std::string, JsonValue>& asObject() const {
        if (type != JsonType::Object) throw std::runtime_error("Not an object");
        return std::get<std::map<std::string, JsonValue>>(value);
    }

    const std::vector<JsonValue>& asArray() const {
        if (type != JsonType::Array) throw std::runtime_error("Not an array");
        return std::get<std::vector<JsonValue>>(value);
    }

    // Type check helpers
    bool isNull() const { return type == JsonType::Null; }
    bool isString() const { return type == JsonType::String; }
    bool isNumber() const { return type == JsonType::Number; }
    bool isBool() const { return type == JsonType::Boolean; }
    bool isObject() const { return type == JsonType::Object; }
    bool isArray() const { return type == JsonType::Array; }
};

class JsonParser {
public:
    explicit JsonParser(JsonTokenizer tokenizer)
        : tokenizer_(std::move(tokenizer)), currentToken_(Token(TokenType::EndOfInput)) {}

    JsonValue parse();

private:
    JsonTokenizer tokenizer_;
    Token currentToken_;

    void nextToken() { currentToken_ = tokenizer_.nextToken(); }

    JsonValue parseValue();
    JsonValue parseObject();
    JsonValue parseArray();
    JsonValue parseString();
    JsonValue parseNumber();
    JsonValue parseBoolean();
    JsonValue parseNull();
};

#endif // JSON_PARSER_H