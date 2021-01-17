#include "pretty.h"

#include <iostream>
#include <utility>

using namespace std;

int func_value() {
    return 42;
}

int& func_ref() {
    static int value = 42;
    return value;
}

const int& func_cref() {
    static const int value = 42;
    return value;
}

int* func_ptr() {
    static int value = 42;
    return &value;
}

const int * func_cptr() {
    static int value = 42;
    return &value;
}

const int * const func_ccptr() {
    static const int value = 42;
    return &value;
}

int&& func_rref() {
    int value = 42;
    return std::move(value);
}

int main(int argc, char * argv[]) 
{
    {
        // 1.1 - v1 type - int
        auto v1 = func_value();

        // 1.2 - v2 type - Doesn't compile. Otherwise reference would allow to modify possibly not modifyable value 
        // auto& v2 = func_value();    

        // 1.3 - v3 type - const int &
        const auto& v3 = func_value();    

        // 1.4 - v4 type - int&&, (copied?) r-value
        auto&& v4 = func_value();          
    }

    {
        // 2.1 - v1 type - int, simple auto ommits references
        auto v1 = func_ref();  
        // 2.2 - v2 type - int&         
        auto& v2 = func_ref();    
        // 2.3 - v3 type - const int&     
        const auto& v3 = func_ref();  
        // 2.4 - v4 type - int&, && ->& due to auto&& and l-value reference returned  
        auto&& v4 = func_ref();        
    }

    {
        // 3.1 - v1 type - int, simple auto ommits const and references
        auto v1 = func_cref();  
        // 3.2 - v2 type - const int&, & is explicitly specified, const is retained due to it      
        auto& v2 = func_cref();  
        // 3.3 - v3 type - const int&, as auto explicitly specifies const and &     
        const auto& v3 = func_cref(); 
        // 3.4 - v4 type - const int&, && -> & as l-value returned and auto&& specified, const retained
        auto&& v4 = func_cref();      
    }

    {
        // 4.1 - v1 type - int *
        auto v1 = func_ptr();
        // 4.2 - v2 type - Doesn't compile, return value is l-value pointer, not constant         
        // auto& v2 = func_ptr();     
        // 4.3 - v3 type - int * const &, const explicitly specified
        const auto& v3 = func_ptr(); 
        // 4.4 - v4 type - int * &&, int * considered ad r-value 
        auto&& v4 = func_ptr();       
    }

    {
        // 5.1 - v1 type - const int *
        auto v1 = func_cptr(); 
        // 5.2 - v2 type - Doesn't compile        
        //auto& v2 = func_cptr();  
        // 5.3 - v3 type - const int * const(?) &      
        const auto& v3 = func_cptr(); 
        // 5.4 - v4 type - const int * &&, r-value 
        auto&& v4 = func_cptr();       
    }

    {
        // 6.1 - v1 type - const int * const
        auto v1 = func_ccptr();  
        // 6.2 - v2 type - Doesn't compile       
        //auto& v2 = func_ccptr();   
        // 6.3 - v3 type - const int * const &     
        const auto& v3 = func_ccptr(); 
        // 6.4 - v4 type - const int *&&, r-value 
        auto&& v4 = func_ccptr();       
    }

    {
        // 7.1 - v1 type - int, simple auto ommits references
        auto v1 = func_rref();    
        // 7.2 - v2 type - Doesn't compile    
        //auto& v2 = func_rref();  
        // 7.3 - v3 type - const int &     
        const auto& v3 = func_rref(); 
        // 7.4 - v4 type - int &&, r-value retained
        auto&& v4 = func_rref();      
    }

    {
        // 8.1 - v1 type - int
        auto v1(10);  
        // 8.2 - v2 type - int                
        auto v2 = 10;  
        // 8.3 - v3 type - int               
        auto v3{10};   
        // 8.4 - v4 type - std::initializer_list<int>               
        auto v4 = {10}; 
        // 8.5 - v5 type - std::initializer_list<int>              
        auto v5 = {1, 2, 3};  
        // 8.6 - v6 type - Doesn't compile, different types in list        
        //auto v6 = {1, 2, 3.0};        
    }

    return 0;
}