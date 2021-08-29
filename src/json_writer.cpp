#include <unordered_set>
#include <ctime>
#include <iomanip>
#include <nlohmann/json.hpp>

#include "json_writer.hpp"
#include "helper.hpp"

namespace lab {

class JsonWriter::Impl {
public:
    std::string parse(std::shared_ptr<arrow::Schema> schema) {
        nlohmann::json root;
        for (auto& field: schema->fields()) {
            root[field->name()] = get_json_type(field->type());
        }
        return root.dump();
    }

    nlohmann::json get_json_type(std::shared_ptr<arrow::DataType> arrow_type) {
        if (arrow_type->id() == arrow::Type::NA) {
            return nullptr;
        } else if (arrow_type->id() == arrow::Type::BOOL) {
            return default_value<bool>();
        } else if (is_integer(arrow_type->id()) || is_timestamp(arrow_type->id())) {
            return 0;
        } else if (is_float(arrow_type->id())) {
            return 0.0;
        } else if (is_timedate(arrow_type->id())) {
            return default_timestamp();
        } else if (arrow_type->id() == arrow::Type::STRING) {
            return default_string();
        } else if (arrow_type->id() == arrow::Type::LIST) {
            std::vector<nlohmann::json> children;
            children.push_back(get_json_type(arrow_type->fields()[0]->type()));
            return children;
        } else if (arrow_type->id() == arrow::Type::STRUCT) {
            nlohmann::json sub_object;
            for (auto& field: arrow_type->fields()) {
                sub_object[field->name()] = get_json_type(field->type());
            }
            return sub_object;
        }
        return nullptr;
    }

private:
    bool is_integer(const arrow::Type::type type) {
        static const std::unordered_set<arrow::Type::type> integer_types = {
            arrow::Type::INT64,
            arrow::Type::INT32,
            arrow::Type::INT16,
            arrow::Type::INT8,
            arrow::Type::UINT64,
            arrow::Type::UINT32,
            arrow::Type::UINT16,
            arrow::Type::UINT8
        };
        return integer_types.count(type);
    }
    bool is_float(const arrow::Type::type type) {
        return type == arrow::Type::DOUBLE || type == arrow::Type::FLOAT;
    }
    bool is_timestamp(arrow::Type::type type) {
        static const std::unordered_set<arrow::Type::type> timestamp_types = {
            arrow::Type::DATE32,
            arrow::Type::DATE64,
            arrow::Type::TIME32,
            arrow::Type::TIME64
        };
        return timestamp_types.count(type);
    }
    bool is_timedate(const arrow::Type::type type) {
        return type == arrow::Type::TIMESTAMP;
    }
};

JsonWriter::JsonWriter(): impl_(new JsonWriter::Impl) {

}

JsonWriter::~JsonWriter() = default;

std::string JsonWriter::parse(std::shared_ptr<arrow::Schema> schema) {
    return impl_->parse(schema);
}

} // namespace lab
