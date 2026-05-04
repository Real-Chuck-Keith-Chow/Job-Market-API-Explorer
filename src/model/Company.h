#ifndef COMPANY_H
#define COMPANY_H

#include <string>

struct Company {
    std::string id;
    std::string display_name;

    Company() = default;

    explicit Company(const std::string& name)
        : display_name(name) {}

    bool isValid() const {
        return !display_name.empty();
    }

    std::string toString() const {
        return display_name;
    }
};

#endif
