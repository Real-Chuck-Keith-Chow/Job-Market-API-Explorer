#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>

#include "Company.h"
#include "Location.h"

struct Job {
    std::string id;
    std::string title;

    Company company;
    Location location;

    double salary_min;
    double salary_max;

    std::string description;
    std::string redirect_url;
    std::vector<std::string> technologies;
    std::string created;

    Job()
        : salary_min(0.0),
          salary_max(0.0) {}

    bool isValid() const {
        return !title.empty() && company.isValid();
    }
};

#endif
