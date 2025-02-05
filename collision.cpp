#include "collision.hpp"

#include <chrono>
#include <iostream>
#include <format>
#include <optional>
#include <string>


bool Collision::match() const {
    return true;
}

std::ostream& operator<<(std::ostream& os, const Collision& collision) {
    os << "Collision: {";

    os << std::format("crash_date = {}", collision.crash_date.has_value() ?
        std::format("{:%m/%d/%Y}", *collision.crash_date) : "(no value)") << ", ";
    os << std::format("crash_time = {}", collision.crash_time.has_value() ?
        std::format("{:%H:%M}", *collision.crash_time) : "(no value)") << ", ";
    os << std::format("borough = {}", collision.borough.has_value() ?
        *collision.borough : "(no value)") << ", ";
    os << std::format("zip_code = {}", collision.zip_code.has_value() ?
        std::to_string(*collision.zip_code) : "(no value)") << ", ";
    os << std::format("latitude = {}", collision.latitude.has_value() ?
        std::to_string(*collision.latitude) : "(no value)") << ", ";
    os << std::format("longitude = {}", collision.longitude.has_value() ?
        std::to_string(*collision.longitude) : "(no value)") << ", ";
    os << std::format("location = {}", collision.location.has_value() ?
        *collision.location : "(no value)") << ", ";
    os << std::format("on_street_name = {}", collision.on_street_name.has_value() ?
        *collision.on_street_name : "(no value)") << ", ";
    os << std::format("cross_street_name = {}", collision.cross_street_name.has_value() ?
        *collision.cross_street_name : "(no value)") << ", ";
    os << std::format("off_street_name = {}", collision.off_street_name.has_value() ?
        *collision.off_street_name : "(no value)") << ", ";
    os << std::format("number_of_persons_injured = {}", collision.number_of_persons_injured.has_value() ?
        std::to_string(*collision.number_of_persons_injured) : "(no value)") << ", ";
    os << std::format("number_of_persons_killed = {}", collision.number_of_persons_killed.has_value() ?
        std::to_string(*collision.number_of_persons_killed) : "(no value)") << ", ";
    os << std::format("number_of_pedestrians_injured = {}", collision.number_of_pedestrians_injured.has_value() ?
        std::to_string(*collision.number_of_pedestrians_injured) : "(no value)") << ", ";
    os << std::format("number_of_pedestrians_killed = {}", collision.number_of_pedestrians_killed.has_value() ?
        std::to_string(*collision.number_of_pedestrians_killed) : "(no value)") << ", ";
    os << std::format("number_of_cyclist_injured = {}", collision.number_of_cyclist_injured.has_value() ?
        std::to_string(*collision.number_of_cyclist_injured) : "(no value)") << ", ";
    os << std::format("number_of_cyclist_killed = {}", collision.number_of_cyclist_killed.has_value() ?
        std::to_string(*collision.number_of_cyclist_killed) : "(no value)") << ", ";
    os << std::format("number_of_motorist_injured = {}", collision.number_of_motorist_injured.has_value() ?
        std::to_string(*collision.number_of_motorist_injured) : "(no value)") << ", ";
    os << std::format("number_of_motorist_killed = {}", collision.number_of_motorist_killed.has_value() ?
        std::to_string(*collision.number_of_motorist_killed) : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_1 = {}", collision.contributing_factor_vehicle_1.has_value() ?
        *collision.contributing_factor_vehicle_1 : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_2 = {}", collision.contributing_factor_vehicle_2.has_value() ?
        *collision.contributing_factor_vehicle_2 : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_3 = {}", collision.contributing_factor_vehicle_3.has_value() ?
        *collision.contributing_factor_vehicle_3 : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_4 = {}", collision.contributing_factor_vehicle_4.has_value() ?
        *collision.contributing_factor_vehicle_4 : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_5 = {}", collision.contributing_factor_vehicle_5.has_value() ?
        *collision.contributing_factor_vehicle_5 : "(no value)") << ", ";
    os << std::format("collision_id = {}", collision.collision_id.has_value() ?
        std::to_string(*collision.collision_id) : "(no value)") << ", ";
    os << std::format("vehicle_type_code_1 = {}", collision.vehicle_type_code_1.has_value() ?
        *collision.vehicle_type_code_1 : "(no value)") << ", ";
    os << std::format("vehicle_type_code_2 = {}", collision.vehicle_type_code_2.has_value() ?
        *collision.vehicle_type_code_2 : "(no value)") << ", ";
    os << std::format("vehicle_type_code_3 = {}", collision.vehicle_type_code_3.has_value() ?
        *collision.vehicle_type_code_3 : "(no value)") << ", ";
    os << std::format("vehicle_type_code_4 = {}", collision.vehicle_type_code_4.has_value() ?
        *collision.vehicle_type_code_4 : "(no value)") << ", ";
    os << std::format("vehicle_type_code_5 = {}", collision.vehicle_type_code_5.has_value() ?
        *collision.vehicle_type_code_5 : "(no value)") << ", ";

    os << "}";
    return os;
}
