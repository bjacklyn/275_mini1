#include "collision.hpp"
#include "collision_manager.hpp"

#include <filesystem>
#include <iostream>
#include <string>

using namespace std::chrono;

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path to CSV file to be parsed> " << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::filesystem::path filepath(filename);
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "Provided filename " << filename << " does not exist." << std::endl;
        return 1;
    } else if (!std::filesystem::is_regular_file(filepath)) {
        std::cerr << "Provided filename " << filename << " is not a file." << std::endl;
        return 1;
    }

    CollisionManager collision_manager{filename};
    if (!collision_manager.is_initialized()) {
        std::cerr << "Failed to initialize collisions: " << collision_manager.get_initialization_error() << std::endl;
        return 1;
    }

    auto startDate = year{2021}/month{9}/day{11};
    auto endDate = year{2021}/month{12}/day{12};
    //auto endDate = year{2025}/month{02}/day{31};

    auto collisionDate = year{2025}/month{02}/day{31};

    std::string borough = "BROOKLYN";

    // Get all collisions information
    std::vector<const Collision*> collisions = collision_manager.getAllCollisions();

    // Give a summary of the accidents using 2 specific dates
    std::vector<const Collision*> collisionsrange = collision_manager.getCollisionRange(startDate,endDate);

    // Date on which there were max number of collisions
    std::pair<const std::chrono::year_month_day, size_t> maxCollisions= collision_manager.getMaxCollisionsbyDate();

    // Number of collisions on a specific date
    size_t count = collision_manager.getCollisionCount(collisionDate) ;

    // Time range of the crashes - Whether they occurred during day or night
    std::pair<size_t , size_t> collisionsByTimeRange = collision_manager.getCollisionsByTimeRange();

    // Location with max number of accidents - based on borough
    std::pair<const std::string, size_t> collisionsByBorough = collision_manager.getMaxCollisionsbyBorough();

    // Latitude and longitude of the specific accident - using BOROUGH
    std::vector<std::pair<float, float>> coordinatesByBorough = collision_manager.getCoordinatesByBorough(borough);

    // Top 5 street names where crashes occurred frequently
    std::vector<std::pair<std::string, size_t>> collisionsByStreet = collision_manager.getTopFiveStreetNames();

    std::cout << *collisions.at(0) << std::endl;
    std::cout << *collisions.at(1) << std::endl;
    std::cout << *collisions.at(2) << std::endl;
    std::cout << *collisions.at(3) << std::endl;
    std::cout << *collisions.at(4) << std::endl;
    std::cout << *collisions.at(5) << std::endl;

    std::cout << " Collisions in Range: " << collisionsrange.size() << std::endl;
    std::cout << *collisionsrange.at(0) << std::endl;
    std::cout << *collisionsrange.at(1) << std::endl;
    std::cout << *collisionsrange.at(2) << std::endl;

    std::cout << " Collisions in Count: " << count << std::endl;

    std::cout <<" Maximum Collisions on date " << maxCollisions.first << " with count " << maxCollisions.second << std::endl;

    std::cout <<" Collisions in day time " << collisionsByTimeRange.first << " and Collisions at night time " << collisionsByTimeRange.second << std::endl;

    std::cout <<" There are max number of collisions in the location of " << collisionsByBorough.first << " and the count is " << collisionsByBorough.second << std::endl;

    std::cout << " Count of coordinates " << coordinatesByBorough.size() <<std::endl;

    std::cout << " The coordinates for " << borough << " are: " << std::endl;
    for(int i = 0 ; i<coordinatesByBorough.size();++i) {
        std::cout << " Latitude: " << coordinatesByBorough[i].first << " Longitude: " <<  coordinatesByBorough[i].second << std::endl;
    }

    std::cout << " Top 5 streetnames with max collisions are : " << std::endl;
    for(int i = 0 ; i<collisionsByStreet.size();++i) {
        std::cout << collisionsByStreet[i].first << std::endl ;
    }

}
