#include "collision_manager.hpp"
#include "collision_parser.hpp"
#include "query.hpp"

#include <string>


CollisionManager::CollisionManager(const std::string& filename) {
    CollisionParser parser{filename};

    try {
        this->collisions_ = parser.parse();
        this->initialization_error_ = "";
    } catch (const std::runtime_error& e) {
        this->collisions_ = {};
        this->initialization_error_ = e.what();
    }
}

CollisionManager::CollisionManager(std::vector<Collision>& collisions) {
    this->collisions_ = collisions;
}

bool CollisionManager::is_initialized() {
    return this->initialization_error_.empty();
}

const std::string& CollisionManager::get_initialization_error() {
    return this->initialization_error_;
}

const std::vector<const Collision*> CollisionManager::search(const Query& query) {
    std::vector<const Collision*> results;

    for (const Collision& collision : this->collisions_) {
        if (collision.match(query)) {
            results.push_back(&collision);
        }
    }

    return results;
}
