#ifndef JOBPARSER_H
#define JOBPARSER_H

#include <vector>
#include <string>
#include "models/Job.h"

class JobParser {
public:
    // Parse technologies from job description
    static std::vector<std::string> extractTechnologies(const std::string& description);
    
    // Categorize job by technology stack
    static std::string categorizeJob(const Job& job);
    
    // Parse salary from string format
    static bool parseSalary(const std::string& salary_str, double& min_salary, double& max_salary);
    
    // Filter jobs by technology
    static std::vector<Job> filterByTechnology(const std::vector<Job>& jobs, const std::string& technology);
    
    // Analyze technology trends from job list
    static std::map<std::string, int> analyzeTechnologyTrends(const std::vector<Job>& jobs);
    
    // Normalize company names
    static std::string normalizeCompanyName(const std::string& company_name);
    
    // Parse location into structured format
    static Location parseLocation(const std::string& location_str);
};

#endif
