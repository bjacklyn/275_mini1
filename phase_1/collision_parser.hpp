#pragma once

#include "collision.hpp"

#include <string>


class CollisionParser {

public:
    CollisionParser(const std::string& filename);
    std::vector<Collision> parse();

private:
    std::string filename;
};
