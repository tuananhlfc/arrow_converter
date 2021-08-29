#ifndef JSON_WRITER_HPP__
#define JSON_WRITER_HPP__

#include <arrow/api.h>

namespace lab {

class JsonWriter {
public:
    JsonWriter();
    ~JsonWriter();
    std::string parse(std::shared_ptr<arrow::Schema> schema);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace lab

#endif // JSON_WRITER_HPP__
