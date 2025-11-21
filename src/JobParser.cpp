#include "JobParser.h"
#include <algorithm>
#include <regex>
#include <map>
#include <set>
#include <cmath>

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
    std::vector<std::string> suffixes = {
        "Inc.", "Inc", "LLC", "Ltd.", "Ltd", "Corp.", "Corp", "Corporation", 
        "Company", "Co.", "Co", "Limited", "The ", "®", "™"
    };
    
    for (const auto& suffix : suffixes) {
        size_t pos = normalized.find(suffix);
        if (pos != std::string::npos) {
            normalized = normalized.substr(0, pos);
        }
    }
    
    // Remove extra whitespace and punctuation
    normalized.erase(0, normalized.find_first_not_of(" \t\n\r\f\v,"));
    normalized.erase(normalized.find_last_not_of(" \t\n\r\f\v,") + 1);
    
    // Remove multiple spaces
    std::regex multiple_spaces("\\s+");
    normalized = std::regex_replace(normalized, multiple_spaces, " ");
    
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

// New ranking and similarity functions
std::vector<Job> JobParser::rankJobsByRelevance(const std::vector<Job>& jobs, 
                                               const std::string& user_skills,
                                               const std::string& preferred_location,
                                               double desired_salary,
                                               const std::vector<std::string>& preferred_technologies) {
    std::vector<std::pair<Job, double>> scored_jobs;
    
    for (const auto& job : jobs) {
        double score = 0.0;
        
        // 1. Technology match score (40% weight)
        double tech_score = calculateTechnologyMatchScore(job, user_skills, preferred_technologies);
        score += tech_score * 0.4;
        
        // 2. Location match score (25% weight)
        double location_score = calculateLocationMatchScore(job, preferred_location);
        score += location_score * 0.25;
        
        // 3. Salary match score (20% weight)
        double salary_score = calculateSalaryMatchScore(job, desired_salary);
        score += salary_score * 0.2;
        
        // 4. Job quality score (15% weight)
        double quality_score = calculateJobQualityScore(job) / 100.0; // Normalize to 0-1
        score += quality_score * 0.15;
        
        scored_jobs.push_back({job, score});
    }
    
    // Sort jobs by relevance score (descending)
    std::sort(scored_jobs.begin(), scored_jobs.end(),
              [](const std::pair<Job, double>& a, const std::pair<Job, double>& b) {
                  return a.second > b.second;
              });
    
    // Extract sorted jobs
    std::vector<Job> ranked_jobs;
    for (const auto& scored_job : scored_jobs) {
        ranked_jobs.push_back(scored_job.first);
    }
    
    return ranked_jobs;
}

std::vector<Job> JobParser::findSimilarJobs(const Job& reference_job, 
                                           const std::vector<Job>& all_jobs,
                                           int max_results) {
    std::vector<std::pair<Job, double>> similarity_scores;
    
    auto ref_technologies = extractTechnologies(reference_job.description);
    std::string ref_category = categorizeJob(reference_job);
    
    for (const auto& job : all_jobs) {
        if (job.id == reference_job.id) continue; // Skip the reference job itself
        
        double similarity = 0.0;
        
        // 1. Category similarity (30%)
        if (categorizeJob(job) == ref_category) {
            similarity += 0.3;
        }
        
        // 2. Technology overlap (40%)
        auto job_technologies = extractTechnologies(job.description);
        std::vector<std::string> common_techs;
        std::set_intersection(ref_technologies.begin(), ref_technologies.end(),
                             job_technologies.begin(), job_technologies.end(),
                             std::back_inserter(common_techs));
        
        double tech_similarity = static_cast<double>(common_techs.size()) / 
                                std::max(ref_technologies.size(), job_technologies.size());
        similarity += tech_similarity * 0.4;
        
        // 3. Salary range similarity (20%)
        double ref_avg = (reference_job.salary_min + reference_job.salary_max) / 2.0;
        double job_avg = (job.salary_min + job.salary_max) / 2.0;
        
        if (ref_avg > 0 && job_avg > 0) {
            double salary_similarity = 1.0 - std::abs(ref_avg - job_avg) / std::max(ref_avg, job_avg);
            similarity += salary_similarity * 0.2;
        } else {
            similarity += 0.1; // Small bonus if one has no salary info
        }
        
        // 4. Company size similarity (10%)
        if (reference_job.company.display_name == job.company.display_name) {
            similarity += 0.1;
        }
        
        similarity_scores.push_back({job, similarity});
    }
    
    // Sort by similarity score (descending)
    std::sort(similarity_scores.begin(), similarity_scores.end(),
              [](const std::pair<Job, double>& a, const std::pair<Job, double>& b) {
                  return a.second > b.second;
              });
    
    // Return top matches
    std::vector<Job> similar_jobs;
    for (int i = 0; i < std::min(max_results, static_cast<int>(similarity_scores.size())); i++) {
        similar_jobs.push_back(similarity_scores[i].first);
    }
    
    return similar_jobs;
}

