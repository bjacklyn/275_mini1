#pragma once

#include <cassert>
#include <chrono>
#include <string>
#include <string_view>

using Value = std::variant<float, size_t, std::string, std::chrono::year_month_day, std::chrono::hh_mm_ss<std::chrono::minutes>>;

enum class QueryType { HAS_VALUE, EQUALS, LESS_THAN, GREATER_THAN, CONTAINS };
enum class Qualifier { NONE, NOT, CASE_INSENSITIVE };

class FieldQuery {
private:
    FieldQuery(const std::string& name, const QueryType& type, const Value& value, bool invert_match, bool case_insensitive)
      : name_{name},
        type_{type},
        value_{value},
        invert_match_{invert_match},
        case_insensitive_{case_insensitive} {}

    const std::string name_;
    const QueryType type_;
    const Value value_;
    const bool invert_match_;
    const bool case_insensitive_;

public:
    friend class Query;

    const std::string& get_name() const {
        return name_;
    }

    const QueryType& get_type() const {
        return type_;
    }

    const Value& get_value() const {
        return value_;
    }

    const bool invert_match() const {
        return invert_match_;
    }

    const bool case_insensitive() const {
        return case_insensitive_;
    }
};

class Query {
private:
    Query(FieldQuery&& query)
      : queries{query}
      {}

    std::vector<FieldQuery> queries;

    static FieldQuery create_field_query(const std::string_view& name,
                                         const Qualifier& not_qualifier,
                                         const QueryType& type,
                                         const Value value,
                                         const Qualifier& case_insensitive_qualifier) {
        std::visit([&name](auto&& val) {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, float>) {
                assert((name == "latitude" || name == "longitude") && "Invalid field_name provided!");
            } else if constexpr (std::is_same_v<T, std::size_t>) {
                assert((name == "zip_code" || name == "number_of_persons_injured" ||
                    name == "number_of_persons_killed" || name == "number_of_pedestrians_injured" ||
                    name == "number_of_pedestrians_killed" || name == "number_of_cyclist_injured" ||
                    name == "number_of_cyclist_killed" || name == "number_of_motorist_injured" ||
                    name == "number_of_motorist_killed" || name == "collision_id") && "Invalid field_name provided!");
            } else if constexpr (std::is_same_v<T, std::string>) {
                assert((name == "borough" || name == "location" || name == "on_street_name" ||
                    name == "cross_street_name" || name == "off_street_name" ||
                    name == "contributing_factor_vehicle_1" || name == "contributing_factor_vehicle_2" ||
                    name == "contributing_factor_vehicle_3" || name == "contributing_factor_vehicle_4" ||
                    name == "contributing_factor_vehicle_5" || name == "vehicle_type_code_1" ||
                    name == "vehicle_type_code_2" || name == "vehicle_type_code_3" ||
                    name == "vehicle_type_code_4" || name == "vehicle_type_code_5") && "Invalid field_name provided!");
            } else if constexpr (std::is_same_v<T, std::chrono::year_month_day>) {
                assert((name == "crash_date") && "Invalid field_name provided!");
            } else if constexpr (std::is_same_v<T, std::chrono::hh_mm_ss<std::chrono::minutes>>) {
                assert((name == "crash_time") && "Invalid field_name provided!");
            }
        }, value);

        return FieldQuery(std::string(name),
                          type,
                          value,
                          not_qualifier == Qualifier::NOT,
                          case_insensitive_qualifier == Qualifier::CASE_INSENSITIVE);
    }

public:
    const std::vector<FieldQuery>& get() const {
        return queries;
    }

    Query& add(const std::string_view& name, const QueryType& type, const Value value) {
        return add(name, Qualifier::NONE, type, value, Qualifier::NONE);
    }

    Query& add(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value) {
        return add(name, not_qualifier, type, value, Qualifier::NONE);
    }

    Query& add(const std::string_view& name, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
        return add(name, Qualifier::NONE, type, value, case_insensitive_qualifier);
    }

    Query& add(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
        queries.push_back(create_field_query(std::string(name),
                                            not_qualifier,
                                            type,
                                            value,
                                            case_insensitive_qualifier));
        return *this;
    }

    static Query create(const std::string_view& name, const QueryType& type, const Value value) {
        return create(name, Qualifier::NONE, type, value, Qualifier::NONE);
    }

    static Query create(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value) {
        return create(name, not_qualifier, type, value, Qualifier::NONE);
    }

    static Query create(const std::string_view& name, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
        return create(name, Qualifier::NONE, type, value, case_insensitive_qualifier);
    }

    static Query create(const std::string_view& name, const Qualifier& not_qualifier, const QueryType& type, const Value value, const Qualifier& case_insensitive_qualifier) {
        return Query(create_field_query(std::string(name),
                                        not_qualifier,
                                        type,
                                        value,
                                        case_insensitive_qualifier));
    }
};
