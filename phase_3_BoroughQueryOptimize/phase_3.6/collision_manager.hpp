#pragma once

#include "collision.hpp"
#include "query.hpp"

#include <string>


class CollisionManager {

public:
    CollisionManager(const std::string& filename);

    bool is_initialized();
    const std::string& get_initialization_error();
    const std::vector<CollisionProxy*> searchOpenMp(const Query& query);

    void print_bucket();
    void fetch_collisions(std:: string borough);
    void fetch_collisions_parallel(std::string borough);
    void fetch_collisions_ondemand_construct(std:: string borough);



    friend class CollisionManagerTest;

private:
    CollisionManager(Collisions& collisions);
    CollisionManager(const std::vector<Collision>& collisions);

    const CollisionProxy index_to_collision(const std::size_t index);
    void create_proxies();

    // borough buckets
    void construct_borough_buckets();
    

    std::string initialization_error_;
    Collisions collisions_;
    std::vector<CollisionProxy> collision_proxies_;

};
