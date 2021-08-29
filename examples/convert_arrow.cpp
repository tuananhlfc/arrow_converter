#include <api.hpp>
#include <iostream>
#include <arrow/api.h>

using namespace arrow;

int main() {
    std::shared_ptr<Schema> schema_ = arrow::schema({
        field("a", list(int64())),
        field("e", list(float32())),
        field("b", struct_({field("c", boolean()),
        field("d", timestamp(TimeUnit::SECOND))}))
    });
    lab::JsonWriter writer;
    auto json = writer.parse(schema_);
    // json: {"a":[0],"b":{"c":false,"d":"2021-08-29 21:27:23"},"e":[0.0]}
    std::cout << json << std::endl;
}
