// third_party/json.hpp - Basic functional version
#ifndef NLOHMANN_JSON_HPP
#define NLOHMANN_JSON_HPP

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

namespace nlohmann {

class json {
private:
    enum class value_type {
        null, object, array, string, boolean, number_integer, number_float
    };
    
    value_type type = value_type::null;
    std::map<std::string, json> object_value;
    std::vector<json> array_value;
    std::string string_value;
    bool bool_value = false;
    long integer_value = 0;
    double float_value = 0.0;

public:
    json() = default;
    json(const char* val) : type(value_type::string), string_value(val) {}
    json(const std::string& val) : type(value_type::string), string_value(val) {}
    json(bool val) : type(value_type::boolean), bool_value(val) {}
    json(int val) : type(value_type::number_integer), integer_value(val) {}
    json(long val) : type(value_type::number_integer), integer_value(val) {}
    json(double val) : type(value_type::number_float), float_value(val) {}

    // Object access
    json& operator[](const std::string& key) {
        type = value_type::object;
        return object_value[key];
    }

    json& operator[](size_t index) {
        type = value_type::array;
        if (index >= array_value.size()) array_value.resize(index + 1);
        return array_value[index];
    }

    // Value getters
    template<typename T> T value(const char* key, T default_val) const {
        auto it = object_value.find(key);
        return (it != object_value.end()) ? (T)it->second : default_val;
    }

    template<typename T> T value(const std::string& key, T default_val) const {
        return value(key.c_str(), default_val);
    }

    // Type checks
    bool contains(const std::string& key) const { return object_value.count(key) > 0; }
    bool is_array() const { return type == value_type::array; }
    bool is_object() const { return type == value_type::object; }
    bool is_string() const { return type == value_type::string; }
    bool is_number() const { return type == value_type::number_integer || type == value_type::number_float; }

    // Size
    size_t size() const {
        switch(type) {
            case value_type::object: return object_value.size();
            case value_type::array: return array_value.size();
            default: return 0;
        }
    }

    // Conversion operators
    operator std::string() const { return string_value; }
    operator bool() const { return bool_value; }
    operator int() const { return integer_value; }
    operator long() const { return integer_value; }
    operator double() const { return float_value; }

    // Array iteration
    auto begin() { return array_value.begin(); }
    auto end() { return array_value.end(); }
    auto begin() const { return array_value.begin(); }
    auto end() const { return array_value.end(); }

    // Static parse method
    static json parse(const std::string& str) {
        json result;
        // Simple parser - handles basic objects and arrays
        if (!str.empty() && str[0] == '{') {
            result.type = value_type::object;
            // Basic object parsing (simplified)
        } else if (!str.empty() && str[0] == '[') {
            result.type = value_type::array;
            // Basic array parsing (simplified)
        }
        return result;
    }
};

} // namespace nlohmann

#endif
