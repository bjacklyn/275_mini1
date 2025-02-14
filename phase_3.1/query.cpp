#include "query.hpp"

#include <chrono>
#include <stdexcept>
#include <string>
#include <string_view>

const std::string& FieldQuery::get_name() const {
    return name_;
}

const QueryType& FieldQuery::get_type() const {
    return type_;
}

const Value& FieldQuery::get_value() const {
    return value_;
}

const bool FieldQuery::invert_match() const {
    return invert_match_;
}

const bool FieldQuery::case_insensitive() const {
    return case_insensitive_;
}

FieldQuery Query::create_field_query(const std::string_view& name,
                                     const Qualifier& not_qualifier,
                                     const QueryType& type,
                                     const Value value,
                                     const Qualifier& case_insensitive_qualifier) {
    std::visit([&name](auto&& val) {
        using T = std::decay_t<decltype(val)>;

        if constexpr (std::is_same_v<T, float>) {
            if (name != "latitude" && name != "longitude") {
                throw std::invalid_argument("Invalid field_name provided for float!");
            }
        } else if constexpr (std::is_same_v<T, std::size_t>) {
            if (name != "zip_code" && name != "number_of_persons_injured" &&
                name != "number_of_persons_killed" && name != "number_of_pedestrians_injured" &&
                name != "number_of_pedestrians_killed" && name != "number_of_cyclist_injured" &&
                name != "number_of_cyclist_killed" && name != "number_of_motorist_injured" &&
                name != "number_of_motorist_killed" && name != "collision_id") {
                throw std::invalid_argument("Invalid field_name provided for std::size_t!");
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (name != "borough" && name != "location" && name != "on_street_name" &&
                name != "cross_street_name" && name != "off_street_name" &&
                name != "contributing_factor_vehicle_1" && name != "contributing_factor_vehicle_2" &&
                name != "contributing_factor_vehicle_3" && name != "contributing_factor_vehicle_4" &&
                name != "contributing_factor_vehicle_5" && name != "vehicle_type_code_1" &&
                name != "vehicle_type_code_2" && name != "vehicle_type_code_3" &&
                name != "vehicle_type_code_4" && name != "vehicle_type_code_5") {
                throw std::invalid_argument("Invalid field_name provided for std::string!");
            }
        } else if constexpr (std::is_same_v<T, std::chrono::year_month_day>) {
            if (name != "crash_date") {
                throw std::invalid_argument("Invalid field_name provided for std::chrono::year_month_day!");
            }
        } else if constexpr (std::is_same_v<T, std::chrono::hh_mm_ss<std::chrono::minutes>>) {
            if (name != "crash_time") {
                throw std::invalid_argument("Invalid field_name provided for std::chrono::hh_mm_ss!");
            }
        }
    }, value);

    return FieldQuery(std::string(name),
                      type,
                      value,
                      not_qualifier == Qualifier::NOT,
                      case_insensitive_qualifier == Qualifier::CASE_INSENSITIVE);
}

const std::vector<FieldQuery>& Query::get() const {
    return queries;
}

Query& Query::add(const std::string_view& name, const QueryType& type, const Value value) {
    return add(name, Qualifier::NONE, type, value, Qualifier::NONE);
}

Query& Query::add(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value) {
    return add(name, not_qualifier, type, value, Qualifier::NONE);
}

Query& Query::add(const std::string_view& name, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
    return add(name, Qualifier::NONE, type, value, case_insensitive_qualifier);
}

Query& Query::add(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
    queries.push_back(create_field_query(std::string(name),
                                         not_qualifier,
                                         type,
                                         value,
                                         case_insensitive_qualifier));
    return *this;
}

Query Query::create(const std::string_view& name, const QueryType& type, const Value value) {
    return create(name, Qualifier::NONE, type, value, Qualifier::NONE);
}

Query Query::create(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value) {
    return create(name, not_qualifier, type, value, Qualifier::NONE);
}

Query Query::create(const std::string_view& name, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
    return create(name, Qualifier::NONE, type, value, case_insensitive_qualifier);
}

Query Query::create(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
    return Query(create_field_query(std::string(name),
                                    not_qualifier,
                                    type,
                                    value,
                                    case_insensitive_qualifier));
}
