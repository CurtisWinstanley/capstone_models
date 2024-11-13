#ifndef DECIDER_HPP
#define DECIDER_HPP

#include <cadmium/modeling/devs/atomic.hpp>
#include "td_helpers.hpp"

#include <iostream>
// C++ Standard Library Headers
#include <limits>
#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <variant>
#include <fstream>
#include <sstream>

using namespace cadmium;

struct Decider_State {
	int nExternals, nInternals, nInputs;
	double clock, sigma;
	Decider_State(): nInternals(), nExternals(), nInputs(), clock(), sigma(std::numeric_limits<double>::infinity()) {}
};

std::ostream &operator << (std::ostream& os, const Decider_State& x) {
	os << "<" << x.nInternals << "," << x.nExternals << "," << x.nInputs << "," << x.clock << "," << x.sigma << ">";
	return os;
}


class Decider: public Atomic<Decider_State> {

    public:

		Port<comparator_report_struct> report_port;

		explicit Decider(const std::string& id, std::map<std::string, std::vector<std::string>> tp, int n, std::string lf): Atomic<Decider_State>(id, Decider_State()) {
			
			report_port = addInPort<comparator_report_struct>("report_port");
			test_paths = tp;
			num_of_conditions = n;
			test_conditions.assign(num_of_conditions, false);
			log_file = lf;
			
		}

		using Atomic<Decider_State>::internalTransition;
		using Atomic<Decider_State>::externalTransition;
		using Atomic<Decider_State>::confluentTransition;
		using Atomic<Decider_State>::output;
		using Atomic<Decider_State>::timeAdvance;

		const Decider_State& getState() {
			return state;
		}

		void internalTransition(Decider_State& s) const override {
            s.clock += s.sigma;
			s.sigma = std::numeric_limits<double>::infinity();
            return;
		}

		void externalTransition(Decider_State& s, double e) const override {
			s.clock += e;

			bool received_report = !report_port->empty(); //Port flags here

			std::vector<comparator_report_struct> reports;

			if(received_report)
			{
				reports = report_port->getBag();

				for (auto r : reports)
				{
					//check if the report is an unexpected data report
					if(r.condition_entry == -1)
					{
						num_of_unexpected_outputs = num_of_unexpected_outputs + 1;
						return;
					}

					//this if clause makes sure that we are receiving the expected outputs in order
					if(r.condition_entry == anticipated_condition)
					{
						test_conditions[anticipated_condition - 1] = r.result;
						anticipated_condition = anticipated_condition + 1;
					}
					// if its not in order, check if it came in late
					else if(check_if_missed(r.condition_entry, missed_conditions))
					{
						late_conditions.push_back(r.condition_entry);
						missed_conditions.erase(std::remove(missed_conditions.begin(), missed_conditions.end(), r.condition_entry), missed_conditions.end());
						test_conditions[r.condition_entry - 1] = r.result;
					}
					// if we get a report that is ahead of the current expected output (ex we anticipated report 2 but got report 4)
					// then skip ahead and add the outputs we never received to the missed vector.
					else if(r.condition_entry > anticipated_condition)
					{
						for(int i = anticipated_condition; i < r.condition_entry; i++)
						{
							missed_conditions.push_back(i);
						}
						test_conditions[r.condition_entry -1] = r.result;
						anticipated_condition = r.condition_entry + 1;
					}

				}
			}
			// This checks to see if we are finished waiting for inputs by seeing if we are over the anticipated condition number
			// This could be improved by executing a callback at the end of the sim
			if(anticipated_condition > test_conditions.size())
			{
				make_output_decision(test_conditions);
				for (const auto& p : test_paths) {
					std::string model_name = p.first;
					std::vector<std::string> path = p.second;
					make_path_decision(log_file, model_name, path);
				}
			}
			s.sigma = std::numeric_limits<double>::infinity(); 
			return;
		}

		void output(const Decider_State& s) const override {
			
			return;
		}


		[[nodiscard]] double timeAdvance(const Decider_State& s) const override {
			return s.sigma;
		}

    
    private:
		mutable int anticipated_condition = 1;

        mutable int num_of_conditions;

		mutable std::vector<int> missed_conditions;

		mutable std::vector<int> late_conditions;

		mutable int num_of_unexpected_outputs = 0;

		mutable std::vector<bool> test_conditions;

		mutable std::vector<int> failed_condtitions;

		std::map<std::string, std::vector<std::string>> test_paths;

		mutable std::string log_file;

