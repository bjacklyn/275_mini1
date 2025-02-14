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
		if (!collision_manager->is_initialized()) {
                    std::cerr << "Could not initialize collision manager: " << collision_manager->get_initialization_error() << std::endl;
                std::exit(1);
            }
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

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleStringFieldNoMatchesParallel)(benchmark::State& state) {
    Query query = Query::create("borough", QueryType::EQUALS, "Nothing should match me");

    for (auto _ : state) {
        std::vector<const Collision*> results = collision_manager->searchOpenMp(query);
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

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleStringFieldSomeMatchesParallel)(benchmark::State& state) {
    Query query = Query::create("borough", QueryType::EQUALS, "BROOKLYN");

    for (auto _ : state) {
        std::vector<const Collision*> results = collision_manager->searchOpenMp(query);
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

BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringFieldNoMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringFieldNoMatchesParallel)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringFieldSomeMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringFieldSomeMatchesParallel)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleSizeTFieldNoMatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleSizeTFieldSomeMatches)->Iterations(50);

BENCHMARK_MAIN();
