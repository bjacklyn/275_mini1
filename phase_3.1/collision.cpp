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

bool Collisions::match_field(const FieldQuery& query, const std::size_t index) const {
    const std::string& name = query.get_name();

    if (name == "crash_date") {
        return do_match(query, this->crash_dates[index]);
    } else if (name == "crash_time") {
        return do_match(query, this->crash_times[index]);
    } else if (name == "borough") {
        return do_match(query, this->boroughs[index]);
    } else if (name == "zip_code") {
        return do_match(query, this->zip_codes[index]);
    } else if (name == "latitude") {
        return do_match(query, this->latitudes[index]);
    } else if (name == "longitude") {
        return do_match(query, this->longitudes[index]);
    } else if (name == "location") {
        return do_match(query, this->locations[index]);
    } else if (name == "on_street_name") {
        return do_match(query, this->on_street_names[index]);
    } else if (name == "cross_street_name") {
        return do_match(query, this->cross_street_names[index]);
    } else if (name == "off_street_name") {
        return do_match(query, this->off_street_names[index]);
    } else if (name == "number_of_persons_injured") {
        return do_match(query, this->numbers_of_persons_injured[index]);
    } else if (name == "number_of_persons_killed") {
        return do_match(query, this->numbers_of_persons_killed[index]);
    } else if (name == "number_of_pedestrians_injured") {
        return do_match(query, this->numbers_of_pedestrians_injured[index]);
    } else if (name == "number_of_pedestrians_killed") {
        return do_match(query, this->numbers_of_pedestrians_killed[index]);
    } else if (name == "number_of_cyclist_injured") {
        return do_match(query, this->numbers_of_cyclist_injured[index]);
    } else if (name == "number_of_cyclist_killed") {
        return do_match(query, this->numbers_of_cyclist_killed[index]);
    } else if (name == "number_of_motorist_injured") {
        return do_match(query, this->numbers_of_motorist_injured[index]);
    } else if (name == "number_of_motorist_killed") {
        return do_match(query, this->numbers_of_motorist_killed[index]);
    } else if (name == "contributing_factor_vehicle_1") {
        return do_match(query, this->contributing_factor_vehicles_1[index]);
    } else if (name == "contributing_factor_vehicle_2") {
        return do_match(query, this->contributing_factor_vehicles_2[index]);
    } else if (name == "contributing_factor_vehicle_3") {
        return do_match(query, this->contributing_factor_vehicles_3[index]);
    } else if (name == "contributing_factor_vehicle_4") {
        return do_match(query, this->contributing_factor_vehicles_4[index]);
    } else if (name == "contributing_factor_vehicle_5") {
        return do_match(query, this->contributing_factor_vehicles_5[index]);
    } else if (name == "collision_id") {
        return do_match(query, this->collision_ids[index]);
    } else if (name == "vehicle_type_code_1") {
        return do_match(query, this->vehicle_type_codes_1[index]);
    } else if (name == "vehicle_type_code_2") {
        return do_match(query, this->vehicle_type_codes_2[index]);
    } else if (name == "vehicle_type_code_3") {
        return do_match(query, this->vehicle_type_codes_3[index]);
    } else if (name == "vehicle_type_code_4") {
        return do_match(query, this->vehicle_type_codes_4[index]);
    } else if (name == "vehicle_type_code_5") {
        return do_match(query, this->vehicle_type_codes_5[index]);
    }

    return false;
}

