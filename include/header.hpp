// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
//
// Created by od341 on 25.09.2019.
//
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

template<typename T>
SharedPtr<T>::SharedPtr():_ptr(nullptr){}

template<typename T>
SharedPtr<T>::SharedPtr(T *ptr):_ptr(ptr) {
    SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))++;
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr &r):_ptr(r._ptr) {
    SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))++;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr &&r):_ptr(r->ptr) {
    SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))++;
}

template<typename T>
std::map<int64_t, std::atomic_uint> *SharedPtr<T>::
        ptr_table = new std::map<int64_t, std::atomic_uint>();

template<typename T>
SharedPtr<T>::~SharedPtr() {
    SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))--;
    if (SharedPtr::ptr_table->operator[]
            (reinterpret_cast<int64_t>(_ptr)) == 0) {
        SharedPtr::ptr_table->erase(reinterpret_cast<int64_t>(this->_ptr));
        delete _ptr;
    }
}

template<typename T>
SharedPtr<T>::operator bool() const {
    if (this->_ptr != NULL) return true;
    return false;
}

template<typename T>
void SharedPtr<T>::reset() {
    if (this->ptr_table->operator[](reinterpret_cast<int64_t>(_ptr)) == 1) {
        SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))--;
        SharedPtr::ptr_table->erase(reinterpret_cast<int64_t>(this->_ptr));
        delete _ptr;
        _ptr = nullptr;

    } else {
        SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))--;
        SharedPtr::ptr_table->erase(reinterpret_cast<int64_t>(this->_ptr));
        _ptr = nullptr;
    }
}

template<typename T>
void SharedPtr<T>::reset(T *ptr) {
    if (this->ptr_table->operator[](reinterpret_cast<int64_t>(_ptr)) == 1) {
        SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))--;
        SharedPtr::ptr_table->erase(reinterpret_cast<int64_t>(this->_ptr));
        delete _ptr;
        _ptr = ptr;
        SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(ptr))++;
    } else {
        SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr))--;
        SharedPtr::ptr_table->erase(reinterpret_cast<int64_t>(this->_ptr));
        _ptr = ptr;
        SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(ptr))++;
    }
}

template<typename T>
void SharedPtr<T>::Pswap(SharedPtr &r) {
    SharedPtr tmp;
    tmp = *this;
    *this = r;
    r = tmp;
}

template<typename T>
auto SharedPtr<T>::operator*() const -> T & {
    return *(this->_ptr);
}


template<typename T>
auto SharedPtr<T>::operator=(const SharedPtr &r) -> SharedPtr & {
    if (this->_ptr != r._ptr) {
        this->reset(r._ptr);
    }
    return *this;
}

template<typename T>
auto SharedPtr<T>::get() -> T * {
    return this->_ptr;
}

template<typename T>
auto SharedPtr<T>::operator->() const -> T*{
    return this->_ptr;
}

template<typename T>
auto SharedPtr<T>::use_count() const -> size_t {
    return SharedPtr::ptr_table->operator[](reinterpret_cast<int64_t>(_ptr));
}

#endif // INCLUDE_HEADER_HPP_
