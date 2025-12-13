#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>
#include "model/Job.h"  // note corrected include path

class Database {
private:
    std::string database_path;
    std::string connection_string;

    // In-memory cache for performance
    std::vector<Job> job_cache;
    bool cache_dirty;

    void initializeDatabase();
    void createTables();
    void updateCache();

public:
    Database(const std::string& path = "job_market.db");
    ~Database();

    // Job operations
    bool storeJob(const Job& job);
    bool storeJobs(const std::vector<Job>& jobs);
    std::vector<Job> loadJobs();
    std::vector<Job> searchJobs(const std::string& query,
                               const std::string& location = "",
                               const std::string& technology = "",
                               double min_salary = 0,
                               int limit = 100);

    // Analytics and statistics
    std::map<std::string, int> getJobCountByCompany();
    std::map<std::string, int> getJobCountByTechnology();
    std::map<std::string, int> getJobCountByLocation();
    std::map<std::string, double> getAverageSalaryByTechnology();
    std::map<std::string, int> getDailyJobTrends(int days = 30);

    // Cache management
    void clearCache();
    void refreshCache();

    // Utility functions
    bool clearOldJobs(int days_old = 30);
    int getTotalJobCount();
    bool isJobExists(const std::string& job_id);

    // Backup and maintenance
    bool backupDatabase(const std::string& backup_path);
    bool optimizeDatabase();
};

#endif
