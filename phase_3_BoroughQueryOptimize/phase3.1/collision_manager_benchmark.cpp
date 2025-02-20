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
            collision_manager = std::make_unique<CollisionManager>(std::string("/home/suriya-018231499/cpp_projects/parser_data/Motor_Vehicle_Collisions_-_Crashes_20250204.csv"));
        }
    }
};

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, SearchSingleString)(benchmark::State& state) {
    Query query = Query::create("borough", QueryType::EQUALS, "BROOKLYN");

    for (auto _ : state) {
        std::vector<Collision> results = collision_manager->search(query);
        benchmark::DoNotOptimize(results);
    }
}




BENCHMARK_DEFINE_F(CollisionManagerBenchmark, boroughmatches)(benchmark::State& state) {
    
    for (auto _ : state) {
        
        collision_manager->fetch_collisions("BROOKLYN");
        
        
    }
}


BENCHMARK_DEFINE_F(CollisionManagerBenchmark, boroughmatches_parallel)(benchmark::State& state) {
    
    for (auto _ : state) {
        
        collision_manager->fetch_collisions_parallel("BROOKLYN");
        
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, boroughmatches_custom)(benchmark::State& state) {
    
    for (auto _ : state) {
        
        collision_manager->fetch_collisions_selective("BROOKLYN");
        
    }
}

BENCHMARK_DEFINE_F(CollisionManagerBenchmark, boroughmatches_custom_parallel)(benchmark::State& state) {
    
    for (auto _ : state) {
        
        collision_manager->fetch_collisions_selective_parallel("BROOKLYN");
        
    }
}


//BENCHMARK_REGISTER_F(CollisionManagerBenchmark, SearchSingleStringIndex)->Iterations(50);



BENCHMARK_REGISTER_F(CollisionManagerBenchmark, boroughmatches)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, boroughmatches_parallel)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, boroughmatches_custom)->Iterations(50);
BENCHMARK_REGISTER_F(CollisionManagerBenchmark, boroughmatches_custom_parallel)->Iterations(50);



BENCHMARK_MAIN();
