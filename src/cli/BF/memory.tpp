#include "memory.hpp"

namespace BF
{
template <class T>
MemoryStaticBase<T>::MemoryStaticBase(std::size_t size)
{
    this->size_ = size;
    this->arr   = new T[size]();
    this->ptr   = this->arr;
}

template <class T>
MemoryStaticBase<T>::~MemoryStaticBase()
{
    delete[] this->arr;
}

template <class T>
void MemoryStaticBase<T>::reset()
{
    this->ptr = this->arr;

    for (std::size_t i = 0; i < this->size_; i++)
    {
        this->arr[i] = 0;
    }
}

template <class T>
void MemoryStaticBase<T>::inc_val()
{
    ++(*this->ptr);
}

template <class T>
void MemoryStaticBase<T>::dec_val()
{
    --(*this->ptr);
}

template <class T>
T &MemoryStaticBase<T>::ref()
{
    return *this->ptr;
}

template <class T>
void MemoryStaticUnsafe<T>::inc_ptr()
{
    ++(this->ptr);
}

template <class T>
void MemoryStaticUnsafe<T>::dec_ptr()
{
    --(this->ptr);
}

template <class T>
void MemoryStaticSafe<T>::inc_ptr()
{
    if ((this->ptr - this->arr) == (this->size_ - 1))
    {
        // TODO: Add error message
        throw std::out_of_range("");
    }
    ++(this->ptr);
}

template <class T>
void MemoryStaticSafe<T>::dec_ptr()
{
    if (this->ptr == this->arr)
    {
        // TODO: Add error message
        throw std::out_of_range("");
    }
    --(this->ptr);
}

template <class T>
void MemoryStaticLoop<T>::inc_ptr()
{
    if ((this->ptr - this->arr) == (this->size_ - 1))
    {
        this->ptr = this->arr;
        return;
    }
    ++(this->ptr);
}

template <class T>
void MemoryStaticLoop<T>::dec_ptr()
{
    if (this->ptr == this->arr)
    {
        this->ptr = this->arr + this->size_ - 1;
        return;
    }
    --(this->ptr);
}

template <class T>
MemoryDynamic<T>::MemoryDynamic()
{
    this->vec.resize(1);
    this->index = 0;
}

template <class T>
void MemoryDynamic<T>::reset()
{
    this->index = 0;

    for (std::size_t i = 0; i < this->vec.size(); i++)
    {
        vec[i] = 0;
    }
}

template <class T>
void MemoryDynamic<T>::inc_val()
{
    ++(this->vec[this->index]);
}

template <class T>
void MemoryDynamic<T>::dec_val()
{
    --(this->vec[this->index]);
}

template <class T>
void MemoryDynamic<T>::inc_ptr()
{
    ++(this->index);
    if (this->index == this->vec.size())
    {
        this->vec.resize(this->vec.size() + 1);
    }
}

template <class T>
void MemoryDynamic<T>::dec_ptr()
{
    if (this->index == 0)
    {
        // TODO: Add error message
        throw std::out_of_range("");
    }

    --(this->index);
}

template <class T>
T &MemoryDynamic<T>::ref()
{
    return this->vec[this->index];
}

} // namespace BF
