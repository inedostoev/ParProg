#ifndef BLOCK_HPP
#define BLOCK_HPP

template<typename T, size_t N>
struct Block
{
    T data_NN[N][N];
    Block(): data_NN{}
    {}

    Block operator+(const Block &right) const
    {
        Block result = *this;
        for (size_t i = 0 ; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                result.data_NN[i][j] += right.data_NN[i][j];
        return result;
    }

    Block& operator+=(const Block &right)
    {
        *this = (*this) + right;
        return *this;
    }

    Block operator*(const Block &right) const
    {
        Block result;
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                for (size_t k = 0; k < N; ++k)
                    result.data_NN[i][j] += this->data_NN[i][k] * right.data_NN[k][j];
        return result;
    }

    Block& operator*=(const Block &right)
    {
        *this = (*this) * right;
        return *this;
    }
};


#endif
