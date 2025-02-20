#include "collision_manager.hpp"
#include "collision_parser.hpp"
#include "query.hpp"

#include <string>

#include <omp.h>

//using namespace chrono;

CollisionManager::CollisionManager(const std::string& filename) {
    CollisionParser parser{filename};

    try {
        this->collisions_ = parser.parse();
        this->initialization_error_ = "";
    } catch (const std::runtime_error& e) {
        this->collisions_ = {};
        this->initialization_error_ = e.what();
    }

    create_proxies();
    construct_borough_buckets();
}

CollisionManager::CollisionManager(Collisions& collisions) {
    this->collisions_ = collisions;
    create_proxies();
    construct_borough_buckets();
}

CollisionManager::CollisionManager(const std::vector<Collision>& collisions) {
    collisions_ = {};
    for (const Collision& collision : collisions) {
        collisions_.add(collision);
    }
    create_proxies();
    construct_borough_buckets();
}

bool CollisionManager::is_initialized() {
    return this->initialization_error_.empty();
}

const std::string& CollisionManager::get_initialization_error() {
    return this->initialization_error_;
}

const CollisionProxy CollisionManager::index_to_collision(const std::size_t index) {
    CollisionProxy collision{};
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

void CollisionManager::create_proxies() {
    for (std::size_t index = 0; index < collisions_.size(); ++index) {
        collision_proxies_.push_back(index_to_collision(index));
    }

    std::cout << "Collisions size: " << collisions_.size() << std::endl;
    std::cout << "Collisions Proxies size: " << collision_proxies_.size() << std::endl;
}

void CollisionManager::construct_borough_buckets(){
    
    int index = 0;
    for (const auto& borough : collisions_.boroughs){
        
        if (borough) {
            
            const std::string key = *borough;  // Dereference the optional to get the string key
            auto it = collisions_.borough_buckets.find(key);
            if (it != collisions_.borough_buckets.end()) {
                // If the key exists, add the index to the existing vector
                it->second.push_back(index);
            } else {
                // If the key doesn't exist, create a new entry with the key and add the index
                collisions_.borough_buckets[key] = {index};
            }

        }
        ++index;
    } 
}

void CollisionManager::print_bucket(){
    
    std::cout << "inside print bucket";
    for (const auto& entry : this->collisions_.borough_buckets) {
        std::cout << "Key: " << entry.first << " -> Indices: ";
        for (int index : entry.second) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
        break;
    }
}


void CollisionManager::fetch_collisions(std:: string borough){

    //auto start = high_resolution_clock::now();
    std::vector<CollisionProxy> results;
    
    auto it = this->collisions_.borough_buckets.find(borough);
    

    if( it != this->collisions_.borough_buckets.end()){

        for (int& index : it->second){

            results.push_back(index_to_collision(index));
        }

    }
    
    //auto end = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>(end-start);


    //std::cout<< "Single thread on construct results size" << results.size() << std::endl; 
    //std::cout<< "Duration" << duration << std::endl; 

    

}

/*
void CollisionManager::fetch_collisions(std:: string borough){

    std::vector<CollisionProxy> results;
    auto it = this->collisions_.borough_buckets.find(borough);
    

    if( it != this->collisions_.borough_buckets.end()){

        for (int& index : it->second){

            results.push_back(collision_proxies_[index]);
        }

    }

    //std::cout<< "Single thread results size" << results.size() << std::endl; 

    /*std::cout << "Search only indices" << results.size() << std::endl ;

    for (CollisionProxy& col : results){
        std::cout << * (*col.borough) <<"\t Date" << * (*col.crash_date) << std::endl;
    } 

} */


/*
void CollisionManager::fetch_collisions_parallel(std::string borough){
    
    
    
    std::vector<CollisionProxy> results;

    auto it = this->collisions_.borough_buckets.find(borough);

    if (it != this->collisions_.borough_buckets.end()){

        //std::cout<< "Inside" << std::endl; 
        
        #pragma omp parallel 
        {

            std::vector<CollisionProxy> thread_local_result;

            #pragma omp for 
            for (size_t i = 0 ; i < it->second.size() ; i++ ){
                thread_local_result.push_back(collision_proxies_[it->second[i]]);
            }

            #pragma omp critical
            {
                results.insert(results.end(),thread_local_result.begin(),thread_local_result.end());
            }

        }

    }

    //std::cout<< "Parallel results size" << results.size() << std::endl; 

    




    /*if (it != this->collisions_.borough_buckets.end()){
        
        
        //unsigned long num_threads = omp_get_max_threads();
        //std::vector<CollisionProxy> thread_local_collisions{num_threads};

        std::vector<CollisionProxy> results;

        // Parallelize the loop that fetches collisions
        #pragma omp parallel for 
        for (size_t i = 0; i < it->second.size(); ++i) {
            
            //int thread_id = omp_get_thread_num();
            results.push_back(collision_proxies_[it->second[i]]);
            //thread_local_collisions[thread_id].
            //thread_local_collisions[thread_id].push_back(collision_proxies_[it->second[i]]);
        }

        // Merge the results from the parallel section back into the main results vector
        
        /*for (const auto& thread_collisions : thread_local_collisions) {
            results.insert(results.end(), thread_collisions.begin(), thread_collisions.end());
        }
        
        std::cout<< "Parallel results size" << results.size() << std::endl; 

    }
}
*/


const std::vector<CollisionProxy*> CollisionManager::searchOpenMp(const Query& query) {
    const std::vector<FieldQuery>& field_queries = query.get();
    std::vector<CollisionProxy*> results;

    unsigned long num_threads = omp_get_max_threads();
    std::vector<std::vector<CollisionProxy*>> thread_local_results(num_threads);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        int chunk_size = collisions_.size() / num_threads;
        int start_index = thread_id * chunk_size;
        int end_index = (thread_id == num_threads - 1) ? collisions_.size() : start_index + chunk_size;

        // Initialize all matches to true initialially
        std::vector<std::uint8_t> matches(end_index - start_index, true);

        for (const FieldQuery& field_query : field_queries) {
            collisions_.match(field_query, start_index, end_index, matches);
        }

        for (std::size_t index = 0; index < matches.size(); ++index) {
            if (matches[index]) {
                thread_local_results[thread_id].push_back(&collision_proxies_.at(start_index + index));
            }
        }
    }

    for (const auto& local_results : thread_local_results) {
        results.insert(results.end(), local_results.begin(), local_results.end());
    }

    return results;
} 
