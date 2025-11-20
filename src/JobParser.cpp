#include "JobParser.h"
#include <algorithm>
#include <regex>
#include <map>
#include <set>
#include <cmath>  // ADD THIS for sqrt() and abs()

std::vector<std::string> JobParser::extractTechnologies(const std::string& description) {
    std::vector<std::string> technologies;
    std::string desc_lower = description;
    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);
    
    // Common programming languages and technologies
    std::vector<std::string> common_techs = {
        "c++", "cpp", "python", "java", "javascript", "typescript",
        "react", "angular", "vue", "node.js", "express", "django",
        "flask", "spring", "ruby", "rails", "php", "laravel",
        "sql", "mysql", "postgresql", "mongodb", "redis",
        "docker", "kubernetes", "aws", "azure", "gcp", "cloud",
        "git", "jenkins", "ci/cd", "terraform", "ansible",
        "machine learning", "ml", "ai", "data science",
        "html", "css", "sass", "less", "webpack", "babel",
        "rust", "go", "golang", "swift", "kotlin", "scala"
    };
    
    for (const auto& tech : common_techs) {
        if (desc_lower.find(tech) != std::string::npos) {
            // Handle special cases
            if (tech == "cpp" || tech == "c++") {
                technologies.push_back("C++");
            } else if (tech == "node.js") {
                technologies.push_back("Node.js");
            } else if (tech == "ci/cd") {
                technologies.push_back("CI/CD");
            } else if (tech == "ml") {
                technologies.push_back("Machine Learning");
            } else if (tech == "ai") {
                technologies.push_back("Artificial Intelligence");
            } else if (tech == "gcp") {
                technologies.push_back("Google Cloud");
            } else {
                // Capitalize first letter
                std::string formatted_tech = tech;
                formatted_tech[0] = std::toupper(formatted_tech[0]);
                technologies.push_back(formatted_tech);
            }
        }
    }
    
    // Remove duplicates
    std::sort(technologies.begin(), technologies.end());
    technologies.erase(std::unique(technologies.begin(), technologies.end()), technologies.end());
    
    return technologies;
}

std::string JobParser::categorizeJob(const Job& job) {
    std::string desc_lower = job.description;
    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);
    
    // Check for specific technology patterns
    auto technologies = extractTechnologies(job.description);
    
    // Categorize based on technologies found
    if (std::find(technologies.begin(), technologies.end(), "React") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Angular") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Vue") != technologies.end()) {
        return "Frontend Development";
    }
    
    if (std::find(technologies.begin(), technologies.end(), "Node.js") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Django") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Spring") != technologies.end()) {
        return "Backend Development";
    }
    
    if (std::find(technologies.begin(), technologies.end(), "Machine Learning") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Data Science") != technologies.end()) {
        return "Data Science";
    }
    
    if (std::find(technologies.begin(), technologies.end(), "AWS") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Azure") != technologies.end() ||
        std::find(technologies.begin(), technologies.end(), "Docker") != technologies.end()) {
        return "DevOps/Cloud";
    }
    
    if (std::find(technologies.begin(), technologies.end(), "C++") != technologies.end()) {
        return "C++ Development";
    }
    
    if (std::find(technologies.begin(), technologies.end(), "Python") != technologies.end()) {
        return "Python Development";
    }
    
    if (std::find(technologies.begin(), technologies.end(), "Java") != technologies.end()) {
        return "Java Development";
    }
    
    // Fallback based on title
    std::string title_lower = job.title;
    std::transform(title_lower.begin(), title_lower.end(), title_lower.begin(), ::tolower);
    
    if (title_lower.find("frontend") != std::string::npos) return "Frontend Development";
    if (title_lower.find("backend") != std::string::npos) return "Backend Development";
    if (title_lower.find("fullstack") != std::string::npos) return "Full Stack Development";
    if (title_lower.find("devops") != std::string::npos) return "DevOps/Cloud";
    if (title_lower.find("data") != std::string::npos) return "Data Science";
    if (title_lower.find("machine learning") != std::string::npos) return "Machine Learning";
    
    return "Software Development";
}

