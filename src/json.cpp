#include "json.h"
#include "json_parser.h"
#include "json_tokenizer.h"

namespace Json {
    JsonValue parse(const std::string& data) {
        JsonTokenizer tokenizer(data);
        JsonParser parser(std::move(tokenizer));
        JsonValue value = parser.parse();
        return value;
    }
}