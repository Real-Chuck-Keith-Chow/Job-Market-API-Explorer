# 🚀 Job Market API Explorer (C++17)

A clean, modern **C++17 console application** that fetches real job listings from the **Adzuna API**, displays structured results, and provides simple market insights.

This project focuses on **practical C++ engineering**, including HTTP requests, JSON parsing, modular design, and lightweight data persistence.

---

## ✨ Highlights

- 🔌 Live job search via Adzuna REST API  
- 📊 Basic market statistics (companies, salary trends)  
- ⚙️ Clean, simplified C++17 architecture  
- 🧰 Uses libcurl + nlohmann/json  
- 🏗 Works with both **CMake** and **Makefile**  
- 🗄 SQLite storage layer (basic, optional)

---

## 🧠 What This Project Demonstrates

| Skill Area | Description |
|----------|------------|
| C++17 | STL, RAII, modular design |
| Networking | HTTP requests using libcurl |
| Data Parsing | JSON parsing with nlohmann/json |
| Build Systems | CMake + Makefile support |
| Databases | Basic SQLite integration |
| Code Quality | Simplified, maintainable structure |

---

## 🔍 Features

- Search jobs by:
  - Keyword
  - Location
  - Minimum salary

- Display:
  - Job title
  - Company
  - Location
  - Salary range
  - Posting date

- Statistics:
  - Top hiring companies
  - Average minimum salary

---

## ⚠️ Notes

- GitHub Jobs API was removed (service is discontinued)
- Codebase intentionally simplified (no over-engineering)
- Database layer is minimal (can be extended)

---

## 🧰 Tech Stack

- **Language:** C++17  
- **HTTP Client:** libcurl  
- **JSON:** nlohmann/json (header-only)  
- **Database:** SQLite3  
- **Build Systems:** CMake + Makefile  

---

## 📦 Prerequisites

- C++17 compiler (GCC / Clang / MSVC)
- libcurl
- SQLite3
- CMake (optional)

### Linux (Ubuntu)

```bash
sudo apt install build-essential libcurl4-openssl-dev libsqlite3-dev