// Private helper functions
double JobParser::calculateTechnologyMatchScore(const Job& job, 
                                               const std::string& user_skills,
                                               const std::vector<std::string>& preferred_technologies) {
    double score = 0.0;
    
    // Extract technologies from job description
    auto job_technologies = extractTechnologies(job.description);
    if (job_technologies.empty()) return 0.0;
    
    // Convert user skills to lowercase for matching
    std::string user_skills_lower = user_skills;
    std::transform(user_skills_lower.begin(), user_skills_lower.end(), user_skills_lower.begin(), ::tolower);
    
    int matches = 0;
    int total_technologies = job_technologies.size();
    
    for (const auto& tech : job_technologies) {
        std::string tech_lower = tech;
        std::transform(tech_lower.begin(), tech_lower.end(), tech_lower.begin(), ::tolower);
        
        // Check if technology matches user skills
        if (user_skills_lower.find(tech_lower) != std::string::npos) {
            matches++;
            score += 1.0;
        }
        
        // Bonus for preferred technologies
        if (std::find(preferred_technologies.begin(), preferred_technologies.end(), tech) != preferred_technologies.end()) {
            score += 0.5;
        }
    }
    
    // Calculate match ratio
    double match_ratio = static_cast<double>(matches) / total_technologies;
    score += match_ratio * 2.0;
    
    return std::min(score, 10.0); // Cap at 10 points
}

double JobParser::calculateLocationMatchScore(const Job& job, const std::string& preferred_location) {
    if (preferred_location.empty()) return 5.0; // Neutral score if no preference
    
    std::string job_location_lower = job.location.display_name;
    std::string preferred_location_lower = preferred_location;
    std::transform(job_location_lower.begin(), job_location_lower.end(), job_location_lower.begin(), ::tolower);
    std::transform(preferred_location_lower.begin(), preferred_location_lower.end(), preferred_location_lower.begin(), ::tolower);
    
    // Exact match
    if (job_location_lower == preferred_location_lower) {
        return 10.0;
    }
    
    // Partial match (contains)
    if (job_location_lower.find(preferred_location_lower) != std::string::npos ||
        preferred_location_lower.find(job_location_lower) != std::string::npos) {
        return 7.0;
    }
    
    // Remote work bonus
    if (job_location_lower.find("remote") != std::string::npos) {
        return 8.0;
    }
    
    // Same country/state matching
    if (!job.location.country.empty() && preferred_location_lower.find(job.location.country) != std::string::npos) {
        return 6.0;
    }
    
    return 3.0; // Low score for no match
}

double JobParser::calculateSalaryMatchScore(const Job& job, double desired_salary) {
    if (desired_salary <= 0) return 5.0; // Neutral if no salary preference
    
    double job_avg_salary = (job.salary_min + job.salary_max) / 2.0;
    
    if (job_avg_salary <= 0) return 3.0; // Low score if no salary info
    
    double salary_ratio = job_avg_salary / desired_salary;
    
    if (salary_ratio >= 1.2) return 10.0; // 20%+ above desired
    if (salary_ratio >= 1.0) return 8.0;  // Meets or exceeds desired
    if (salary_ratio >= 0.8) return 6.0;  // Within 20% below desired
    if (salary_ratio >= 0.6) return 4.0;  // Within 40% below desired
    
    return 2.0; // More than 40% below desired
}
