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


    // Add this function to JobParser.cpp - Comprehensive market analytics
JobMarketAnalytics JobParser::generateMarketAnalytics(const std::vector<Job>& jobs, int days_back) {
    JobMarketAnalytics analytics;
    
    if (jobs.empty()) {
        return analytics;
    }
    
    // Basic counts
    analytics.total_jobs = jobs.size();
    analytics.unique_companies = getUniqueCompanies(jobs);
    analytics.unique_locations = getUniqueLocations(jobs);
    
    // Salary analytics
    calculateSalaryAnalytics(analytics, jobs);
    
    // Technology trends
    analytics.technology_trends = analyzeTechnologyTrends(jobs);
    analytics.hot_technologies = getHotTechnologies(analytics.technology_trends, 10);
    
    // Job type distribution
    analytics.job_type_distribution = getJobTypeDistribution(jobs);
    
    // Experience level distribution
    analytics.experience_distribution = getExperienceDistribution(jobs);
    
    // Company analytics
    analytics.top_companies = getTopCompaniesByJobCount(jobs, 10);
    analytics.company_size_distribution = getCompanySizeDistribution(jobs);
    
    // Location analytics
    analytics.location_distribution = getLocationDistribution(jobs);
    analytics.remote_ratio = calculateRemoteRatio(jobs);
    
    // Temporal analytics
    analytics.daily_trends = getDailyJobTrends(jobs, days_back);
    analytics.growth_rate = calculateGrowthRate(analytics.daily_trends);
    
    // Quality metrics
    analytics.average_quality_score = calculateAverageQualityScore(jobs);
    analytics.high_quality_jobs = countHighQualityJobs(jobs);
    
    // Market health score
    analytics.market_health_score = calculateMarketHealthScore(analytics);
    
    return analytics;
}

// Helper function to get unique companies
std::set<std::string> JobParser::getUniqueCompanies(const std::vector<Job>& jobs) {
    std::set<std::string> companies;
    for (const auto& job : jobs) {
        if (!job.company.display_name.empty()) {
            companies.insert(normalizeCompanyName(job.company.display_name));
        }
    }
    return companies;
}

// Helper function to get unique locations
std::set<std::string> JobParser::getUniqueLocations(const std::vector<Job>& jobs) {
    std::set<std::string> locations;
    for (const auto& job : jobs) {
        if (!job.location.display_name.empty()) {
            locations.insert(job.location.display_name);
        }
    }
    return locations;
}

// Calculate comprehensive salary analytics
void JobParser::calculateSalaryAnalytics(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    double total_min = 0.0, total_max = 0.0;
    int salary_count = 0;
    std::vector<double> all_salaries;
    
    for (const auto& job : jobs) {
        if (job.salary_min > 0 && job.salary_max > 0) {
            double avg_salary = (job.salary_min + job.salary_max) / 2.0;
            total_min += job.salary_min;
            total_max += job.salary_max;
            all_salaries.push_back(avg_salary);
            salary_count++;
        }
    }
    
    if (salary_count > 0) {
        analytics.average_salary_min = total_min / salary_count;
        analytics.average_salary_max = total_max / salary_count;
        analytics.average_salary = (analytics.average_salary_min + analytics.average_salary_max) / 2.0;
        
        // Calculate salary percentiles
        std::sort(all_salaries.begin(), all_salaries.end());
        analytics.salary_25th_percentile = calculatePercentile(all_salaries, 0.25);
        analytics.salary_50th_percentile = calculatePercentile(all_salaries, 0.5);
        analytics.salary_75th_percentile = calculatePercentile(all_salaries, 0.75);
        analytics.salary_90th_percentile = calculatePercentile(all_salaries, 0.9);
        
        analytics.jobs_with_salary = salary_count;
        analytics.salary_reporting_rate = static_cast<double>(salary_count) / jobs.size() * 100.0;
    }
}

// Get top technologies by count
std::vector<std::pair<std::string, int>> JobParser::getHotTechnologies(const std::map<std::string, int>& tech_trends, int top_n) {
    std::vector<std::pair<std::string, int>> sorted_techs(tech_trends.begin(), tech_trends.end());
    
    std::sort(sorted_techs.begin(), sorted_techs.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    if (sorted_techs.size() > top_n) {
        sorted_techs.resize(top_n);
    }
    
    return sorted_techs;
}

// Get job type distribution
std::map<std::string, int> JobParser::getJobTypeDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> distribution;
    for (const auto& job : jobs) {
        std::string category = categorizeJob(job);
        distribution[category]++;
    }
    return distribution;
}

