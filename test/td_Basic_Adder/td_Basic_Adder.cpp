
// Model Under Test
#include "top_coupled_Basic_Adder.hpp"

//cadmium headers
#include "cadmium/simulation/logger/csv.hpp"
#include <cadmium/simulation/root_coordinator.hpp>

//c++ headers
#include <limits>
#include <filesystem>

//helpers
#include "td_helpers.hpp"

//Test data header
#include "test_data.hpp"
//seek the Variant Goblin to find your variants...
#include "Basic_Adder_Variant_Goblin.hpp"

	using hclock = std::chrono::high_resolution_clock;

	int main() {
		std::cout << "------------------ running tests for: Basic_Adder ------------------" << std::endl;

		std::filesystem::path cpp_file_path = __FILE__;

		std::string model_under_test_name = "Basic_Adder";

		int test_set_size = get_test_set_size(); //From generated tests	

		std::map<int, std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>>> t = get_test_cases(); //From generated tests

		std::map<int, std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>>> c = get_comparator_data(); //From generated tests

		std::map<int, std::map<std::string, std::vector<std::string>>> p = get_path_data(); //From generated tests

		std::map<int, std::map<int, Variant_Goblin>> co = get_constructor_data(); //From generated tests

		std::map<int, std::string> names = get_test_names(); // From generated tests


		for(int test_set_enumeration = 1; test_set_enumeration <= test_set_size; test_set_enumeration++)
		{
			std::cout << "+++++ running test case: " << names[test_set_enumeration] << " +++++"<< std::endl; 
			std::string log_file = get_log_file_dir(cpp_file_path, names[test_set_enumeration]);

			auto td = std::make_shared<top_Basic_Adder>("Top_"+model_under_test_name, test_set_enumeration, log_file, t, c, p, co);

			auto rootCoordinator = cadmium::RootCoordinator(td);
			//auto logger = std::make_shared<cadmium::CSVLogger>(log_file, ";"); //initialize logger
			//rootCoordinator.setLogger(logger);
   			rootCoordinator.setLogger<cadmium::CSVLogger>(log_file, ";");

			auto start = hclock::now(); //to measure simulation execution time

			rootCoordinator.start(); //start the coordinator to begin simulation
			rootCoordinator.simulate(std::numeric_limits<double>::infinity());
			rootCoordinator.stop();

			auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
			std::cout<< "Simulation test case " + names[test_set_enumeration] + " took: " + std::to_string(elapsed) + " seconds"<< std::endl;
			std::cout<<  "TD: " << model_under_test_name <<std::endl;
			std::cout<<"-------------------------------"<<std::endl;
		}
		return 0;
	}

