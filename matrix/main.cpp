#include "matrix.h"
#include "tuple_repeat.h"

#include <cassert>
#include <iostream>

using namespace std;

int main (int, char **)
{
    Matrix<int, 2, 0> matrix;
    for (size_t i = 0; i <= 9; i++)
    {
        matrix[i][i] = i;
    }
    
    for (size_t i = 0; i <= 9; i++)
    {
        matrix[i][9-i] = 9 - i;
    }

    size_t row_start = 1;
    size_t row_end = 8;
    size_t col_start = 1;
    size_t col_end = 8;

    for (size_t r = row_start; r <= row_end; r++)
    {
        cout << matrix[r][col_start];
        for (size_t c = col_start + 1; c <= col_end; c++)
        {
            cout<< ' ' << matrix[r][c];
        }
        cout << endl;
    }

    cout << matrix.size() << endl;

    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x <<' ' << y << ' ' << v << std::endl;
    }

    //((matrix[100][100] = 314) = 0) = 217;
    //cout << matrix.size() << endl;
    //cout << matrix[100][100] << endl;

    return 0;
}