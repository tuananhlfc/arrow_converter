#include <nlohmann/json.hpp>
#include "json_reader.hpp"
#include "helper.hpp"

namespace lab {

class JsonReader::Impl {
public:
    std::shared_ptr<arrow::Schema> parse(const std::string& in) {
        nlohmann::json data;
        try {
            data = nlohmann::json::parse(in);
        } catch (std::exception& e) {
            return nullptr;
        }

        std::vector<std::shared_ptr<arrow::Field>> fields;
        for (auto& field: data.items()) {
            auto child = get_arrow_type(field.value());
            if (!child) {
                return nullptr;
            }
            fields.push_back(arrow::field(field.key(), child));
        }
        return arrow::schema(fields);
    }

    std::shared_ptr<arrow::DataType> get_arrow_type(nlohmann::json& value) {
        if (value.is_null()) {
            return arrow::null();
        } else if (value.is_boolean()) {
            return arrow::boolean();
        } else if (value.is_number_integer()) {
            return arrow::int64();
        } else if (value.is_number_float()) {
            return arrow::float64();
        } else if (value.is_string()) {
            if (valid_timestamp(value)) {
                return arrow::timestamp(arrow::TimeUnit::SECOND);
            }
            return arrow::utf8();
        } else if (value.is_array()) {
            if (value.empty()) {
                return arrow::list(arrow::null());
            }
            return value.empty() ? arrow::list(arrow::null()) : arrow::list(get_arrow_type(value[0]));
        } else if (value.is_object()) {
            std::vector<std::shared_ptr<arrow::Field>> fields;
            for (auto& field: value.items()) {
                fields.push_back(arrow::field(field.key(), get_arrow_type(field.value())));
            }
            return arrow::struct_(fields);
        }
        return nullptr;
    }
};

JsonReader::JsonReader(): impl_(new JsonReader::Impl) {

}

JsonReader::~JsonReader() = default;

std::shared_ptr<arrow::Schema> JsonReader::parse(const std::string& json) {
    return impl_->parse(json);
}

} // namespace lab
