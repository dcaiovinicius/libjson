#include <gtest/gtest.h>
#include <json.h>
#include <fstream>
#include <sstream>
#include <string>

TEST(JsonParserTest, ParseSimpleJson) {
    const char* input = "{\"name\": \"Alice\", \"age\": 30}";

    auto output = Json::parse(input);

    ASSERT_EQ(output["name"].asString(), "Alice");
    ASSERT_EQ(output["age"].asInt(), 30);
}