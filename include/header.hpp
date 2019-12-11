// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include <string>
#include <atomic>
#include <map>

template <typename T>
class SharedPtr {
public:
    SharedPtr();
    explicit SharedPtr(T* ptr);
    explicit SharedPtr(const SharedPtr& r);
    explicit SharedPtr(SharedPtr&& r);
    ~SharedPtr();
    auto operator=(const SharedPtr& r) -> SharedPtr&;
    auto operator=(SharedPtr&& r) -> SharedPtr&;

    operator bool() const;
    auto operator*() const -> T&;
    auto operator->() const -> T*;

    auto get() -> T*;
    void reset();
    void reset(T* ptr);
    void Pswap(SharedPtr& r);
    auto use_count() const -> size_t;
    static std::map<int64_t, std::atomic_uint >* ptr_table;
private:
    T* _ptr;
};

#endif // INCLUDE_HEADER_HPP_
