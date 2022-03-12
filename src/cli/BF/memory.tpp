#include "memory.hpp"

namespace BF
{

// #############################################################################
// #############################################################################
// #### STATIC MEMORY ##########################################################
// #############################################################################
// #############################################################################

// #############################################################################
// #### STATIC BASE ############################################################
// #############################################################################

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

// #############################################################################
// #### STATIC UNSAFE ##########################################################
// #############################################################################

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
void MemoryStaticUnsafe<T>::move_ptr(std::ptrdiff_t distance)
{
    this->ptr += distance;
}

// #############################################################################
// #### STATIC SAFE ############################################################
// #############################################################################

template <class T>
void MemoryStaticSafe<T>::inc_ptr()
{
    if (static_cast<std::size_t>(this->ptr - this->arr) == (this->size_ - 1))
    {
        throw std::out_of_range(strings::increment_error);
    }
    ++(this->ptr);
}

template <class T>
void MemoryStaticSafe<T>::dec_ptr()
{
    if (this->ptr == this->arr)
    {
        throw std::out_of_range(strings::decrement_error);
    }
    --(this->ptr);
}

template <class T>
void MemoryStaticSafe<T>::move_ptr(std::ptrdiff_t distance)
{
    if (distance < (this->arr - this->ptr))
    {
        throw std::out_of_range(strings::decrement_error);
    }
    else if (distance >= ((this->arr + this->size_) - this->ptr))
    {
        throw std::out_of_range(strings::increment_error);
    }
    this->ptr += distance;
}

// #############################################################################
// #### STATIC LOOP ############################################################
// #############################################################################

template <class T>
void MemoryStaticLoop<T>::inc_ptr()
{
    if (static_cast<std::size_t>(this->ptr - this->arr) == (this->size_ - 1))
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
void MemoryStaticLoop<T>::move_ptr(std::ptrdiff_t distance)
{
    while (distance < (this->arr - this->ptr))
    {
        distance += this->size_;
    }
    while (distance >= ((this->arr + this->size_) - this->ptr))
    {
        this->ptr -= this->size_;
    }
    this->ptr += distance;
}

// #############################################################################
// #############################################################################
// #### DYNAMIC MEMORY #########################################################
// #############################################################################
// #############################################################################

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
        throw std::out_of_range(strings::decrement_error);
    }

    --(this->index);
}

template <class T>
T &MemoryDynamic<T>::ref()
{
    return this->vec[this->index];
}

template <class T>
void MemoryDynamic<T>::move_ptr(std::ptrdiff_t distance)
{
    if (distance < (-static_cast<std::ptrdiff_t>(this->index)))
    {
        throw std::out_of_range(strings::decrement_error);
    }
    else if (distance >= (static_cast<std::ptrdiff_t>(this->vec.size()) -
                          static_cast<std::ptrdiff_t>(this->index)))
    {
        this->vec.resize(this->index + 1 + distance);
    }
    this->index += distance;
}

// #############################################################################
// #############################################################################
// #### MEMORY DEBUGGER ########################################################
// #############################################################################
// #############################################################################

// #############################################################################
// #### MEMORY DEBUGGER for static memory ######################################
// #############################################################################

template <class T>
MemDbgrStatic<T>::MemDbgrStatic(MemoryStaticBase<T> *mem) : mem(mem)
{
    if (this->mem == nullptr)
    {
        throw std::invalid_argument(strings::mem_ptr_null_error);
    }
}

template <class T>
std::size_t MemDbgrStatic<T>::get_current_size()
{
    return this->mem->size_;
}

template <class T>
std::size_t MemDbgrStatic<T>::get_current_index()
{
    return (this->mem->ptr - this->mem->arr);
}

template <class T>
T &MemDbgrStatic<T>::get_ref(std::size_t index)
{
    if (index >= 0 && index < this->get_current_size())
    {
        return this->mem->arr[index];
    }
    else
    {
        throw std::out_of_range(std::string(strings::mem_index_error) +
                                std::to_string(index));
    }
}

// #############################################################################
// #### MEMORY DEBUGGER for dynamic memory #####################################
// #############################################################################

template <class T>
MemDbgrDynamic<T>::MemDbgrDynamic(MemoryDynamic<T> *mem) : mem(mem)
{
    if (this->mem == nullptr)
    {
        throw std::invalid_argument(strings::mem_ptr_null_error);
    }
}

template <class T>
std::size_t MemDbgrDynamic<T>::get_current_size()
{
    return this->mem->vec.size();
}

template <class T>
std::size_t MemDbgrDynamic<T>::get_current_index()
{
    return this->mem->index;
}

template <class T>
T &MemDbgrDynamic<T>::get_ref(std::size_t index)
{
    if (index >= 0 && index < this->get_current_size())
    {
        return this->mem->vec[index];
    }
    else
    {
        throw std::out_of_range(std::string(strings::mem_index_error) +
                                std::to_string(index));
    }
}

// #############################################################################
// #############################################################################
// #### FACTORY CLASS ##########################################################
// #############################################################################
// #############################################################################

template <class T>
MemoryBase<T> *MemFactory::new_mem()
{
    MemoryBase<T> *mem = nullptr;

    switch (this->mem_type)
    {
    case MemoryType::static_unsafe:
        DEBUG_PRINT("[MemFactory]: Creating static unsafe memory of size: "
                    << this->mem_size);
        mem = new MemoryStaticUnsafe<T>(this->mem_size);
        break;
    case MemoryType::static_safe:
        DEBUG_PRINT("[MemFactory]: Creating static safe memory of size: "
                    << this->mem_size);
        mem = new MemoryStaticSafe<T>(this->mem_size);
        break;
    case MemoryType::static_loop:
        DEBUG_PRINT("[MemFactory]: Creating static loop memory of size: "
                    << this->mem_size);
        mem = new MemoryStaticLoop<T>(this->mem_size);
        break;
    case MemoryType::dynamic:
        DEBUG_PRINT("[MemFactory]: Creating dynamic memory");
        mem = new MemoryDynamic<T>();
        break;
    case MemoryType::none:
    default:
        throw mem_type_error();
    }

    return mem;
}

template <class T>
std::pair<MemoryBase<T> *, MemDbgrBase<T> *> MemFactory::new_debug_mem()
{
    std::pair<MemoryBase<T> *, MemDbgrBase<T> *> rv;
    rv.first = this->new_mem<T>();

    switch (this->mem_type)
    {
    case MemoryType::static_unsafe:
    case MemoryType::static_safe:
    case MemoryType::static_loop:
        DEBUG_PRINT("[MemFactory]: Creating static memory debugger.");
        rv.second =
            new MemDbgrStatic<T>(static_cast<MemoryStaticBase<T> *>(rv.first));
        break;
    case MemoryType::dynamic:
        DEBUG_PRINT("[MemFactory]: Creating dynamic memory debugger");
        rv.second =
            new MemDbgrDynamic<T>(static_cast<MemoryDynamic<T> *>(rv.first));
        break;
    case MemoryType::none:
    default:
        throw mem_type_error();
    }

    return rv;
}

} // namespace BF
