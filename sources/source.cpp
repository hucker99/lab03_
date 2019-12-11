// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>

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
