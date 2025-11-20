#ifndef JOBPARSER_H
#define JOBPARSER_H

#include <vector>
#include <string>
#include <map>
#include "models/Job.h"
#include "models/Location.h"

class JobParser {
public:
    // Technology analysis
    static std::vector<std::string> extractTechnologies(const std::string& description);
    static std::string categorizeJob(const Job& job);
    
    // Salary processing
    static bool parseSalary(const std::string& salary_str, double& min_salary, double& max_salary);
    static bool validateSalaryRange(double min_salary, double max_salary);
    static void normalizeSalaryRange(double& min_salary, double& max_salary);
    static bool isSalaryOutlier(double salary, const std::vector<Job>& jobs);
    
    // Job filtering and analysis
    static std::vector<Job> filterByTechnology(const std::vector<Job>& jobs, const std::string& technology);
    static std::map<std::string, int> analyzeTechnologyTrends(const std::vector<Job>& jobs);
    
    // Data normalization
    static std::string normalizeCompanyName(const std::string& company_name);
    static Location parseLocation(const std::string& location_str);
    
    // Job quality scoring (new utility function)
    static double calculateJobQualityScore(const Job& job);
    
    // Experience level detection (new utility function)
    static std::string detectExperienceLevel(const Job& job);
};

#endif
