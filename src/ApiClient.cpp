#include "ApiClient.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

ApiClient::ApiClient(const std::string& app_id, const std::string& app_key, 
                     const std::string& github_url,
                     bool enable_github) 
    : adzuna_app_id(app_id), adzuna_app_key(app_key),
      github_jobs_url(github_url.empty() ? "https://jobs.github.com/positions.json" : github_url),
      enable_github_jobs(enable_github) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

size_t ApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}

std::string ApiClient::makeHttpRequest(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Job-Market-API-Explorer/1.0");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        
        CURLcode res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_easy_cleanup(curl);
    }
    
    return response;
}

std::vector<Job> ApiClient::fetchFromAdzuna(const std::string& query, 
                                          const std::string& location,
                                          int results_per_page,
                                          double min_salary) {
    std::vector<Job> jobs;
    // Simple pagination over a few pages without changing the public signature.
    const int kMaxPages = 3;
    std::string what_param;
    std::string where_param;

    // Pre-escape query/location once.
    if (!query.empty() || !location.empty()) {
        CURL* curl = curl_easy_init();
        if (curl) {
            if (!query.empty()) {
                char* esc = curl_easy_escape(curl, query.c_str(), static_cast<int>(query.size()));
                if (esc) { what_param = "&what=" + std::string(esc); curl_free(esc); }
            }
            if (!location.empty()) {
                char* esc = curl_easy_escape(curl, location.c_str(), static_cast<int>(location.size()));
                if (esc) { where_param = "&where=" + std::string(esc); curl_free(esc); }
            }
            curl_easy_cleanup(curl);
        }
    }

    for (int page = 1; page <= kMaxPages; ++page) {
        std::stringstream url;
        url << "https://api.adzuna.com/v1/api/jobs/ca/search/" << page << "?"
            << "app_id=" << adzuna_app_id
            << "&app_key=" << adzuna_app_key
            << "&results_per_page=" << results_per_page
            << what_param
            << where_param;

        if (min_salary > 0) {
            url << "&salary_min=" << static_cast<long long>(min_salary);
        }
        
        std::string response = makeHttpRequest(url.str());
        std::size_t before = jobs.size();
        
        try {
            json data = json::parse(response);
            
            if (data.contains("results")) {
                for (const auto& item : data["results"]) {
                    Job job;
                    job.id = item.value("id", "");
                    job.title = item.value("title", "");
                    job.company.display_name = item.value("company", {}).value("display_name", "");
                    job.location.display_name = item.value("location", {}).value("display_name", "");
                    job.salary_min = item.value("salary_min", 0.0);
                    job.salary_max = item.value("salary_max", 0.0);
                    job.redirect_url = item.value("redirect_url", "");
                    job.created = item.value("created", "");
                    job.description = item.value("description", "");
                    
                    jobs.push_back(job);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Adzuna API response: " << e.what() << std::endl;
        }

        // Stop if no new results came back.
        if (jobs.size() == before) break;
    }
    
    return jobs;
}

std::vector<Job> ApiClient::fetchFromGitHubJobs(const std::string& description,
                                              const std::string& location) {
    std::vector<Job> jobs;
    
    std::stringstream url;
    url << github_jobs_url << "?";
    
    CURL* curl = curl_easy_init();
    if (curl) {
        if (!description.empty()) {
            char* esc = curl_easy_escape(curl, description.c_str(), static_cast<int>(description.size()));
            if (esc) { url << "description=" << esc; curl_free(esc); }
        }
        
        if (!location.empty()) {
            if (!description.empty()) url << "&";
            char* esc = curl_easy_escape(curl, location.c_str(), static_cast<int>(location.size()));
            if (esc) { url << "location=" << esc; curl_free(esc); }
        }
        curl_easy_cleanup(curl);
    }
    
    std::string response = makeHttpRequest(url.str());
    
    try {
        json data = json::parse(response);
        
        for (const auto& item : data) {
            Job job;
            job.id = item.value("id", "");
            job.title = item.value("title", "");
            job.company.display_name = item.value("company", "");
            job.location.display_name = item.value("location", "");
            job.description = item.value("description", "");
            job.redirect_url = item.value("url", "");
            job.created = item.value("created_at", "");
            
            jobs.push_back(job);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing GitHub Jobs API response: " << e.what() << std::endl;
    }
    
    return jobs;
}

std::vector<Job> ApiClient::searchJobs(const std::string& query, 
                                     const std::string& location,
                                     double min_salary) {
    std::vector<Job> all_jobs;
    
    // Fetch from Adzuna
    auto adzuna_jobs = fetchFromAdzuna(query, location, 50, min_salary);
    all_jobs.insert(all_jobs.end(), adzuna_jobs.begin(), adzuna_jobs.end());
    
    // Fetch from GitHub Jobs
    if (enable_github_jobs) {
        auto github_jobs = fetchFromGitHubJobs(query, location);
        all_jobs.insert(all_jobs.end(), github_jobs.begin(), github_jobs.end());
    }
    
    // Filter by minimum salary if specified
    if (min_salary > 0) {
        std::vector<Job> filtered_jobs;
        for (const auto& job : all_jobs) {
            if (job.salary_min >= min_salary || job.salary_max >= min_salary) {
                filtered_jobs.push_back(job);
            }
        }
        return filtered_jobs;
    }
    
    return all_jobs;
}
