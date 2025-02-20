#include "collision_parser.hpp"

#include "collision.hpp"

#include <chrono>
#include <iostream>
#include <format>
#include <fstream>
#include <omp.h>
#include <string>
#include <string_view>

namespace {

bool contains_non_whitespace(const std::string_view& field) {
    for (char c : field) {
        if (!std::isspace(c)) {
            return true;
        }
    }
    return false;
}

std::optional<std::chrono::year_month_day> convert_year_month_day_date(const std::string_view& field) {
    std::size_t first_slash = field.find('/');
    if (first_slash == std::string_view::npos) {
        std::cerr << "Error parsing date: " << field << std::endl;
        return {};
    }

    std::size_t second_slash = field.find('/', first_slash + 1);
    if (second_slash == std::string_view::npos) {
        std::cerr << "Error parsing date: " << field << std::endl;
        return {};
    }

    unsigned int month, day, year;

    auto month_result = std::from_chars(field.data(), field.data() + first_slash, month);
    auto day_result = std::from_chars(field.data() + first_slash + 1, field.data() + second_slash, day);
    auto year_result = std::from_chars(field.data() + second_slash + 1, field.data() + field.size() + 1, year);

    if (month_result.ec != std::errc() || day_result.ec != std::errc() || year_result.ec != std::errc()) {
        std::cerr << "Error parsing date: " << std::quoted(field) << std::endl;
        return {};
    }

    return std::chrono::year_month_day{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
}

std::optional<std::chrono::hh_mm_ss<std::chrono::minutes>> convert_hour_minute_time(const std::string_view& field) {
    std::size_t colon_index = field.find(':');
    if (colon_index == std::string_view::npos) {
        std::cerr << "Error parsing time: " << field << std::endl;
        return {};
    }

    unsigned int hour, minute;

    auto hour_result = std::from_chars(field.data(), field.data() + colon_index, hour);
    auto minute_result = std::from_chars(field.data() + colon_index + 1, field.data() + field.size(), minute);

    if (hour_result.ec != std::errc() || minute_result.ec != std::errc()) {
        std::cerr << "Error parsing time: " << std::quoted(field) << std::endl;
        return {};
    }

    return std::chrono::hh_mm_ss{std::chrono::hours(hour) + std::chrono::minutes(minute)};
}

std::optional<std::string> convert_string(const std::string_view& field) {
    return std::string(field);
}

template<typename T>
std::optional<T> convert_number(const std::string_view& field) {
    T number;

    auto number_result = std::from_chars(field.data(), field.data() + field.size(), number);

    if (number_result.ec != std::errc()) {
        std::cerr << "Error parsing number: " << std::quoted(field) << std::endl;
        return {};
    }

    return number;
}

void parseline(const std::string& line, Collisions& collisions) {

    bool is_inside_quote = false;
    std::size_t count = 0;
    std::size_t last_comma = 0;
    std::size_t next_comma = 0;
    std::size_t field_index = 0;

    std::optional<std::chrono::year_month_day> crash_date{};
    std::optional<std::chrono::hh_mm_ss<std::chrono::minutes>> crash_time{};
    std::optional<std::string> borough{};
    std::optional<std::size_t> zip_code{};
    std::optional<float> latitude{};
    std::optional<float> longitude{};
    std::optional<std::string> location{};
    std::optional<std::string> on_street_name{};
    std::optional<std::string> cross_street_name{};
    std::optional<std::string> off_street_name{};
    std::optional<std::size_t> number_of_persons_injured{};
    std::optional<std::size_t> number_of_persons_killed{};
    std::optional<std::size_t> number_of_pedestrians_injured{};
    std::optional<std::size_t> number_of_pedestrians_killed{};
    std::optional<std::size_t> number_of_cyclist_injured{};
    std::optional<std::size_t> number_of_cyclist_killed{};
    std::optional<std::size_t> number_of_motorist_injured{};
    std::optional<std::size_t> number_of_motorist_killed{};
    std::optional<std::string> contributing_factor_vehicle_1{};
    std::optional<std::string> contributing_factor_vehicle_2{};
    std::optional<std::string> contributing_factor_vehicle_3{};
    std::optional<std::string> contributing_factor_vehicle_4{};
    std::optional<std::string> contributing_factor_vehicle_5{};
    std::optional<std::size_t> collision_id{};
    std::optional<std::string> vehicle_type_code_1{};
    std::optional<std::string> vehicle_type_code_2{};
    std::optional<std::string> vehicle_type_code_3{};
    std::optional<std::string> vehicle_type_code_4{};
    std::optional<std::string> vehicle_type_code_5{};

    for (char c : line) {
        count++;

        if (c == '"') {
            is_inside_quote = !is_inside_quote;
        // Is this a comma?
        } else if (c == ',') {
            if (is_inside_quote) {
                continue;
            }

            next_comma = count - 1;

            // Is the field non-empty?
            if ((next_comma - last_comma) > 1) {
                std::string_view field = {line.data() + last_comma + (field_index > 0 ? 1 : 0), next_comma - last_comma - 1};

                if (contains_non_whitespace(field)) {
                    CollisionField collision_field = CollisionField::UNDEFINED;
                    if (field_index < static_cast<std::underlying_type_t<CollisionField>>(CollisionField::UNDEFINED)) {
                        collision_field = static_cast<CollisionField>(field_index);
                    }

                    switch(collision_field) {
                        case CollisionField::CRASH_DATE:
                            crash_date = convert_year_month_day_date(field);
                            break;
                        case CollisionField::CRASH_TIME:
                            crash_time = convert_hour_minute_time(field);
                            break;
                        case CollisionField::BOROUGH:
                            borough = convert_string(field);
                            break;
                        case CollisionField::ZIP_CODE:
                            zip_code = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::LATITUDE:
                            latitude = convert_number<float>(field);
                            break;
                        case CollisionField::LONGITUDE:
                            longitude = convert_number<float>(field);
                            break;
                        case CollisionField::LOCATION:
                            location = convert_string(field);
                            break;
                        case CollisionField::ON_STREET_NAME:
                            on_street_name = convert_string(field);
                            break;
                        case CollisionField::CROSS_STREET_NAME:
                            cross_street_name = convert_string(field);
                            break;
                        case CollisionField::OFF_STREET_NAME:
                            off_street_name = convert_string(field);
                            break;
                        case CollisionField::NUMBER_OF_PERSONS_INJURED:
                            number_of_persons_injured = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_PERSONS_KILLED:
                            number_of_persons_killed = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_PEDESTRIANS_INJURED:
                            number_of_pedestrians_injured = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_PEDESTRIANS_KILLED:
                            number_of_pedestrians_killed = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_CYCLIST_INJURED:
                            number_of_cyclist_injured = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_CYCLIST_KILLED:
                            number_of_cyclist_killed = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_MOTORIST_INJURED:
                            number_of_motorist_injured = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::NUMBER_OF_MOTORIST_KILLED:
                            number_of_motorist_killed = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::CONTRIBUTING_FACTOR_VEHICLE_1:
                            contributing_factor_vehicle_1 = convert_string(field);
                            break;
                        case CollisionField::CONTRIBUTING_FACTOR_VEHICLE_2:
                            contributing_factor_vehicle_2 = convert_string(field);
                            break;
                        case CollisionField::CONTRIBUTING_FACTOR_VEHICLE_3:
                            contributing_factor_vehicle_3 = convert_string(field);
                            break;
                        case CollisionField::CONTRIBUTING_FACTOR_VEHICLE_4:
                            contributing_factor_vehicle_4 = convert_string(field);
                            break;
                        case CollisionField::CONTRIBUTING_FACTOR_VEHICLE_5:
                            contributing_factor_vehicle_5 = convert_string(field);
                            break;
                        case CollisionField::COLLISION_ID:
                            collision_id = convert_number<std::size_t>(field);
                            break;
                        case CollisionField::VEHICLE_TYPE_CODE_1:
                            vehicle_type_code_1 = convert_string(field);
                            break;
                        case CollisionField::VEHICLE_TYPE_CODE_2:
                            vehicle_type_code_2 = convert_string(field);
                            break;
                        case CollisionField::VEHICLE_TYPE_CODE_3:
                            vehicle_type_code_3 = convert_string(field);
                            break;
                        case CollisionField::VEHICLE_TYPE_CODE_4:
                            vehicle_type_code_4 = convert_string(field);
                            break;
                        case CollisionField::VEHICLE_TYPE_CODE_5:
                            vehicle_type_code_5 = convert_string(field);
                            break;
                        case CollisionField::UNDEFINED:
                        default:
                            std::cerr << "Unknown field_index: " << field_index << std::endl;
                    }
                }
            }

            last_comma = next_comma;
            field_index++;
        }
    }

    if (field_index != 28) {
        std::cerr << "Too few fields on csv line: " << line << std::endl;
        return;
    }

    collisions.crash_dates.push_back(crash_date);
    collisions.crash_times.push_back(crash_time);
    collisions.boroughs.push_back(borough);
    collisions.zip_codes.push_back(zip_code);
    collisions.latitudes.push_back(latitude);
    collisions.longitudes.push_back(longitude);
    collisions.locations.push_back(location);
    collisions.on_street_names.push_back(on_street_name);
    collisions.cross_street_names.push_back(cross_street_name);
    collisions.off_street_names.push_back(off_street_name);
    collisions.numbers_of_persons_injured.push_back(number_of_persons_injured);
    collisions.numbers_of_persons_killed.push_back(number_of_persons_killed);
    collisions.numbers_of_pedestrians_injured.push_back(number_of_pedestrians_injured);
    collisions.numbers_of_pedestrians_killed.push_back(number_of_pedestrians_killed);
    collisions.numbers_of_cyclist_injured.push_back(number_of_cyclist_injured);
    collisions.numbers_of_cyclist_killed.push_back(number_of_cyclist_killed);
    collisions.numbers_of_motorist_injured.push_back(number_of_motorist_injured);
    collisions.numbers_of_motorist_killed.push_back(number_of_motorist_killed);
    collisions.contributing_factor_vehicles_1.push_back(contributing_factor_vehicle_1);
    collisions.contributing_factor_vehicles_2.push_back(contributing_factor_vehicle_2);
    collisions.contributing_factor_vehicles_3.push_back(contributing_factor_vehicle_3);
    collisions.contributing_factor_vehicles_4.push_back(contributing_factor_vehicle_4);
    collisions.contributing_factor_vehicles_5.push_back(contributing_factor_vehicle_5);
    collisions.collision_ids.push_back(collision_id);
    collisions.vehicle_type_codes_1.push_back(vehicle_type_code_1);
    collisions.vehicle_type_codes_2.push_back(vehicle_type_code_2);
    collisions.vehicle_type_codes_3.push_back(vehicle_type_code_3);
    collisions.vehicle_type_codes_4.push_back(vehicle_type_code_4);
    collisions.vehicle_type_codes_5.push_back(vehicle_type_code_5);

    /*
    if (borough){
        std::cout << *borough;
    }

    

    auto index = collisions.boroughs.size() - 1 ;
    const std::string key = *borough;  // Dereference the optional to get the string key
    
    auto it = collisions.buckets.find(key);
    
    if (it != collisions.buckets.end()) {
        // If the key exists, add the index to the existing vector
        it->second.push_back(index);
    } else {
        // If the key doesn't exist, create a new entry with the key and add the index
        collisions.buckets[key] = {index};
    } 
    */
   
    

    
}

}  // namespace

CollisionParser::CollisionParser(const std::string& filename)
  : filename(filename) {}

Collisions CollisionParser::parse() {
    std::ifstream file{std::string(this->filename)};

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + this->filename);
    }

    std::string line;
    std::vector<std::string> lines;

    bool is_first_line = true;
    while (std::getline(file, line)) {
        if (is_first_line) {
            is_first_line = false;
            continue;
        }

        lines.push_back(line);
    }

    Collisions collisions{};

    unsigned long num_threads = omp_get_max_threads();
    std::vector<Collisions> thread_local_collisions{num_threads};

    #pragma omp parallel for schedule(static)
    for (const std::string& line : lines) {
        int thread_id = omp_get_thread_num();
        parseline(line, thread_local_collisions[thread_id]);
    }

    for (const auto& thread_collisions : thread_local_collisions) {
        collisions.combine(thread_collisions);
    }

    int index = 0;
    for (const auto& borough : collisions.boroughs){
        
        if (borough) {
            
            //std::cout << *borough << std::endl;  // Dereference to get the string
            const std::string key = *borough;  // Dereference the optional to get the string key
            auto it = collisions.buckets.find(key);
            if (it != collisions.buckets.end()) {
                // If the key exists, add the index to the existing vector
                it->second.push_back(index);
            } else {
                // If the key doesn't exist, create a new entry with the key and add the index
                collisions.buckets[key] = {index};
            }

        }
        ++index;
    }    

    
    




    return collisions;
}
