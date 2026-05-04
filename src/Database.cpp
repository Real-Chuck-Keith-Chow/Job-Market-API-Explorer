#include "Database.h"

#include <iostream>

#include <sqlite3.h>

#include "JobParser.h"
#include "json.hpp"

using json = nlohmann::json;

Database::Database(const std::string& path)
    : database_path(path), cache_dirty(true) {
    initializeDatabase();
}

Database::~Database() = default;

void Database::initializeDatabase() {
    createTables();
}

void Database::createTables() {
    sqlite3* db = nullptr;

    if (sqlite3_open(database_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return;
    }

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS jobs (
            id TEXT PRIMARY KEY,
            title TEXT,
            company_name TEXT,
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
            last_updated TEXT DEFAULT CURRENT_TIMESTAMP
        );

        CREATE INDEX IF NOT EXISTS idx_jobs_title ON jobs(title);
        CREATE INDEX IF NOT EXISTS idx_jobs_company ON jobs(company_name);
        CREATE INDEX IF NOT EXISTS idx_jobs_location ON jobs(location_display);
        CREATE INDEX IF NOT EXISTS idx_jobs_salary ON jobs(salary_min, salary_max);
    )";

    char* error_message = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &error_message) != SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << '\n';
        sqlite3_free(error_message);
    }

    sqlite3_close(db);
}

bool Database::storeJob(const Job& job) {
    sqlite3* db = nullptr;

    if (sqlite3_open(database_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return false;
    }

    const char* sql = R"(
        INSERT OR REPLACE INTO jobs
        (
            id,
            title,
            company_name,
            company_id,
            location_display,
            location_area,
            location_country,
            salary_min,
            salary_max,
            description,
            redirect_url,
            technologies,
            category,
            created,
            last_updated
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, CURRENT_TIMESTAMP);
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return false;
    }

    auto technologies = JobParser::extractTechnologies(job.description);
    std::string technologies_json = json(technologies).dump();
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
    sqlite3_bind_text(stmt, 12, technologies_json.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 13, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 14, job.created.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;

    if (!success) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    cache_dirty = true;
    return success;
}

bool Database::storeJobs(const std::vector<Job>& jobs) {
    bool all_success = true;

    for (const auto& job : jobs) {
        if (!storeJob(job)) {
            all_success = false;
        }
    }

    return all_success;
}

std::vector<Job> Database::loadJobs() {
    if (!cache_dirty) {
        return job_cache;
    }

    updateCache();
    return job_cache;
}

void Database::updateCache() {
    job_cache.clear();

    sqlite3* db = nullptr;

    if (sqlite3_open(database_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return;
    }

    const char* sql = R"(
        SELECT
            id,
            title,
            company_name,
            company_id,
            location_display,
            location_area,
            location_country,
            salary_min,
            salary_max,
            description,
            redirect_url,
            technologies,
            category,
            created
        FROM jobs
        ORDER BY created DESC;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Job job;

        auto textAt = [&](int col) -> std::string {
            const unsigned char* text = sqlite3_column_text(stmt, col);
            return text ? reinterpret_cast<const char*>(text) : "";
        };

        job.id = textAt(0);
        job.title = textAt(1);
        job.company.display_name = textAt(2);
        job.company.id = textAt(3);
        job.location.display_name = textAt(4);
        job.location.area = textAt(5);
        job.location.country = textAt(6);
        job.salary_min = sqlite3_column_double(stmt, 7);
        job.salary_max = sqlite3_column_double(stmt, 8);
        job.description = textAt(9);
        job.redirect_url = textAt(10);

        try {
            json technologies = json::parse(textAt(11));

            if (technologies.is_array()) {
                for (const auto& tech : technologies) {
                    if (tech.is_string()) {
                        job.technologies.push_back(tech.get<std::string>());
                    }
                }
            }
        } catch (...) {
            job.technologies.clear();
        }

        job.created = textAt(13);

        job_cache.push_back(job);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    cache_dirty = false;
}

void Database::clearCache() {
    job_cache.clear();
    cache_dirty = true;
}

void Database::refreshCache() {
    cache_dirty = true;
    updateCache();
}

bool Database::isJobExists(const std::string& job_id) {
    sqlite3* db = nullptr;

    if (sqlite3_open(database_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return false;
    }

    const char* sql = "SELECT 1 FROM jobs WHERE id = ? LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, job_id.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = sqlite3_step(stmt) == SQLITE_ROW;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return exists;
}