// Get experience level distribution
std::map<std::string, int> JobParser::getExperienceDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> distribution;
    for (const auto& job : jobs) {
        std::string level = detectExperienceLevel(job);
        distribution[level]++;
    }
    return distribution;
}

// Get top companies by job count
std::vector<std::pair<std::string, int>> JobParser::getTopCompaniesByJobCount(const std::vector<Job>& jobs, int top_n) {
    std::map<std::string, int> company_counts;
    for (const auto& job : jobs) {
        if (!job.company.display_name.empty()) {
            std::string normalized_name = normalizeCompanyName(job.company.display_name);
            company_counts[normalized_name]++;
        }
    }
    
    std::vector<std::pair<std::string, int>> sorted_companies(company_counts.begin(), company_counts.end());
    std::sort(sorted_companies.begin(), sorted_companies.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });
    
    if (sorted_companies.size() > top_n) {
        sorted_companies.resize(top_n);
    }
    
    return sorted_companies;
}

// Get company size distribution (estimated)
std::map<std::string, int> JobParser::getCompanySizeDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> size_distribution = {
        {"Startup (1-9)", 0},
        {"Small (10-49)", 0},
        {"Medium (50-199)", 0},
        {"Large (200-499)", 0},
        {"Enterprise (500+)", 0}
    };
    
    std::map<std::string, int> company_job_counts;
    for (const auto& job : jobs) {
        if (!job.company.display_name.empty()) {
            std::string normalized_name = normalizeCompanyName(job.company.display_name);
            company_job_counts[normalized_name]++;
        }
    }
    
    for (const auto& [company, count] : company_job_counts) {
        std::string size_category;
        if (count >= 500) size_category = "Enterprise (500+)";
        else if (count >= 200) size_category = "Large (200-499)";
        else if (count >= 50) size_category = "Medium (50-199)";
        else if (count >= 10) size_category = "Small (10-49)";
        else size_category = "Startup (1-9)";
        
        size_distribution[size_category]++;
    }
    
    return size_distribution;
}

// Get location distribution
std::map<std::string, int> JobParser::getLocationDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> distribution;
    for (const auto& job : jobs) {
        std::string location = job.location.display_name.empty() ? "Unknown" : job.location.display_name;
        distribution[location]++;
    }
    return distribution;
}

// Calculate remote work ratio
double JobParser::calculateRemoteRatio(const std::vector<Job>& jobs) {
    int remote_count = 0;
    for (const auto& job : jobs) {
        std::string location_lower = job.location.display_name;
        std::transform(location_lower.begin(), location_lower.end(), location_lower.begin(), ::tolower);
        if (location_lower.find("remote") != std::string::npos ||
            location_lower.find("anywhere") != std::string::npos ||
            location_lower.find("distributed") != std::string::npos) {
            remote_count++;
        }
    }
    return static_cast<double>(remote_count) / jobs.size() * 100.0;
}

// Get daily job posting trends
std::map<std::string, int> JobParser::getDailyJobTrends(const std::vector<Job>& jobs, int days_back) {
    std::map<std::string, int> daily_trends;
    // Simplified implementation - in real scenario, you'd parse dates
    for (const auto& job : jobs) {
        std::string date = extractDate(job.created);
        if (!date.empty()) {
            daily_trends[date]++;
        }
    }
    return daily_trends;
}

// Calculate market growth rate
double JobParser::calculateGrowthRate(const std::map<std::string, int>& daily_trends) {
    if (daily_trends.size() < 2) return 0.0;
    
    auto it = daily_trends.begin();
    int first_day = it->second;
    std::advance(it, daily_trends.size() - 1);
    int last_day = it->second;
    
    if (first_day == 0) return 0.0;
    
    return static_cast<double>(last_day - first_day) / first_day * 100.0;
}

// Calculate average job quality score
double JobParser::calculateAverageQualityScore(const std::vector<Job>& jobs) {
    double total_score = 0.0;
    int count = 0;
    for (const auto& job : jobs) {
        double score = calculateJobQualityScore(job);
        if (score > 0) {
            total_score += score;
            count++;
        }
    }
    return count > 0 ? total_score / count : 0.0;
}

