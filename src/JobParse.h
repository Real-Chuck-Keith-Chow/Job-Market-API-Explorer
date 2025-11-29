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
    static std::vector<std::string> extractTechnologiesWithAliases(const std::string& description);
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
    
    // Job quality and experience analysis
    static double calculateJobQualityScore(const Job& job);
    static std::string detectExperienceLevel(const Job& job);
    
    // Advanced ranking and recommendations
    static std::vector<Job> rankJobsByRelevance(const std::vector<Job>& jobs,
                                               const std::string& user_skills,
                                               const std::string& preferred_location = "",
                                               double desired_salary = 0,
                                               const std::vector<std::string>& preferred_technologies = {});
    
    static std::vector<Job> findSimilarJobs(const Job& reference_job,
                                           const std::vector<Job>& all_jobs,
                                           int max_results = 10);

private:
    // Helper functions for ranking algorithms
    static double calculateTechnologyMatchScore(const Job& job,
                                               const std::string& user_skills,
                                               const std::vector<std::string>& preferred_technologies);
    
    static double calculateLocationMatchScore(const Job& job, const std::string& preferred_location);
    
    static double calculateSalaryMatchScore(const Job& job, double desired_salary);
};

// Add to public section of JobParser class:
static std::vector<Job> rankJobsByRelevance(const std::vector<Job>& jobs,
                                           const std::string& user_skills,
                                           const std::string& preferred_location = "",
                                           double desired_salary = 0,
                                           const std::vector<std::string>& preferred_technologies = {});

static std::vector<Job> findSimilarJobs(const Job& reference_job,
                                       const std::vector<Job>& all_jobs,
                                       int max_results = 10);

private:
// Add private helper functions:
static double calculateTechnologyMatchScore(const Job& job,
                                           const std::string& user_skills,
                                           const std::vector<std::string>& preferred_technologies);

static double calculateLocationMatchScore(const Job& job, const std::string& preferred_location);

static double calculateSalaryMatchScore(const Job& job, double desired_salary);

// Add new enums and structs (before class declaration)
enum class AlertType {
    JOB_MATCH,
    SALARY_TREND,
    TECHNOLOGY_TREND,
    COMPANY_HIRING,
    SKILL_GAP
};

struct UserPreferences {
    std::vector<std::string> skills;
    std::vector<std::string> preferred_technologies;
    std::vector<std::string> preferred_locations;
    std::vector<std::string> preferred_companies;
    std::vector<std::string> job_types;
    std::vector<std::string> emerging_technologies_interest;
    std::string experience_level;
    double desired_salary;
    double min_match_threshold;
    
    UserPreferences() : desired_salary(0), min_match_threshold(70.0) {}
};

struct JobAlert {
    AlertType type;
    std::string title;
    std::string message;
    Job job;
    int priority;
    std::string timestamp;
    
    JobAlert() : priority(5) {}
};

// Add to public section of JobParser class:
static std::vector<JobAlert> generateJobAlerts(const std::vector<Job>& new_jobs,
                                              const UserPreferences& preferences,
                                              const std::vector<Job>& previous_jobs = {});

private:
// Add private helper functions:
static std::vector<Job> findMatchingJobs(const std::vector<Job>& jobs, const UserPreferences& preferences);
static double calculateJobMatchScore(const Job& job, const UserPreferences& preferences);
static bool isExperienceCompatible(const std::string& job_level, const std::string& user_level);
static int calculateAlertPriority(const Job& job, const UserPreferences& preferences);
static std::vector<JobAlert> generateSalaryAlerts(const std::vector<Job>& new_jobs,
                                                 const UserPreferences& preferences,
                                                 const std::vector<Job>& previous_jobs);
static std::vector<JobAlert> generateTechnologyAlerts(const std::vector<Job>& new_jobs,
                                                     const UserPreferences& preferences);
static std::vector<JobAlert> generateCompanyAlerts(const std::vector<Job>& new_jobs,
                                                  const UserPreferences& preferences);
static double calculateAverageSalary(const std::vector<Job>& jobs);

// Add new enums for search criteria (before class declaration)
enum class KeywordMatchType { ANY, ALL };
enum class TechnologyMatchType { ANY, ALL };
enum class LocationMatchType { EXACT, PARTIAL };
enum class SortBy { NONE, RELEVANCE, SALARY, DATE, COMPANY, LOCATION, TITLE };
enum class SortOrder { ASCENDING, DESCENDING };

struct SearchCriteria {
    std::vector<std::string> keywords;
    KeywordMatchType keyword_match_type = KeywordMatchType::ANY;
    
    std::vector<std::string> technologies;
    TechnologyMatchType tech_match_type = TechnologyMatchType::ANY;
    
    std::vector<std::string> locations;
    LocationMatchType location_match_type = LocationMatchType::PARTIAL;
    
    std::vector<std::string> companies;
    std::vector<std::string> job_types;
    std::vector<std::string> experience_levels;
    
    double min_salary = 0;
    double max_salary = 0;
    bool remote_only = false;
    int posted_within_days = 0;
    int max_results = 0;
    
    SortBy sort_by = SortBy::RELEVANCE;
    SortOrder sort_order = SortOrder::DESCENDING;
};

// Add to public section of JobParser class:
static std::vector<Job> advancedJobSearch(const std::vector<Job>& jobs,
                                         const SearchCriteria& criteria);

private:
// Add private helper functions for search:
static std::vector<Job> filterByKeywords(const std::vector<Job>& jobs,
                                        const std::vector<std::string>& keywords,
                                        KeywordMatchType match_type);
static bool containsKeyword(const Job& job, const std::string& keyword);
static std::vector<Job> filterByTechnologies(const std::vector<Job>& jobs,
                                            const std::vector<std::string>& technologies,
                                            TechnologyMatchType match_type);
static std::vector<Job> filterByLocations(const std::vector<Job>& jobs,
                                         const std::vector<std::string>& locations,
                                         LocationMatchType match_type);
static std::vector<Job> filterBySalaryRange(const std::vector<Job>& jobs,
                                           double min_salary, double max_salary);
static std::vector<Job> filterByCompanies(const std::vector<Job>& jobs,
                                         const std::vector<std::string>& companies);
static std::vector<Job> filterByJobTypes(const std::vector<Job>& jobs,
                                        const std::vector<std::string>& job_types);
static std::vector<Job> filterByExperienceLevels(const std::vector<Job>& jobs,
                                                const std::vector<std::string>& experience_levels);
static std::vector<Job> filterRemoteJobs(const std::vector<Job>& jobs);
static std::vector<Job> filterByPostDate(const std::vector<Job>& jobs, int days_back);
static std::vector<Job> sortJobs(const std::vector<Job>& jobs,
                                SortBy sort_by, SortOrder sort_order);

#endif
