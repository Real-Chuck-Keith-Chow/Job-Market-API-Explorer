#include <iostream>
#include <iomanip>
#include <fstream>
#include "ApiClient.h"
#include "third_party/json.hpp"

using json = nlohmann::json;

void displayJob(const Job& job) {
    std::cout << "Title: " << job.title << std::endl;
    std::cout << "Company: " << job.company.display_name << std::endl;
    std::cout << "Location: " << job.location.display_name << std::endl;
    
    if (job.salary_min > 0 || job.salary_max > 0) {
        std::cout << "Salary: $" << std::fixed << std::setprecision(0) 
                  << job.salary_min << " - $" << job.salary_max << std::endl;
    }
    
    std::cout << "URL: " << job.redirect_url << std::endl;
    std::cout << "Posted: " << job.created << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void displayStatistics(const std::vector<Job>& jobs) {
    if (jobs.empty()) {
        std::cout << "No jobs found." << std::endl;
        return;
    }
    
    std::cout << "\n=== JOB MARKET STATISTICS ===" << std::endl;
    std::cout << "Total jobs found: " << jobs.size() << std::endl;
    
    // Count jobs by company
    std::map<std::string, int> company_counts;
    double total_salary = 0;
    int salary_count = 0;
    
    for (const auto& job : jobs) {
        company_counts[job.company.display_name]++;
        
        if (job.salary_min > 0) {
            total_salary += job.salary_min;
            salary_count++;
        }
    }
    
    // Display top companies
    std::cout << "\nTop companies hiring:" << std::endl;
    int count = 0;
    for (const auto& [company, job_count] : company_counts) {
        if (count++ >= 5) break;
        std::cout << "  " << company << ": " << job_count << " jobs" << std::endl;
    }
    
    // Display average salary
    if (salary_count > 0) {
        double avg_salary = total_salary / salary_count;
        std::cout << "Average minimum salary: $" << std::fixed << std::setprecision(0) 
                  << avg_salary << std::endl;
    }
}

int main() {
    // Load configuration
    std::ifstream config_file("config.json");
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config.json" << std::endl;
        std::cerr << "Please create config.json with your API keys" << std::endl;
        return 1;
    }
    
    json config;
    config_file >> config;
    
    // Initialize API client
    ApiClient client(config["adzuna_app_id"], config["adzuna_app_key"]);
    
    // Interactive search
    std::string query, location;
    double min_salary = 0;
    
    std::cout << "=== JOB MARKET API EXPLORER ===" << std::endl;
    std::cout << "Enter search query (or press enter for all jobs): ";
    std::getline(std::cin, query);
    
    std::cout << "Enter location (or press enter for all locations): ";
    std::getline(std::cin, location);
    
    // In main.cpp - Replace the current salary input with this:

std::cout << "Enter minimum salary (0 for no filter): ";
std::string salary_input;
std::getline(std::cin, salary_input);

// Add input validation
if (!salary_input.empty()) {
    try {
        min_salary = std::stod(salary_input);
        if (min_salary < 0) {
            std::cout << "Warning: Negative salary converted to 0" << std::endl;
            min_salary = 0;
        }
        if (min_salary > 1000000) { // Reasonable salary cap
            std::cout << "Warning: Salary capped at 1,000,000" << std::endl;
            min_salary = 1000000;
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid salary input '" << salary_input << "'. Using 0." << std::endl;
        min_salary = 0;
    }
} else {
    min_salary = 0;
}
    }
    
    std::cout << "\nSearching for jobs..." << std::endl;
    
    // Perform search
    auto jobs = client.searchJobs(query, location, min_salary);
    
    // Display results
    std::cout << "\n=== SEARCH RESULTS ===" << std::endl;
    for (const auto& job : jobs) {
        displayJob(job);
    }
    
    // Display statistics
    displayStatistics(jobs);
    
    return 0;
}
