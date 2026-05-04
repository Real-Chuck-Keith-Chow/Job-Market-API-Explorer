#include "JobParser.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <regex>
#include <set>

std::vector<std::string> JobParser::extractTechnologies(const std::string& description) {
    std::vector<std::string> technologies;

    std::string desc_lower = description;
    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    const std::vector<std::string> techs = {
        "c++", "python", "java", "javascript", "typescript",
        "react", "angular", "vue", "node.js",
        "docker", "kubernetes", "aws", "azure",
        "sql", "mongodb", "redis"
    };

    for (const auto& tech : techs) {
        if (desc_lower.find(tech) != std::string::npos) {
            if (tech == "c++") technologies.push_back("C++");
            else if (tech == "node.js") technologies.push_back("Node.js");
            else {
                std::string t = tech;
                t[0] = std::toupper(t[0]);
                technologies.push_back(t);
            }
        }
    }

    std::sort(technologies.begin(), technologies.end());
    technologies.erase(std::unique(technologies.begin(), technologies.end()), technologies.end());

    return technologies;
}

std::string JobParser::categorizeJob(const Job& job) {
    std::string title = job.title;
    std::transform(title.begin(), title.end(), title.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (title.find("frontend") != std::string::npos) return "Frontend";
    if (title.find("backend") != std::string::npos) return "Backend";
    if (title.find("devops") != std::string::npos) return "DevOps";
    if (title.find("data") != std::string::npos) return "Data";

    return "General";
}

bool JobParser::parseSalary(const std::string& salary_str,
                            double& min_salary,
                            double& max_salary) {
    if (salary_str.empty()) return false;

    std::regex range(R"((\d+)[^\d]+(\d+))");
    std::smatch match;

    if (std::regex_search(salary_str, match, range)) {
        try {
            min_salary = std::stod(match[1]);
            max_salary = std::stod(match[2]);
            return true;
        } catch (...) {
            return false;
        }
    }

    return false;
}

bool JobParser::validateSalaryRange(double min_salary, double max_salary) {
    return min_salary >= 0 && max_salary >= min_salary;
}

void JobParser::normalizeSalaryRange(double& min_salary, double& max_salary) {
    if (min_salary > max_salary) {
        std::swap(min_salary, max_salary);
    }
}

bool JobParser::isSalaryOutlier(double salary, const std::vector<Job>& jobs) {
    if (jobs.empty() || salary <= 0) return false;

    double total = 0.0;
    int count = 0;

    for (const auto& job : jobs) {
        double avg = (job.salary_min + job.salary_max) / 2.0;
        if (avg > 0) {
            total += avg;
            count++;
        }
    }

    if (count == 0) return false;

    double mean = total / count;
    return std::abs(salary - mean) > mean * 2.0;
}

std::vector<Job> JobParser::filterByTechnology(const std::vector<Job>& jobs,
                                               const std::string& tech) {
    std::vector<Job> result;

    std::string t = tech;
    std::transform(t.begin(), t.end(), t.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (const auto& job : jobs) {
        std::string desc = job.description;
        std::transform(desc.begin(), desc.end(), desc.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (desc.find(t) != std::string::npos) {
            result.push_back(job);
        }
    }

    return result;
}

std::map<std::string, int> JobParser::analyzeTechnologyTrends(const std::vector<Job>& jobs) {
    std::map<std::string, int> counts;

    for (const auto& job : jobs) {
        for (const auto& tech : extractTechnologies(job.description)) {
            counts[tech]++;
        }
    }

    return counts;
}

std::string JobParser::normalizeCompanyName(const std::string& name) {
    std::string result = name;

    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return result;
}

Location JobParser::parseLocation(const std::string& str) {
    return Location(str);
}

double JobParser::calculateJobQualityScore(const Job& job) {
    double score = 0;

    if (!job.title.empty()) score += 20;
    if (!job.company.display_name.empty()) score += 20;
    if (!job.location.display_name.empty()) score += 20;
    if (!job.description.empty()) score += 20;
    if (job.salary_min > 0 || job.salary_max > 0) score += 20;

    return score;
}

std::string JobParser::detectExperienceLevel(const Job& job) {
    std::string text = job.title + " " + job.description;

    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (text.find("senior") != std::string::npos) return "Senior";
    if (text.find("junior") != std::string::npos) return "Junior";

    return "Mid";
}

std::vector<std::string> JobParser::extractTechnologiesWithAliases(const std::string& description) {
    return extractTechnologies(description);
}

std::vector<Job> JobParser::rankJobsByRelevance(const std::vector<Job>& jobs,
                                               const std::string&,
                                               const std::string&,
                                               double,
                                               const std::vector<std::string>&) {
    return jobs; // simplified
}

std::vector<Job> JobParser::findSimilarJobs(const Job&,
                                           const std::vector<Job>& jobs,
                                           int max_results) {
    if ((int)jobs.size() <= max_results) return jobs;
    return std::vector<Job>(jobs.begin(), jobs.begin() + max_results);
}
