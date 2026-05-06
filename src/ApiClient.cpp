#include "ApiClient.h"

#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "json.hpp"

using json = nlohmann::json;

ApiClient::ApiClient(const std::string& app_id, const std::string& app_key)
    : adzuna_app_id(app_id), adzuna_app_key(app_key) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

ApiClient::~ApiClient() {
    curl_global_cleanup();
}

size_t ApiClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

std::string ApiClient::makeHttpRequest(const std::string& url) const {
    CURL* curl = curl_easy_init();
    std::string response;

    if (!curl) {
        throw std::runtime_error("Failed to init CURL");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "JobMarketExplorer/1.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "HTTP error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    return response;
}

std::string ApiClient::urlEncode(const std::string& value) {
    CURL* curl = curl_easy_init();
    if (!curl) return value;

    char* output = curl_easy_escape(curl, value.c_str(), static_cast<int>(value.length()));
    std::string encoded = output ? output : value;

    if (output) curl_free(output);
    curl_easy_cleanup(curl);

    return encoded;
}

std::vector<Job> ApiClient::fetchFromAdzuna(const std::string& query,
                                            const std::string& location,
                                            int results_per_page,
                                            double min_salary,
                                            int max_pages) const {
    std::vector<Job> jobs;

    if (max_pages < 1) {
        max_pages = 1;
    }

    for (int page = 1; page <= max_pages; page++) {
        std::stringstream url;
        url << "https://api.adzuna.com/v1/api/jobs/us/search/" << page << "?"
            << "app_id=" << adzuna_app_id
            << "&app_key=" << adzuna_app_key
            << "&results_per_page=" << results_per_page;

        if (!query.empty()) {
            url << "&what=" << urlEncode(query);
        }

        if (!location.empty()) {
            url << "&where=" << urlEncode(location);
        }

        if (min_salary > 0) {
            url << "&salary_min=" << static_cast<int>(min_salary);
        }

        std::string response = makeHttpRequest(url.str());

        try {
            json data = json::parse(response);

            if (!data.contains("results") || !data["results"].is_array()) {
                break;
            }

            if (data["results"].empty()) {
                break;
            }

            for (const auto& item : data["results"]) {
                Job job;

                job.id = item.value("id", "");
                job.title = item.value("title", "");

                if (item.contains("company") && item["company"].is_object()) {
                    job.company.display_name = item["company"].value("display_name", "");
                }

                if (item.contains("location") && item["location"].is_object()) {
                    job.location.display_name = item["location"].value("display_name", "");
                }

                job.salary_min = item.value("salary_min", 0.0);
                job.salary_max = item.value("salary_max", 0.0);
                job.description = item.value("description", "");
                job.redirect_url = item.value("redirect_url", "");
                job.created = item.value("created", "");

                jobs.push_back(job);
            }

        } catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            break;
        }
    }

    return jobs;
}

std::vector<Job> ApiClient::searchJobs(const std::string& query,
                                       const std::string& location,
                                       double min_salary,
                                       int max_pages) const {
    return fetchFromAdzuna(query, location, 50, min_salary, max_pages);
}
