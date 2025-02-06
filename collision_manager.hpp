#pragma once

#include "collision.hpp"

#include <string>


class CollisionManager {

public:
    CollisionManager(const std::string& filename);
    bool is_initialized();
    const std::string& get_initialization_error();
    const std::vector<const Collision*> search();
    const std::vector<const Collision*> getAllCollisions();
    const std::vector<const Collision*> getCollisionRange(const std::chrono::year_month_day& startDate, const std::chrono::year_month_day& endDate);

private:
    std::string initialization_error;
    std::vector<Collision> collisions;

};
