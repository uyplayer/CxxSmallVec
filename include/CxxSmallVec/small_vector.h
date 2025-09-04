#pragma once

#include <array>
#include <memory>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <pybind11/pybind11.h>


namespace cxxsmallvec
{

    /**
     * @brief 小容量优先存栈的向量容器
     *
     * 类似 `std::vector`，前 N 个元素存在栈上，超过则转移到堆上
     *
     * @tparam T 元素类型
     * @tparam N 栈内容量
     * @tparam Alloc 分配器类型，如果不传用默认分配器
     */
    template <typename T, std::size_t N, typename Alloc = std::allocator<T>>
    class SmalVector
    {
    public:
        // 定义类型
        using value_type = T;
        using size_type = std::size_t;
        using reference = T &;
        using const_reference = const T &;
        using pointer = T *;
        using const_pointer = const T *;
        using iterator = T *;
        using const_iterator = const T *;
        using allocator_type = Alloc;
        using alloc_traits = std::allocator_traits<allocator_type>;

        // --- 构造 / 析构  
        small_vector();
        ~small_vector();
        small_vector(const small_vector &other);
        small_vector(small_vector &&other) noexcept;
        small_vector &operator=(const small_vector &other);
        small_vector &operator=(small_vector &&other) noexcept;

        // --- 容量 ---
        size_type size() const noexcept;
        size_type capacity() const noexcept;
        bool empty() const noexcept;
        void reserve(size_type new_cap);
        void resize(size_type new_size);
        void shrink_to_fit_stack();

        // --- 访问 ---
        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;
        reference at(size_type pos);
        const_reference at(size_type pos) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        T *data() noexcept;
        const T *data() const noexcept;

        // --- 修改 ---
        void push_back(const T &value);
        void push_back(T &&value);
        template <class... Args>
        reference emplace_back(Args &&...args);
        void pop_back();
        void clear();
        iterator insert(const_iterator pos, const T &value);
        iterator erase(const_iterator pos);

        // --- 迭代器 ---
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

    private:
    
        size_type _size = 0;
        size_type _capacity = N;
        T *_data = nullptr;  
        allocator_type _alloc{};

        alignas(T) unsigned char _stack_storage[sizeof(T) * N];
 
        T *_stack_ptr() noexcept { return reinterpret_cast<T *>(_stack_storage); }
        const T *_stack_ptr() const noexcept { return reinterpret_cast<const T *>(_stack_storage); }

        bool using_stack() const noexcept { return _data == nullptr; }
    };

}
