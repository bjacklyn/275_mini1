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

TEST_F(CollisionManagerTest, CompoundMatchEquals) {
    Collision collision1{};
    collision1.borough = "BROOKLYN";
    collision1.collision_id = 1;

    Collision collision2{};
    collision2.borough = "BROOKLYN";
    collision2.collision_id = 2;

    Collision collision3{};
    collision3.borough = "QUEENS";
    collision3.collision_id = 3;

    std::vector<Collision> collisions{collision1, collision2, collision3};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query1 = Query::create("borough", QueryType::EQUALS, "BROOKLYN")
        .add("collision_id", QueryType::EQUALS, 10ULL);
    std::vector<const Collision*> results1 = collision_manager.search(query1);
    EXPECT_EQ(results1.size(), 0);

    Query query2 = Query::create("borough", QueryType::EQUALS, "BROOKLYN")
        .add("collision_id", QueryType::EQUALS, 1ULL);
    std::vector<const Collision*> results2 = collision_manager.search(query2);
    EXPECT_EQ(results2.size(), 1);
    EXPECT_EQ(results2[0]->borough, "BROOKLYN");
    EXPECT_EQ(results2[0]->collision_id, 1ULL);

    Query query3 = Query::create("borough", QueryType::EQUALS, "QUEENS")
        .add("collision_id", QueryType::EQUALS, 3ULL);
    std::vector<const Collision*> results3 = collision_manager.search(query3);
    EXPECT_EQ(results3.size(), 1);
    EXPECT_EQ(results3[0]->borough, "QUEENS");
    EXPECT_EQ(results3[0]->collision_id, 3ULL);

    Query query4 = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    std::vector<const Collision*> results4 = collision_manager.search(query4);
    EXPECT_EQ(results4.size(), 2);
    EXPECT_EQ(results4[0]->borough, "BROOKLYN");
    EXPECT_EQ(results4[0]->collision_id, 1ULL);
    EXPECT_EQ(results4[1]->borough, "BROOKLYN");
    EXPECT_EQ(results4[1]->collision_id, 2ULL);
}
