#include "collision.hpp"
#include "collision_manager.hpp"
#include "query.hpp"

#include <filesystem>
#include <iostream>
#include <string>


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

    //collision_manager.print_bucket();
    //collision_manager.fetch_collisions_selective("BROOKLYN");
    collision_manager.fetch_collisions("MANHATTAN");
    collision_manager.fetch_collisions_parallel("MANHATTAN");
    collision_manager.fetch_collisions_selective("MANHATTAN");
    collision_manager.fetch_collisions_selective_parallel("MANHATTAN");

    /* 
    Query query = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    std::vector<Collision> results = collision_manager.search(query);
    std::cout << "Search all" << results.size() << std::endl; */

    
    /*
    for (Collision& col : results){
        std::cout << col << std::endl;
    }*/

/*
    std::vector<const Collision*> collisions = collision_manager.search();
    std::cout << *collisions.at(0) << std::endl;
    std::cout << *collisions.at(1) << std::endl;
    std::cout << *collisions.at(2) << std::endl;
    std::cout << *collisions.at(3) << std::endl;
    std::cout << *collisions.at(4) << std::endl;
    std::cout << *collisions.at(5) << std::endl;
*/
    /*
    Query query = Query::create("latitude", QueryType::LESS_THAN, 100000.0f);
    Query query2 = Query::create("collision_id", QueryType::LESS_THAN, 10000ULL);
    Query query3 = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    //Query query = Query::create("crash_date", QueryType::LESS_THAN, 1ULL);
    //Query query = Query::create("crash_time", QueryType::LESS_THAN, 1ULL);

    std::vector<Collision> collisions = collision_manager.search(query3);
    std::cout << collisions.at(0) << std::endl;
    std::cout << collisions.at(1) << std::endl;
    std::cout << collisions.at(2) << std::endl;
    std::cout << collisions.at(3) << std::endl;
    std::cout << collisions.at(4) << std::endl;


    Query query4 = Query::create("borough", Qualifier::NOT, QueryType::EQUALS, "BROOKLYN");
    collisions = collision_manager.search(query4);
    std::cout << collisions.at(0) << std::endl;
    std::cout << collisions.at(1) << std::endl;
    std::cout << collisions.at(2) << std::endl;
    std::cout << collisions.at(3) << std::endl;
    std::cout << collisions.at(4) << std::endl;
    */
}
