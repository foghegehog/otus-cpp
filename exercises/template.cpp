#include "pretty.h"

#include <iostream>

using namespace std;

template <typename T>
void func1(T param) {
    // some very interesting code
    std::cout << __PRETTY_FUNCTION__ << endl;
}

template <typename T>
void func2(T& param) {
    // some very interesting code
    std::cout << __PRETTY_FUNCTION__ << endl;
}

template <typename T>
void func3(T* param) {
    // some very interesting code
    std::cout << __PRETTY_FUNCTION__ << endl;
}

template <typename T>
void func4(T&& param) {
    // some very interesting code
    std::cout << __PRETTY_FUNCTION__ << endl;
}

int main(int argc, char * argv[]) 
{
    int i = 10;
    const int ci = 15;
    const int &cir = ci;
    int * pi = &i;
    const int * cpi = &i;

    func1(i); // 1.1. param type            - int, copy of i value
    func1(ci); // 1.2. param type           - int, copy of ci value
    func1(cir); // 1.3. param type          - int, copy of cir value
    func1(42); // 1.4. param type           - int, r-value
    func1(pi); // 1.5. param type           - int *
    func1(cpi); // 1.6. param type          - const int *
    // func1({1}); // 1.7. param type       - Doesn't compile
    // func1({1, 2, 3}); // 1.8. param type - Doesn't compile

    func2(i); // 2.1. param type            - int&
    func2(ci); // 2.2. param type ?         - const int&
    func2(cir); // 2.3. param type ?        - const int&
    // func2(42); // 2.4. param type ?      - Doesn't compile, cannot pass r-value by l-value ref
    func2(pi); // 2.5. param type ?         - int *&
    func2(cpi); // 2.6. param type ?        - const int *&

    // func3(i); // 3.1. param type ?       - Doesn't compile, int i is not a pointer
    // func3(ci); // 3.2. param type ?      - Doesn't compile, const int is not a pointer
    // func3(cir); // 3.3. param type ?     - Doesn't compile, const int& is not a pointer
    // func3(42); // 3.4. param type ?      - Doesn't compile, r-value cannot be converted to a pointer
    func3(pi); // 3.5. param type ?         - int * 
    func3(cpi); // 3.6. param type ?        - const int * 
    
    func4(i); // 4.1. param type ?          - int&, reference collapsing
    func4(ci); // 4.2. param type ?         - const int&, reference collapsing
    func4(cir); // 4.3. param type ?        - const int &, reference collapsing
    func4(42); // 4.4. param type ?         - int&&, r-value
    func4(pi); // 4.5. param type ?         - int * &, reference collapsing
    func4(cpi); // 4.6. param type ?        - const int * &, reference collapsing
    return 0;
}