#include <gtest/gtest.h>
#include <json.h>
#include <fstream>
#include <sstream>
#include <string>

class JsonParserTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(JsonParserTest, ParseSimpleJson) {
    const char* input = "{\"name\": \"Alice\", \"age\": 30}";

    auto output = Json::parse(input);

    ASSERT_EQ(output["name"].asString(), "Alice");
    ASSERT_EQ(output["age"].asInt(), 30);
}