bool Collisions::match(const Query& query, const std::size_t index) const {
    const std::vector<FieldQuery>& field_queries = query.get();

    for (const FieldQuery& field_query : field_queries) {
        bool match = match_field(field_query, index);
        match = field_query.invert_match() ? !match : match;
        if (!match) {
            return false;
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const Collision& collision) {
    os << "Collision: {";

    os << std::format("crash_date = {}", collision.crash_date->has_value() ?
        std::format("{:%m/%d/%Y}", collision.crash_date->value()) : "(no value)") << ", ";
    os << std::format("crash_time = {}", collision.crash_time->has_value() ?
        std::format("{:%H:%M}", collision.crash_time->value()) : "(no value)") << ", ";
    os << std::format("borough = {}", collision.borough->has_value() ?
        collision.borough->value() : "(no value)") << ", ";
    os << std::format("zip_code = {}", collision.zip_code->has_value() ?
        std::to_string(collision.zip_code->value()) : "(no value)") << ", ";
    os << std::format("latitude = {}", collision.latitude->has_value() ?
        std::to_string(collision.latitude->value()) : "(no value)") << ", ";
    os << std::format("longitude = {}", collision.longitude->has_value() ?
        std::to_string(collision.longitude->value()) : "(no value)") << ", ";
    os << std::format("location = {}", collision.location->has_value() ?
        collision.location->value() : "(no value)") << ", ";
    os << std::format("on_street_name = {}", collision.on_street_name->has_value() ?
        collision.on_street_name->value() : "(no value)") << ", ";
    os << std::format("cross_street_name = {}", collision.cross_street_name->has_value() ?
        collision.cross_street_name->value() : "(no value)") << ", ";
    os << std::format("off_street_name = {}", collision.off_street_name->has_value() ?
        collision.off_street_name->value() : "(no value)") << ", ";
    os << std::format("number_of_persons_injured = {}", collision.number_of_persons_injured->has_value() ?
        std::to_string(collision.number_of_persons_injured->value()) : "(no value)") << ", ";
    os << std::format("number_of_persons_killed = {}", collision.number_of_persons_killed->has_value() ?
        std::to_string(collision.number_of_persons_killed->value()) : "(no value)") << ", ";
    os << std::format("number_of_pedestrians_injured = {}", collision.number_of_pedestrians_injured->has_value() ?
        std::to_string(collision.number_of_pedestrians_injured->value()) : "(no value)") << ", ";
    os << std::format("number_of_pedestrians_killed = {}", collision.number_of_pedestrians_killed->has_value() ?
        std::to_string(collision.number_of_pedestrians_killed->value()) : "(no value)") << ", ";
    os << std::format("number_of_cyclist_injured = {}", collision.number_of_cyclist_injured->has_value() ?
        std::to_string(collision.number_of_cyclist_injured->value()) : "(no value)") << ", ";
    os << std::format("number_of_cyclist_killed = {}", collision.number_of_cyclist_killed->has_value() ?
        std::to_string(collision.number_of_cyclist_killed->value()) : "(no value)") << ", ";
    os << std::format("number_of_motorist_injured = {}", collision.number_of_motorist_injured->has_value() ?
        std::to_string(collision.number_of_motorist_injured->value()) : "(no value)") << ", ";
    os << std::format("number_of_motorist_killed = {}", collision.number_of_motorist_killed->has_value() ?
        std::to_string(collision.number_of_motorist_killed->value()) : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_1 = {}", collision.contributing_factor_vehicle_1->has_value() ?
        collision.contributing_factor_vehicle_1->value() : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_2 = {}", collision.contributing_factor_vehicle_2->has_value() ?
        collision.contributing_factor_vehicle_2->value() : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_3 = {}", collision.contributing_factor_vehicle_3->has_value() ?
        collision.contributing_factor_vehicle_3->value() : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_4 = {}", collision.contributing_factor_vehicle_4->has_value() ?
        collision.contributing_factor_vehicle_4->value() : "(no value)") << ", ";
    os << std::format("contributing_factor_vehicle_5 = {}", collision.contributing_factor_vehicle_5->has_value() ?
        collision.contributing_factor_vehicle_5->value() : "(no value)") << ", ";
    os << std::format("collision_id = {}", collision.collision_id->has_value() ?
        std::to_string(collision.collision_id->value()) : "(no value)") << ", ";
    os << std::format("vehicle_type_code_1 = {}", collision.vehicle_type_code_1->has_value() ?
        collision.vehicle_type_code_1->value() : "(no value)") << ", ";
    os << std::format("vehicle_type_code_2 = {}", collision.vehicle_type_code_2->has_value() ?
        collision.vehicle_type_code_2->value() : "(no value)") << ", ";
    os << std::format("vehicle_type_code_3 = {}", collision.vehicle_type_code_3->has_value() ?
        collision.vehicle_type_code_3->value() : "(no value)") << ", ";
    os << std::format("vehicle_type_code_4 = {}", collision.vehicle_type_code_4->has_value() ?
        collision.vehicle_type_code_4->value() : "(no value)") << ", ";
    os << std::format("vehicle_type_code_5 = {}", collision.vehicle_type_code_5->has_value() ?
        collision.vehicle_type_code_5->value() : "(no value)") << ", ";

    os << "}";
    return os;
}

void Collisions::combine(const Collisions& other) {
    crash_dates.insert(crash_dates.end(), other.crash_dates.begin(), other.crash_dates.end());
    crash_times.insert(crash_times.end(), other.crash_times.begin(), other.crash_times.end());
    boroughs.insert(boroughs.end(), other.boroughs.begin(), other.boroughs.end());
    zip_codes.insert(zip_codes.end(), other.zip_codes.begin(), other.zip_codes.end());
    latitudes.insert(latitudes.end(), other.latitudes.begin(), other.latitudes.end());
    longitudes.insert(longitudes.end(), other.longitudes.begin(), other.longitudes.end());
    locations.insert(locations.end(), other.locations.begin(), other.locations.end());
    on_street_names.insert(on_street_names.end(), other.on_street_names.begin(), other.on_street_names.end());
    cross_street_names.insert(cross_street_names.end(), other.cross_street_names.begin(), other.cross_street_names.end());
    off_street_names.insert(off_street_names.end(), other.off_street_names.begin(), other.off_street_names.end());
    numbers_of_persons_injured.insert(numbers_of_persons_injured.end(), other.numbers_of_persons_injured.begin(), other.numbers_of_persons_injured.end());
    numbers_of_persons_killed.insert(numbers_of_persons_killed.end(), other.numbers_of_persons_killed.begin(), other.numbers_of_persons_killed.end());
    numbers_of_pedestrians_injured.insert(numbers_of_pedestrians_injured.end(), other.numbers_of_pedestrians_injured.begin(), other.numbers_of_pedestrians_injured.end());
    numbers_of_pedestrians_killed.insert(numbers_of_pedestrians_killed.end(), other.numbers_of_pedestrians_killed.begin(), other.numbers_of_pedestrians_killed.end());
    numbers_of_cyclist_injured.insert(numbers_of_cyclist_injured.end(), other.numbers_of_cyclist_injured.begin(), other.numbers_of_cyclist_injured.end());
    numbers_of_cyclist_killed.insert(numbers_of_cyclist_killed.end(), other.numbers_of_cyclist_killed.begin(), other.numbers_of_cyclist_killed.end());
    numbers_of_motorist_injured.insert(numbers_of_motorist_injured.end(), other.numbers_of_motorist_injured.begin(), other.numbers_of_motorist_injured.end());
    numbers_of_motorist_killed.insert(numbers_of_motorist_killed.end(), other.numbers_of_motorist_killed.begin(), other.numbers_of_motorist_killed.end());
    contributing_factor_vehicles_1.insert(contributing_factor_vehicles_1.end(), other.contributing_factor_vehicles_1.begin(), other.contributing_factor_vehicles_1.end());
    contributing_factor_vehicles_2.insert(contributing_factor_vehicles_2.end(), other.contributing_factor_vehicles_2.begin(), other.contributing_factor_vehicles_2.end());
    contributing_factor_vehicles_3.insert(contributing_factor_vehicles_3.end(), other.contributing_factor_vehicles_3.begin(), other.contributing_factor_vehicles_3.end());
    contributing_factor_vehicles_4.insert(contributing_factor_vehicles_4.end(), other.contributing_factor_vehicles_4.begin(), other.contributing_factor_vehicles_4.end());
    contributing_factor_vehicles_5.insert(contributing_factor_vehicles_5.end(), other.contributing_factor_vehicles_5.begin(), other.contributing_factor_vehicles_5.end());
    collision_ids.insert(collision_ids.end(), other.collision_ids.begin(), other.collision_ids.end());
    vehicle_type_codes_1.insert(vehicle_type_codes_1.end(), other.vehicle_type_codes_1.begin(), other.vehicle_type_codes_1.end());
    vehicle_type_codes_2.insert(vehicle_type_codes_2.end(), other.vehicle_type_codes_2.begin(), other.vehicle_type_codes_2.end());
    vehicle_type_codes_3.insert(vehicle_type_codes_3.end(), other.vehicle_type_codes_3.begin(), other.vehicle_type_codes_3.end());
    vehicle_type_codes_4.insert(vehicle_type_codes_4.end(), other.vehicle_type_codes_4.begin(), other.vehicle_type_codes_4.end());
    vehicle_type_codes_5.insert(vehicle_type_codes_5.end(), other.vehicle_type_codes_5.begin(), other.vehicle_type_codes_5.end());

    size_ = crash_dates.size();
}

std::size_t Collisions::size() {
    return size_;
}