bool JobParser::parseSalary(const std::string& salary_str, double& min_salary, double& max_salary) {
    if (salary_str.empty()) return false;
    
    std::regex salary_pattern(R"((\d+(?:,\d+)*(?:\.\d+)?)\s*-\s*\$?\s*(\d+(?:,\d+)*(?:\.\d+)?))", std::regex::icase);
    std::regex single_salary_pattern(R"(\$?\s*(\d+(?:,\d+)*(?:\.\d+)?))", std::regex::icase);
    
    std::smatch matches;
    
    // Try range format: "50000 - 80000" or "$50,000 - $80,000"
    if (std::regex_search(salary_str, matches, salary_pattern) && matches.size() == 3) {
        std::string min_str = matches[1];
        std::string max_str = matches[2];
        
        // Remove commas
        min_str.erase(std::remove(min_str.begin(), min_str.end(), ','), min_str.end());
        max_str.erase(std::remove(max_str.begin(), max_str.end(), ','), max_str.end());
        
        try {
            min_salary = std::stod(min_str);
            max_salary = std::stod(max_str);
            return true;
        } catch (const std::exception& e) {
            return false;
        }
    }
    // Try single salary format: "$75000" or "75,000"
    else if (std::regex_search(salary_str, matches, single_salary_pattern) && matches.size() >= 1) {
        std::string salary_str_clean = matches[1];
        salary_str_clean.erase(std::remove(salary_str_clean.begin(), salary_str_clean.end(), ','), salary_str_clean.end());
        
        try {
            double salary = std::stod(salary_str_clean);
            min_salary = salary;
            max_salary = salary;
            return true;
        } catch (const std::exception& e) {
            return false;
        }
    }
    
    return false;
}

std::vector<Job> JobParser::filterByTechnology(const std::vector<Job>& jobs, const std::string& technology) {
    std::vector<Job> filtered_jobs;
    std::string tech_lower = technology;
    std::transform(tech_lower.begin(), tech_lower.end(), tech_lower.begin(), ::tolower);
    
    for (const auto& job : jobs) {
        auto technologies = extractTechnologies(job.description);
        std::string title_lower = job.title;
        std::transform(title_lower.begin(), title_lower.end(), title_lower.begin(), ::tolower);
        
        // Check if technology is in description or title
        bool found_in_desc = false;
        for (const auto& tech : technologies) {
            std::string tech_compare = tech;
            std::transform(tech_compare.begin(), tech_compare.end(), tech_compare.begin(), ::tolower);
            if (tech_compare.find(tech_lower) != std::string::npos) {
                found_in_desc = true;
                break;
            }
        }
        
        bool found_in_title = title_lower.find(tech_lower) != std::string::npos;
        
        if (found_in_desc || found_in_title) {
            filtered_jobs.push_back(job);
        }
    }
    
    return filtered_jobs;
}

std::map<std::string, int> JobParser::analyzeTechnologyTrends(const std::vector<Job>& jobs) {
    std::map<std::string, int> tech_counts;
    
    for (const auto& job : jobs) {
        auto technologies = extractTechnologies(job.description);
        for (const auto& tech : technologies) {
            tech_counts[tech]++;
        }
    }
    
    return tech_counts;
}

std::string JobParser::normalizeCompanyName(const std::string& company_name) {
    if (company_name.empty()) return "";
    
    std::string normalized = company_name;
    
    // Remove common suffixes and clean up
    std::vector<std::string> suffixes = {"Inc.", "Inc", "LLC", "Ltd.", "Corp.", "Corporation", "Company"};
    for (const auto& suffix : suffixes) {
        size_t pos = normalized.find(suffix);
        if (pos != std::string::npos) {
            normalized = normalized.substr(0, pos);
        }
    }
    
    // Trim whitespace
    normalized.erase(0, normalized.find_first_not_of(" \t\n\r\f\v"));
    normalized.erase(normalized.find_last_not_of(" \t\n\r\f\v") + 1);
    
    return normalized;
}