		/**
		 * \brief 	Function that decides if a test has passed or not by tracking outputted states in
		 * 			the log file and comparing it to the provided test path
		 * \param	log_file   						file to search for state transitions of model under test
		 * \param	model_name   					name of model under test
		 * \param	test_path   					test path provided to check
		 * 
		 * \return  void
		 */
		void make_path_decision(std::string log_file, std::string model_name, std::vector<std::string> test_path) const
		{

			//now check that the test path is correct
			std::ifstream inputFile(log_file);

			if (!inputFile.is_open()) {
				std::cerr << "Error opening file." << std::endl;
				return;
			}
			std::string line;
			// Read and process each line in the CSV file
			int current_state = 0;
			std::string tracked_state = test_path[current_state]; //set the tracked state to the initial state of the test path
			bool ran_out_of_trackable_states = false;
			
			std::string next_tracked_state = test_path[current_state + 1];
			bool test_path_success = true;
			while (std::getline(inputFile, line)) {
				if(line.find(model_name) != std::string::npos && line.find("~:state:~") != std::string::npos) 
				{
					if((line.find(tracked_state) != std::string::npos) || (line.find(next_tracked_state) != std::string::npos))
					{ 
						//if the state we read is the next part of the test path increment the pointer
						if((line.find(next_tracked_state) != std::string::npos)) 
						{

							if(ran_out_of_trackable_states)
							{
								//do nothing, just let the program keep searching
							}
							else
							{
								tracked_state = next_tracked_state;
								current_state = current_state + 1;
								// This makes sure we dont try to index the vector past its maximum size
								if(current_state == test_path.size() - 1)
								{
									ran_out_of_trackable_states = true;
								}
								else
								{
									next_tracked_state = test_path[current_state + 1];
								}
							}
						}
					}
					else{test_path_success = false;}
				}
			}

			inputFile.close();

			//Now display results
			if(test_path_success)
			{
				std::cout << std:: endl;
				std::cout<< model_name <<" EXPECTED PATHS PASSED"<<std::endl;
				std::cout << std:: endl;
			}
			else
			{
				std::cout << std:: endl;
				std::cout<< model_name << " EXPECTED PATHS FAILED"<<std::endl;
				std::cout << std:: endl;
			}
		}

		/**
		 * \brief 	Function that decides if a test has passed or not by tracking test conditions
		 * \param	test_conditions   	    		array of booleans set to false to indicate success of tested conditions
		 * 
		 * \return  void
		 */
		void make_output_decision(std::vector<bool> test_conditions) const
		{
			bool test_conditions_success = true;

			//first check if the output_conditions are satisfied
			for (int i = 1; i <= test_conditions.size(); ++i) 
			{
				//note I started "i" to start from one so we can think of it as condition 1
        		if(test_conditions[i-1] == false) 
				{
					failed_condtitions.push_back(i); 
					test_conditions_success = false;
				}

				if(test_conditions_success == false)
				{
					for(int c: failed_condtitions)
					{
						//for all of the failed conditions, check if any were missed. if they were not tell the user that the comparison failed.
						if(!check_if_missed(c, missed_conditions))
						{
							std::cout << "Expected output: " << c << " has failed. The expected value and outputted value did not match." << std::endl;
						}
					}
				}
    		}
			// check if any outputs came later than expected or were out of order
			if(late_conditions.size() != 0)
			{
				test_conditions_success = false;
				for(int c: late_conditions)
				{
					std::cout << "Expected output: " << c << " was outputted later than expected" << std::endl;
				}
			}

			if(missed_conditions.size() != 0)
			{
				test_conditions_success = false;
				for (int c: missed_conditions)
				{
					std::cout << "Expected output: " << c << "was never found" << std::endl;
				}
			}

			if(test_conditions_success)
			{
				std::cout << std:: endl;
				std::cout << "EXPECTED OUTPUTS PASSED" << std::endl;
				std::cout << std:: endl;
			}
			else
			{
				std::cout << std:: endl;
				std::cout << "EXPECTED OUTPUTS FAILED" << std::endl;
				std::cout << std:: endl;
			}
		}

		bool check_if_missed(int target, std::vector<int> myVector) const
		{
			auto it = std::find(myVector.begin(), myVector.end(), target);

			// Check if the value was found
			if (it != myVector.end()) {
				//std::cout << "Value " << target << " found in the vector." << std::endl;
				return true;
			} else {
				//std::cout << "Value " << target << " not found in the vector." << std::endl;
				return false;
			}
		}

};
#endif //DECIDER_HPP

