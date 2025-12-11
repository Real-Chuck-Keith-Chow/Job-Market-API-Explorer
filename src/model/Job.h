#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>

struct Location {
    std::string display_name;
    std::string area;
    std::string country;
};

struct Company {
    std::string display_name;
    std::string id;
};

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

    Job() : salary_min(0), salary_max(0) {}
};

#endif
