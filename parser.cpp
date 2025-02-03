#include <chrono>
#include <iostream>
#include <format>
#include <fstream>
#include <string>
#include <string_view>

const std::string_view CSV_FILENAME = "Motor_Vehicle_Collisions_-_Crashes_20250123.csv";

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
};

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
    auto year_result = std::from_chars(field.data() + second_slash + 1, field.data() + field.size(), year);

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

Collision parseline(const std::string& line) {

    bool is_inside_quote = false;
    std::size_t count = 0;
    std::size_t last_comma = 0;
    std::size_t next_comma = 0;
    std::size_t field_index = 0;

    Collision collision{};
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
                    switch(field_index) {
                        case 0:
                            collision.crash_date = convert_year_month_day_date(field);
                            break;
                        case 1:
                            collision.crash_time = convert_hour_minute_time(field);
                            break;
                        case 2:
                            collision.borough = convert_string(field);
                            break;
                        case 3:
                            collision.zip_code = convert_number<std::size_t>(field);
                            break;
                        case 4:
                            collision.latitude = convert_number<float>(field);
                            break;
                        case 5:
                            collision.longitude = convert_number<float>(field);
                            break;
                        case 6:
                            collision.location = convert_string(field);
                            break;
                        case 7:
                            collision.on_street_name = convert_string(field);
                            break;
                        case 8:
                            collision.cross_street_name = convert_string(field);
                            break;
                        case 9:
                            collision.off_street_name = convert_string(field);
                            break;
                        case 10:
                            collision.number_of_persons_injured = convert_number<std::size_t>(field);
                            break;
                        case 11:
                            collision.number_of_persons_killed = convert_number<std::size_t>(field);
                            break;
                        case 12:
                            collision.number_of_pedestrians_injured = convert_number<std::size_t>(field);
                            break;
                        case 13:
                            collision.number_of_pedestrians_killed = convert_number<std::size_t>(field);
                            break;
                        case 14:
                            collision.number_of_cyclist_injured = convert_number<std::size_t>(field);
                            break;
                        case 15:
                            collision.number_of_cyclist_killed = convert_number<std::size_t>(field);
                            break;
                        case 16:
                            collision.number_of_motorist_injured = convert_number<std::size_t>(field);
                            break;
                        case 17:
                            collision.number_of_motorist_killed = convert_number<std::size_t>(field);
                            break;
                        case 18:
                            collision.contributing_factor_vehicle_1 = convert_string(field);
                            break;
                        case 19:
                            collision.contributing_factor_vehicle_2 = convert_string(field);
                            break;
                        case 20:
                            collision.contributing_factor_vehicle_3 = convert_string(field);
                            break;
                        case 21:
                            collision.contributing_factor_vehicle_4 = convert_string(field);
                            break;
                        case 22:
                            collision.contributing_factor_vehicle_5 = convert_string(field);
                            break;
                        case 23:
                            collision.collision_id = convert_number<std::size_t>(field);
                            break;
                        case 24:
                            collision.vehicle_type_code_1 = convert_string(field);
                            break;
                        case 25:
                            collision.vehicle_type_code_2 = convert_string(field);
                            break;
                        case 26:
                            collision.vehicle_type_code_3 = convert_string(field);
                            break;
                        case 27:
                            collision.vehicle_type_code_4 = convert_string(field);
                            break;
                        case 28:
                            collision.vehicle_type_code_5 = convert_string(field);
                            break;

//                        default:
//                            std::cerr << "Unknown field_index: " << field_index << std::endl;
                    }
                }
            }

            last_comma = next_comma;
            field_index++;
        }
    }

    if (field_index != 28) {
        std::cerr << "Too few fields on csv line: " << line << std::endl;
    }

    return collision;
}

std::vector<Collision> parsefile(std::ifstream& file) {
    std::string line;
    std::vector<Collision> collisions;

    bool is_first_line = true;
    while (std::getline(file, line)) {
        if (is_first_line) {
            is_first_line = false;
            continue;
        }

        Collision collision = parseline(line);
        collisions.push_back(collision);
    }

    return collisions;
}

int main() {
    std::ifstream file{std::string(CSV_FILENAME)};

    if (!file.is_open()) {
        std::cerr << "Could not open file " << CSV_FILENAME << std::endl;
        return 1;
    }

    std::vector<Collision> collisions = parsefile(file);

    const Collision& collision = collisions.at(0);
    std::cout << collision << std::endl;
    std::cout << collisions.at(1) << std::endl;
    std::cout << collisions.at(2) << std::endl;
    std::cout << collisions.at(3) << std::endl;
    std::cout << collisions.at(4) << std::endl;
    std::cout << collisions.at(5) << std::endl;
}
