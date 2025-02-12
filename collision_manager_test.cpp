#include "collision_manager.hpp"
#include <chrono>
#include <gtest/gtest.h>

namespace {
    const char* const kSubsetDataset = "../MotorVehicleCollisionData_subset.csv";
}

class CollisionManagerTest : public ::testing::Test {
protected:
    CollisionManager create_collision_manager(std::vector<Collision>& collisions) {
        return CollisionManager(collisions);
    }

    CollisionManager create_collision_manager_from_csv(const std::string& filename) {
        return CollisionManager(filename);
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

TEST_F(CollisionManagerTest, MatchNotEquals) {
    Collision collision1{};
    collision1.borough = "BROOKLYN";

    Collision collision2{};
    collision2.borough = "QUEENS";

    std::vector<Collision> collisions{collision1, collision2};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query1 = Query::create("borough", QueryType::EQUALS, "Nothing should match me");
    std::vector<const Collision*> results1 = collision_manager.search(query1);
    EXPECT_EQ(results1.size(), 0);

    Query query2 = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    std::vector<const Collision*> results2 = collision_manager.search(query2);
    EXPECT_EQ(results2.size(), 1);
    EXPECT_EQ(results2[0]->borough, "BROOKLYN");

    Query query3 = Query::create("borough", Qualifier::NOT, QueryType::EQUALS, "BROOKLYN");
    std::vector<const Collision*> results3 = collision_manager.search(query3);
    EXPECT_EQ(results3.size(), 1);
    EXPECT_EQ(results3[0]->borough, "QUEENS");
}

TEST_F(CollisionManagerTest, MatchCaseInsensitive) {
    Collision collision1{};
    collision1.borough = "BROOKLYN";

    Collision collision2{};
    collision2.borough = "QUEENS";

    std::vector<Collision> collisions{collision1, collision2};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query1 = Query::create("borough", QueryType::EQUALS, "Nothing should match me");
    std::vector<const Collision*> results1 = collision_manager.search(query1);
    EXPECT_EQ(results1.size(), 0);

    Query query2 = Query::create("borough", QueryType::EQUALS, "BROOKLYN");
    std::vector<const Collision*> results2 = collision_manager.search(query2);
    EXPECT_EQ(results2.size(), 1);
    EXPECT_EQ(results2[0]->borough, "BROOKLYN");

    Query query3 = Query::create("borough", QueryType::EQUALS, "brooklyn", Qualifier::CASE_INSENSITIVE);
    std::vector<const Collision*> results3 = collision_manager.search(query3);
    EXPECT_EQ(results3.size(), 1);
    EXPECT_EQ(results3[0]->borough, "BROOKLYN");


   // Query query = Query::create(...).and(...).and(...)
}


TEST_F(CollisionManagerTest, MatchEqualsDate) {
    Collision collision1{};
    std::chrono::year_month_day date{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };
    collision1.crash_date = date;

    std::vector<Collision> collisions{collision1};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query1 = Query::create("crash_date", QueryType::EQUALS, date);
    std::vector<const Collision*> results1 = collision_manager.search(query1);
    EXPECT_EQ(results1.size(), 1);
}

TEST_F(CollisionManagerTest, MatchGreaterThanDate) {
    Collision collision1{} , collision2{};
    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };

    std::chrono::year_month_day date2{
        std::chrono::year{2022},
        std::chrono::month{3},
        std::chrono::day{26}
    };

    collision1.crash_date = date1;
    collision2.crash_date = date2;

    std::vector<Collision> collisions{collision1,collision2};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query = Query::create("crash_date", QueryType::GREATER_THAN, date1);
    std::vector<const Collision*> results = collision_manager.search(query);
    EXPECT_EQ(results.size(), 1);
}

TEST_F(CollisionManagerTest, MatchLessThanDate) {
    Collision collision1{} , collision2{};

    std::chrono::year_month_day date1{
        std::chrono::year{2022},
        std::chrono::month{3},
        std::chrono::day{26}
    };

    std::chrono::year_month_day date2{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };

    collision1.crash_date = date1;
    collision2.crash_date = date2;

    std::vector<Collision> collisions{collision1,collision2};

    CollisionManager collision_manager = create_collision_manager(collisions);

    Query query = Query::create("crash_date", QueryType::LESS_THAN, date1);
    std::vector<const Collision*> results = collision_manager.search(query);
    EXPECT_EQ(results.size(), 1);
}

TEST_F(CollisionManagerTest, CSV_Query_MatchLessThanDate) {

    std::string filename(kSubsetDataset);
    CollisionManager collision_manager = create_collision_manager_from_csv(filename);

    std::chrono::year_month_day date1{
        std::chrono::year{2022},
        std::chrono::month{3},
        std::chrono::day{26}
    };

    Query query = Query::create("crash_date", QueryType::LESS_THAN, date1);
    std::vector<const Collision*> results = collision_manager.search(query);

    EXPECT_GT(results.size(), 0) << "Search should return at least one result";

    for (const auto* collision : results) {
        EXPECT_TRUE(collision->crash_date < date1)
            << "Each result should have date less than " << date1;
    }

    std::cout << "Found " << results.size() << " collisions before " << date1 << std::endl;
}

TEST_F(CollisionManagerTest, CSV_Query_MatchGreaterThanDate) {

    std::string filename(kSubsetDataset);
    CollisionManager collision_manager = create_collision_manager_from_csv(filename);

    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };

