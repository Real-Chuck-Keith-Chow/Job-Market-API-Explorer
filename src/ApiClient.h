#ifndef APICLIENT_H
#define APICLIENT_H

#include <string>
#include <vector>
#include "model/Job.h"

class ApiClient {
private:
    std::string adzuna_app_id;
    std::string adzuna_app_key;
    std::string github_jobs_url;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);
    std::string makeHttpRequest(const std::string& url);
    
public:
    ApiClient(const std::string& app_id, const std::string& app_key);
    
    std::vector<Job> fetchFromAdzuna(const std::string& query = "", 
                                    const std::string& location = "",
                                    int results_per_page = 50);
                                    
    std::vector<Job> fetchFromGitHubJobs(const std::string& description = "",
                                        const std::string& location = "");
    
    std::vector<Job> searchJobs(const std::string& query, 
                               const std::string& location,
                               double min_salary = 0);
};

#endif
