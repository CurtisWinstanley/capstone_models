#ifndef COMPARATOR_HPP
#define COMPARATOR_HPP

#include <cadmium/modeling/devs/atomic.hpp>

#include <iostream>
// C++ Standard Library Headers
#include <limits>
#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <variant>

using namespace cadmium;

struct Comparator_State {
	int nExternals, nInternals, nInputs;
	double clock, sigma;
	Comparator_State(): nInternals(), nExternals(), nInputs(), clock(), sigma(std::numeric_limits<double>::infinity()) {}
};

std::ostream &operator << (std::ostream& os, const Comparator_State& x) {
	os << "<" << x.nInternals << "," << x.nExternals << "," << x.nInputs << "," << x.clock << "," << x.sigma << ">";
	return os;
}

template<typename TYPE>
class Comparator: public Atomic<Comparator_State> {

    public:
		Port<TYPE> input_port;
		Port<comparator_report_struct> report_port;

		explicit Comparator(const std::string& id, std::vector<std::tuple<int, TYPE>> e_data): Atomic<Comparator_State>(id, Comparator_State()) {
			MODEL_NAME = id;
			input_port = addInPort<TYPE>("input_port");
			report_port = addOutPort<comparator_report_struct>("report_port");
            expected_data = e_data;

			if(expected_data.size() == 0)
			{
				expecting_nothing = true;
			}
		}

		using Atomic<Comparator_State>::internalTransition;
		using Atomic<Comparator_State>::externalTransition;
		using Atomic<Comparator_State>::confluentTransition;
		using Atomic<Comparator_State>::output;
		using Atomic<Comparator_State>::timeAdvance;

		const Comparator_State& getState() {
			return state;
		}

		void internalTransition(Comparator_State& s) const override {
            s.clock += s.sigma;
			s.sigma = std::numeric_limits<double>::infinity();
            return;
		}

		void externalTransition(Comparator_State& s, double e) const override {
			s.clock += e;

			bool received_data = !input_port->empty(); //Port flags here

			std::vector<TYPE> v_data;

			if(received_data)
			{
				std::cout << "model ID: " <<  MODEL_NAME << " comparing..." << std::endl;
				v_data = input_port->getBag();
				s.sigma = 0;

				//check if the vector is expecting more inputs. if not then the model under test is outputting unexpected data
				if((index > expected_data.size()-1) || expecting_nothing)
				{
					comparator_report_struct r = comparator_report_struct();
					r.condition_entry = -1;
					r.result = false;
					comparator_report.push_back(r);
					return;
				}

				for (auto data : v_data)
				{
					comparator_report_struct r = comparator_report_struct();
					r.condition_entry = std::get<0>(expected_data[index]);
					std::cout << "			model ID: " <<  MODEL_NAME << " received output number: " << r.condition_entry << std::endl;

					if(data == std::get<1>(expected_data[index]))
					{
						//the first part of each tuple in the report is the condition index
						r.result = true;
						comparator_report.push_back(r);
					}
					else
					{
						r.result = false;
						comparator_report.push_back(r);
					}
					index = index + 1; //condition incrementor
				}
			}
			return;
		}

		void output(const Comparator_State& s) const override {
			
			for(auto r : comparator_report)
			{
				report_port->addMessage(r);
			}
			return;
		}


		[[nodiscard]] double timeAdvance(const Comparator_State& s) const override {
			return s.sigma;
		}

    
    private:

        mutable int index = 0;

        std::vector<std::tuple<int, TYPE>> expected_data;
		mutable std::vector<comparator_report_struct> comparator_report;

		mutable std::string MODEL_NAME;

		mutable bool expecting_nothing = false;

};
#endif //GENERATE_HPP

