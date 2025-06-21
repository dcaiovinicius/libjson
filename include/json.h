#ifndef JSON_H
#define JSON_H

#include <optional>
#include "json_parser.h"

namespace Json {
     JsonValue parse(const std::string& data);
}
#endif