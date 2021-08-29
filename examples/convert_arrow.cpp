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
