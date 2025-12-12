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

// Below are duplicate/loose declarations and structs sitting outside the class
// (file needs cleanup to move them inside appropriate scopes and fix include paths)

static std::vector<Job> rankJobsByRelevance(const std::vector<Job>& jobs,
                                           const std::string& user_skills,
                                           const std::string& preferred_location = "",
                                           double desired_salary = 0,
                                           const std::vector<std::string>& preferred_technologies = {});

static std::vector<Job> findSimilarJobs(const Job& reference_job,
                                       const std::vector<Job>& all_jobs,
                                       int max_results = 10);

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

static std::vector<JobAlert> generateJobAlerts(const std::vector<Job>& new_jobs,
                                              const UserPreferences& preferences,
                                              const std::vector<Job>& previous_jobs = {});

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

static std::vector<Job> advancedJobSearch(const std::vector<Job>& jobs,
                                         const SearchCriteria& criteria);

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

struct TechnologyTrend {
    std::string technology;
    int current_demand;
    double predicted_growth_rate;
    int predicted_demand_increase;
    double confidence_score;
};

struct SalaryPrediction {
    std::string category;
    double current_average;
    double predicted_change_percentage;
    double predicted_absolute_change;
    double confidence_score;
};

struct CategoryDemand {
    std::string category;
    double current_market_share;
    int current_job_count;
    double predicted_demand_change;
    std::string growth_outlook;
};

struct EmergingOpportunity {
    std::string technology_combination;
    int current_occurrences;
    int estimated_market_size;
    double average_salary;
    double salary_premium_percentage;
    double growth_potential;
    std::string risk_level;
};

struct MarketPredictions {
    int forecast_period_days;
    std::string generation_date;
    double trend_job_growth_rate;
    int historical_data_points;
    double overall_confidence;
    
    std::vector<TechnologyTrend> technology_trends;
    std::vector<SalaryPrediction> salary_predictions;
    std::vector<CategoryDemand> category_demand;
    std::vector<EmergingOpportunity> emerging_opportunities;
    
    MarketPredictions() : forecast_period_days(0), trend_job_growth_rate(0),
                         historical_data_points(0), overall_confidence(0) {}
};

static MarketPredictions predictMarketTrends(const std::vector<Job>& historical_jobs,
                                            int forecast_days = 30);

static void analyzeHistoricalTrends(MarketPredictions& predictions,
                                   const std::vector<Job>& jobs);
static void predictTechnologyTrends(MarketPredictions& predictions,
                                   const std::vector<Job>& jobs,
                                   int forecast_days);
static void predictSalaryTrends(MarketPredictions& predictions,
                               const std::vector<Job>& jobs,
                               int forecast_days);
static void predictCategoryDemand(MarketPredictions& predictions,
                                 const std::vector<Job>& jobs,
                                 int forecast_days);
static void identifyEmergingOpportunities(MarketPredictions& predictions,
                                         const std::vector<Job>& jobs);
static void calculatePredictionConfidence(MarketPredictions& predictions,
                                         const std::vector<Job>& jobs);

static std::string extractDateKey(const std::string& timestamp);
static double calculateGrowthRate(const std::vector<int>& counts);
static double calculateSalaryTrend(const std::vector<double>& salaries);
static double calculateAverage(const std::vector<double>& values);
static double predictDemandChange(const std::string& category,
                                 const std::vector<Job>& jobs);
static std::string classifyGrowthOutlook(double change);
static bool isEmergingTechnology(const std::string& tech);
static double calculateSalaryPremium(double opportunity_salary,
                                    const std::vector<Job>& jobs);
static double calculateGrowthPotential(const std::string& tech_combo,
                                      const std::vector<Job>& jobs);
static std::string calculateRiskLevel(int occurrences, double growth_potential);
static double calculateTechConfidenceScore(const std::vector<int>& counts);
static double calculateSalaryConfidenceScore(const std::vector<double>& salaries);

#endif
