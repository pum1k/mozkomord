#ifndef CLI_BF_MEMORY_HPP_
#define CLI_BF_MEMORY_HPP_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../common/debugprint.hpp"
#include "except.hpp"
#include "strings.hpp"

namespace BF
{

// #############################################################################
// #############################################################################
// #### MEMORY #################################################################
// #############################################################################
// #############################################################################

/**
 * This class defines interface for all memory classes.
 */
template <class T>
class MemoryBase
{
 protected:
    MemoryBase(){};

 public:
    using value_type = T;

    virtual ~MemoryBase(){};
    MemoryBase(const MemoryBase<T> &) = delete;
    MemoryBase(MemoryBase<T> &&)      = delete;
    MemoryBase &operator=(const MemoryBase<T> &) = delete;
    MemoryBase &operator=(MemoryBase<T> &&) = delete;

    virtual void reset() = 0;

    virtual void inc_val() = 0;
    virtual void dec_val() = 0;
    virtual void inc_ptr() = 0;
    virtual void dec_ptr() = 0;
    virtual T &ref()       = 0;

    virtual void move_ptr(std::ptrdiff_t distance) = 0;
};

template <class T>
class MemDbgrStatic;

/**
 * All memory types derived from this class have fixed size.
 */
template <class T>
class MemoryStaticBase : public MemoryBase<T>
{
 protected:
    T *arr;
    T *ptr;
    std::size_t size_;

    MemoryStaticBase(std::size_t size);

 public:
    virtual ~MemoryStaticBase() override;

    virtual void reset() override;

    virtual void inc_val() override;
    virtual void dec_val() override;
    virtual T &ref() override;

    friend class MemDbgrStatic<T>;
};

/**
 * No checks for accessing memory out of the storage array.
 * Fixed size.
 */
template <class T>
class MemoryStaticUnsafe : public MemoryStaticBase<T>
{
 protected:
 public:
    MemoryStaticUnsafe(std::size_t size) : MemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;

    virtual void move_ptr(std::ptrdiff_t distance) override;
};

/**
 * Throws std::out_of_range when trying to decrement pointer to the first
 * element or increment pointer to the last element.
 * Fixed size.
 */
template <class T>
class MemoryStaticSafe : public MemoryStaticBase<T>
{
 protected:
 public:
    MemoryStaticSafe(std::size_t size) : MemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;

    virtual void move_ptr(std::ptrdiff_t distance) override;
};

/**
 * Decrementing pointer to the first element goes to the last one and vice
 * versa.
 * Fixed size.
 */
template <class T>
class MemoryStaticLoop : public MemoryStaticBase<T>
{
 protected:
 public:
    MemoryStaticLoop(std::size_t size) : MemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;

    virtual void move_ptr(std::ptrdiff_t distance) override;
};

template <class T>
class MemDbgrDynamic;

/**
 * Dynamic size.
 * Incrementing pointer to the last element increases the size of this memory.
 * Throws std::out_of_range when trying to decrement pointer to the first
 * element.
 */
template <class T>
class MemoryDynamic : public MemoryBase<T>
{
 protected:
    std::vector<T> vec;
    std::size_t index;

 public:
    MemoryDynamic();

    virtual void reset() override;

    virtual void inc_val() override;
    virtual void dec_val() override;
    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
    virtual T &ref() override;

    virtual void move_ptr(std::ptrdiff_t distance) override;

    friend class MemDbgrDynamic<T>;
};

// #############################################################################
// #############################################################################
// #### MEMORY DEBUGGER ########################################################
// #############################################################################
// #############################################################################

/**
 * Classes derrived from this class provide additional access to data stored
 * in memory classes. These are used for debugging BF programs.
 */
template <class T>
class MemDbgrBase
{
 public:
    virtual ~MemDbgrBase(){};

    virtual std::size_t get_current_size()  = 0;
    virtual std::size_t get_current_index() = 0;
    virtual T &get_ref(std::size_t index)   = 0;
};

template <class T>
class MemDbgrStatic : public MemDbgrBase<T>
{
 private:
    MemoryStaticBase<T> *mem;

 public:
    MemDbgrStatic(MemoryStaticBase<T> *mem);

    virtual std::size_t get_current_size() override;
    virtual std::size_t get_current_index() override;
    virtual T &get_ref(std::size_t index) override;
};

template <class T>
class MemDbgrDynamic : public MemDbgrBase<T>
{
 private:
    MemoryDynamic<T> *mem;

 public:
    MemDbgrDynamic(MemoryDynamic<T> *mem);

    virtual std::size_t get_current_size() override;
    virtual std::size_t get_current_index() override;
    virtual T &get_ref(std::size_t index) override;
};

// #############################################################################
// #############################################################################
// #### MEMORY FACTORY #########################################################
// #############################################################################
// #############################################################################

/**
 * Enumeration representing supported memory data types
 */
enum class MemDataType
{
    bit8,
    bit16,
};

/**
 * Types of memory classes.
 */
enum class MemoryType
{
    none,
    static_unsafe,
    static_safe,
    static_loop,
    dynamic,
};

/**
 * Factory class for creating memory objects with or without attached debugger.
 */
class MemFactory
{
 private:
    MemoryType mem_type;
    std::size_t mem_size;

 public:
    MemFactory(MemoryType mem_type = MemoryType::none, std::size_t mem_size = 0)
        : mem_type(mem_type), mem_size(mem_size)
    {
    }
    void set_mem_type(MemoryType mem_type) { this->mem_type = mem_type; }
    void set_mem_size(std::size_t mem_size) { this->mem_size = mem_size; }

    template <class T>
    MemoryBase<T> *new_mem();
    template <class T>
    std::pair<MemoryBase<T> *, MemDbgrBase<T> *> new_debug_mem();
};

} // namespace BF

#include "memory.tpp"

#endif // CLI_BF_MEMORY_HPP_
