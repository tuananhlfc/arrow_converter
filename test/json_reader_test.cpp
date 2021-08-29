#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <json_reader.hpp>

lab::JsonReader reader;

TEST_CASE("Empty json object") {
    auto empty_json = R"({})";
    REQUIRE(*reader.parse(empty_json) == *arrow::schema({}));
}

TEST_CASE("Non valid json string") {
    auto json = R"({a: 1})";
    REQUIRE(reader.parse(json) == nullptr);
}

TEST_CASE("Implicit conversion data type") {
    auto json = R"({"a": 1, "b": 2.0, "c": true, "d": "2021-05-22", "e": "hello"})";
    auto schema = arrow::schema({
        arrow::field("a", arrow::int64()),
        arrow::field("b", arrow::float64()),
        arrow::field("c", arrow::boolean()),
        arrow::field("d", arrow::timestamp(arrow::TimeUnit::SECOND)),
        arrow::field("e", arrow::utf8())
    });
    REQUIRE(*reader.parse(json) == *schema);

    auto json1 = R"({"a": ["2021-05-22", "2021-05-21"] })";
    auto schema1 = arrow::schema({arrow::field("a", arrow::list(arrow::timestamp(arrow::TimeUnit::SECOND)))});
    REQUIRE(*reader.parse(json1) == *schema1);

    auto json2 = R"({"a": {"b": "value"} })";
    auto schema2 = arrow::schema({
        arrow::field("a", arrow::struct_({
            arrow::field("b", arrow::utf8())
        }))
    });
    REQUIRE(*reader.parse(json2) == *schema2);
}

TEST_CASE("Multi hierarchy json string (recursive)") {
    auto json = R"({"a": {"b": {"c": [1, 2, 3] } } })";
    auto schema = arrow::schema({
        arrow::field("a", arrow::struct_({
            arrow::field("b", arrow::struct_({
                arrow::field("c", arrow::list(arrow::int64()))
            }))
        }))
    });
    REQUIRE(*reader.parse(json) == *schema);

    auto json1 = R"(
    {
        "glossary": {
            "title": "example glossary",
            "GlossDiv": {
                "title": "S",
                "GlossList": {
                    "GlossEntry": {
                        "ID": "SGML",
                        "SortAs": "SGML",
                        "GlossTerm": "Standard Generalized Markup Language",
                        "Acronym": "SGML",
                        "Abbrev": "ISO 8879:1986",
                        "GlossDef": {
                            "para": "A meta-markup language, used to create markup languages such as DocBook.",
                            "GlossSeeAlso": ["GML", "XML"]
                        },
                        "GlossSee": "markup"
                    }
                }
            }
        }
    }
    )";
    auto schema1 = arrow::schema({
        arrow::field("glossary", arrow::struct_({
            arrow::field("GlossDiv", arrow::struct_({
                arrow::field("GlossList", arrow::struct_({
                    arrow::field("GlossEntry", arrow::struct_({
                        arrow::field("Abbrev", arrow::utf8()),
                        arrow::field("Acronym", arrow::utf8()),                        
                        arrow::field("GlossDef", arrow::struct_({
                            arrow::field("GlossSeeAlso", arrow::list(arrow::utf8())),
                            arrow::field("para", arrow::utf8())
                        })),
                        arrow::field("GlossSee", arrow::utf8()),
                        arrow::field("GlossTerm", arrow::utf8()),
                        arrow::field("ID", arrow::utf8()),
                        arrow::field("SortAs", arrow::utf8()),
                    }))
                })),
                arrow::field("title", arrow::utf8())
            })),
            arrow::field("title", arrow::utf8())
        }))
    });
    REQUIRE(*reader.parse(json1) == *schema1);
}
