#pragma once

#include "collision.hpp"
#include "query.hpp"

#include <string>


class CollisionManager {

public:
    CollisionManager(const std::string& filename);

    bool is_initialized();
    const std::string& get_initialization_error();
    const std::vector<const Collision*> searchOpenMp(const Query& query);

    friend class CollisionManagerTest;

private:
    CollisionManager(std::vector<Collision>& collisions);

    std::string initialization_error_;
    std::vector<Collision> collisions_;

};
