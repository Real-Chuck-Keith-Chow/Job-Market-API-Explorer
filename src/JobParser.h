#ifndef JOBPARSER_H
#define JOBPARSER_H

#include <map>
#include <string>
#include <vector>

#include "model/Job.h"
#include "model/Location.h"

class JobParser {
public:
    static std::vector<std::string> extractTechnologies(const std::string& description);
    static std::vector<std::string> extractTechnologiesWithAliases(const std::string& description);

    static std::string categorizeJob(const Job& job);

    static bool parseSalary(const std::string& salary_str,
                            double& min_salary,
                            double& max_salary);

    static bool validateSalaryRange(double min_salary, double max_salary);
    static void normalizeSalaryRange(double& min_salary, double& max_salary);
    static bool isSalaryOutlier(double salary, const std::vector<Job>& jobs);

    static std::vector<Job> filterByTechnology(const std::vector<Job>& jobs,
                                               const std::string& technology);

    static std::map<std::string, int> analyzeTechnologyTrends(const std::vector<Job>& jobs);

    static std::string normalizeCompanyName(const std::string& company_name);
    static Location parseLocation(const std::string& location_str);

    static double calculateJobQualityScore(const Job& job);
    static std::string detectExperienceLevel(const Job& job);

    static std::vector<Job> rankJobsByRelevance(
        const std::vector<Job>& jobs,
        const std::string& user_skills,
        const std::string& preferred_location = "",
        double desired_salary = 0.0,
        const std::vector<std::string>& preferred_technologies = {}
    );

    static std::vector<Job> findSimilarJobs(const Job& reference_job,
                                            const std::vector<Job>& all_jobs,
                                            int max_results = 10);

private:
    static double calculateTechnologyMatchScore(
        const Job& job,
        const std::string& user_skills,
        const std::vector<std::string>& preferred_technologies
    );

    static double calculateLocationMatchScore(const Job& job,
                                              const std::string& preferred_location);

    static double calculateSalaryMatchScore(const Job& job,
                                            double desired_salary);
};

#endif
