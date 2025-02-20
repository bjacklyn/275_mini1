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

/*
    std::vector<const Collision*> collisions = collision_manager.search();
    std::cout << *collisions.at(0) << std::endl;
    std::cout << *collisions.at(1) << std::endl;
    std::cout << *collisions.at(2) << std::endl;
    std::cout << *collisions.at(3) << std::endl;
    std::cout << *collisions.at(4) << std::endl;
    std::cout << *collisions.at(5) << std::endl;
*/
    Query query = Query::create("latitude", QueryType::LESS_THAN, 100000.0f);
    Query query2 = Query::create("collision_id", QueryType::LESS_THAN, 10000ULL);
    Query query3 = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    //Query query = Query::create("crash_date", QueryType::LESS_THAN, 1ULL);
    //Query query = Query::create("crash_time", QueryType::LESS_THAN, 1ULL);

    std::vector<const Collision*> collisions = collision_manager.searchOpenMp(query3);
    std::cout << *collisions.at(0) << std::endl;
    std::cout << *collisions.at(1) << std::endl;
    std::cout << *collisions.at(2) << std::endl;
    std::cout << *collisions.at(3) << std::endl;
    std::cout << *collisions.at(4) << std::endl;


    Query query4 = Query::create("borough", Qualifier::NOT, QueryType::EQUALS, "BROOKLYN");
    collisions = collision_manager.searchOpenMp(query4);
    std::cout << *collisions.at(0) << std::endl;
    std::cout << *collisions.at(1) << std::endl;
    std::cout << *collisions.at(2) << std::endl;
    std::cout << *collisions.at(3) << std::endl;
    std::cout << *collisions.at(4) << std::endl;

   std::string borough = "BROOKLYN";
    float latitude = 40.667202;
    float longitude = -73.8665;

    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{1},
        std::chrono::day{1}};

    std::chrono::year_month_day date2{
        std::chrono::year{2022},
        std::chrono::month{12},
        std::chrono::day{31}};

 // The range we're searching
    float epsilon = 0.01f; // approximately 1 km radius

    Query query6 = Query::create("borough", QueryType::EQUALS, borough)
                       .add("latitude", QueryType::GREATER_THAN, latitude - epsilon)
                       .add("latitude", QueryType::LESS_THAN, latitude + epsilon)
                       .add("longitude", QueryType::GREATER_THAN, longitude - epsilon)
                       .add("longitude", QueryType::LESS_THAN, longitude + epsilon)
                       .add("crash_date", QueryType::GREATER_THAN, date1)
                       .add("crash_date", QueryType::LESS_THAN, date2);

    collisions = collision_manager.searchOpenMp(query6);


    std::stringstream lat_range, lon_range;
    lat_range << std::fixed << std::setprecision(6)
              << (latitude - epsilon) << " to " << (latitude + epsilon);
    lon_range << std::fixed << std::setprecision(6)
              << (longitude - epsilon) << " to " << (longitude + epsilon);

    std::cout << "\nQuery Parameters:" << std::endl;
    std::cout << std::setfill('-') << std::setw(85) << "-" << std::endl;
    std::cout << std::setfill(' ')
              << std::left << std::setw(15) << "Borough"
              << std::setw(35) << "Latitude Range"
              << std::setw(35) << "Longitude Range" << std::endl;
    std::cout << std::setfill('-') << std::setw(85) << "-" << std::endl;
    std::cout << std::setfill(' ')
              << std::left << std::setw(15) << borough
              << std::setw(35) << lat_range.str()
              << std::setw(35) << lon_range.str()
              << std::endl;

    if (collisions.empty())
    {
        std::cout << "No matching collisions found" << std::endl;
    }
    else
    {
        std::cout << "Found " << collisions.size() << " matching collisions:\n"
                  << std::endl;

        std::cout << "Top 10 Results:" << std::endl;

        std::cout << std::setfill('-') << std::setw(90) << "-" << std::endl;
        std::cout << std::setfill(' ')
                  << std::left << std::setw(12) << "ID"
                  << std::setw(15) << "Date"
                  << std::setw(10) << "Time"
                  << std::setw(15) << "Borough"
                  << std::setw(15) << "Latitude"
                  << std::setw(15) << "Longitude"
                  << std::setw(8) << "Injured" << std::endl;
        std::cout << std::setfill('-') << std::setw(90) << "-" << std::endl;

        for (size_t i = 0; i < std::min(collisions.size(), size_t(10)); ++i)
        {
            const auto *collision = collisions[i];
            std::cout << std::setfill(' ')
                      << std::left
                      << std::setw(12) << (collision->collision_id.has_value() ? std::to_string(collision->collision_id.value()) : "N/A")
                      << std::setw(15) << (collision->crash_date.has_value() ? (std::to_string(int(collision->crash_date.value().year())) + "-" + std::to_string(unsigned(collision->crash_date.value().month())) + "-" + std::to_string(unsigned(collision->crash_date.value().day()))) : "N/A")
                      << std::setw(10) << (collision->crash_time.has_value() ? (std::to_string(int(collision->crash_time.value().hours().count())) + ":" + std::to_string(int(collision->crash_time.value().minutes().count()))) : "N/A")
                      << std::setw(15) << (collision->borough.has_value() ? collision->borough.value() : "N/A")
                      << std::fixed << std::setprecision(6)
                      << std::setw(15) << (collision->latitude.has_value() ? std::to_string(collision->latitude.value()) : "N/A")
                      << std::setw(15) << (collision->longitude.has_value() ? std::to_string(collision->longitude.value()) : "N/A")
                      << std::setw(8) << (collision->number_of_persons_injured.has_value() ? std::to_string(collision->number_of_persons_injured.value()) : "N/A")
                      << std::endl;
        }

        std::cout << std::setfill('-') << std::setw(90) << "-" << std::endl;
    }
}
