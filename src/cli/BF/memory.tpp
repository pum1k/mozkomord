#include "memory.hpp"

template <class T>
BFMemoryStaticBase<T>::BFMemoryStaticBase(std::size_t size)
{
    this->size_ = size;
    this->arr   = new T[size]();
    this->ptr   = this->arr;
}

template <class T>
BFMemoryStaticBase<T>::~BFMemoryStaticBase()
{
    delete[] this->arr;
}

template <class T>
void BFMemoryStaticBase<T>::inc_val()
{
    ++(*this->ptr);
}

template <class T>
void BFMemoryStaticBase<T>::dec_val()
{
    --(*this->ptr);
}

template <class T>
T &BFMemoryStaticBase<T>::ref()
{
    return *this->ptr;
}

template <class T>
void BFMemoryStaticUnsafe<T>::inc_ptr()
{
    ++(this->ptr);
}

template <class T>
void BFMemoryStaticUnsafe<T>::dec_ptr()
{
    --(this->ptr);
}

template <class T>
void BFMemoryStaticSafe<T>::inc_ptr()
{
    if ((this->ptr - this->arr) == (this->size_ - 1))
    {
        // TODO: Add error message
        throw std::out_of_range("");
    }
    ++(this->ptr);
}

template <class T>
void BFMemoryStaticSafe<T>::dec_ptr()
{
    if (this->ptr == this->arr)
    {
        // TODO: Add error message
        throw std::out_of_range("");
    }
    --(this->ptr);
}

template <class T>
void BFMemoryStaticLoop<T>::inc_ptr()
{
    if ((this->ptr - this->arr) == (this->size_ - 1))
    {
        this->ptr = this->arr;
        return;
    }
    ++(this->ptr);
}

template <class T>
void BFMemoryStaticLoop<T>::dec_ptr()
{
    if (this->ptr == this->arr)
    {
        this->ptr = this->arr + this->size_ - 1;
        return;
    }
    --(this->ptr);
}

template <class T>
BFMemoryDynamic<T>::BFMemoryDynamic()
{
    this->vec.resize(1);
    this->index = 0;
}

template <class T>
void BFMemoryDynamic<T>::inc_val()
{
    ++(this->vec[this->index]);
}

template <class T>
void BFMemoryDynamic<T>::dec_val()
{
    --(this->vec[this->index]);
}

template <class T>
void BFMemoryDynamic<T>::inc_ptr()
{
    ++(this->index);
    if (this->index == this->vec.size())
    {
        this->vec.resize(this->vec.size() + 1);
    }
}

template <class T>
void BFMemoryDynamic<T>::dec_ptr()
{
    if (this->index == 0)
    {
        // TODO: Add error message
        throw std::out_of_range("");
    }

    --(this->index);
}

template <class T>
T &BFMemoryDynamic<T>::ref()
{
    return this->vec[this->index];
}
