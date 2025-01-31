#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

const std::string_view CSV_FILENAME = "Motor_Vehicle_Collisions_-_Crashes_20250123.csv";

struct Collision {
    std::optional<std::chrono::year_month_day> crash_date;
};

std::optional<std::chrono::year_month_day> convert_date(const std::string_view& field) {
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

    int month, day, year;

    auto month_result = std::from_chars(field.data(), field.data() + first_slash, month);
    auto day_result = std::from_chars(field.data() + first_slash + 1, field.data() + second_slash, day);
    auto year_result = std::from_chars(field.data() + second_slash + 1, field.data() + field.size(), year);

    if (month_result.ec != std::errc() || day_result.ec != std::errc() || year_result.ec != std::errc()) {
        std::cerr << "Error parsing date: " << field << std::endl;
        return {};
    }

    return std::chrono::year_month_day{std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
}

Collision parseline(const std::string& line) {

    std::size_t count = 0;
    std::size_t last_comma = 0;
    std::size_t next_comma = 0;
    std::size_t field_index = 0;

    Collision collision{};
    for (char c : line) {
        // Is this a comma?
        if (c == ',') {
            next_comma = count;

            // Is the field non-empty?
            if ((next_comma - last_comma) > 1) {
                std::string_view field = {line.data() + last_comma, next_comma - last_comma};

                switch(field_index) {
                    case 0:
                        collision.crash_date = convert_date(field);
                        break;
//                    default:
//                        std::cerr << "Unknown field_index: " << field_index << std::endl;
                }
            }

            last_comma = next_comma;
            field_index++;
        }

        count++;
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
}
