#include "collision_manager.hpp"
#include "collision_parser.hpp"
#include "collision.hpp"

#include <string>
#include <chrono>
#include <optional>
#include <vector>


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