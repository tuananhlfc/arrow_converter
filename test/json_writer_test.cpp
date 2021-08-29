#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <json_writer.hpp>

lab::JsonWriter writer;

TEST_CASE("Empty schema") {
    auto empty_schema = arrow::schema({});
    REQUIRE(writer.parse(empty_schema) == "null");
}

TEST_CASE("Simple conversion") {
    auto schema = arrow::schema({arrow::field("a", arrow::int64())});
    auto schema1 = arrow::schema({arrow::field("a", arrow::float64())});
    auto schema2 = arrow::schema({arrow::field("a", arrow::int8())});
    auto schema3 = arrow::schema({arrow::field("a", arrow::timestamp(arrow::TimeUnit::SECOND))});
    auto schema4 = arrow::schema({arrow::field("a", arrow::utf8())});
    auto schema5 = arrow::schema({arrow::field("a", arrow::list(arrow::utf8()))});

    REQUIRE(writer.parse(schema) == R"({"a":0})");
    REQUIRE(writer.parse(schema1) == R"({"a":0.0})");
    REQUIRE(writer.parse(schema2) == R"({"a":0})");
    REQUIRE(writer.parse(schema3) == R"({"a":"2021-08-29 21:27:23"})");
    REQUIRE(writer.parse(schema4) == R"({"a":"sample string"})");
    REQUIRE(writer.parse(schema5) == R"({"a":["sample string"]})");
}

TEST_CASE("More than two dimension schema") {
    auto schema = arrow::schema({
        arrow::field("a", arrow::list(arrow::int64())),
        arrow::field("b", arrow::struct_({
            arrow::field("c", arrow::boolean()),
            arrow::field("d", arrow::timestamp(arrow::TimeUnit::SECOND)),
            arrow::field("e", arrow::struct_({
                arrow::field("f", arrow::struct_({
                    arrow::field("g", arrow::list(arrow::float32()))
                }))
            }))
        }))
    });
    REQUIRE(writer.parse(schema) == R"({"a":[0],"b":{"c":false,"d":"2021-08-29 21:27:23","e":{"f":{"g":[0.0]}}}})");
}
