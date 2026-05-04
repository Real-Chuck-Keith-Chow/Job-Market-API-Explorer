#ifndef LOCATION_H
#define LOCATION_H

#include <string>

struct Location {
    std::string display_name;
    std::string area;
    std::string country;

    Location() = default;

    explicit Location(const std::string& name)
        : display_name(name) {}

    bool isValid() const {
        return !display_name.empty();
    }

    std::string toString() const {
        return display_name;
    }
};

#endif
