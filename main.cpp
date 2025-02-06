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

    std::vector<const Collision*> collisions = collision_manager.getAllCollisions();
    std::vector<const Collision*> collisionsrange = collision_manager.getCollisionRange(startDate,endDate);
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
}
