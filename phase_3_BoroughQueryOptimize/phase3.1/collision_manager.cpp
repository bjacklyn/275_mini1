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

CollisionManager::CollisionManager(Collisions& collisions) {
    this->collisions_ = collisions;
}

bool CollisionManager::is_initialized() {
    return this->initialization_error_.empty();
}

const std::string& CollisionManager::get_initialization_error() {
    return this->initialization_error_;
}

const Collision CollisionManager::index_to_collision(const std::size_t index) {
    Collision collision{};
    collision.crash_date = &(collisions_.crash_dates[index]);
    collision.crash_time = &(collisions_.crash_times[index]);
    collision.borough = &(collisions_.boroughs[index]);
    collision.zip_code = &(collisions_.zip_codes[index]);
    collision.latitude = &(collisions_.latitudes[index]);
    collision.longitude = &(collisions_.longitudes[index]);
    collision.location = &(collisions_.locations[index]);
    collision.on_street_name = &(collisions_.on_street_names[index]);
    collision.cross_street_name = &(collisions_.cross_street_names[index]);
    collision.off_street_name = &(collisions_.off_street_names[index]);
    collision.number_of_persons_injured = &(collisions_.numbers_of_persons_injured[index]);
    collision.number_of_persons_killed = &(collisions_.numbers_of_persons_killed[index]);
    collision.number_of_pedestrians_injured = &(collisions_.numbers_of_pedestrians_injured[index]);
    collision.number_of_pedestrians_killed = &(collisions_.numbers_of_pedestrians_killed[index]);
    collision.number_of_cyclist_injured = &(collisions_.numbers_of_cyclist_injured[index]);
    collision.number_of_cyclist_killed = &(collisions_.numbers_of_cyclist_killed[index]);
    collision.number_of_motorist_injured = &(collisions_.numbers_of_motorist_injured[index]);
    collision.number_of_motorist_killed = &(collisions_.numbers_of_motorist_killed[index]);
    collision.contributing_factor_vehicle_1 = &(collisions_.contributing_factor_vehicles_1[index]);
    collision.contributing_factor_vehicle_2 = &(collisions_.contributing_factor_vehicles_2[index]);
    collision.contributing_factor_vehicle_3 = &(collisions_.contributing_factor_vehicles_3[index]);
    collision.contributing_factor_vehicle_4 = &(collisions_.contributing_factor_vehicles_4[index]);
    collision.contributing_factor_vehicle_5 = &(collisions_.contributing_factor_vehicles_5[index]);
    collision.collision_id = &(collisions_.collision_ids[index]);
    collision.vehicle_type_code_1 = &(collisions_.vehicle_type_codes_1[index]);
    collision.vehicle_type_code_2 = &(collisions_.vehicle_type_codes_2[index]);
    collision.vehicle_type_code_3 = &(collisions_.vehicle_type_codes_3[index]);
    collision.vehicle_type_code_4 = &(collisions_.vehicle_type_codes_4[index]);
    collision.vehicle_type_code_5 = &(collisions_.vehicle_type_codes_5[index]);
    return collision;
}

const std::vector<Collision> CollisionManager::search(const Query& query) {
    std::vector<Collision> results;

    for (std::size_t index = 0; index < collisions_.size(); ++index) {
        if (collisions_.match(query, index)) {
            results.push_back(index_to_collision(index));
        }
    }

    return results;
}

void CollisionManager::print_bucket(){
    
    std::cout << "inside print bucket";
    for (const auto& entry : this->collisions_.buckets) {
        std::cout << "Key: " << entry.first << " -> Indices: ";
        for (int index : entry.second) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }
}


void CollisionManager::fetch_collisions(std:: string borough){

    
    std::vector<Collision> results;
    
    auto it = this->collisions_.buckets.find(borough);
    

    if( it != this->collisions_.buckets.end()){

        for (int& index : it->second){

            results.push_back(index_to_collision(index));
        }

    }
    
    //std::cout << "Search only indices" << results.size() << std::endl ;

    /*for (Collision& col : results){
        std::cout << col << std::endl;
    }*/
    std::cout << "fetch_collisions_selective" << results.size() << std::endl ;

}


void CollisionManager::fetch_collisions_parallel(std::string borough){
    std::vector<Collision> results;

    auto it = this->collisions_.buckets.find(borough);

    if (it != this->collisions_.buckets.end()){
        
        std::vector<Collision> local_results(it->second.size());

        // Parallelize the loop that fetches collisions
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < it->second.size(); ++i) {
          
            local_results[i] = index_to_collision(it->second[i]);
        }

        // Merge the results from the parallel section back into the main results vector
        results.insert(results.end(), local_results.begin(), local_results.end());
    }

    //std::cout << "Search only indices parallel " << results.size() << std::endl;
}


void CollisionManager::fetch_collisions_selective(std:: string borough){

   std::vector<Collision_Selective> results;

    
    auto it = this->collisions_.buckets.find(borough);
    

    if( it != this->collisions_.buckets.end()){

        for (int& index : it->second){
            Collision_Selective data{};
            data.borough = &collisions_.boroughs[index];
            data.on_street_name = &collisions_.on_street_names[index];
            results.push_back(data);
            
        }

    }
    //std::cout << "fetch_collisions_selective" << results.size() << std::endl ;
}


void CollisionManager::fetch_collisions_selective_parallel(std::string borough){
    std::vector<Collision_Selective> results;

    auto it = this->collisions_.buckets.find(borough);

    if (it != this->collisions_.buckets.end()){
       
        std::vector<Collision_Selective> local_results(it->second.size());

        // Parallelize the loop that fetches selective collisions
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < it->second.size(); ++i) {
            Collision_Selective data{};
            int index = it->second[i];
            data.borough = &collisions_.boroughs[index];
            data.on_street_name = &collisions_.on_street_names[index];
            local_results[i] = data;
        }

        // Merge the local results into the final results vector
        #pragma omp critical
        results.insert(results.end(), local_results.begin(), local_results.end());
    }

    //std::cout << "fetch_collisions_selective_parallel" << results.size() << std::endl ;

    
}


