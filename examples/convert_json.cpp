#include <api.hpp>
#include <iostream>

int main() {
    auto text = R"({"a": 1, "a1": true, "b": 2.0, "c": "foo", "d": [3, 4, 5], "e": {"a": null, "b": {"a": 1}}})";
    lab::JsonReader reader;
    auto schema = reader.parse(text);
    if (schema) {
        std::cout << schema->ToString() << std::endl;
    }
}
