#include "collision_manager.hpp"
#include "collision_parser.hpp"
#include "query.hpp"

#include <string>

#include <omp.h>


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

const std::vector<const Collision*> CollisionManager::searchOpenMp(const Query& query) {
    std::vector<const Collision*> results;

    unsigned long num_threads = omp_get_max_threads();
    std::vector<std::vector<const Collision*>> thread_local_results(num_threads);

    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < collisions_.size(); ++i) {
        if (collisions_[i].match(query)) {
            int thread_id = omp_get_thread_num();
            thread_local_results[thread_id].push_back(&collisions_[i]);
        }
    }

    for (const auto& local_results : thread_local_results) {
        results.insert(results.end(), local_results.begin(), local_results.end());
    }

    return results;
}
