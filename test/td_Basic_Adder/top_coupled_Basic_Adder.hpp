
#ifndef top_Basic_Adder_TD
#define top_Basic_Adder_TD

#include <cadmium/modeling/devs/coupled.hpp>


#include "td_helpers.hpp"
#include "Generate.hpp"
#include "Comparator.hpp"
#include "Decider.hpp"

#include <tuple>
#include <map>
#include <vector>
#include <variant>
#include <limits>
#include <string>
#include <cassert>

#include "atomic_models\Basic_Adder.hpp"
#include "Basic_Adder_Variant_Goblin.hpp"


	/**
	* @class Top_Basic_Adder
	* @brief Model used to organize the testing environment of the Basic_Adder DEVS model.
	*
	* This class is used to connect the generator and oracle model to the model under test. The code that needs to be modified for
	* anyone to use it are the port connections, the names of the models, and the test cases.
	*/
	class top_Basic_Adder : public Coupled {

		public:
			/**
			* Constructor function for a Top model.
			* @param id ID of the model.
			* @param test_set_enumeration Test set number that the set_test_cases() function uses to give the input data to the generator.
			* @param log_file_name name of log file.
			*/
			top_Basic_Adder(const std::string& id, int test_set_enumeration, std::string log_file_name, 
								std::map<int, std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>>> generator_data,
								std::map<int, std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>>> comparator_data,
								std::map<int, std::map<std::string, std::vector<std::string>>> path_data,
        						std::map<int, std::map<int, Variant_Goblin>> con_args_data) : Coupled(id) {

            auto gen_arg1			= addComponent<Generator<int>>("gen_arg1", set_test_data<int, Variant_Goblin>(test_set_enumeration, "arg1", generator_data));
            auto gen_arg2			= addComponent<Generator<int>>("gen_arg2", set_test_data<int, Variant_Goblin>(test_set_enumeration, "arg2", generator_data));

            auto model_Basic_Adder        	= addComponent<Basic_Adder>("Basic_Adder");
            //auto comp_sum			= addComponent<Comparator<int>>("comp_sum", set_oracle_data<int, Variant_Goblin>(test_set_enumeration, "sum", comparator_data));

            //auto decider				= addComponent<Decider>("decider", set_path_data(test_set_enumeration, path_data), get_number_of_conditions<Variant_Goblin>(comparator_data[test_set_enumeration]), log_file_name);
            addCoupling(gen_arg1->output_port, model_Basic_Adder->arg1);
            addCoupling(gen_arg2->output_port, model_Basic_Adder->arg2);

            //addCoupling(model_Basic_Adder->sum, comp_sum->input_port);

            //addCoupling(comp_sum->report_port, decider->report_port);


		}

};

#endif //top_Basic_Adder_TD
