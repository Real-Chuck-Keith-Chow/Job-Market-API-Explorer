#ifndef APICLIENT_H
#define APICLIENT_H

#include <string>
#include <vector>

#include "model/Job.h"

class ApiClient {
private:
    std::string adzuna_app_id;
    std::string adzuna_app_key;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    std::string makeHttpRequest(const std::string& url) const;
    static std::string urlEncode(const std::string& value);

public:
    ApiClient(const std::string& app_id, const std::string& app_key);
    ~ApiClient();

    ApiClient(const ApiClient&) = delete;
    ApiClient& operator=(const ApiClient&) = delete;

    std::vector<Job> fetchFromAdzuna(const std::string& query = "",
                                     const std::string& location = "",
                                     int results_per_page = 50,
                                     double min_salary = 0.0) const;

    std::vector<Job> searchJobs(const std::string& query,
                                const std::string& location,
                                double min_salary = 0.0) const;
};

#endif