// Count high quality jobs (score >= 80)
int JobParser::countHighQualityJobs(const std::vector<Job>& jobs) {
    int count = 0;
    for (const auto& job : jobs) {
        if (calculateJobQualityScore(job) >= 80.0) {
            count++;
        }
    }
    return count;
}

// Calculate overall market health score
double JobParser::calculateMarketHealthScore(const JobMarketAnalytics& analytics) {
    double score = 0.0;
    
    // Job volume factor (30%)
    if (analytics.total_jobs >= 1000) score += 30.0;
    else if (analytics.total_jobs >= 500) score += 25.0;
    else if (analytics.total_jobs >= 200) score += 20.0;
    else if (analytics.total_jobs >= 100) score += 15.0;
    else score += 10.0;
    
    // Salary factor (25%)
    if (analytics.average_salary >= 100000) score += 25.0;
    else if (analytics.average_salary >= 80000) score += 20.0;
    else if (analytics.average_salary >= 60000) score += 15.0;
    else if (analytics.average_salary > 0) score += 10.0;
    
    // Company diversity factor (20%)
    if (analytics.unique_companies.size() >= 100) score += 20.0;
    else if (analytics.unique_companies.size() >= 50) score += 15.0;
    else if (analytics.unique_companies.size() >= 20) score += 10.0;
    else score += 5.0;
    
    // Job quality factor (15%)
    if (analytics.average_quality_score >= 80.0) score += 15.0;
    else if (analytics.average_quality_score >= 60.0) score += 12.0;
    else if (analytics.average_quality_score >= 40.0) score += 8.0;
    else score += 4.0;
    
    // Growth factor (10%)
    if (analytics.growth_rate >= 20.0) score += 10.0;
    else if (analytics.growth_rate >= 10.0) score += 8.0;
    else if (analytics.growth_rate >= 0.0) score += 5.0;
    else score += 2.0;
    
    return std::min(score, 100.0);
}

// Helper function to calculate percentiles
double JobParser::calculatePercentile(const std::vector<double>& data, double percentile) {
    if (data.empty()) return 0.0;
    int index = static_cast<int>(data.size() * percentile);
    return data[std::min(index, static_cast<int>(data.size() - 1))];
}

// Helper function to extract date from timestamp
std::string JobParser::extractDate(const std::string& timestamp) {
    if (timestamp.empty()) return "";
    // Simple extraction - assumes format like "2024-01-15"
    if (timestamp.length() >= 10) {
        return timestamp.substr(0, 10);
    }
    return timestamp;
}
    // Add comprehensive job market analytics dashboard
JobMarketAnalytics JobParser::generateMarketAnalytics(const std::vector<Job>& jobs, int days_back) {
    JobMarketAnalytics analytics;
    
    if (jobs.empty()) return analytics;
    
    // Basic statistics
    analytics.total_jobs = jobs.size();
    analytics.analysis_date = getCurrentTimestamp();
    
    // Technology trends
    analytics.technology_trends = analyzeTechnologyTrends(jobs);
    
    // Salary analytics
    calculateSalaryAnalytics(analytics, jobs);
    
    // Company analytics
    calculateCompanyAnalytics(analytics, jobs);
    
    // Location analytics
    calculateLocationAnalytics(analytics, jobs);
    
    // Experience level distribution
    calculateExperienceDistribution(analytics, jobs);
    
    // Job category distribution
    calculateCategoryDistribution(analytics, jobs);
    
    // Market trends and insights
    calculateMarketInsights(analytics, jobs, days_back);
    
    // Top skills in demand
    analytics.top_skills_demand = extractTopSkills(jobs, 10);
    
    return analytics;
}

// Helper function to calculate salary analytics
void JobParser::calculateSalaryAnalytics(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    double total_min = 0.0, total_max = 0.0;
    int salary_count = 0;
    std::vector<double> all_salaries;
    
    for (const auto& job : jobs) {
        if (job.salary_min > 0 && job.salary_max > 0) {
            double avg_salary = (job.salary_min + job.salary_max) / 2.0;
            total_min += job.salary_min;
            total_max += job.salary_max;
            all_salaries.push_back(avg_salary);
            salary_count++;
        }
    }
    
    if (salary_count > 0) {
        analytics.average_salary_min = total_min / salary_count;
        analytics.average_salary_max = total_max / salary_count;
        analytics.average_salary = (analytics.average_salary_min + analytics.average_salary_max) / 2.0;
        
        // Calculate salary percentiles
        std::sort(all_salaries.begin(), all_salaries.end());
        analytics.salary_25th_percentile = all_salaries[all_salaries.size() * 0.25];
        analytics.salary_75th_percentile = all_salaries[all_salaries.size() * 0.75];
        analytics.salary_90th_percentile = all_salaries[all_salaries.size() * 0.90];
    }
    
    analytics.jobs_with_salary_info = salary_count;
}

