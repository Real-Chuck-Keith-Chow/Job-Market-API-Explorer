# 🚀 Job Market API Explorer (C++17)

A clean, modern **C++17 console application** that fetches real job listings from the **Adzuna API**, displays structured results, and provides lightweight market insights.

This project focuses on practical C++ engineering, including:
- REST API integration
- JSON parsing
- Modular software design
- Pagination
- Filtering
- Build systems
- Optional SQLite persistence

---

# ✨ Highlights

- 🔌 Live job search via Adzuna REST API
- 📊 Basic market statistics and salary insights
- 🌍 Remote-only job filtering
- 📄 Multi-page API fetching (pagination)
- ⚙️ Clean, simplified C++17 architecture
- 🧰 Uses libcurl + nlohmann/json
- 🏗 Supports both CMake and Makefile builds
- 🗄 Optional SQLite persistence layer

---

# 🧠 What This Project Demonstrates

| Skill Area | Description |
|---|---|
| C++17 | STL, RAII, modular architecture |
| Networking | HTTP requests using libcurl |
| Data Parsing | JSON parsing with nlohmann/json |
| Build Systems | CMake + Makefile workflows |
| API Design | Pagination and filtering |
| Databases | Basic SQLite integration |
| Code Quality | Simplified, maintainable structure |

---

# 🔍 Features

## Search jobs by:
- Keyword
- Location
- Minimum salary

## Filtering:
- Remote-only jobs
- Technology keyword filtering

## Pagination:
- Fetch multiple pages of API results
- Adjustable page count from CLI

## Display:
- Job title
- Company
- Location
- Salary range
- Posting date
- Application URL

## Statistics:
- Top hiring companies
- Average minimum salary
- Job result counts

---

# ⚠️ Notes

- GitHub Jobs API support was removed because the service is discontinued
- Codebase intentionally simplified to reduce unnecessary complexity
- SQLite layer exists but remains optional/minimal
- Designed primarily as a portfolio/demo engineering project

---

# 🧰 Tech Stack

- **Language:** C++17
- **HTTP Client:** libcurl
- **JSON Parsing:** nlohmann/json
- **Database:** SQLite3
- **Build Systems:** CMake + Makefile

---

# 📦 Prerequisites

- C++17-compatible compiler
- libcurl
- SQLite3
- CMake (optional)

---

# 🐧 Linux (Ubuntu)

```bash
sudo apt install build-essential libcurl4-openssl-dev libsqlite3-dev
