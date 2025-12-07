#include "JobParser.h"
#include <algorithm>
#include <regex>
#include <map>
#include <set>
#include <cmath>
#include <ctime>
#include <cstdlib>

// [Keep ALL your existing functions up to calculateSalaryMatchScore...]

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
} // ← THIS WAS MISSING!

// Choose ONE version of generateMarketAnalytics - remove the duplicate:

// Version 1 (Comprehensive) - KEEP THIS ONE:
JobMarketAnalytics JobParser::generateMarketAnalytics(const std::vector<Job>& jobs, int days_back) {
    JobMarketAnalytics analytics;
    
    if (jobs.empty()) return analytics;
    
    // Basic counts
    analytics.total_jobs = jobs.size();
    
    // Get unique companies and locations
    std::set<std::string> unique_companies;
    std::set<std::string> unique_locations;
    for (const auto& job : jobs) {
        if (!job.company.display_name.empty()) {
            unique_companies.insert(normalizeCompanyName(job.company.display_name));
        }
        if (!job.location.display_name.empty()) {
            unique_locations.insert(job.location.display_name);
        }
    }
    analytics.unique_companies = unique_companies.size();
    analytics.unique_locations = unique_locations.size();
    
    // Salary analytics
    calculateSalaryAnalytics(analytics, jobs);
    
    // Technology trends
    analytics.technology_trends = analyzeTechnologyTrends(jobs);
    
    // Get hot technologies
    std::vector<std::pair<std::string, int>> hot_techs;
    for (const auto& [tech, count] : analytics.technology_trends) {
        hot_techs.push_back({tech, count});
    }
    std::sort(hot_techs.begin(), hot_techs.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    if (hot_techs.size() > 10) hot_techs.resize(10);
    analytics.hot_technologies.clear();
    for (const auto& [tech, count] : hot_techs) {
        analytics.hot_technologies[tech] = count;
    }
    
    // Job type distribution
    analytics.job_type_distribution = getJobTypeDistribution(jobs);
    
    // Experience level distribution
    analytics.experience_distribution = getExperienceDistribution(jobs);
    
    // Company analytics
    std::map<std::string, int> company_counts;
    for (const auto& job : jobs) {
        if (!job.company.display_name.empty()) {
            std::string normalized = normalizeCompanyName(job.company.display_name);
            company_counts[normalized]++;
        }
    }
    std::vector<std::pair<std::string, int>> top_companies(company_counts.begin(), company_counts.end());
    std::sort(top_companies.begin(), top_companies.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    if (top_companies.size() > 10) top_companies.resize(10);
    analytics.top_companies.clear();
    for (const auto& [company, count] : top_companies) {
        analytics.top_companies[company] = count;
    }
    
    // Location distribution
    analytics.location_distribution = getLocationDistribution(jobs);
    
    // Remote ratio
    analytics.remote_ratio = calculateRemoteRatio(jobs);
    
    // Temporal analytics (simplified)
    std::map<std::string, int> daily_trends;
    for (const auto& job : jobs) {
        if (!job.created.empty() && job.created.length() >= 10) {
            std::string date = job.created.substr(0, 10);
            daily_trends[date]++;
        }
    }
    analytics.daily_trends = daily_trends;
    analytics.growth_rate = calculateGrowthRate(daily_trends);
    
    // Quality metrics
    analytics.average_quality_score = calculateAverageQualityScore(jobs);
    analytics.high_quality_jobs = countHighQualityJobs(jobs);
    
    // Market health score
    analytics.market_health_score = calculateMarketHealthScore(analytics);
    
    return analytics;
}

// REMOVE THE DUPLICATE generateMarketAnalytics function that starts with:
// "// Add comprehensive job market analytics dashboard"
// This is the duplicate - delete it!

// Keep all your other function implementations, but fix syntax errors:

// In calculateCompanyAnalytics - fix the parameter name:
void JobParser::calculateCompanyAnalytics(JobMarketAnalytics& analytics, const std::vector<Job>& jobs) {
    std::map<std::string, int> company_counts;
    std::map<std::string, double> company_avg_salaries;
    std::map<std::string, int> company_salary_counts;
    
    for (const auto& job : jobs) { // ← FIXED: was "job" instead of "jobs"
        std::string company_normalized = normalizeCompanyName(job.company.display_name);
        company_counts[company_normalized]++;
        
        if (job.salary_min > 0 && job.salary_max > 0) {
            double avg_salary = (job.salary_min + job.salary_max) / 2.0;
            company_avg_salaries[company_normalized] += avg_salary;
            company_salary_counts[company_normalized]++;
        }
    }
    
    // ... rest of the function
}

// Add missing implementations for helper functions:

std::map<std::string, int> JobParser::getJobTypeDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> distribution;
    for (const auto& job : jobs) {
        std::string category = categorizeJob(job);
        distribution[category]++;
    }
    return distribution;
}

std::map<std::string, int> JobParser::getExperienceDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> distribution;
    for (const auto& job : jobs) {
        std::string level = detectExperienceLevel(job);
        distribution[level]++;
    }
    return distribution;
}

