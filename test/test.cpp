// Author: Suresh Sundriyal
// License: CC0 - No rights reserved.

#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

#include "rangejoin.hpp"

using namespace rangejoin;

void
test_single_range_join_all_containers()
{
    std::vector<std::string> vec = {"red", "green", "blue"};
    assert(join(" | ", vec) == "red | green | blue");

    std::deque<std::string> deq = {"sun", "moon"};
    assert(join(" + ", deq) == "sun + moon");

    std::list<std::string> lst = {"alpha", "beta", "gamma"};
    assert(join(" -> ", lst) == "alpha -> beta -> gamma");

    std::set<std::string> st = {"dog", "cat", "bird"};
    assert(join(", ", st) == "bird, cat, dog");  // set is ordered
}

void
test_empty_containers()
{
    // Empty containers
    std::vector<std::string> empty_vec;
    std::deque<std::string> empty_deq;
    std::list<std::string> empty_lst;
    std::set<std::string> empty_set;

    assert(join(":", empty_vec) == "");
    assert(join(":", empty_deq) == "");
    assert(join(":", empty_lst) == "");
    assert(join(":", empty_set) == "");

    std::deque<std::string> deq = {"C"};
    std::list<std::string> lst = {"D", "E"};
    std::vector<std::string> vec = {"A", "B"};
    std::set<std::string> st = {"F", "G"};

    // Mixed empty and non-empty
    assert(join(":", empty_vec, deq, st) == "C:F:G");
    assert(join(":", vec, empty_lst, empty_set) == "A:B");

    // All empty
    assert(join(",", empty_vec, empty_deq, empty_lst, empty_set) == "");

    auto to_string = [](int x) { return std::to_string(x); };

    // Empty int containers
    std::vector<int> empty_int_vec;
    std::deque<int> empty_int_deq;
    std::list<int> empty_int_lst;
    std::set<int> empty_int_set;

    assert(join(":", to_string, empty_int_vec, empty_int_deq) == "");
    assert(join(":", to_string, empty_int_lst, empty_int_set) == "");
}

void
test_multiple_range_join_all_combinations()
{
    std::vector<std::string> vec = {"A", "B"};
    std::deque<std::string> deq = {"C"};
    std::list<std::string> lst = {"D", "E"};
    std::set<std::string> st = {"F", "G"};

    assert(join("-", vec, deq, lst, st) == "A-B-C-D-E-F-G");
}

void
test_transform_join_all_containers()
{
    std::vector<int> vec = {1, 2};
    std::deque<int> deq = {3};
    std::list<int> lst = {4, 5};
    std::set<int> st = {6, 7};

    auto to_string = [](int x) { return std::to_string(x); };
    assert(join(",", to_string, vec, deq, lst, st) == "1,2,3,4,5,6,7");

    std::list<char> chars = {'x', 'y'};
    auto char_to_str = [](char c) { return std::string(1, c); };
    assert(join("", char_to_str, chars) == "xy");
}

void
test_single_element()
{
    std::vector<std::string> a = {"hello"};
    assert(join(", ", a) == "hello");
}

void
test_const_char_ptrs()
{
    std::vector<const char*> a = {"foo", "bar"};
    assert(join("-", a) == "foo-bar");
}

void
test_empty_transform()
{
    std::vector<std::string> a = {"a", "b"};
    auto transform = [](const std::string&) { return ""; };
    assert(join(",", transform, a) == ",");
}

void
test_transform_side_effects()
{
    std::vector<std::string> a = {"x", "y"};
    int counter = 0;
    auto transform
        = [&](const std::string& s) { return std::to_string(counter++) + s; };
    assert(join("-", transform, a) == "0x-1y");
}

void
test_mixed_range_transform()
{
    std::vector<std::string> a = {"a"};
    std::list<std::string> b = {"b", "c"};
    auto transform = [](const std::string& s) { return s + "!"; };
    assert(rangejoin::join(" ", transform, a, b) == "a! b! c!");
}

void
test_char_range()
{
    std::vector<char> a = {'a', 'b', 'c'};
    assert(rangejoin::join(",", a) == "a,b,c");
}

void
test_join_with_char_ptr_range()
{
    std::vector<const char*> words = {"hello", "world"};
    std::string result = join(" ", words);
    assert(result == "hello world");
}

int
main()
{
    test_char_range();
    test_const_char_ptrs();
    test_empty_containers();
    test_empty_transform();
    test_join_with_char_ptr_range();
    test_mixed_range_transform();
    test_multiple_range_join_all_combinations();
    test_single_element();
    test_single_range_join_all_containers();
    test_transform_join_all_containers();
    test_transform_side_effects();
    std::cout << "✅ All join tests passed successfully!\n";
    return 0;
}
