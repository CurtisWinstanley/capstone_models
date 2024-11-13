#ifndef TD_HELPERS_HPP
#define TD_HELPERS_HPP

// C++ Standard Library Headers
#include <limits>
#include <string>
#include <map>
#include <vector>
#include <variant>
#include <cassert>
#include <filesystem>


struct comparator_report_struct{
    int condition_entry;
    bool result;

};

std::ostream& operator<<(std::ostream& out, const comparator_report_struct& s) {
    out << "{ REPORT: ";
    out << (unsigned int) s.condition_entry << ", " <<  s.result;
    out << " }";

    return out;
}

// enum Execution_Location{INTERNAL_TRANSITION, OUTPUT};

// template <typename... Types>
// void generate(const std::map<int, std::pair<std::string, std::variant<Types...>>>& generated_values,
//             const std::vector<float> output_times,
//             const std::vector<std::shared_ptr<cadmium::PortInterface>>& model_ports,
//             Execution_Location location,
//             int* transition_counter,
//             double* sigma_ptr,
//             double clock) 
// {
//     if (location == Execution_Location::INTERNAL_TRANSITION) {
//         handle_internal(output_times, sigma_ptr, transition_counter, clock);
//     }

//     if (location == Execution_Location::OUTPUT) 
//     {
//         handle_output(generated_values, model_ports, transition_counter);
//     }
// }


// template <typename T>
// void handlePortType(const std::shared_ptr<cadmium::PortInterface>& port, const T& value) 
// {
//     if (auto typedPort = std::dynamic_pointer_cast<cadmium::_Port<T>>(port)) {
//         // Do something specific for ports of type _Port<T>
//         typedPort->addMessage(value);
//     }
// }



// void handle_internal(const std::vector<float>& output_times, double* sigma_ptr, int* transition_counter, double clock)
// {
//     if(*transition_counter ==  std::size(output_times))
//     {
//         *sigma_ptr = std::numeric_limits<double>::infinity();
//         return;
//     }
//     *sigma_ptr = output_times[*transition_counter] - clock;
//     //*sigma_ptr = output_times[*transition_counter];
//     *transition_counter = *transition_counter + 1;
//     return;
// }

// template <typename... Types>
// void handle_output(const std::map<int, std::pair<std::string, std::variant<Types...>>>& generated_values, 
//             const std::vector<std::shared_ptr<cadmium::PortInterface>>& model_ports,
//             int* transition_counter)
// {
//     for (const auto& port : model_ports) 
//     {
//         auto it = generated_values.find(*transition_counter);
//         std::string p = it->second.first;
//         const auto value = it->second.second;
//         if (port->getId() == p) {
//             std::visit([&](const auto& typedValue) {
//                 handlePortType(port, typedValue);
//             }, value);
//         }
//     }
// }

/**
 * @brief Function that organizes the test cases and provides the generator with the test data.
 *
 * This function takes the test data provided in the main function and sorts is based off of the test
 * enumeration and the port we desire to send our data on
 * 
 * @param test_enum test case enumeration number
 * @param port port given that needs the test data
 * @param test_data the input data for every test
 * 
 * @return The data and corrosponding times for each peice of data organized in a vector
 */
template<typename T, typename VariantType>
std::vector<std::tuple<double, T>> set_test_data(int test_enum, std::string port,
                    std::map<int, std::map<std::string, std::vector<std::tuple<double, VariantType>>>> test_data)
{
    auto it = test_data.find(test_enum);
    if (it == test_data.end()) 
    {
        std::cout << "ERROR: test enumeration " <<test_enum<< " does not have any written test case" << std::endl;
        assert(it != test_data.end());
    }
    
    std::vector<std::tuple<double, VariantType>> data = test_data[test_enum][port];
    std::vector<std::tuple<double, T>> converted_data;

    for (const auto& tuple : data) {
        double firstValue = std::get<0>(tuple);
        VariantType variantValue = std::get<1>(tuple);

        // Convert VariantType to T
        T convertedValue = std::get<T>(variantValue);

        converted_data.emplace_back(firstValue, convertedValue);
    }


    return converted_data;
}

/**
 * @brief Function that organizes the test cases and provides the oracle with the expected outputs.
 *
 * This function takes the expected outpurs provided in the main function and sorts it based off of the test
 * enumeration and the port we expect the output to be on
 * 
 * @param test_enum test case enumeration number
 * @param port port given that needs the expected data
 * @param comparator_data the expected data for every test
 * 
 * @return The expected oracle data for the specific port specified as a parameter
 */
template<typename TYPE, typename VariantType>
std::vector<std::tuple<int, TYPE>> set_oracle_data(int test_enum, std::string port,
                    std::map<int, std::map<std::string, std::vector<std::tuple<int, VariantType>>>> comparator_data)
{

    auto it = comparator_data.find(test_enum);
    if (it == comparator_data.end()) 
    {
        std::cout << "ERROR: test enumeration " <<test_enum<< " does not have any written oracle" << std::endl;
        assert(it != comparator_data.end());
    }
    
    std::vector<std::tuple<int, VariantType>> data = comparator_data[test_enum][port];
    std::vector<std::tuple<int, TYPE>> converted_data;

    for (const auto& tuple : data) {
        double firstValue = std::get<0>(tuple);
        VariantType variantValue = std::get<1>(tuple);

        // Convert VariantType to T
        TYPE convertedValue = std::get<TYPE>(variantValue);

        converted_data.emplace_back(firstValue, convertedValue);
    }


    return converted_data;
}




/**
 * @brief Function that organizes the test cases and provides the Decider with test paths.
 *
 * This function takes the expected outpurs provided in the main function and sorts it based off of the test
 * enumeration and the port we expect the output to be on
 * 
 * @param test_enum test case enumeration number
 * @param path_data the expected test paths
 * 
 * @return The expected test paths
 */
std::map<std::string, std::vector<std::string>> set_path_data(int test_enum,
                    std::map<int, std::map<std::string, std::vector<std::string>>> path_data)
{

    auto it = path_data.find(test_enum);
    if (it == path_data.end()) 
    {
        std::cout << "ERROR: test enumeration " <<test_enum<< " does not have any written paths" << std::endl;
        assert(it != path_data.end());
    }
    
    std::map<std::string, std::vector<std::string>> paths = path_data[test_enum];

    return paths;
}

template <typename MyVariant>
int get_number_of_conditions(const std::map<std::string, std::vector<std::tuple<int, MyVariant>>>& myMap) {
    int num = 0;

    for (const auto& pair : myMap) {
        num += pair.second.size();
    }

    return num;
}



// Function that takes cpp_file_path as an argument
std::string get_log_file_dir(const std::filesystem::path& cpp_file_path, std::string test_name) {
    // Extract the directory containing the given file
    std::filesystem::path dir_path = cpp_file_path.parent_path();

    // Append "simulation_results" to the directory path
    std::filesystem::path log_file_dir = dir_path / "simulation_results";

    // Create the directory if it doesn't exist
    std::filesystem::create_directories(log_file_dir);

    // Construct the full path of the log file
    std::string log_file_name = test_name + ".csv";
    std::filesystem::path log_file_path = log_file_dir / log_file_name;

    return log_file_path.string();
}

#endif //TD_HELPERS_HPP