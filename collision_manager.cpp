#include "collision_manager.hpp"
#include "collision_parser.hpp"
#include "collision.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <optional>
#include <vector>
#include <map>


CollisionManager::CollisionManager(const std::string& filename) {
    CollisionParser parser{filename};

    try {
        this->collisions = parser.parse();
        this->initialization_error = "";
    } catch (const std::runtime_error& e) {
        this->collisions = {};
        this->initialization_error = e.what();
    }
}

bool CollisionManager::is_initialized() {
    return this->initialization_error.empty();
}

const std::string& CollisionManager::get_initialization_error() {
    return this->initialization_error;
}

const std::vector<const Collision*> CollisionManager::search() {

    std::vector<const Collision*> results;

    for (const Collision& collision : this->collisions) {
        if (collision.match()) {
            results.push_back(&collision);
        }
    }
    return results;
}

const std::vector<const Collision*> CollisionManager::getAllCollisions() {
    std::vector<const Collision*> results;

    for(const Collision& collision : this->collisions) {
        results.push_back(&collision);
    }
    return results;
}

const std::vector<const Collision*> CollisionManager::getCollisionRange(const std::chrono::year_month_day& startDate,const std::chrono::year_month_day& endDate) {

    if(!startDate.ok() && !endDate.ok()) {
        std::cerr << "Invalid date provided" << std::endl;
        return std::vector<const Collision*>();
    } else if (!startDate.ok()) {
        std::cerr << "Invalid Start date provided" << std::endl;
        return std::vector<const Collision*>();
    } else if (!endDate.ok()) {
        std::cerr << "Invalid End date provided" << std::endl;
        return std::vector<const Collision*>();
    }

    std::vector<const Collision*> results;

    for(const Collision& collision : this->collisions) {
        if(collision.crash_date.has_value()) {
            const std::chrono::year_month_day date = collision.crash_date.value();
            if (date >= startDate && date <= endDate) {
                results.push_back(&collision);
            }
        }
    }
    return results;

}

size_t CollisionManager::getCollisionCount(const std::chrono::year_month_day& collisionDate) {

    if(!collisionDate.ok()) {
        std::cerr << "Invalid date provided" << std::endl;
        return 0;
    }

    std::vector<const Collision*> results;
    for(const Collision& collision : this->collisions) {
        if(collision.crash_date.has_value()) {
            const std::chrono::year_month_day date = collision.crash_date.value();
            if (date == collisionDate) {
                results.push_back(&collision);
            }
        }
    }
    return results.size();
}

std::pair<const std::chrono::year_month_day , size_t> CollisionManager::getMaxCollisionsbyDate() {
    std::map<std::chrono::year_month_day, size_t> collisionsByDate;

    for (const Collision &collision : this->collisions) {
        if (collision.crash_date.has_value()) {
            const std::chrono::year_month_day date = collision.crash_date.value();
            if (date.ok()) {
                collisionsByDate[date]++;
            }
        }
    }
    std::chrono::year_month_day maxDate;
    size_t maxCount = 0;

    for(const auto& [date, count] : collisionsByDate) {
        if(count>maxCount) {
            maxCount = count;
            maxDate = date;
        }
    }

    return {maxDate , maxCount};
}

std::pair<size_t, size_t> CollisionManager::getCollisionsByTimeRange() {
    std::map<std::chrono::minutes, size_t> collisionsByTime;

    size_t dayCount = 0;
    size_t nightCount = 0;

    for(const Collision &collision : this->collisions) {
        if(collision.crash_time.has_value()) {
            auto time = collision.crash_time.value().to_duration();

            if (time >= std::chrono::hours(6) && time < std::chrono::hours(18)){
                dayCount++;
            } else {
                nightCount++;
            }
        }
    }
    return { dayCount, nightCount};
}

std::pair<const std::string, size_t> CollisionManager::getMaxCollisionsbyBorough() {
    std::map<std::string, size_t> collisionsByBorough;

    for(const Collision &collision : this->collisions) {
        if(collision.borough.has_value()) {
            const std::string boroughName = collision.borough.value();
            collisionsByBorough[boroughName]++;
        }
    }

    std::string maxLocation;
    size_t maxCount = 0;

    for(const auto& [boroughName, count] : collisionsByBorough) {
        if(count > maxCount) {
            maxCount = count;
            maxLocation = boroughName;
        }
    }

    return {maxLocation , maxCount};
}

std::vector<std::pair<float, float>> CollisionManager::getCoordinatesByBorough(const std::string &borough) {

    std::vector<std::pair<float, float>> results ;
    for (const Collision &collision : this->collisions)
    {
        if (collision.borough.has_value())
        {
            const std::string boroughValue = collision.borough.value();

            if (collision.latitude.has_value() && collision.longitude.has_value()) {
                results.push_back({
                    collision.latitude.value(),
                    collision.longitude.value()
                });
            }
        }
    }
    return results;
}

std::vector<std::pair<std::string, size_t>> CollisionManager::getTopFiveStreetNames() {
    std::map<std::string, size_t> collisionsByStreet;
    for(const Collision &collision : this->collisions) {
        if(collision.on_street_name.has_value()) {
            const std::string streetname = collision.on_street_name.value();
            collisionsByStreet[streetname]++ ;
        }
    }

    std::vector<std::pair<std::string, float>> results(collisionsByStreet.begin(), collisionsByStreet.end());

    std::sort(results.begin(), results.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
    });

    return std::vector<std::pair<std::string, size_t>>(
        results.begin(),
        results.begin() + std::min(size_t(5), results.size())
    );
}

