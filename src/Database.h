#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

#include "model/Job.h"

class Database {
private:
    std::string database_path;
    std::vector<Job> job_cache;
    bool cache_dirty;

    void initializeDatabase();
    void createTables();
    void updateCache();

public:
    explicit Database(const std::string& path = "job_market.db");
    ~Database();

    bool storeJob(const Job& job);
    bool storeJobs(const std::vector<Job>& jobs);

    std::vector<Job> loadJobs();

    void clearCache();
    void refreshCache();

    bool isJobExists(const std::string& job_id);
};

#endif
