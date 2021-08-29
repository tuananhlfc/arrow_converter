Converter between arrow schema and json string
==============================================

This project provides API for converting from json string to arrow schema and vice versa. The arrow cpp reposity did provide interface to read from json file but does not involve the opposite convert from table/schema to json.

Presiquites
===========

This project uses `cmake` as build file generator (min version 3.14), `conan` for package manager (optional), `libarrow-dev`, and make or ninja as build tool. Other dependencies are header-only libraries (nlohmann json, catch2), so I include them in third_party folder

Install conan (you can skip if you have `libarrow-dev` installed)
-----------------------------------------------------------------

```
pip3 install conan
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
mkdir build
conan install ..
```

Install libarrow-dev
--------------------

If you installed conan as above, then no need to install arrow because conan will download arrow to `build` folder. Otherwise, please make sure you installed `libarrow-dev` followed these instructions: [https://arrow.apache.org/install/](https://arrow.apache.org/install/)

Build instructions (tested on ubuntu 20.04)
===========================================

```
mkdir build
cmake ..
make -j4
```

The shared lib is built at `build/src/libarrow_json_converter.so`. Some examples are at `build/examples` folder and `build/test` contains the unit test executable.

Some other useful cmake flags: `-DENABLE_SANITIZER_ADDRESS=ON`, `-DENABLE_SANITIZER_MEMORY=ON`, `-DENABLE_SANITIZER_THREAD=ON`, `-DENABLE_SANITIZER_UNDEFINED_BEHAVIOR=ON`, `-DENABLE_CPPCHECK=ON`, `-DENABLE_CLANG_TIDY=ON`

Usage
=====

There are two classes: `JsonReader` (convert json to schema), and `JsonWriter` (convert schema to json). Because schema only has key and type of value (not the value), so for `JsonWriter`, I use the default value for each type corresponding to `arrow::Field` or `arrow:DataType` (int: 0, float: 0.0, datetime, string: "sample string")

Examples
--------

[convert_json_to_schema](https://github.com/tuananhlfc/arrow_converter/blob/main/examples/convert_json.cpp)
```
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
```

[convert_schema_to_json](https://github.com/tuananhlfc/arrow_converter/blob/main/examples/convert_arrow.cpp)
```
#include <iostream>
#include <api.hpp>
#include <arrow/api.h>

using namespace arrow;

int main() {
    std::shared_ptr<Schema> schema_ = arrow::schema({
        field("a", list(int64())),
        field("b", struct_({
            field("c", boolean()),
            field("d", timestamp(TimeUnit::SECOND)),
            field("e", struct_({
                field("f", struct_({
                    field("g", list(float32()))
                }))
            }))
        }))
    });
    lab::JsonWriter writer;
    auto json = writer.parse(schema_);
    // json: {"a":[0],"b":{"c":false,"d":"2021-08-29 21:27:23","e":{"f":{"g":[0.0]}}}}
    std::cout << json << std::endl;
}
```

How it works
============

JsonReader uses nlohmann json to parse json string to json object. Then recursively iterate all the members to get the arrow schema. In contrast, JsonWriter converts schema to json object, then convert json object to string

Because arrow data types and json value types are different, some implicit conversion must be taken into account. Type inference are referenced from here: [https://arrow.apache.org/docs/cpp/json.html](https://arrow.apache.org/docs/cpp/json.html)

* Null values are always null
* Number will be check as integer before check for float
* Boolean is boolean
* String is checked for timestamp first. If not valid then it's string
* Array is arrow list
* Object is arrow struct

Limitations and what to do next
===============================

1. The parser flow (using DOM) from json string to json object is unnecessary. Instead, we can use SAX event parser to parse json string. The converter is processed during the parsing, so it may be much faster and less memory consuming
2. Because json object is map type, the order of items inside an object is undefined. Therefore, the order of sub fields in a Schema is undetermined after passing
3. This library currently doesn't handle the case where a json array contains values with different types. For example, for array `[3, "a", 4.0]`, `JsonReader` only use the first value to determine the type inside the list. In this case, this array is converted as `arrow::list(arrow::int64())`
