#include "matrix.h"
#include "tuple_repeat.h"

#include <cassert>
#include <iostream>

using namespace std;

int main (int, char **)
{
    Matrix<int, 2, -1> matrix;
    assert(matrix.size() == 0); 
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    cout << matrix[100][100] << endl;
    int x, y, z;
    double c;
    array<int, 3> arr = {1, 2, 3};
    auto t = create_tuple(arr, 7.0);
    tie(x,y,z,c) = t; //cell<int, int, int, double>().get_cell(make_tuple(1,2,3, 4.0));
    cout << x<< ' ' << y << ' '<< z << ' ' << c <<endl;

    return 0;
}