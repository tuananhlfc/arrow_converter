#ifndef JSON_READER_HPP__
#define JSON_READER_HPP__

#include <arrow/api.h>

namespace lab {

class JsonReader {
public:
    JsonReader();
    ~JsonReader();
    std::shared_ptr<arrow::Schema> parse(const std::string& json);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace lab

#endif // JSON_READER_HPP__
