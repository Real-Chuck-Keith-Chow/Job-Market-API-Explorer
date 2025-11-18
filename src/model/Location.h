#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <map>

struct Location {
    std::string display_name;
    std::string area;
    std::string city;
    std::string state;
    std::string country;
    std::string country_code;
    double latitude;
    double longitude;
    
    // Additional metadata
    std::string timezone;
    int population;
    bool is_remote;
    
    Location() 
        : latitude(0.0), 
          longitude(0.0), 
          population(0),
          is_remote(false) {}
    
    // Utility methods
    std::string getFullAddress() const {
        std::string address;
        if (!city.empty()) address += city;
        if (!state.empty()) address += (address.empty() ? "" : ", ") + state;
        if (!country.empty()) address += (address.empty() ? "" : ", ") + country;
        return address.empty() ? display_name : address;
    }
    
    std::string getCountryFlag() const {
        // Simple emoji flag based on country code
        std::map<std::string, std::string> flag_map = {
            {"US", "🇺🇸"}, {"CA", "🇨🇦"}, {"GB", "🇬🇧"}, 
            {"DE", "🇩🇪"}, {"FR", "🇫🇷"}, {"AU", "🇦🇺"},
            {"IN", "🇮🇳"}, {"CN", "🇨🇳"}, {"JP", "🇯🇵"},
            {"BR", "🇧🇷"}, {"MX", "🇲🇽"}, {"ES", "🇪🇸"}
        };
        
        auto it = flag_map.find(country_code);
        return it != flag_map.end() ? it->second : "🏳️";
    }
    
    bool isValid() const {
        return !display_name.empty() || 
               (!city.empty() && !country.empty()) ||
               is_remote;
    }
    
    bool isInCountry(const std::string& target_country) const {
        return country.find(target_country) != std::string::npos ||
               country_code.find(target_country) != std::string::npos;
    }
    
    bool isInState(const std::string& target_state) const {
        return state.find(target_state) != std::string::npos;
    }
    
    double distanceTo(const Location& other) const {
        if (latitude == 0.0 || longitude == 0.0 || 
            other.latitude == 0.0 || other.longitude == 0.0) {
            return -1.0; // Invalid coordinates
        }
        
        // Haversine formula for distance calculation
        double lat1 = latitude * M_PI / 180.0;
        double lon1 = longitude * M_PI / 180.0;
        double lat2 = other.latitude * M_PI / 180.0;
        double lon2 = other.longitude * M_PI / 180.0;
        
        double dlat = lat2 - lat1;
        double dlon = lon2 - lon1;
        
        double a = sin(dlat/2) * sin(dlat/2) + 
                   cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));
        
        return 6371.0 * c; // Distance in kilometers
    }
    
    std::string toString() const {
        if (is_remote) return "Remote";
        
        std::string result;
        if (!city.empty()) result += city;
        if (!state.empty()) result += (result.empty() ? "" : ", ") + state;
        if (!country.empty()) result += (result.empty() ? "" : ", ") + country;
        
        return result.empty() ? display_name : result;
    }
    
    static Location createRemote() {
        Location remote;
        remote.display_name = "Remote";
        remote.is_remote = true;
        return remote;
    }
    
    static Location createFromString(const std::string& location_str) {
        Location loc;
        loc.display_name = location_str;
        
        // Simple parsing logic (can be enhanced)
        if (location_str.find("Remote") != std::string::npos ||
            location_str.find("remote") != std::string::npos) {
            loc.is_remote = true;
        }
        
        return loc;
    }
};

// Location comparison and utility functions
struct LocationCompare {
    static bool byCity(const Location& a, const Location& b) {
        return a.city < b.city;
    }
    
    static bool byCountry(const Location& a, const Location& b) {
        return a.country < b.country;
    }
    
    static bool byPopulation(const Location& a, const Location& b) {
        return a.population > b.population;
    }
};

#endif