std::map<std::string, int> JobParser::getLocationDistribution(const std::vector<Job>& jobs) {
    std::map<std::string, int> distribution;
    for (const auto& job : jobs) {
        std::string location = job.location.display_name.empty() ? "Unknown" : job.location.display_name;
        distribution[location]++;
    }
    return distribution;
}

double JobParser::calculateRemoteRatio(const std::vector<Job>& jobs) {
    int remote_count = 0;
    for (const auto& job : jobs) {
        std::string location_lower = job.location.display_name;
        std::transform(location_lower.begin(), location_lower.end(), location_lower.begin(), ::tolower);
        if (location_lower.find("remote") != std::string::npos ||
            location_lower.find("anywhere") != std::string::npos) {
            remote_count++;
        }
    }
    return jobs.empty() ? 0.0 : (static_cast<double>(remote_count) / jobs.size() * 100.0);
}

double JobParser::calculateGrowthRate(const std::map<std::string, int>& daily_trends) {
    if (daily_trends.size() < 2) return 0.0;
    
    auto it = daily_trends.begin();
    int first = it->second;
    auto last_it = daily_trends.rbegin();
    int last = last_it->second;
    
    return first > 0 ? ((last - first) / static_cast<double>(first)) * 100.0 : 0.0;
}

double JobParser::calculateAverageQualityScore(const std::vector<Job>& jobs) {
    double total = 0.0;
    int count = 0;
    for (const auto& job : jobs) {
        double score = calculateJobQualityScore(job);
        if (score > 0) {
            total += score;
            count++;
        }
    }
    return count > 0 ? total / count : 0.0;
}

int JobParser::countHighQualityJobs(const std::vector<Job>& jobs) {
    int count = 0;
    for (const auto& job : jobs) {
        if (calculateJobQualityScore(job) >= 80.0) {
            count++;
        }
    }
    return count;
}

double JobParser::calculateMarketHealthScore(const JobMarketAnalytics& analytics) {
    double score = 0.0;
    
    if (analytics.total_jobs >= 1000) score += 30.0;
    else if (analytics.total_jobs >= 500) score += 25.0;
    else if (analytics.total_jobs >= 200) score += 20.0;
    else if (analytics.total_jobs >= 100) score += 15.0;
    else score += 10.0;
    
    if (analytics.average_salary >= 100000) score += 25.0;
    else if (analytics.average_salary >= 80000) score += 20.0;
    else if (analytics.average_salary >= 60000) score += 15.0;
    else if (analytics.average_salary > 0) score += 10.0;
    
    if (analytics.unique_companies >= 100) score += 20.0;
    else if (analytics.unique_companies >= 50) score += 15.0;
    else if (analytics.unique_companies >= 20) score += 10.0;
    else score += 5.0;
    
    if (analytics.average_quality_score >= 80.0) score += 15.0;
    else if (analytics.average_quality_score >= 60.0) score += 12.0;
    else if (analytics.average_quality_score >= 40.0) score += 8.0;
    else score += 4.0;
    
    if (analytics.growth_rate >= 20.0) score += 10.0;
    else if (analytics.growth_rate >= 10.0) score += 8.0;
    else if (analytics.growth_rate >= 0.0) score += 5.0;
    else score += 2.0;
    
    return std::min(score, 100.0);
}

// Continue with the rest of your functions...
// [Make sure to implement all the helper functions you declared]
