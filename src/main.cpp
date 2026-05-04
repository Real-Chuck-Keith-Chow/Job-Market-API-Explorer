#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "JobParser.h"
#include "ApiClient.h"
#include "json.hpp"

using json = nlohmann::json;

void displayJob(const Job& job) {
    std::cout << "Title: " << job.title << '\n';
    std::cout << "Company: " << job.company.display_name << '\n';
    std::cout << "Location: " << job.location.display_name << '\n';

    if (job.salary_min > 0 || job.salary_max > 0) {
        std::cout << "Salary: $"
                  << std::fixed << std::setprecision(0)
                  << job.salary_min << " - $" << job.salary_max << '\n';
    }

    std::cout << "URL: " << job.redirect_url << '\n';
    std::cout << "Posted: " << job.created << '\n';
    std::cout << "----------------------------------------\n";
}

void displayStatistics(const std::vector<Job>& jobs) {
    if (jobs.empty()) {
        std::cout << "No jobs found.\n";
        return;
    }

    std::cout << "\n=== JOB MARKET STATISTICS ===\n";
    std::cout << "Total jobs found: " << jobs.size() << '\n';

    std::map<std::string, int> company_counts;
    double total_salary = 0.0;
    int salary_count = 0;

    for (const auto& job : jobs) {
        if (!job.company.display_name.empty()) {
            company_counts[job.company.display_name]++;
        }

        if (job.salary_min > 0) {
            total_salary += job.salary_min;
            salary_count++;
        }
    }

    std::cout << "\nCompanies found:\n";
    int shown = 0;
    for (const auto& [company, count] : company_counts) {
        if (shown >= 5) break;
        std::cout << "  " << company << ": " << count << " job(s)\n";
        shown++;
    }

    if (salary_count > 0) {
        std::cout << "Average minimum salary: $"
                  << std::fixed << std::setprecision(0)
                  << total_salary / salary_count << '\n';
    }
}

int main() {
    std::ifstream config_file("config.json");

    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config.json\n";
        std::cerr << "Create config.json with adzuna_app_id and adzuna_app_key.\n";
        return 1;
    }

    json config;

    try {
        config_file >> config;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing config.json: " << e.what() << '\n';
        return 1;
    }

    if (!config.contains("adzuna_app_id") || !config["adzuna_app_id"].is_string() ||
        !config.contains("adzuna_app_key") || !config["adzuna_app_key"].is_string()) {
        std::cerr << "Error: config.json must contain string values for:\n";
        std::cerr << "  adzuna_app_id\n";
        std::cerr << "  adzuna_app_key\n";
        return 1;
    }

    std::string app_id = config["adzuna_app_id"];
    std::string app_key = config["adzuna_app_key"];

    ApiClient client(app_id, app_key);

    std::string query;
    std::string location;
    std::string salary_input;
    double min_salary = 0.0;

    std::cout << "=== JOB MARKET API EXPLORER ===\n";

    std::cout << "Search keyword: ";
    std::getline(std::cin, query);

    std::cout << "Location: ";
    std::getline(std::cin, location);

    std::cout << "Minimum salary (press Enter for none): ";
    std::getline(std::cin, salary_input);

    if (!salary_input.empty()) {
        try {
            min_salary = std::stod(salary_input);
            if (min_salary < 0) {
                min_salary = 0.0;
            }
        } catch (...) {
            std::cout << "Invalid salary. Using 0.\n";
            min_salary = 0.0;
        }
    }

    std::cout << "\nSearching jobs...\n";

    std::vector<Job> jobs = client.searchJobs(query, location, min_salary);
    std::string remote_only;

    std::cout << "Show remote jobs only? (y/n): ";
    std::getline(std::cin, remote_only);

if (remote_only == "y" || remote_only == "Y") {
    jobs = JobParser::filterRemoteJobs(jobs);
}

    std::cout << "\n=== SEARCH RESULTS ===\n";

    for (const auto& job : jobs) {
        displayJob(job);
    }

    displayStatistics(jobs);

    return 0;
}
