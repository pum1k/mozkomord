#ifndef CLI_BF_MEMORY_HPP_
#define CLI_BF_MEMORY_HPP_

#include <algorithm>
#include <stdexcept>
#include <vector>

template <class T>
class BFMemoryBase
{
 protected:
    BFMemoryBase(){};

 public:
    virtual ~BFMemoryBase(){};
    BFMemoryBase(const BFMemoryBase<T> &) = delete;
    BFMemoryBase(BFMemoryBase<T> &&)      = delete;
    BFMemoryBase &operator=(const BFMemoryBase<T> &) = delete;
    BFMemoryBase &operator=(BFMemoryBase<T> &&) = delete;

    virtual void inc_val() = 0;
    virtual void dec_val() = 0;
    virtual void inc_ptr() = 0;
    virtual void dec_ptr() = 0;
    virtual T &ref()       = 0;
};

template <class T>
class BFMemoryStaticBase : public BFMemoryBase<T>
{
 protected:
    T *arr;
    T *ptr;
    std::size_t size_;

    BFMemoryStaticBase(std::size_t size);

 public:
    virtual ~BFMemoryStaticBase() override;

    virtual void inc_val() override;
    virtual void dec_val() override;
    virtual T &ref() override;
};

template <class T>
class BFMemoryStaticUnsafe : public BFMemoryStaticBase<T>
{
 protected:
 public:
    BFMemoryStaticUnsafe(std::size_t size) : BFMemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
};

template <class T>
class BFMemoryStaticSafe : public BFMemoryStaticBase<T>
{
 protected:
 public:
    BFMemoryStaticSafe(std::size_t size) : BFMemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
};

template <class T>
class BFMemoryStaticLoop : public BFMemoryStaticBase<T>
{
 protected:
 public:
    BFMemoryStaticLoop(std::size_t size) : BFMemoryStaticBase<T>(size) {}

    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
};

template <class T>
class BFMemoryDynamic : public BFMemoryBase<T>
{
 protected:
    std::vector<T> vec;
    std::size_t index;

 public:
    BFMemoryDynamic();
    virtual void inc_val() override;
    virtual void dec_val() override;
    virtual void inc_ptr() override;
    virtual void dec_ptr() override;
    virtual T &ref() override;
};

#include "memory.tpp"

#endif // CLI_BF_MEMORY_HPP_