Location JobParser::parseLocation(const std::string& location_str) {
    Location location;
    location.display_name = location_str;
    
    if (location_str.empty()) return location;
    
    // Simple parsing - look for city, state/country patterns
    std::regex city_state_pattern(R"(([^,]+),\s*([^,]+))");
    std::smatch matches;
    
    if (std::regex_search(location_str, matches, city_state_pattern) && matches.size() == 3) {
        location.display_name = matches[1];
        location.area = matches[2];
        
        // Simple country detection
        if (location.area.find("CA") != std::string::npos || 
            location.area.find("Canada") != std::string::npos) {
            location.country = "Canada";
        } else if (location.area.find("US") != std::string::npos ||
                  location.area.find("USA") != std::string::npos ||
                  location.area.find("United States") != std::string::npos) {
            location.country = "United States";
        } else if (location.area.find("UK") != std::string::npos ||
                  location.area.find("United Kingdom") != std::string::npos) {
            location.country = "United Kingdom";
        }
    } else {
        location.display_name = location_str;
    }
    
    return location;
}

// Add this function to JobParser.cpp
bool JobParser::validateSalaryRange(double min_salary, double max_salary) {
    // Check for valid salary range
    if (min_salary < 0 || max_salary < 0) {
        return false;
    }
    
    // Max salary should be greater than or equal to min salary
    if (max_salary > 0 && min_salary > max_salary) {
        return false;
    }
    
    // Reasonable salary limits (adjust based on currency/region)
    const double MAX_REASONABLE_SALARY = 1000000.0; // $1 million
    if (min_salary > MAX_REASONABLE_SALARY || max_salary > MAX_REASONABLE_SALARY) {
        return false;
    }
    
    return true;
}

// Add this function to normalize salary ranges
void JobParser::normalizeSalaryRange(double& min_salary, double& max_salary) {
    // If both are 0, nothing to normalize
    if (min_salary == 0 && max_salary == 0) {
        return;
    }
    
    // If min is 0 but max has value, set min to reasonable percentage of max
    if (min_salary == 0 && max_salary > 0) {
        min_salary = max_salary * 0.7; // Assume min is 70% of max
    }
    
    // If max is 0 but min has value, set max to reasonable multiple of min
    if (max_salary == 0 && min_salary > 0) {
        max_salary = min_salary * 1.5; // Assume max is 150% of min
    }
    
    // Ensure min <= max
    if (min_salary > max_salary) {
        std::swap(min_salary, max_salary);
    }
}

// Add this function to detect salary outliers
bool JobParser::isSalaryOutlier(double salary, const std::vector<Job>& jobs) {
    if (salary <= 0) return false;
    
    // Calculate average salary from jobs that have salaries
    double total_salary = 0.0;
    int count = 0;
    
    for (const auto& job : jobs) {
        double job_avg = (job.salary_min + job.salary_max) / 2.0;
        if (job_avg > 0) {
            total_salary += job_avg;
            count++;
        }
    }
    
    if (count == 0) return false;
    
    double average_salary = total_salary / count;
    double standard_deviation = 0.0;
    
    // Calculate standard deviation
    for (const auto& job : jobs) {
        double job_avg = (job.salary_min + job.salary_max) / 2.0;
        if (job_avg > 0) {
            double diff = job_avg - average_salary;
            standard_deviation += diff * diff;
        }
    }
    
    standard_deviation = std::sqrt(standard_deviation / count);
    
    // Consider outlier if more than 3 standard deviations from mean
    return std::abs(salary - average_salary) > (3 * standard_deviation);
}

