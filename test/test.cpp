// Author: Suresh Sundriyal
// License: CC0 - No rights reserved.

#include "rangejoin.hpp"
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

using namespace rangejoin;

void test_single_range_join_all_containers() {
    std::vector<std::string> vec = {"red", "green", "blue"};
    assert(join(" | ", vec) == "red | green | blue");

    std::deque<std::string> deq = {"sun", "moon"};
    assert(join(" + ", deq) == "sun + moon");

    std::list<std::string> lst = {"alpha", "beta", "gamma"};
    assert(join(" -> ", lst) == "alpha -> beta -> gamma");

    std::set<std::string> st = {"dog", "cat", "bird"};
    assert(join(", ", st) == "bird, cat, dog"); // set is ordered

    // Empty containers
    std::vector<std::string> empty_vec;
    std::deque<std::string> empty_deq;
    std::list<std::string> empty_lst;
    std::set<std::string> empty_set;

    assert(join(":", empty_vec) == "");
    assert(join(":", empty_deq) == "");
    assert(join(":", empty_lst) == "");
    assert(join(":", empty_set) == "");
}

void test_multiple_range_join_all_combinations() {
    std::vector<std::string> vec = {"A", "B"};
    std::deque<std::string> deq = {"C"};
    std::list<std::string> lst = {"D", "E"};
    std::set<std::string> st = {"F", "G"};

    assert(join("-", vec, deq, lst, st) == "A-B-C-D-E-F-G");

    std::vector<std::string> empty_vec;
    std::deque<std::string> empty_deq;
    std::list<std::string> empty_lst;
    std::set<std::string> empty_set;

    // Mixed empty and non-empty
    assert(join(":", empty_vec, deq, st) == "C:F:G");
    assert(join(":", vec, empty_lst, empty_set) == "A:B");

    // All empty
    assert(join(",", empty_vec, empty_deq, empty_lst, empty_set) == "");
}

void test_transform_join_all_containers() {
    std::vector<int> vec = {1, 2};
    std::deque<int> deq = {3};
    std::list<int> lst = {4, 5};
    std::set<int> st = {6, 7};

    auto to_string = [](int x) { return std::to_string(x); };
    assert(join(",", to_string, vec, deq, lst, st) == "1,2,3,4,5,6,7");

    std::list<char> chars = {'x', 'y'};
    auto char_to_str = [](char c) { return std::string(1, c); };
    assert(join("", char_to_str, chars) == "xy");

    // Empty containers
    std::vector<int> empty_vec;
    std::deque<int> empty_deq;
    std::list<int> empty_lst;
    std::set<int> empty_set;

    assert(join(":", to_string, empty_vec, empty_deq) == "");
    assert(join(":", to_string, empty_lst, empty_set) == "");
}

int main() {
    test_single_range_join_all_containers();
    test_multiple_range_join_all_combinations();
    test_transform_join_all_containers();
    std::cout << "✅ All join tests passed successfully!\n";
    return 0;
}
