#pragma once

#include "collision.hpp"

#include <string>


class CollisionManager {

public:
    CollisionManager(const std::string& filename);
    bool is_initialized();
    const std::string& get_initialization_error();
    const std::vector<const Collision*> search();

    // Get all collisions information
    const std::vector<const Collision*> getAllCollisions();

    // Give a summary of the accidents using 2 specific dates
    const std::vector<const Collision*> getCollisionRange(const std::chrono::year_month_day& startDate, const std::chrono::year_month_day& endDate);

    // Number of collisions on a specific date
    size_t getCollisionCount(const std::chrono::year_month_day& collisionDate);

    // Date on which there were max number of collisions
    std::pair<const std::chrono::year_month_day, size_t> getMaxCollisionsbyDate();

    // Time range of the crashes - Whether they occurred during day or night
    std::pair<size_t , size_t> getCollisionsByTimeRange();

    // Location with max number of accidents - based on borough
    std::pair<const std::string, size_t> getMaxCollisionsbyBorough();

    // Latitude and longitude of the specific accident - using BOROUGH
    std::vector<std::pair<float, float>> getCoordinatesByBorough(const std::string& borough);

    // Top 5 street names where crashes occurred frequently
    std::vector<std::pair<std::string, size_t>> getTopFiveStreetNames();

private:
    std::string initialization_error;
    std::vector<Collision> collisions;

};