// Calculate job quality score based on completeness and data quality
double JobParser::calculateJobQualityScore(const Job& job) {
    double score = 0.0;
    int criteria_met = 0;
    int total_criteria = 8;
    
    // Check title completeness
    if (!job.title.empty() && job.title.length() > 5) {
        score += 15.0;
        criteria_met++;
    }
    
    // Check company information
    if (!job.company.display_name.empty()) {
        score += 15.0;
        criteria_met++;
    }
    
    // Check location information
    if (!job.location.display_name.empty()) {
        score += 15.0;
        criteria_met++;
    }
    
    // Check salary information
    if (job.salary_min > 0 || job.salary_max > 0) {
        score += 20.0;
        criteria_met++;
        
        // Bonus for complete salary range
        if (job.salary_min > 0 && job.salary_max > 0 && validateSalaryRange(job.salary_min, job.salary_max)) {
            score += 10.0;
        }
    }
    
    // Check job description
    if (!job.description.empty() && job.description.length() > 50) {
        score += 15.0;
        criteria_met++;
    }
    
    // Check technologies detected
    auto technologies = extractTechnologies(job.description);
    if (!technologies.empty()) {
        score += 10.0;
        criteria_met++;
    }
    
    // Check redirect URL
    if (!job.redirect_url.empty()) {
        score += 5.0;
        criteria_met++;
    }
    
    // Check creation date
    if (!job.created.empty()) {
        score += 5.0;
        criteria_met++;
    }
    
    // Normalize score to 0-100 range
    if (criteria_met > 0) {
        score = (score / 100.0) * 100.0;
    }
    
    return std::min(score, 100.0);
}

// Detect experience level from job title and description
std::string JobParser::detectExperienceLevel(const Job& job) {
    std::string title_lower = job.title;
    std::string desc_lower = job.description;
    std::transform(title_lower.begin(), title_lower.end(), title_lower.begin(), ::tolower);
    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);
    
    // Keywords for different experience levels
    std::vector<std::string> intern_keywords = {"intern", "internship", "student", "graduate", "entry level", "junior"};
    std::vector<std::string> senior_keywords = {"senior", "sr.", "lead", "principal", "architect", "staff", "experienced"};
    std::vector<std::string> manager_keywords = {"manager", "director", "head of", "vp", "vice president", "cto", "ceo"};
    
    // Check for manager level
    for (const auto& keyword : manager_keywords) {
        if (title_lower.find(keyword) != std::string::npos || desc_lower.find(keyword) != std::string::npos) {
            return "Management";
        }
    }
    
    // Check for senior level
    for (const auto& keyword : senior_keywords) {
        if (title_lower.find(keyword) != std::string::npos || desc_lower.find(keyword) != std::string::npos) {
            return "Senior";
        }
    }
    
    // Check for intern level
    for (const auto& keyword : intern_keywords) {
        if (title_lower.find(keyword) != std::string::npos || desc_lower.find(keyword) != std::string::npos) {
            return "Entry Level";
        }
    }
    
    // Default to mid-level
    return "Mid Level";
}

// Fix: Add missing implementation for technology aliases enhancement
std::vector<std::string> JobParser::extractTechnologiesWithAliases(const std::string& description) {
    auto technologies = extractTechnologies(description);
    
    // Add additional technology aliases and variations
    std::string desc_lower = description;
    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);
    
    // Enhanced technology detection with more aliases
    std::vector<std::pair<std::string, std::string>> enhanced_techs = {
        {"next.js", "Next.js"}, {"nextjs", "Next.js"},
        {"vue.js", "Vue.js"}, {"vuejs", "Vue.js"},
        {"angular.js", "AngularJS"}, {"angularjs", "AngularJS"},
        {"express.js", "Express.js"}, {"expressjs", "Express.js"},
        {"nest.js", "Nest.js"}, {"nestjs", "Nest.js"},
        {"graphql", "GraphQL"}, {"gql", "GraphQL"},
        {"rest api", "REST API"}, {"restful", "REST API"},
        {"microservices", "Microservices"}, {"microservice", "Microservices"},
        {"serverless", "Serverless"}, {"lambda", "AWS Lambda"},
        {"kubernetes", "Kubernetes"}, {"k8s", "Kubernetes"},
        {"terraform", "Terraform"}, {"tf", "Terraform"},
        {"ansible", "Ansible"}, {"puppet", "Puppet"}, {"chef", "Chef"}
    };
    
    for (const auto& [alias, tech_name] : enhanced_techs) {
        if (desc_lower.find(alias) != std::string::npos) {
            // Check if already added
            if (std::find(technologies.begin(), technologies.end(), tech_name) == technologies.end()) {
                technologies.push_back(tech_name);
            }
        }
    }
    
    // Remove duplicates
    std::sort(technologies.begin(), technologies.end());
    technologies.erase(std::unique(technologies.begin(), technologies.end()), technologies.end());
    
    return technologies;
}
