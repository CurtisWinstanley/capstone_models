#ifndef GENERATE_HPP
#define GENERATE_HPP

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

struct Generator_State {
	int nExternals, nInternals, nInputs;
	double clock, sigma;
	Generator_State(): nInternals(), nExternals(), nInputs(), clock(), sigma() {}
};

std::ostream &operator << (std::ostream& os, const Generator_State& x) {
	os << "<" << x.nInternals << "," << x.nExternals << "," << x.nInputs << "," << x.clock << "," << x.sigma << ">";
	return os;
}

template<typename TYPE>
class Generator: public Atomic<Generator_State> {

    public:
		Port<TYPE> output_port;

		explicit Generator(const std::string& id, std::vector<std::tuple<double, TYPE>> output_data): Atomic<Generator_State>(id, Generator_State()) {
			MODEL_NAME = id;

			output_port = addOutPort<TYPE>("output_port");
            data_to_generate = output_data;
			if (data_to_generate.empty()) 
			{
				// The vector is empty so tell the decider nothing left to output
				state.sigma = std::numeric_limits<double>::infinity();
			} 
			else 
			{
				// The vector is not empty
				state.sigma = std::get<0>(data_to_generate[index]);
			}
		}

		using Atomic<Generator_State>::internalTransition;
		using Atomic<Generator_State>::externalTransition;
		using Atomic<Generator_State>::confluentTransition;
		using Atomic<Generator_State>::output;
		using Atomic<Generator_State>::timeAdvance;

		const Generator_State& getState() {
			return state;
		}

		void internalTransition(Generator_State& s) const override {
            s.clock += s.sigma;

            index = index + 1;
            if(data_to_generate.size() == index) // if this condition is true then end this model simulation
            {
                s.sigma = std::numeric_limits<double>::infinity();
                return;
            }
            s.sigma = std::get<0>(data_to_generate[index]) - s.clock;
            return;
		}

		/*EXT TRANSITION NEVER USED FOR GENERATOR MODELS*/
		void externalTransition(Generator_State& s, double e) const override {
			s.clock += e;
			s.sigma = ++s.nExternals;
			return;
		}

		void output(const Generator_State& s) const override {
			
			std::cout << "model ID: " <<  MODEL_NAME << " generating..." << std::endl;
			output_port->addMessage(std::get<1>(data_to_generate[index]));
			return;
		}


		[[nodiscard]] double timeAdvance(const Generator_State& s) const override {
			return s.sigma;
		}

    
    private:

        mutable int index = 0;

        std::vector<std::tuple<double, TYPE>> data_to_generate;

		mutable std::string MODEL_NAME;

};
#endif //GENERATE_HPP

