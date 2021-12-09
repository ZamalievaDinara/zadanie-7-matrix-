#include <iostream>

using namespace std;

int randomElement()
{
    const int from = 1;
    const int to = 9;
    return from + rand() % (to - from + 1);
}

int fillZero() { return 0; }


template <typename T, typename F>
void randomMatrix(T** matrix, size_t rows, size_t cols, F randomFunction)
{
    for (size_t row = 0; row < rows; row++)
        for (size_t col = 0; col < cols; col++)
            //matrix[row][col] = randomFunction();
            matrix[row][col] = randomFunction() * 100 + (row + 1) * 10 + col + 1;
}

template <typename T, typename F>
T** createMatrix(size_t rows, size_t cols, F randomFunction = NULL)
{
    T** matrix = new T*[rows];
    for (size_t row = 0; row < rows; row++)
        matrix[row] = new T[cols];

    if (randomFunction)
        randomMatrix(matrix, rows, cols, randomFunction);

    return matrix;
}

template <typename T>
void deleteMatrix(T** matrix, size_t rows)
{
    for (size_t row = 0; row < rows; row++)
        delete[] matrix[row];
    delete[] matrix;
}

template <typename T>
void deleteRow(T** matrix, size_t& rows, size_t deletingRow)
{
    delete[] matrix[deletingRow];
    for (size_t row = deletingRow + 1; row < rows; row++)
        matrix[row - 1] = matrix[row];
    rows--;
}

template <typename T>
void deleteColumn(T** matrix, size_t rows, size_t& cols, size_t deletingCol)
{
    for (size_t row = 0; row < rows; row++) 
    {
        for (size_t col = deletingCol + 1; col < cols; col++)
            matrix[row][col - 1] = matrix[row][col];
    }
    cols--;
}

template <typename T>
void printMatrix(T** matrix, size_t rows, size_t cols)
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
            cout << matrix[row][col] << " ";
        cout << endl;
    }
}

template <typename T>
T minElement(T** matrix, size_t rows, size_t cols, size_t& minRow, size_t& minCol)
{
    minRow = 0;
    minCol = 0;
    for (size_t row = 0; row < rows; row++)
        for (size_t col = 0; col < cols; col++)
            if (matrix[row][col] < matrix[minRow][minCol])
            {
                minRow = row;
                minCol = col;
            }
    return matrix[minRow][minCol];
}

int** multMatrix(int** matrix1, int** matrix2, size_t rows1, size_t cols1, size_t rows2, size_t cols2)
{
    if (cols1 != rows2)
        return NULL;

    int** matrix = createMatrix<int>(rows1, cols2, fillZero);
   
    for (size_t row = 0; row < rows1; row++)
    {
        for (size_t col = 0; col < cols2; col++)
        {
            int mult = 0;
            for (size_t i = 0; i < cols1; i++)
                mult += matrix1[row][i] * matrix2[i][col];
            matrix[row][col] = mult;
        }
    }
    return matrix;
}

template <typename T>
void simmetricSwap(T** matrix, const size_t rows, const size_t cols)
{
    for (size_t col = 0; col < cols - 1; col++)
        for (size_t row = 0; row < rows - col - 1; row++)
            swap(matrix[row][col], matrix[rows - col - 1][cols - row - 1]);
}

template <typename T>
void swapColRow(T** matrix, const size_t rows, const size_t cols, const size_t swapRow, const size_t swapCol)
{
    for (size_t i = 0; i < cols; i++)
        swap(matrix[swapRow][i], matrix[i][swapCol]);
}


void replaceMin(int** matrix, const size_t rows, const size_t cols)
{
    for (size_t row = 1; row < rows; row++)
    {
        size_t maxCol = 0;
        size_t minCol = 0;
        for (size_t col = 1; col < cols; col++)
        {
            if (matrix[row][col] < matrix[row][minCol])
            {
                minCol = col;
            }
            if (matrix[row - 1][col] > matrix[row - 1][maxCol])
            {
                maxCol = col;
            }
        }
        matrix[row][minCol] = matrix[row - 1][maxCol];
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    srand((unsigned int)time(NULL));

    // 1
    {
        size_t rows = 5, cols = 5;
        int** matrix = createMatrix<int>(rows, cols, randomElement);
        printMatrix(matrix, rows, cols);

        size_t minRow, minCol;
        int min = minElement(matrix, rows, cols, minRow, minCol);
        cout << "Минимальный элемент [строка, столбец] = [" << minRow + 1 << ", " << minCol + 1 << "] = " << min << endl;
        deleteRow(matrix, rows, minRow);
        deleteColumn(matrix, rows, cols, minCol);
        printMatrix(matrix, rows, cols);

        deleteMatrix(matrix, rows);
        cout << endl;
    }

    // 2
    {
        size_t rows = 3, cols = 2;
        int** matrix1 = createMatrix<int>(rows, cols, randomElement);
        int** matrix2 = createMatrix<int>(cols, rows, randomElement);
        cout << "Первая матрица: " << endl;
        printMatrix(matrix1, rows, cols);
        cout << "Вторая матрица: " << endl;
        printMatrix(matrix2, cols, rows);

        int** mult = multMatrix(matrix1, matrix2, rows, cols, cols, rows);
        if (mult)
        {
            cout << "Произведение матриц: " << endl;
            printMatrix(mult, rows, rows);
            deleteMatrix(mult, rows);
        }
        else
            cout << "Ошибка: матрицы не согласованы.";
        deleteMatrix(matrix1, rows);
        deleteMatrix(matrix2, cols);
      
        cout << endl;
    }

    // 3
    {
        size_t rows = 5, cols = 5;
        int** matrix = createMatrix<int>(rows, cols, randomElement);
       
        cout << "Матрица: " << endl;
        printMatrix(matrix, rows, cols);
        cout << endl;
        cout << "Матрица, симметричная по побочной диагонали: " << endl;
        simmetricSwap(matrix, rows, cols);
        printMatrix(matrix, rows, cols);

        deleteMatrix(matrix, rows);
        cout << endl;
    }

    // 4 
    {
        size_t rows = 4, cols = 4;
        int** matrix = createMatrix<int>(rows, cols, randomElement);

        cout << "Матрица: " << endl;
        printMatrix(matrix, rows, cols);
        cout << endl;

        size_t minRow, minCol;
        int min = minElement(matrix, rows, cols, minRow, minCol);
        cout << "Минимальный элемент [строка, столбец] = [" << minRow + 1 << ", " << minCol + 1 << "] = " << min << endl;
        swapColRow(matrix, rows, cols, minRow, minCol);

        cout << "Результат обмена строки и столбца: " << endl;
        printMatrix(matrix, rows, cols);

        deleteMatrix(matrix, rows);
        cout << endl;
    }

    // 5
    {
        size_t rows = 4, cols = 5;
        int** matrix = createMatrix<int>(rows, cols, randomElement);

        cout << "Матрица: " << endl;
        printMatrix(matrix, rows, cols);
        cout << endl;

        replaceMin(matrix, rows, cols);
        
        cout << "Результат: " << endl;
        printMatrix(matrix, rows, cols);

        deleteMatrix(matrix, rows);
        cout << endl;
    }

    return 0;
}

