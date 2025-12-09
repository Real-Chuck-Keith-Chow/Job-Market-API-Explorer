Job Market API Explorer
=======================

Small C++17 console app that queries public job APIs, displays results, and prints basic stats (top companies, average minimum salary). Currently wired for Adzuna; a GitHub Jobs endpoint is included but that public API is deprecated.

Features
--------
- Query Adzuna with search term, location, and optional minimum salary filter.
- Prints job details and a short summary (top companies, avg minimum salary).
- JSON parsing via nlohmann/json; HTTP via libcurl; optional SQLite stubs exist but are not fully wired.

Prerequisites
-------------
- CMake ≥ 3.12
- C++17 compiler (tested with MinGW-w64 GCC 13) and Ninja or Make
- libcurl (with OpenSSL) headers and libs
- nlohmann/json is vendored at `third_party/json.hpp`
- (Optional) SQLite3 dev package if you plan to use `Database.cpp`

Windows + MinGW quickstart with vcpkg
-------------------------------------hell
# 1) Install vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\tools\vcpkg
C:\tools\vcpkg\bootstrap-vcpkg.bat

# 2) Install curl for MinGW (adjust triplet if needed)
C:\tools\vcpkg\vcpkg install curl:x64-mingw-dynamic --host-triplet x64-mingw-dynamic

# (Optional) SQLite
C:\tools\vcpkg\vcpkg install sqlite3:x64-mingw-dynamic --host-triplet x64-mingw-dynamicConfiguration
-------------
Create `config.json` in the project root with your Adzuna credentials:
{
  "adzuna_app_id": "your_app_id",
  "adzuna_app_key": "your_app_key"
}Build
-----
From the repo root:hell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
cmake --build build --config Release- Omit `--config Release` if you’re using a single-config generator (e.g., Makefiles).
- If you installed SQLite, also add `-DWITH_SQLITE=ON` and update CMake to `find_package(SQLite3 REQUIRED)` and link it.

Run
---hell
.\build\job_explorer.exeEnter a search query, location, and minimum salary when prompted; results and stats print to the terminal.

Notes and Known Gaps
--------------------
- GitHub Jobs endpoint is deprecated; expect empty responses unless you swap in another API.
- `Database.cpp` references SQLite but the default CMake only links curl/OpenSSL. Add SQLite find_package + linking if you enable it.
- Some headers still use `model/` includes; ensure include paths match `src/model`.

License
-------
MIT (if provided). Pull requests are welcome.
