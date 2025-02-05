#pragma once

#include <chrono>
#include <iostream>
#include <format>
#include <optional>
#include <string>


struct Collision {
    std::optional<std::chrono::year_month_day> crash_date;
    std::optional<std::chrono::hh_mm_ss<std::chrono::minutes>> crash_time;
    std::optional<std::string> borough;
    std::optional<std::size_t> zip_code;
    std::optional<float> latitude;
    std::optional<float> longitude;
    std::optional<std::string> location;
    std::optional<std::string> on_street_name;
    std::optional<std::string> cross_street_name;
    std::optional<std::string> off_street_name;
    std::optional<std::size_t> number_of_persons_injured;
    std::optional<std::size_t> number_of_persons_killed;
    std::optional<std::size_t> number_of_pedestrians_injured;
    std::optional<std::size_t> number_of_pedestrians_killed;
    std::optional<std::size_t> number_of_cyclist_injured;
    std::optional<std::size_t> number_of_cyclist_killed;
    std::optional<std::size_t> number_of_motorist_injured;
    std::optional<std::size_t> number_of_motorist_killed;
    std::optional<std::string> contributing_factor_vehicle_1;
    std::optional<std::string> contributing_factor_vehicle_2;
    std::optional<std::string> contributing_factor_vehicle_3;
    std::optional<std::string> contributing_factor_vehicle_4;
    std::optional<std::string> contributing_factor_vehicle_5;
    std::optional<std::size_t> collision_id;
    std::optional<std::string> vehicle_type_code_1;
    std::optional<std::string> vehicle_type_code_2;
    std::optional<std::string> vehicle_type_code_3;
    std::optional<std::string> vehicle_type_code_4;
    std::optional<std::string> vehicle_type_code_5;

    bool match() const;
};

std::ostream& operator<<(std::ostream& os, const Collision& collision);
