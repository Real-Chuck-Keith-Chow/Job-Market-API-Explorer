#include "ApiClient.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include "third_party/json.hpp"

using json = nlohmann::json;

ApiClient::ApiClient(const std::string& app_id, const std::string& app_key) 
    : adzuna_app_id(app_id), adzuna_app_key(app_key),
      github_jobs_url("https://jobs.github.com/positions.json") {
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
                                          int results_per_page) {
    std::vector<Job> jobs;
    
    std::stringstream url;
    url << "https://api.adzuna.com/v1/api/jobs/ca/search/1?"
        << "app_id=" << adzuna_app_id
        << "&app_key=" << adzuna_app_key
        << "&results_per_page=" << results_per_page;
    
    if (!query.empty()) {
        url << "&what=" << curl_easy_escape(nullptr, query.c_str(), query.length());
    }
    
    if (!location.empty()) {
        url << "&where=" << curl_easy_escape(nullptr, location.c_str(), location.length());
    }
    
    std::string response = makeHttpRequest(url.str());
    
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
                
                jobs.push_back(job);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing Adzuna API response: " << e.what() << std::endl;
    }
    
    return jobs;
}

std::vector<Job> ApiClient::fetchFromGitHubJobs(const std::string& description,
                                              const std::string& location) {
    std::vector<Job> jobs;
    
    std::stringstream url;
    url << github_jobs_url << "?";
    
    if (!description.empty()) {
        url << "description=" << curl_easy_escape(nullptr, description.c_str(), description.length());
    }
    
    if (!location.empty()) {
        if (!description.empty()) url << "&";
        url << "location=" << curl_easy_escape(nullptr, location.c_str(), location.length());
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
    
    auto adzuna_jobs = fetchFromAdzuna(query, location);
    all_jobs.insert(all_jobs.end(), adzuna_jobs.begin(), adzuna_jobs.end());
    
    auto github_jobs = fetchFromGitHubJobs(query, location);
    all_jobs.insert(all_jobs.end(), github_jobs.begin(), github_jobs.end());
    
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
