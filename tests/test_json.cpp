#include <gtest/gtest.h>
#include "../include/json.hpp"
using namespace nlohmann;
TEST(JsonTest, TestParseJson) {
    json j;
    j["name"] = "John";
    j["age"] = 30;
    j["city"] = "New York";
    std::string json_str = j.dump();
    json parsed_json = json::parse(json_str);
    EXPECT_EQ(parsed_json["name"], "John");
    EXPECT_EQ(parsed_json["age"], 30);
    EXPECT_EQ(parsed_json["city"], "New York");
}
