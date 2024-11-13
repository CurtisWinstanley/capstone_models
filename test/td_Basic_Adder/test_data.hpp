
#ifndef TEST_DATA_Basic_Adder
#define TEST_DATA_Basic_Adder
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <variant>


//seek the Variant Goblin to find your variants...
#include "Basic_Adder_Variant_Goblin.hpp"
std::map<int, std::string> get_test_names()
{
    std::map<int, std::string> names;
    names[1] = "two_positive_numbers";
    names[2] = "two_negative_numbers";
    names[3] = "two_numbers_twice";
    return names;
}
    //---------------------------------------------------------
    //Test Case Data   ---------------------------------------------------------
    //---------------------------------------------------------
std::map<int, std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>>> get_test_cases()
{

    //Test Case: 1 Input Data ---------------------------------------------------------
    std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>> tc1;
    std::vector<std::tuple<double, Variant_Goblin>> arg1_1; 
    arg1_1.push_back(std::make_tuple(1, int(50)));
    tc1["arg1"] = arg1_1;
    std::vector<std::tuple<double, Variant_Goblin>> arg2_1; 
    arg2_1.push_back(std::make_tuple(3, int(40)));
    tc1["arg2"] = arg2_1;

    //Test Case: 2 Input Data ---------------------------------------------------------
    std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>> tc2;
    std::vector<std::tuple<double, Variant_Goblin>> arg1_2; 
    arg1_2.push_back(std::make_tuple(2, int(-20)));
    tc2["arg1"] = arg1_2;
    std::vector<std::tuple<double, Variant_Goblin>> arg2_2; 
    arg2_2.push_back(std::make_tuple(4, int(-20)));
    tc2["arg2"] = arg2_2;

    //Test Case: 3 Input Data ---------------------------------------------------------
    std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>> tc3;
    std::vector<std::tuple<double, Variant_Goblin>> arg1_3; 
    arg1_3.push_back(std::make_tuple(1, int(5)));
    arg1_3.push_back(std::make_tuple(3, int(-10)));
    tc3["arg1"] = arg1_3;
    std::vector<std::tuple<double, Variant_Goblin>> arg2_3; 
    arg2_3.push_back(std::make_tuple(2, int(5)));
    arg2_3.push_back(std::make_tuple(4, int(5)));
    tc3["arg2"] = arg2_3;

    std::map<int, std::map<std::string, std::vector<std::tuple<double, Variant_Goblin>>>> test_cases;
    test_cases[1] = tc1;
    test_cases[2] = tc2;
    test_cases[3] = tc3;
    return test_cases;
}

    //---------------------------------------------------------
    //Expected Outputs   ---------------------------------------------------------
    //---------------------------------------------------------
std::map<int, std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>>> get_comparator_data()
{

    //Test Case: 1 Expected Outputs ---------------------------------------------------------
    std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>> eo1;
    std::vector<std::tuple<int, Variant_Goblin>> sum_1_eo; 
    sum_1_eo.push_back(std::make_tuple(1, int(90)));
    eo1["sum"] = sum_1_eo;

    //Test Case: 2 Expected Outputs ---------------------------------------------------------
    std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>> eo2;
    std::vector<std::tuple<int, Variant_Goblin>> sum_2_eo; 
    sum_2_eo.push_back(std::make_tuple(1, int(-40)));
    eo2["sum"] = sum_2_eo;

    //Test Case: 3 Expected Outputs ---------------------------------------------------------
    std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>> eo3;
    std::vector<std::tuple<int, Variant_Goblin>> sum_3_eo; 
    sum_3_eo.push_back(std::make_tuple(1, int(10)));
    sum_3_eo.push_back(std::make_tuple(2, int(-5)));
    eo3["sum"] = sum_3_eo;

    std::map<int, std::map<std::string, std::vector<std::tuple<int, Variant_Goblin>>>> comparator_data;
    comparator_data[1] = eo1;
    comparator_data[2] = eo2;
    comparator_data[3] = eo3;
    return comparator_data;
}

    //---------------------------------------------------------
    //Expected State Transitions   ---------------------------------------------------------
    //---------------------------------------------------------
std::map<int, std::map<std::string, std::vector<std::string>>> get_path_data()
{

    //Test Case: 1 State Transitions ---------------------------------------------------------
    std::vector<std::string> Basic_Adder_st_1 = {"WAIT_A1", "WAIT_A2", "SUM", "WAIT_A1"};
    std::map<std::string, std::vector<std::string>> test_paths_tc1;
    test_paths_tc1["Basic_Adder"] = Basic_Adder_st_1;

    //Test Case: 2 State Transitions ---------------------------------------------------------
    std::vector<std::string> Basic_Adder_st_2 = {};
    std::map<std::string, std::vector<std::string>> test_paths_tc2;
    test_paths_tc2["Basic_Adder"] = Basic_Adder_st_2;

    //Test Case: 3 State Transitions ---------------------------------------------------------
    std::vector<std::string> Basic_Adder_st_3 = {"WAIT_A1", "SUM", "WAIT_A2", "WAIT_A1", "WAIT_A2", "SUM", "WAIT_A1"};
    std::map<std::string, std::vector<std::string>> test_paths_tc3;
    test_paths_tc3["Basic_Adder"] = Basic_Adder_st_3;

    std::map<int, std::map<std::string, std::vector<std::string>>> path_data;
    path_data[1] = test_paths_tc1;
    path_data[2] = test_paths_tc2;
    path_data[3] = test_paths_tc3;
    return path_data;
}

    //---------------------------------------------------------
    //Constructor Args   ---------------------------------------------------------
    //---------------------------------------------------------
std::map<int, std::map<int, Variant_Goblin>> get_constructor_data()
{

    //Test Case: 1 Constructor Args ---------------------------------------------------------
    std::map<int, Variant_Goblin> Basic_Adder_ca_1;

    //Test Case: 2 Constructor Args ---------------------------------------------------------
    std::map<int, Variant_Goblin> Basic_Adder_ca_2;

    //Test Case: 3 Constructor Args ---------------------------------------------------------
    std::map<int, Variant_Goblin> Basic_Adder_ca_3;
    std::map<int,  std::map<int, Variant_Goblin>> con_args_data;
    con_args_data[1] = Basic_Adder_ca_1;
    con_args_data[2] = Basic_Adder_ca_2;
    con_args_data[3] = Basic_Adder_ca_3;
    return con_args_data;
}
int get_test_set_size()
{
    return 3;
}
#endif;
