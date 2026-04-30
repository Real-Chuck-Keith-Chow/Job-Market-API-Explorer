# 🚀 Job Market API Explorer (C++17)

Job Market API Explorer is a modern **C++17 console application** that queries public job listing APIs, displays structured job results, and computes basic market statistics such as **top hiring companies** and **average minimum salary**.

This project is designed to showcase **real-world C++ engineering skills**, including API integration, JSON parsing, networking, and CMake-based builds.

---

## ✨ Highlights

- 🔌 Live job search via public REST APIs (Adzuna)
- 📊 Aggregates results into useful market statistics
- ⚙️ Modern C++17 with clean, modular design
- 🧰 Industry-standard libraries (libcurl, nlohmann/json)
- 🏗 Cross-platform build using CMake
- 🗄 Optional SQLite persistence layer (scaffolded)

---

## 🧠 What This Project Demonstrates

| Skill Area | Description |
|-----------|-------------|
| **C++17** | STL usage, RAII, modular architecture |
| **Networking** | REST API requests via libcurl |
| **Data Parsing** | JSON handling with nlohmann/json |
| **Build Systems** | CMake configuration and toolchains |
| **Dependency Management** | vcpkg + vendored headers |
| **Extensibility** | Multi-API design & optional database layer |

---

## 🔍 Features

- Search jobs by **keyword**, **location**, and **minimum salary**
- Display formatted job listings in the terminal
- Compute summary statistics:
  - Top hiring companies
  - Average minimum salary
- Architecture supports multiple job APIs  
  (GitHub Jobs stub included for demonstration)

---

## 🧰 Tech Stack

- **Language:** C++17  
- **Build System:** CMake (≥ 3.12)  
- **HTTP Client:** libcurl (OpenSSL)  
- **JSON Parsing:** nlohmann/json (vendored)  
- **Database (optional):** SQLite3  

---

## 📦 Prerequisites

- C++17-compatible compiler  
  - Tested with MinGW-w64 GCC 13
- CMake ≥ 3.12
- Ninja or Make
- libcurl development libraries
- (Optional) SQLite3 development libraries

---

## 🪟 Windows (MinGW) Quick Start — vcpkg

```bash
# 1) Install vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\tools\vcpkg
C:\tools\vcpkg\bootstrap-vcpkg.bat

# 2) Install curl
C:\tools\vcpkg\vcpkg install curl:x64-mingw-dynamic --host-triplet x64-mingw-dynamic

# (Optional) SQLite support
C:\tools\vcpkg\vcpkg install sqlite3:x64-mingw-dynamic --host-triplet x64-mingw-dynamic

---



