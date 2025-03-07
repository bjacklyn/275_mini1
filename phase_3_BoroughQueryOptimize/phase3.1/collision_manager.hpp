#pragma once

#include "collision.hpp"
#include "query.hpp"

#include <string>


class CollisionManager {

public:
    CollisionManager(const std::string& filename);

    bool is_initialized();
    const std::string& get_initialization_error();
    const std::vector<Collision> search(const Query& query);

    friend class CollisionManagerTest;

    void print_bucket();
    
    void fetch_collisions(std:: string borough);
    void fetch_collisions_parallel(std:: string borough);
    
    void fetch_collisions_selective(std:: string borough);
    void fetch_collisions_selective_parallel(std:: string borough);
   

private:
    CollisionManager(Collisions& collisions);

    const Collision index_to_collision(const std::size_t index);

    std::string initialization_error_;
    Collisions collisions_;

};
