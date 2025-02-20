#include "collision_manager.hpp"

#include <benchmark/benchmark.h>
#include <limits>

static std::unique_ptr<CollisionManager> collision_manager;

class CollisionManagerBenchmark : public benchmark::Fixture
{
public:

    void SetUp(const ::benchmark::State& state)
    {
        if (collision_manager.get() == nullptr) {
            collision_manager = std::make_unique<CollisionManager>(std::string("../Motor_Vehicle_Collisions_-_Crashes_20250123.csv"));
        }
    }
};

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleStringFieldNoMatches)(benchmark::State& state) {
    Query query = Query::create("borough", QueryType::EQUALS, "Nothing should match me");

    for (auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleStringFieldSomeMatches)(benchmark::State& state) {
    Query query = Query::create("borough", QueryType::EQUALS, "BROOKLYN");

    for (auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleSizeTFieldNoMatches)(benchmark::State& state) {
    Query query = Query::create("zip_code", QueryType::EQUALS, std::numeric_limits<size_t>::max());

    for (auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleSizeTFieldSomeMatches)(benchmark::State& state) {
    Query query = Query::create("zip_code", QueryType::EQUALS, 11208ULL);

    for (auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchLatitudeSomeMatches)(benchmark::State& state) {
    float latitude = 40.63165f;
    Query query = Query::create("latitude", QueryType::EQUALS, latitude);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchLesserThanLatitudeSomeMatches)(benchmark::State& state) {
    float latitude = 40.63165f;
    Query query = Query::create("latitude", QueryType::LESS_THAN, latitude);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchGreaterThanLatitudeSomeMatches)(benchmark::State& state) {
    float latitude = 40.63165f;
    Query query = Query::create("latitude", QueryType::GREATER_THAN, latitude);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchLongitudeSomeMatches)(benchmark::State& state) {
    float longitude = -73.88505f;
    Query query = Query::create("latitude", QueryType::EQUALS, longitude);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchLesserThanLongitudeSomeMatches)(benchmark::State& state) {
    float longitude = -73.88505f;
    Query query = Query::create("latitude", QueryType::LESS_THAN, longitude);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchGreaterThanLongitudeSomeMatches)(benchmark::State& state) {
    float longitude = -73.88505f;
    Query query = Query::create("latitude", QueryType::GREATER_THAN, longitude);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchBorough_LessThanLatitudeSomeMatches)(benchmark::State& state) {

    float latitude = 40.63165f;
    Query query = Query::create("latitude", QueryType::LESS_THAN, latitude).add("borough", QueryType::EQUALS, "BROOKLYN");

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchRangeofCoordinatesSomeMatches)(benchmark::State& state) {
    float latitude = 40.63165f;
    float longitude = -73.88505f;

 // The range we're searching
    float epsilon = 0.01f; // approximately 1 km radius

    Query query = Query::create("latitude", QueryType::GREATER_THAN, latitude - epsilon)
                      .add("latitude", QueryType::LESS_THAN, latitude + epsilon)
                      .add("longitude", QueryType::GREATER_THAN, longitude - epsilon)
                      .add("longitude", QueryType::LESS_THAN, longitude + epsilon);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchDatesEqualsSomeMatches)(benchmark::State& state){

    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}};

    Query query = Query::create("crash_date", QueryType::EQUALS, date1);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchDatesRangeSomeMatches)(benchmark::State& state){

    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };

    std::chrono::year_month_day date2{
        std::chrono::year{2022},
        std::chrono::month{6},
        std::chrono::day{29}
    };

    Query query = Query::create("crash_date", QueryType::GREATER_THAN, date1).add("crash_date", QueryType::LESS_THAN, date2);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchRangeofCoordinates_DateRangeSomeMatches)(benchmark::State& state) {

    std::string borough = "BROOKLYN";
    float latitude = 40.63165f;
    float longitude = -73.88505f;

    std::chrono::year_month_day date1{
        std::chrono::year{2021},
        std::chrono::month{9},
        std::chrono::day{11}
    };

    std::chrono::year_month_day date2{
        std::chrono::year{2022},
        std::chrono::month{6},
        std::chrono::day{29}
    };

    float epsilon = 0.01f; // approximately 1 km radius

    Query query = Query::create("borough", QueryType::EQUALS, borough)
                       .add("zip_code", QueryType::EQUALS, 11208ULL)
                       .add("latitude", QueryType::GREATER_THAN, latitude - epsilon)
                       .add("latitude", QueryType::LESS_THAN, latitude + epsilon)
                       .add("longitude", QueryType::GREATER_THAN, longitude - epsilon)
                       .add("longitude", QueryType::LESS_THAN, longitude + epsilon)
                       .add("crash_date", QueryType::GREATER_THAN, date1)
                       .add("crash_date", QueryType::LESS_THAN, date2);

    for(auto _ : state) {
        std::vector<const Collision*> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringFieldNoMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringFieldSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleSizeTFieldNoMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleSizeTFieldSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchLatitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchLongitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchLesserThanLatitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchGreaterThanLatitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchLesserThanLongitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchGreaterThanLongitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchBorough_LessThanLatitudeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchRangeofCoordinatesSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchDatesEqualsSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchDatesRangeSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchRangeofCoordinates_DateRangeSomeMatches)->Iterations(50);

BENCHMARK_MAIN();
