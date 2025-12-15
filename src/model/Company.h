#ifndef COMPANY_H
#define COMPANY_H

#include <string>
#include <vector>
#include <map>

struct Company {
    std::string id;
    std::string display_name;
    std::string normalized_name;
    std::string website;
    std::string description;
    std::string industry;
    std::string size; // e.g., "1-10", "11-50", "51-200", "201-500", "501-1000", "1000+"
    std::string founded_year;
    std::string headquarters;
    
    // Company metrics
    int total_job_count;
    int active_job_count;
    double average_salary;
    double min_salary;
    double max_salary;
    
    // Technology focus
    std::vector<std::string> primary_technologies;
    std::map<std::string, int> technology_counts;
    
    // Location data
    std::vector<std::string> locations;
    std::map<std::string, int> location_counts;
    
    // Temporal data
    std::string first_seen;
    std::string last_seen;
    std::string last_updated;
    
    // Ratings and reviews (if available)
    double rating;
    int review_count;
    
    // Social media and links
    std::string linkedin_url;
    std::string github_url;
    std::string crunchbase_url;
    
    Company() 
        : total_job_count(0), 
          active_job_count(0), 
          average_salary(0.0), 
          min_salary(0.0), 
          max_salary(0.0),
          rating(0.0),
          review_count(0) {}
    
    // Utility methods
    void addTechnology(const std::string& technology) {
        auto it = std::find(primary_technologies.begin(), primary_technologies.end(), technology);
        if (it == primary_technologies.end()) {
            primary_technologies.push_back(technology);
        }
        technology_counts[technology]++;
    }
    
    void addLocation(const std::string& location) {
        auto it = std::find(locations.begin(), locations.end(), location);
        if (it == locations.end()) {
            locations.push_back(location);
        }
        location_counts[location]++;
    }
    
    void updateSalaryStats(double salary) {
        if (salary > 0) {
            if (min_salary == 0 || salary < min_salary) {
                min_salary = salary;
            }
            if (salary > max_salary) {
                max_salary = salary;
            }
            
            double total_salary_sum = average_salary * total_job_count + salary;
            total_job_count++;
            average_salary = total_salary_sum / total_job_count;
        }
    }
    
    std::string getPrimaryLocation() const {
        if (location_counts.empty()) return "";
        auto max_it = std::max_element(
            location_counts.begin(), 
            location_counts.end(),
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                return a.second < b.second;
            }
        );
        return max_it->first;
    }
    
    std::vector<std::string> getTopTechnologies(int count = 5) const {
        std::vector<std::pair<std::string, int>> tech_pairs(technology_counts.begin(), technology_counts.end());
        std::sort(tech_pairs.begin(), tech_pairs.end(),
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                return a.second > b.second;
            });
        std::vector<std::string> top_techs;
        for (int i = 0; i < std::min(count, static_cast<int>(tech_pairs.size())); i++) {
            top_techs.push_back(tech_pairs[i].first);
        }
        return top_techs;
    }
    
    bool isTechCompany() const {
        std::vector<std::string> tech_indicators = {
            "software", "technology", "tech", "IT", "computer", "data", "cloud",
            "ai", "machine learning", "developer", "engineering", "platform"
        };
        std::string name_lower = normalized_name;
        std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
        std::string industry_lower = industry;
        std::transform(industry_lower.begin(), industry_lower.end(), industry_lower.begin(), ::tolower);
        for (const auto& indicator : tech_indicators) {
            if (name_lower.find(indicator) != std::string::npos ||
                industry_lower.find(indicator) != std::string::npos) {
                return true;
            }
        }
        return !primary_technologies.empty();
    }
    
    std::string getSizeCategory() const {
        if (size.empty()) {
            if (total_job_count >= 1000) return "Large (1000+)";
            else if (total_job_count >= 500) return "Large (500-999)";
            else if (total_job_count >= 200) return "Medium (200-499)";
            else if (total_job_count >= 50) return "Small (50-199)";
            else if (total_job_count >= 10) return "Small (10-49)";
            else return "Startup (1-9)";
        }
        return size;
    }
};

// Company comparison functions
struct CompanyCompare {
    static bool byJobCount(const Company& a, const Company& b) {
        return a.total_job_count > b.total_job_count;
    }
    static bool byAverageSalary(const Company& a, const Company& b) {
        return a.average_salary > b.average_salary;
    }
    static bool byTechnologyDiversity(const Company& a, const Company& b) {
        return a.primary_technologies.size() > b.primary_technologies.size();
    }
};

// Company analytics structure
struct CompanyAnalytics {
    int total_companies;
    int tech_companies;
    std::map<std::string, int> companies_by_industry;
    std::map<std::string, int> companies_by_size;
    std::map<std::string, int> companies_by_location;
    double overall_average_salary;
    
    CompanyAnalytics() : total_companies(0), tech_companies(0), overall_average_salary(0.0) {}
};

#endif
