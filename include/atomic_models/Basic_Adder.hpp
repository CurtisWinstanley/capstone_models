#ifndef Basic_Adder_HPP
#define Basic_Adder_HPP
#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
// C++ Standard Library Headers
#include <limits>
#include <string>
#include "GenericSharedMemoryModel.hpp"

using namespace cadmium;

// ------ Your includes list ------- 


             
//enum for representing the states
enum Basic_Adder_States {    
   WAIT_A1, 
   SUM, 
   WAIT_A2 

}; //end of the enum


// Function to convert the enum for states to an enum to the corresponding string
std::string enumToString(Basic_Adder_States state) {
    switch (state) {
        case Basic_Adder_States::WAIT_A1:
            return "WAIT_A1" ;

        case Basic_Adder_States::SUM:
            return "SUM" ;

        case Basic_Adder_States::WAIT_A2:
            return "WAIT_A2" ;

        default:
            return "invalid";
    }
}

struct Basic_Adder_shared_memory_struct {
    mutable int curr_arg1;
    mutable int curr_arg2;
    mutable bool arg_1_ready;
    mutable bool arg_2_ready;
    mutable int sum_bags;

};

//struct that contains the state variables of the model (as well as some other stuff)
struct Basic_Adder_State {
	int nInternals, nExternals, nInputs;
	double clock, sigma;
    Basic_Adder_States ___current_state___;
    //State Variables of the model
    mutable int curr_arg1;
    mutable int curr_arg2;
    mutable bool arg_1_ready;
    mutable bool arg_2_ready;
    mutable int sum_bags;
    mutable std::shared_ptr<int> my_ptr;

	Basic_Adder_State(): nInternals(), nExternals(), nInputs(), clock(), sigma(std::numeric_limits<double>::infinity()), ___current_state___() {}
};
    
std::ostream &operator << (std::ostream& os, const Basic_Adder_State& x) {
    os << "[-|state|-]" << enumToString(x.___current_state___) << "[-|state|-]";
    os << "[-|~:curr_arg1:~|-]" << x.curr_arg1 << "[-|~:curr_arg1:~|-]";
    os << "[-|~:curr_arg2:~|-]" << x.curr_arg2 << "[-|~:curr_arg2:~|-]";
    os << "[-|~:arg_1_ready:~|-]" << x.arg_1_ready << "[-|~:arg_1_ready:~|-]";
    os << "[-|~:arg_2_ready:~|-]" << x.arg_2_ready << "[-|~:arg_2_ready:~|-]";
    os << "[-|~:sum_bags:~|-]" << x.sum_bags << "[-|~:sum_bags:~|-]";

    return os;
}

class Basic_Adder: public Atomic<Basic_Adder_State> {
    protected:
        std::string model_id;
        
        /// Model for accessing the shared memory segment.
        // std::shared_ptr<GenericSharedMemoryModel<Basic_Adder_shared_memory_struct>> model;
        
        Basic_Adder_shared_memory_struct struct_for_shared_mem; //struct that will be used to hold state variables for populating shared memory
        
    public:
        Port<int> arg1;
        Port<int> arg2;
        Port<int> sum;


        explicit Basic_Adder(const std::string& id): Atomic<Basic_Adder_State>(id, Basic_Adder_State()) { 
            arg1 = addInPort<int>("arg1");
            arg2 = addInPort<int>("arg2");
            sum = addOutPort<int>("sum");

            state.___current_state___ = Basic_Adder_States::WAIT_A1; //Initial state is WAIT_A1
            state.nExternals = 0;
            state.nInternals = 0;
            state.nInputs = 0;
            model_id = id;
            
            // model = std::make_shared<GenericSharedMemoryModel<Basic_Adder_shared_memory_struct>>(model_id);
            // Connect the shared memory model.
            // if(!model->connect()) {
            //     std::cout << "[ERROR]: Could not connect the " <<model_id<< " shared memory model"<< std::endl;
            // }
            
            //set state vars
            state.curr_arg1 = 0;
            state.curr_arg2 = 0;
            state.arg_1_ready = false;
            state.arg_2_ready = false;

            state.sum_bags = 0;
        }
        using Atomic<Basic_Adder_State>::internalTransition;
        using Atomic<Basic_Adder_State>::externalTransition;
        using Atomic<Basic_Adder_State>::confluentTransition;
        using Atomic<Basic_Adder_State>::output;
        using Atomic<Basic_Adder_State>::timeAdvance;

        const Basic_Adder_State& getState() {
            return state;
        }
        void internalTransition(Basic_Adder_State& state) const override {
            state.clock += state.sigma;
            //state.sigma = ++state.nInternals;
            switch (state.___current_state___) { 
                case Basic_Adder_States::SUM:{
                    state.sum_bags = 0;

                    if(true)
                    {
                        state.___current_state___ = Basic_Adder_States::WAIT_A1;
                    }
                    break;
                
                }
                default:
                    break;
            }
            // write_to_shared_mem(state);
            return;
        }
        
        void externalTransition(Basic_Adder_State& state, double e) const override {
            state.clock += e;
            state.nExternals += 1;

            //store information about total number of inputs
            state.nInputs += (int) arg1->size();
            state.nInputs += (int) arg2->size();
            //flags for port received
            bool r_arg1 = !arg1->empty();
            bool r_arg2 = !arg2->empty();

            switch (state.___current_state___) {
                case Basic_Adder_States::WAIT_A1:{


                    if(r_arg1){
                        state.curr_arg1 = arg1->getBag().back();
                    }

                    if(r_arg1){state.arg_1_ready = true;}


                    if((state.arg_1_ready == true && state.arg_2_ready == false))
                    {
                        state.___current_state___ = Basic_Adder_States::WAIT_A2;
                    }
                    break;
                }
                case Basic_Adder_States::WAIT_A2:{


                    if(r_arg2){
                        state.curr_arg2 = arg2->getBag().back();
                    }

                    if(r_arg2){state.arg_2_ready = true;}


                    if((state.arg_1_ready == true && state.arg_2_ready == true))
                    {
                        state.___current_state___ = Basic_Adder_States::SUM;
                    }
                    break;
                }
                default:
                    break;
            }
            // write_to_shared_mem(state);
            return;
        }
             
        void output(const Basic_Adder_State& state) const override {
            switch (state.___current_state___) { 
                case Basic_Adder_States::SUM:{
                    //reset the flags and add the sum!
                    state.arg_1_ready = false;
                    state.arg_2_ready = false;
                    state.sum_bags = state.curr_arg1 + state.curr_arg2;
                    if(true)
                    {
                        sum->addMessage(state.sum_bags);

                    }
                    break;
                
                }     
                
                default:
                    break;
                
            }
            return;
        }

        [[nodiscard]] double timeAdvance(const Basic_Adder_State& state) const override {
                switch (state.___current_state___) { 

            case Basic_Adder_States::WAIT_A1:
                return std::numeric_limits<double>::infinity();

            case Basic_Adder_States::SUM:
                return 5;

            case Basic_Adder_States::WAIT_A2:
                return std::numeric_limits<double>::infinity();

                default:
                    break;
            }
            return state.sigma;
        }      
};
#endif //Basic_Adder_HPP