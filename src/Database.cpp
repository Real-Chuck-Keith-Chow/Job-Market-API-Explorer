#include "Database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <sqlite3.h>
#include "third_party/json.hpp"
#include "JobParser.h"

using json = nlohmann::json;

Database::Database(const std::string& path) : database_path(path), cache_dirty(true) {
    initializeDatabase();
}

Database::~Database() {
    // Cleanup if needed
}

void Database::initializeDatabase() {
    sqlite3* db;
    int rc = sqlite3_open(database_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    createTables();
    sqlite3_close(db);
}

void Database::createTables() {
    sqlite3* db;
    int rc = sqlite3_open(database_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS jobs (
            id TEXT PRIMARY KEY,
            title TEXT NOT NULL,
            company_name TEXT NOT NULL,
            company_id TEXT,
            location_display TEXT,
            location_area TEXT,
            location_country TEXT,
            salary_min REAL,
            salary_max REAL,
            description TEXT,
            redirect_url TEXT,
            technologies TEXT,
            category TEXT,
            created TEXT,
            last_updated TEXT DEFAULT CURRENT_TIMESTAMP,
            source_api TEXT
        );
        
        CREATE TABLE IF NOT EXISTS companies (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            normalized_name TEXT,
            job_count INTEGER DEFAULT 0,
            last_seen TEXT
        );
        
        CREATE TABLE IF NOT EXISTS technology_trends (
            technology TEXT PRIMARY KEY,
            job_count INTEGER DEFAULT 0,
            average_salary REAL,
            last_updated TEXT
        );
        
        CREATE INDEX IF NOT EXISTS idx_jobs_title ON jobs(title);
        CREATE INDEX IF NOT EXISTS idx_jobs_company ON jobs(company_name);
        CREATE INDEX IF NOT EXISTS idx_jobs_location ON jobs(location_display);
        CREATE INDEX IF NOT EXISTS idx_jobs_salary ON jobs(salary_min, salary_max);
        CREATE INDEX IF NOT EXISTS idx_jobs_created ON jobs(created);
        CREATE INDEX IF NOT EXISTS idx_jobs_technologies ON jobs(technologies);
    )";
    
    char* error_message = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &error_message);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << std::endl;
        sqlite3_free(error_message);
    }
    
    sqlite3_close(db);
}

bool Database::storeJob(const Job& job) {
    sqlite3* db;
    int rc = sqlite3_open(database_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // Extract technologies
    auto technologies = JobParser::extractTechnologies(job.description);
    json tech_json = technologies;
    std::string tech_str = tech_json.dump();
    
    // Categorize job
    std::string category = JobParser::categorizeJob(job);
    
    const char* sql = R"(
        INSERT OR REPLACE INTO jobs 
        (id, title, company_name, company_id, location_display, location_area, 
         location_country, salary_min, salary_max, description, redirect_url, 
         technologies, category, created, last_updated)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, CURRENT_TIMESTAMP)
    )";
    
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, job.id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, job.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, job.company.display_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, job.company.id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, job.location.display_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, job.location.area.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, job.location.country.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 8, job.salary_min);
    sqlite3_bind_double(stmt, 9, job.salary_max);
    sqlite3_bind_text(stmt, 10, job.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 11, job.redirect_url.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 12, tech_str.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 13, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 14, job.created.c_str(), -1, SQLITE_TRANSIENT);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    cache_dirty = true;
    return rc == SQLITE_DONE;
}

bool Database::storeJobs(const std::vector<Job>& jobs) {
    if (jobs.empty()) return true;
    
    sqlite3* db;
    int rc = sqlite3_open(database_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // Begin transaction for better performance
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    
    const char* sql = R"(
        INSERT OR REPLACE INTO jobs 
        (id, title, company_name, company_id, location_display, location_area, 
         location_country, salary_min, salary_max, description, redirect_url, 
         technologies, category, created, last_updated)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, CURRENT_TIMESTAMP)
    )";
    
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    
    int success_count = 0;
    for (const auto& job : jobs) {
        // Extract technologies
        auto technologies = JobParser::extractTechnologies(job.description);
        json tech_json = technologies;
        std::string tech_str = tech_json.dump();
        
        // Categorize job
        std::string category = JobParser::categorizeJob(job);
        
        sqlite3_bind_text(stmt, 1, job.id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, job.title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, job.company.display_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, job.company.id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, job.location.display_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, job.location.area.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, job.location.country.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 8, job.salary_min);
        sqlite3_bind_double(stmt, 9, job.salary_max);
        sqlite3_bind_text(stmt, 10, job.description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 11, job.redirect_url.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 12, tech_str.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 13, category.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 14, job.created.c_str(), -1, SQLITE_TRANSIENT);
        
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            success_count++;
        }
        
        sqlite3_reset(stmt);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    sqlite3_close(db);
    
    cache_dirty = true;
    std::cout << "Stored " << success_count << "/" << jobs.size() << " jobs in database" << std::endl;
    return success_count > 0;
}

std::vector<Job> Database::loadJobs() {
    if (!cache_dirty && !job_cache.empty()) {
        return job_cache;
    }
    
    updateCache();
    return job_cache;
}

void Database::updateCache() {
    job_cache.clear();
    
    sqlite3* db;
    int rc = sqlite3_open(database_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    
    const char* sql = "SELECT * FROM jobs ORDER BY created DESC LIMIT 1000";
    sqlite3_stmt* stmt;
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Job job;
        
        job.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        job.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        job.company.display_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        job.company.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        job.location.display_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        job.location.area = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        job.location.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        job.salary_min = sqlite3_column_double(stmt, 7);
        job.salary_max = sqlite3_column_double(stmt, 8);
        job.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        job.redirect_url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        
        // Parse technologies JSON
        const char* tech_json = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        if (tech_json) {
            try {
                json tech_data = json::parse(tech_json);
                for (const auto& tech : tech_data) {
                    job.technologies.push_back(tech);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing technologies: " << e.what() << std::endl;
            }
        }
        
        job.created = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
        
        job_cache.push_back(job);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    cache_dirty = false;
}

// Additional method implementations would follow similar patterns...
// [Rest of the methods would be implemented following the same SQLite pattern]

std::vector<Job> Database::searchJobs(const std::string& query, 
                                     const std::string& location,
                                     const std::string& technology,
                                     double min_salary,
                                     int limit) {
    std::vector<Job> results;
    
    // This would implement sophisticated SQL queries with the given filters
    // Implementation would be similar to loadJobs() but with WHERE clauses
    
    return results;
}

std::map<std::string, int> Database::getJobCountByCompany() {
    std::map<std::string, int> counts;
    // SQL implementation to count jobs by company
    return counts;
}

// Other analytics methods would be implemented similarly...

void Database::clearCache() {
    job_cache.clear();
    cache_dirty = true;
}

void Database::refreshCache() {
    cache_dirty = true;
    updateCache();
}

bool Database::isJobExists(const std::string& job_id) {
    sqlite3* db;
    int rc = sqlite3_open(database_path.c_str(), &db);
    
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    const char* sql = "SELECT 1 FROM jobs WHERE id = ? LIMIT 1";
    sqlite3_stmt* stmt;
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, job_id.c_str(), -1, SQLITE_TRANSIENT);
    
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return exists;
}
