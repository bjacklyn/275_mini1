#include "collision_manager.hpp"

#include <gtest/gtest.h>

class CollisionManagerTest : public ::testing::Test {
protected:
    CollisionManager create_collision_manager(std::vector<Collision>& collisions) {
        return CollisionManager(collisions);
    }
};

TEST_F(CollisionManagerTest, MatchEmpty) {
    std::vector<Collision> collisions;
    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query = Query::create("borough", QueryType::EQUALS, "Nothing should match me");

    std::vector<const Collision*> results = collision_manager.search(query);
    EXPECT_EQ(results.size(), 0);
}

TEST_F(CollisionManagerTest, MatchEquals) {
    Collision collision1{};
    collision1.borough = "BROOKLYN";

    std::vector<Collision> collisions{collision1};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query1 = Query::create("borough", QueryType::EQUALS, "Nothing should match me");
    std::vector<const Collision*> results1 = collision_manager.search(query1);
    EXPECT_EQ(results1.size(), 0);

    Query query2 = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    std::vector<const Collision*> results2 = collision_manager.search(query2);
    EXPECT_EQ(results2.size(), 1);
}