    Query query = Query::create("crash_date", QueryType::GREATER_THAN, date1);
    std::vector<const Collision*> results = collision_manager.search(query);

    EXPECT_GT(results.size(), 0) << "Search should return at least one result";

    for (const auto* collision : results) {
        EXPECT_TRUE(collision->crash_date > date1)
            << "Each result should have date greater than " << date1;
    }

    std::cout << "Found " << results.size() << " collisions after " << date1 << std::endl;
}

TEST_F(CollisionManagerTest, CSV_Query_MatchEqualsDate) {

    std::string filename(kSubsetDataset);
    CollisionManager collision_manager = create_collision_manager_from_csv(filename);

    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };

    Query query = Query::create("crash_date", QueryType::EQUALS, date1);
    std::vector<const Collision*> results = collision_manager.search(query);

    EXPECT_GT(results.size(), 0) << "Search should return at least one result";

    for (const auto* collision : results) {
        EXPECT_TRUE(collision->crash_date == date1)
            << "Each result should have date greater than " << date1;
    }

    std::cout << "Found " << results.size() << " collisions on " << date1 << std::endl;
}

TEST_F(CollisionManagerTest, CSV_Query_MatchEqualsTime) {

    std::string filename(kSubsetDataset);
    CollisionManager collision_manager = create_collision_manager_from_csv(filename);

    std::chrono::hh_mm_ss<std::chrono::minutes> time1{
        std::chrono::hours{9} + std::chrono::minutes{35}
    };

    Query query = Query::create("crash_time", QueryType::EQUALS, time1);
    std::vector<const Collision*> results = collision_manager.search(query);

    EXPECT_GT(results.size(), 0) << "Search should return at least one result";

    for (const auto* collision : results) {
        EXPECT_TRUE(collision->crash_time.has_value() && collision->crash_time.value().to_duration() == time1.to_duration())
            << "Each result should have time equal to " << time1;
    }

    std::cout << "Found " << results.size() << " collisions on " << time1 << std::endl;
}

TEST_F(CollisionManagerTest, CSV_Query_MatchGreaterThanTime) {

    std::string filename(kSubsetDataset);
    CollisionManager collision_manager = create_collision_manager_from_csv(filename);

    std::chrono::hh_mm_ss<std::chrono::minutes> time1{
        std::chrono::hours{9} + std::chrono::minutes{35}
    };

    Query query = Query::create("crash_time", QueryType::GREATER_THAN, time1);
    std::vector<const Collision*> results = collision_manager.search(query);

    EXPECT_GT(results.size(), 0) << "Search should return at least one result";

    for (const auto* collision : results) {
        EXPECT_TRUE(collision->crash_time.has_value() && collision->crash_time.value().to_duration() > time1.to_duration())
            << "Each result should have time equal to " << time1;
    }

    std::cout << "Found " << results.size() << " collisions after " << time1 << std::endl;
}


TEST_F(CollisionManagerTest, CSV_Query_MatchLessThanTime) {

    std::string filename(kSubsetDataset);
    CollisionManager collision_manager = create_collision_manager_from_csv(filename);

    std::chrono::hh_mm_ss<std::chrono::minutes> time1{
        std::chrono::hours{9} + std::chrono::minutes{35}
    };

    Query query = Query::create("crash_time", QueryType::LESS_THAN, time1);
    std::vector<const Collision*> results = collision_manager.search(query);

    EXPECT_GT(results.size(), 0) << "Search should return at least one result";

    for (const auto* collision : results) {
        EXPECT_TRUE(collision->crash_time.has_value() && collision->crash_time.value().to_duration() < time1.to_duration())
            << "Each result should have time equal to " << time1;
    }

    std::cout << "Found " << results.size() << " collisions before " << time1 << std::endl;
}
