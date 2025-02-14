#include "collision.hpp"
#include "query.hpp"

#include <chrono>
#include <iostream>
#include <format>
#include <optional>
#include <string>

template<class T>
bool do_match(const FieldQuery& query, const std::optional<T>& value) {
    const QueryType& type = query.get_type();

    if (type == QueryType::HAS_VALUE) {
        return value.has_value();
    }

    if (!value.has_value()) {
        return false;
    }

    const T& query_value = std::get<T>(query.get_value());

    if constexpr (std::is_same_v<float, T> || std::is_same_v<std::size_t, T> || std::is_same_v<std::chrono::year_month_day, T>) {
        switch(type) {
        case QueryType::EQUALS:
            return *value == query_value;
        case QueryType::LESS_THAN:
            return *value < query_value;
        case QueryType::GREATER_THAN:
            return *value > query_value;
        case QueryType::CONTAINS:
        default:
            throw std::runtime_error("Unsupported QueryType for float/std::size_t/std::chrono::year_month_day");
        }
    } else if constexpr (std::is_same_v<std::chrono::hh_mm_ss<std::chrono::minutes>, T>) {
        switch(type) {
        case QueryType::EQUALS:
            return (*value).to_duration() == query_value.to_duration();
        case QueryType::LESS_THAN:
            return (*value).to_duration() < query_value.to_duration();
        case QueryType::GREATER_THAN:
            return (*value).to_duration() > query_value.to_duration();
        case QueryType::CONTAINS:
        default:
            throw std::runtime_error("Unsupported QueryType for std::chrono::hh_mm_ss");
        }
    } else if constexpr (std::is_same_v<std::string, T>) {
        std::string first_value = *value;
        std::string second_value = query_value;
        if (query.case_insensitive()) {
            std::transform(first_value.begin(), first_value.end(), first_value.begin(), ::tolower);
            std::transform(second_value.begin(), second_value.end(), second_value.begin(), ::tolower);
        }

        switch(type) {
        case QueryType::EQUALS:
            return first_value == second_value;
        case QueryType::CONTAINS:
            return first_value.find(second_value) != std::string::npos;
        case QueryType::LESS_THAN:
        case QueryType::GREATER_THAN:
        default:
            throw std::runtime_error("Unsupported QueryType for std::string");
        }
    } else {
        static_assert(false, "Unsupported type, Only float, std::size_t, std::string, std::chrono::year_month_day, and std::chrono::hh_mm_ss types are allowed.");
    }

    return false;
}

bool Collision::match_field(const FieldQuery& query) const {
    const std::string& name = query.get_name();

    if (name == "crash_date") {
        return do_match(query, this->crash_date);
    } else if (name == "crash_time") {
        return do_match(query, this->crash_time);
    } else if (name == "borough") {
        return do_match(query, this->borough);
    } else if (name == "zip_code") {
        return do_match(query, this->zip_code);
    } else if (name == "latitude") {
        return do_match(query, this->latitude);
    } else if (name == "longitude") {
        return do_match(query, this->longitude);
    } else if (name == "location") {
        return do_match(query, this->location);
    } else if (name == "on_street_name") {
        return do_match(query, this->on_street_name);
    } else if (name == "cross_street_name") {
        return do_match(query, this->cross_street_name);
    } else if (name == "off_street_name") {
        return do_match(query, this->off_street_name);
    } else if (name == "number_of_persons_injured") {
        return do_match(query, this->number_of_persons_injured);
    } else if (name == "number_of_persons_killed") {
        return do_match(query, this->number_of_persons_killed);
    } else if (name == "number_of_pedestrians_injured") {
        return do_match(query, this->number_of_pedestrians_injured);
    } else if (name == "number_of_pedestrians_killed") {
        return do_match(query, this->number_of_pedestrians_killed);
    } else if (name == "number_of_cyclist_injured") {
        return do_match(query, this->number_of_cyclist_injured);
    } else if (name == "number_of_cyclist_killed") {
        return do_match(query, this->number_of_cyclist_killed);
    } else if (name == "number_of_motorist_injured") {
        return do_match(query, this->number_of_motorist_injured);
    } else if (name == "number_of_motorist_killed") {
        return do_match(query, this->number_of_motorist_killed);
    } else if (name == "contributing_factor_vehicle_1") {
        return do_match(query, this->contributing_factor_vehicle_1);
    } else if (name == "contributing_factor_vehicle_2") {
        return do_match(query, this->contributing_factor_vehicle_2);
    } else if (name == "contributing_factor_vehicle_3") {
        return do_match(query, this->contributing_factor_vehicle_3);
    } else if (name == "contributing_factor_vehicle_4") {
        return do_match(query, this->contributing_factor_vehicle_4);
    } else if (name == "contributing_factor_vehicle_5") {
        return do_match(query, this->contributing_factor_vehicle_5);
    } else if (name == "collision_id") {
        return do_match(query, this->collision_id);
    } else if (name == "vehicle_type_code_1") {
        return do_match(query, this->vehicle_type_code_1);
    } else if (name == "vehicle_type_code_2") {
        return do_match(query, this->vehicle_type_code_2);
    } else if (name == "vehicle_type_code_3") {
        return do_match(query, this->vehicle_type_code_3);
    } else if (name == "vehicle_type_code_4") {
        return do_match(query, this->vehicle_type_code_4);
    } else if (name == "vehicle_type_code_5") {
        return do_match(query, this->vehicle_type_code_5);
    }

    return false;
}

bool Collision::match(const Query& query) const {
    const std::vector<FieldQuery>& field_queries = query.get();

    for (const FieldQuery& field_query : field_queries) {
        bool match = match_field(field_query);
        match = field_query.invert_match() ? !match : match;
        if (!match) {
            return false;
        }
    }

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
