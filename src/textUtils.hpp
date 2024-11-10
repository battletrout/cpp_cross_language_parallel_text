#pragma once

#include <iostream>
#include <string>



// template<typename... Args>
// void print_coll(Args&&... args) {
//     std::cout << args << print_iterator(args)
// }

template <typename T>
void print_coll(T& coll)
{
    for (const auto& elem : coll) {
        std::cout << elem << " ";
    }
}