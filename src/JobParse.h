#ifndef JOBPARSER_H
#define JOBPARSER_H

#include <vector>
#include <string>
#include <map>
#include "models/Job.h"
#include "models/Location.h"

// Forward declarations for structures used in JobParser
struct UserPreferences;
struct JobAlert;
struct SearchCriteria;
struct MarketPredictions;
struct TechnologyTrend;
struct SalaryPrediction;
struct CategoryDemand;
struct EmergingOpportunity;

// Add new enums and structs (BEFORE class declaration)
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

// Add new enums for search criteria
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

// Add prediction structures
struct TechnologyTrend {
    std::string technology;
    int current_demand;
    double predicted_growth_rate; // percentage
    int predicted_demand_increase;
    double confidence_score;
    
    TechnologyTrend() : current_demand(0), predicted_growth_rate(0), 
                       predicted_demand_increase(0), confidence_score(0) {}
};

struct SalaryPrediction {
    std::string category;
    double current_average;
    double predicted_change_percentage;
    double predicted_absolute_change;
    double confidence_score;
    
    SalaryPrediction() : current_average(0), predicted_change_percentage(0),
                        predicted_absolute_change(0), confidence_score(0) {}
};

struct CategoryDemand {
    std::string category;
    double current_market_share;
    int current_job_count;
    double predicted_demand_change;
    std::string growth_outlook;
    
    CategoryDemand() : current_market_share(0), current_job_count(0),
                      predicted_demand_change(0) {}
};

struct EmergingOpportunity {
    std::string technology_combination;
    int current_occurrences;
    int estimated_market_size;
    double average_salary;
    double salary_premium_percentage;
    double growth_potential;
    std::string risk_level;
    
    EmergingOpportunity() : current_occurrences(0), estimated_market_size(0),
                           average_salary(0), salary_premium_percentage(0),
                           growth_potential(0) {}
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

// Add career planning structures
enum class GapType { MISSING_SKILL, NEEDS_IMPROVEMENT, MEETS_REQUIREMENTS, EXCEEDS_REQUIREMENTS };

struct SkillLevel {
    std::string skill_name;
    double current_level; // 1-5 scale
    double years_experience;
    
    SkillLevel() : current_level(0), years_experience(0) {}
};

struct SkillRequirement {
    std::string skill_name;
    double required_level; // 1-5 scale
    double frequency; // percentage of jobs requiring this
    double importance; // calculated score
    
    SkillRequirement() : required_level(0), frequency(0), importance(0) {}
};

struct SkillGap {
    std::string skill_name;
    double current_level;
    double required_level;
    double gap_size;
    GapType gap_type;
    int priority; // 1-10 scale
    
    SkillGap() : current_level(0), required_level(0), gap_size(0), 
                gap_type(GapType::MISSING_SKILL), priority(0) {}
};

struct LearningPhase {
    int phase_number;
    std::string category;
    std::string description;
    std::vector<std::string> skills_to_learn;
    std::map<std::string, double> target_levels;
    int estimated_hours;
    std::vector<std::string> recommended_resources;
    std::vector<std::string> success_metrics;
    int expected_completion_weeks;
    
    LearningPhase() : phase_number(0), estimated_hours(0), expected_completion_weeks(0) {}
};

struct BridgePosition {
    std::string position_title;
    std::string company;
    double estimated_salary;
    double skill_overlap_percentage;
    double target_skill_exposure;
    std::string recommendation_reason;
    
    BridgePosition() : estimated_salary(0), skill_overlap_percentage(0), 
                      target_skill_exposure(0) {}
};

struct UserProfile {
    std::string user_id;
    std::string current_position;
    std::vector<std::string> skills;
    std::map<std::string, double> skill_levels; // skill -> level (1-5)
    std::map<std::string, int> skill_experience; // skill -> years
    double current_salary;
    int years_experience;
    int weekly_study_hours;
    std::vector<std::string> career_interests;
    std::vector<std::string> learning_preferences;
    
    UserProfile() : current_salary(0), years_experience(0), weekly_study_hours(10) {}
};

struct CareerPathPlan {
    std::string user_id;
    std::string generation_date;
    std::string current_position;
    std::string target_position;
    std::string primary_tech_stack;
    
    // Current state
    std::vector<std::string> current_skills;
    std::vector<SkillLevel> current_skill_levels;
    double current_salary;
    int years_experience;
    
    // Target requirements
    std::vector<SkillRequirement> target_requirements;
    double target_salary_min;
    double target_salary_max;
    std::string required_experience_level;
    
    // Gap analysis
    std::vector<SkillGap> skill_gaps;
    
