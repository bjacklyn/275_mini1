#pragma once

#include "collision.hpp"

#include <string>


class CollisionManager {

public:
    CollisionManager(const std::string& filename);

    bool is_initialized();
    const std::string& get_initialization_error();
    const std::vector<const Collision*> search();

private:
    std::string initialization_error;
    std::vector<Collision> collisions;

};
