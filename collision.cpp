#include "collision.hpp"
#include "query.hpp"

#include <chrono>
#include <iostream>
#include <format>
#include <optional>
#include <string>

bool match_size_t(const FieldQuery& query, const std::optional<std::size_t>& value) {
    const QueryType& type = query.get_type();

    if (type == QueryType::HAS_VALUE) {
        return value.has_value();
    }

    if (!value.has_value()) {
        return false;
    }

    const std::size_t& query_value = std::get<std::size_t>(query.get_value());

    switch(type) {
        case QueryType::EQUALS:
            return *value == query_value;
        case QueryType::LESS_THAN:
            return *value < query_value;
        case QueryType::GREATER_THAN:
            return *value > query_value;
    }

    return false;
}

bool match_float(const FieldQuery& query, const std::optional<float>& value) {
    const QueryType& type = query.get_type();

    if (type == QueryType::HAS_VALUE) {
        return value.has_value();
    }

    if (!value.has_value()) {
        return false;
    }

    const float& query_value = std::get<float>(query.get_value());

    switch(type) {
        case QueryType::EQUALS:
            return *value == query_value;
        case QueryType::LESS_THAN:
            return *value < query_value;
        case QueryType::GREATER_THAN:
            return *value > query_value;
    }

    return false;
}

bool match_string(const FieldQuery& query, const std::optional<std::string>& value) {
    const QueryType& type = query.get_type();

    if (type == QueryType::HAS_VALUE) {
        return value.has_value();
    }

    if (!value.has_value()) {
        return false;
    }

    const std::string& query_value = std::get<std::string>(query.get_value());

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
    }

    return false;
}

bool match_date(const FieldQuery& query, const std::optional<std::chrono::year_month_day>& value) {
    const QueryType& type = query.get_type();

    if (type == QueryType::HAS_VALUE) {
        return value.has_value();
    }

    if (!value.has_value()) {
        return false;
    }

    const std::chrono::year_month_day& query_value = std::get<std::chrono::year_month_day>(query.get_value());

    switch(type) {
        case QueryType::EQUALS:
            return *value == query_value;
        case QueryType::LESS_THAN:
            return *value < query_value;
        case QueryType::GREATER_THAN:
            return *value > query_value;
    }

    return false;
}

bool match_time(const FieldQuery& query, const std::optional<std::chrono::hh_mm_ss<std::chrono::minutes>>& value) {
    const QueryType& type = query.get_type();

    if (type == QueryType::HAS_VALUE) {
        return value.has_value();
    }

    if (!value.has_value()) {
        return false;
    }

    const std::chrono::hh_mm_ss<std::chrono::minutes>& query_value = std::get<std::chrono::hh_mm_ss<std::chrono::minutes>>(query.get_value());

    switch(type) {
        case QueryType::EQUALS:
            return (*value).to_duration() == query_value.to_duration();
        case QueryType::LESS_THAN:
            return (*value).to_duration() < query_value.to_duration();
        case QueryType::GREATER_THAN:
            return (*value).to_duration() > query_value.to_duration();
    }

    return false;
}

bool Collision::match_field(const FieldQuery& query) const {
    const std::string& name = query.get_name();

    if (name == "crash_date") {
        return match_date(query, this->crash_date);
    } else if (name == "crash_time") {
        return match_time(query, this->crash_time);
    } else if (name == "borough") {
        return match_string(query, this->borough);
    } else if (name == "zip_code") {
        return match_size_t(query, this->zip_code);
    } else if (name == "latitude") {
        return match_float(query, this->latitude);
    } else if (name == "longitude") {
        return match_float(query, this->longitude);
    } else if (name == "location") {
        return match_string(query, this->location);
    } else if (name == "on_street_name") {
        return match_string(query, this->on_street_name);
    } else if (name == "cross_street_name") {
        return match_string(query, this->cross_street_name);
    } else if (name == "off_street_name") {
        return match_string(query, this->off_street_name);
    } else if (name == "number_of_persons_injured") {
        return match_size_t(query, this->number_of_persons_injured);
    } else if (name == "number_of_persons_killed") {
        return match_size_t(query, this->number_of_persons_killed);
    } else if (name == "number_of_pedestrians_injured") {
        return match_size_t(query, this->number_of_pedestrians_injured);
    } else if (name == "number_of_pedestrians_killed") {
        return match_size_t(query, this->number_of_pedestrians_killed);
    } else if (name == "number_of_cyclist_injured") {
        return match_size_t(query, this->number_of_cyclist_injured);
    } else if (name == "number_of_cyclist_killed") {
        return match_size_t(query, this->number_of_cyclist_killed);
    } else if (name == "number_of_motorist_injured") {
        return match_size_t(query, this->number_of_motorist_injured);
    } else if (name == "number_of_motorist_killed") {
        return match_size_t(query, this->number_of_motorist_killed);
    } else if (name == "contributing_factor_vehicle_1") {
        return match_string(query, this->contributing_factor_vehicle_1);
    } else if (name == "contributing_factor_vehicle_2") {
        return match_string(query, this->contributing_factor_vehicle_2);
    } else if (name == "contributing_factor_vehicle_3") {
        return match_string(query, this->contributing_factor_vehicle_3);
    } else if (name == "contributing_factor_vehicle_4") {
        return match_string(query, this->contributing_factor_vehicle_4);
    } else if (name == "contributing_factor_vehicle_5") {
        return match_string(query, this->contributing_factor_vehicle_5);
    } else if (name == "collision_id") {
        return match_size_t(query, this->collision_id);
    } else if (name == "vehicle_type_code_1") {
        return match_string(query, this->vehicle_type_code_1);
    } else if (name == "vehicle_type_code_2") {
        return match_string(query, this->vehicle_type_code_2);
    } else if (name == "vehicle_type_code_3") {
        return match_string(query, this->vehicle_type_code_3);
    } else if (name == "vehicle_type_code_4") {
        return match_string(query, this->vehicle_type_code_4);
    } else if (name == "vehicle_type_code_5") {
        return match_string(query, this->vehicle_type_code_5);
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
