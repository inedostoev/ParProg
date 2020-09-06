#ifndef FASTMULMATRIX_HPP
#define FASTMULMATRIX_HPP

#include <stddef.h>
#include "Block.hpp"

size_t get_qt_blocks(size_t size, size_t block_size) {
	size_t qt_blocks = size / block_size;
    
	if(size % block_size != 0) qt_blocks += 1;    /* if had odd elements */

	return qt_blocks;
}

template<typename T, size_t N = 8>
class Matrix {

public:
	Matrix(size_t i, size_t j);
	~Matrix();

	Matrix operator*(const Matrix& right) const;
	Matrix& operator*=(const Matrix& right);
	T& operator()(size_t i, size_t j);
	
private:
	Block<T, N>* data;
	size_t rows;
	size_t cols;
};

template<typename T, size_t N>
Matrix<T, N>::Matrix(size_t i, size_t j) : 
	data(new Block<T, N>[get_qt_blocks(i, N) * get_qt_blocks(j, N)]),
	rows(i),
	cols(j)
{
	if(data == NULL) printf("Error: cant allocate memory\n");
}

template<typename T, size_t N>
Matrix<T, N>::~Matrix() {
	delete[] data;
}

template<typename T, size_t N>
T& Matrix<T, N>::operator()(size_t i, size_t j)
{
	size_t i_block = i / N;
	size_t j_block = j / N;
	size_t cur_block = i_block*get_qt_blocks(cols, N) + j_block;
	return data[cur_block].data_NN[i % N][j % N];
}

template<typename T, size_t N>
Matrix<T, N> Matrix<T, N>::operator*(const Matrix<T, N> &right) const
{
	Matrix<T, N> res(this->rows, right.cols);
	for (size_t i = 0; i < get_qt_blocks(res.rows, N); ++i)
		for (size_t j = 0; j < get_qt_blocks(res.cols, N); ++j) {
            
			Block<T, N> sum;
			for (size_t k = 0; k < get_qt_blocks(this->cols, N); ++k) {
				size_t cur_block_1 = i * get_qt_blocks(this->cols, N) + k; 
				size_t cur_block_2 = k * get_qt_blocks(right.cols, N) + j; 
				sum += this->data[cur_block_1] * right.data[cur_block_2];
			}
			res.data[i * get_qt_blocks(res.cols, N) + j] = sum;
		}
    return res;
}

#endif