    // Learning path
    std::vector<LearningPhase> learning_phases;
    int total_estimated_hours;
    int estimated_weeks;
    std::string expected_completion_date;
    
    // Bridge positions
    std::vector<BridgePosition> bridge_positions;
    
    // Success metrics
    double success_probability;
    std::string confidence_level;
    
    // Recommendations
    std::vector<std::string> immediate_actions;
    std::vector<std::string> short_term_goals;
    std::vector<std::string> long_term_goals;
    std::vector<std::string> risk_mitigation;
    
    CareerPathPlan() : current_salary(0), years_experience(0),
                      target_salary_min(0), target_salary_max(0),
                      total_estimated_hours(0), estimated_weeks(0),
                      success_probability(0) {}
};

// Main JobParser class
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
    
    // Alert system
    static std::vector<JobAlert> generateJobAlerts(const std::vector<Job>& new_jobs,
                                                  const UserPreferences& preferences,
                                                  const std::vector<Job>& previous_jobs = {});
    
    // Advanced search
    static std::vector<Job> advancedJobSearch(const std::vector<Job>& jobs,
                                             const SearchCriteria& criteria);
    
    // Market predictions
    static MarketPredictions predictMarketTrends(const std::vector<Job>& historical_jobs,
                                                int forecast_days = 30);
    
    // Career planning
    static CareerPathPlan generateCareerPathPlan(const UserProfile& profile,
                                                const std::vector<Job>& market_jobs,
                                                const std::string& target_position);
    
private:
    // Helper functions for ranking algorithms
    static double calculateTechnologyMatchScore(const Job& job,
                                               const std::string& user_skills,
                                               const std::vector<std::string>& preferred_technologies);
    
    static double calculateLocationMatchScore(const Job& job, const std::string& preferred_location);
    
    static double calculateSalaryMatchScore(const Job& job, double desired_salary);
    
    // Helper functions for alert system
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
    
    // Helper functions for advanced search
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
    
    // Helper functions for market predictions
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
    
    // Helper functions for career planning
    static void analyzeCurrentSkills(CareerPathPlan& plan, const UserProfile& profile);
    static void findTargetRequirements(CareerPathPlan& plan, const std::string& target_position,
                                      const std::vector<Job>& market_jobs);
    static void identifySkillGaps(CareerPathPlan& plan);
    static void generateLearningPath(CareerPathPlan& plan, const std::vector<Job>& market_jobs);
    static void calculateTimeline(CareerPathPlan& plan, const UserProfile& profile);
    static void findBridgePositions(CareerPathPlan& plan, const std::vector<Job>& market_jobs);
    static void calculateSuccessProbability(CareerPathPlan& plan, const UserProfile& profile,
                                           const std::vector<Job>& market_jobs);
    static void generateRecommendations(CareerPathPlan& plan);
    
    static std::string identifyPrimaryTechStack(const UserProfile& profile);
    static bool isSimilarPosition(const std::string& job_title, const std::string& target_position);
    static void extractCommonRequirements(CareerPathPlan& plan, const std::vector<Job>& target_jobs);
    static double calculateSkillPriority(const SkillGap& gap, double frequency);
    static std::string categorizeSkill(const std::string& skill);
    static int estimateLearningHours(const std::vector<SkillGap>& gaps);
    static std::vector<std::string> recommendLearningResources(const std::string& category,
                                                              const std::vector<SkillGap>& gaps,
                                                              const std::vector<Job>& market_jobs);
    static std::vector<std::string> defineSuccessMetrics(const std::string& category,
                                                        const std::vector<SkillGap>& gaps);
    static std::string calculateCompletionDate(int total_weeks);
    static double estimateSkillLevelFromJob(const Job& job);
    static double calculateSkillImportance(double frequency, double required_level);
    static double calculateCurrentOverlapScore(const CareerPathPlan& plan);
    static double calculateLearningCapabilityScore(const UserProfile& profile);
    static double calculateMarketDemandScore(const std::string& target_position,
                                            const std::vector<Job>& market_jobs);
    static double calculateTimelineFeasibilityScore(const CareerPathPlan& plan,
                                                   const UserProfile& profile);
    static std::string calculateConfidenceLevel(const CareerPathPlan& plan, int market_data_points);
    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
    
    // Utility functions
    static std::string getCurrentTimestamp();
    template<typename K, typename V>
    static std::map<K, V> getTopEntries(const std::map<K, V>& input_map, int top_n);
    static double calculatePercentile(const std::vector<double>& data, double percentile);
};

#endif // JOBPARSER_H
