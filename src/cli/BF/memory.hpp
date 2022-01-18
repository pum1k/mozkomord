#ifndef CLI_BF_MEMORY_HPP_
#define CLI_BF_MEMORY_HPP_

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace BF
{
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
};

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
};

template <class T>
class MemoryStaticUnsafe : public MemoryStaticBase<T>
{
 protected:
 public:
    MemoryStaticUnsafe(std::size_t size) : MemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
};

template <class T>
class MemoryStaticSafe : public MemoryStaticBase<T>
{
 protected:
 public:
    MemoryStaticSafe(std::size_t size) : MemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
};

template <class T>
class MemoryStaticLoop : public MemoryStaticBase<T>
{
 protected:
 public:
    MemoryStaticLoop(std::size_t size) : MemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
};

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
};

} // namespace BF

#include "memory.tpp"

#endif // CLI_BF_MEMORY_HPP_