// Helper function to calculate company analytics
void JobParser::calculateCompanyAnalytics(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    std::map<std::string, int> company_counts;
    std::map<std::string, double> company_avg_salaries;
    std::map<std::string, int> company_salary_counts;
    
    for (const auto& job : job) {
        std::string company_normalized = normalizeCompanyName(job.company.display_name);
        company_counts[company_normalized]++;
        
        if (job.salary_min > 0 && job.salary_max > 0) {
            double avg_salary = (job.salary_min + job.salary_max) / 2.0;
            company_avg_salaries[company_normalized] += avg_salary;
            company_salary_counts[company_normalized]++;
        }
    }
    
    // Find top hiring companies
    analytics.top_hiring_companies = getTopEntries(company_counts, 5);
    
    // Find top paying companies (with at least 3 salary data points)
    std::map<std::string, double> valid_company_salaries;
    for (const auto& [company, total] : company_avg_salaries) {
        if (company_salary_counts[company] >= 3) {
            valid_company_salaries[company] = total / company_salary_counts[company];
        }
    }
    analytics.top_paying_companies = getTopEntries(valid_company_salaries, 5);
}

// Helper function to calculate location analytics
void JobParser::calculateLocationAnalytics(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    std::map<std::string, int> location_counts;
    std::map<std::string, double> location_avg_salaries;
    std::map<std::string, int> location_salary_counts;
    
    for (const auto& job : jobs) {
        std::string location_key = job.location.display_name;
        location_counts[location_key]++;
        
        if (job.salary_min > 0 && job.salary_max > 0) {
            double avg_salary = (job.salary_min + job.salary_max) / 2.0;
            location_avg_salaries[location_key] += avg_salary;
            location_salary_counts[location_key]++;
        }
    }
    
    analytics.top_locations = getTopEntries(location_counts, 5);
    
    // Calculate top paying locations (with at least 3 salary data points)
    std::map<std::string, double> valid_location_salaries;
    for (const auto& [location, total] : location_avg_salaries) {
        if (location_salary_counts[location] >= 3) {
            valid_location_salaries[location] = total / location_salary_counts[location];
        }
    }
    analytics.top_paying_locations = getTopEntries(valid_location_salaries, 5);
}

// Helper function to calculate experience level distribution
void JobParser::calculateExperienceDistribution(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    std::map<std::string, int> experience_counts;
    
    for (const auto& job : jobs) {
        std::string experience_level = detectExperienceLevel(job);
        experience_counts[experience_level]++;
    }
    
    analytics.experience_distribution = experience_counts;
}

// Helper function to calculate job category distribution
void JobParser::calculateCategoryDistribution(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    std::map<std::string, int> category_counts;
    
    for (const auto& job : jobs) {
        std::string category = categorizeJob(job);
        category_counts[category]++;
    }
    
    analytics.category_distribution = category_counts;
}

// Helper function to extract top skills in demand
std::map<std::string, int> JobParser::extractTopSkills(const std::vector<Job>& jobs, int top_n) {
    std::map<std::string, int> all_skills;
    
    for (const auto& job : jobs) {
        auto technologies = extractTechnologies(job.description);
        for (const auto& tech : technologies) {
            all_skills[tech]++;
        }
    }
    
    return getTopEntries(all_skills, top_n);
}

// Helper function to get top N entries from a map
template<typename K, typename V>
std::map<K, V> JobParser::getTopEntries(const std::map<K, V>& input_map, int top_n) {
    std::vector<std::pair<K, V>> pairs(input_map.begin(), input_map.end());
    std::sort(pairs.begin(), pairs.end(),
              [](const std::pair<K, V>& a, const std::pair<K, V>& b) {
                  return a.second > b.second;
              });
    
    std::map<K, V> result;
    for (int i = 0; i < std::min(top_n, static_cast<int>(pairs.size())); i++) {
        result[pairs[i].first] = pairs[i].second;
    }
    
    return result;
}

// Helper function to get current timestamp
std::string JobParser::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}
}
