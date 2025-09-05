#pragma once

#include <array>
#include <memory>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
 


namespace cxxsmallvec
{

    /**
     * @brief 小容量优先存栈的向量容器
     *
     * 类似 `std::vector`，前 N 个元素存在栈上，超过则转移到堆上
     *
     * @tparam T 元素类型
     * @tparam N 栈内存储容量 (Small-size optimization)
     * @tparam Alloc 分配器类型，如果不传用默认分配器
     */
    template <typename T, std::size_t N, typename Alloc = std::allocator<T>>
    class SmalVector
    {
    public:
        // 类型定义
        using value_type = T;                ///< 元素类型
        using size_type = std::size_t;       ///< 大小类型
        using reference = T &;               ///< 元素引用
        using const_reference = const T &;   ///< 元素常量引用
        using pointer = T *;                 ///< 元素指针
        using const_pointer = const T *;     ///< 元素常量指针
        using iterator = T *;                ///< 迭代器
        using const_iterator = const T *;    ///< 常量迭代器
        using allocator_type = Alloc;        ///< 分配器类型
        using alloc_traits = std::allocator_traits<allocator_type>; ///< 分配器 traits

        // --- 构造 / 析构 ---
        /**
         * @brief 默认构造函数，初始化为空容器
         */
        SmalVector();

        /**
         * @brief 析构函数，释放资源
         */
        ~SmalVector();

        /**
         * @brief 拷贝构造函数
         * @param other 被拷贝的容器
         */
        SmalVector(const SmalVector &other);

        /**
         * @brief 移动构造函数
         * @param other 被移动的容器
         */
        SmalVector(SmalVector &&other) noexcept;

        /**
         * @brief 拷贝赋值
         * @param other 被赋值的容器
         * @return 当前对象引用
         */
        SmalVector &operator=(const SmalVector &other);

        /**
         * @brief 移动赋值
         * @param other 被赋值的容器
         * @return 当前对象引用
         */
        SmalVector &operator=(SmalVector &&other) noexcept;

        /**
         * @brief 获取当前元素数量
         * @return 元素数量
         */
        size_type size() const noexcept;

        /**
         * @brief 获取当前容量（不小于 size()）
         * @return 容量
         */
        size_type capacity() const noexcept;

        /**
         * @brief 判断容器是否为空
         * @return 为空返回 true，否则返回 false
         */
        bool empty() const noexcept;

        /**
         * @brief 预留存储空间
         * @param new_cap 新的容量
         */
        void reserve(size_type new_cap);

        /**
         * @brief 调整容器大小
         * @param new_size 新的元素数量
         *        若增大则默认构造新元素，若减小则销毁多余元素
         */
        void resize(size_type new_size);

        /**
         * @brief 若当前数据在堆上且元素数量不超过 N，则搬回栈上
         */
        void shrink_to_fit_stack();

        /**
         * @brief 下标访问元素（无越界检查）
         * @param pos 元素下标
         * @return 元素引用
         */
        reference operator[](size_type pos);

        /**
         * @brief 下标访问元素（无越界检查，常量版本）
         * @param pos 元素下标
         * @return 元素常量引用
         */
        const_reference operator[](size_type pos) const;

        /**
         * @brief 下标访问元素（带越界检查）
         * @param pos 元素下标
         * @return 元素引用
         * @throw std::out_of_range 越界时抛出
         */
        reference at(size_type pos);

        /**
         * @brief 下标访问元素（带越界检查，常量版本）
         * @param pos 元素下标
         * @return 元素常量引用
         * @throw std::out_of_range 越界时抛出
         */
        const_reference at(size_type pos) const;

        /**
         * @brief 获取第一个元素的引用
         * @return 第一个元素引用
         */
        reference front();

        /**
         * @brief 获取第一个元素的常量引用
         * @return 第一个元素常量引用
         */
        const_reference front() const;

        /**
         * @brief 获取最后一个元素的引用
         * @return 最后一个元素引用
         */
        reference back();

        /**
         * @brief 获取最后一个元素的常量引用
         * @return 最后一个元素常量引用
         */
        const_reference back() const;

        /**
         * @brief 获取底层数据指针
         * @return 数据指针
         */
        T *data() noexcept;

        /**
         * @brief 获取底层数据常量指针
         * @return 数据常量指针
         */
        const T *data() const noexcept;

        // --- 修改 ---

        /**
         * @brief 在末尾插入元素的拷贝
         * @param value 被插入的元素
         */
        void push_back(const T &value);

        /**
         * @brief 在末尾插入元素（移动语义）
         * @param value 被插入的元素
         */
        void push_back(T &&value);

        /**
         * @brief 在末尾原地构造元素
         * @tparam Args 构造参数类型
         * @param args 构造参数
         * @return 新元素的引用
         */
        template <class... Args>
        reference emplace_back(Args &&...args);

        /**
         * @brief 移除末尾元素
         */
        void pop_back();

        /**
         * @brief 清空所有元素
         */
        void clear();

        /**
         * @brief 在指定位置插入元素的拷贝
         * @param pos 插入位置
         * @param value 被插入的元素
         * @return 指向新插入元素的迭代器
         */
        iterator insert(const_iterator pos, const T &value);

        /**
         * @brief 移除指定位置的元素
         * @param pos 被移除元素的位置
         * @return 指向下一个元素的迭代器
         */
        iterator erase(const_iterator pos);

        // --- 迭代器 ---

        /**
         * @brief 获取起始迭代器
         * @return 起始迭代器
         */
        iterator begin() noexcept;

        /**
         * @brief 获取起始常量迭代器
         * @return 起始常量迭代器
         */
        const_iterator begin() const noexcept;

        /**
         * @brief 获取起始常量迭代器（const 版本）
         * @return 起始常量迭代器
         */
        const_iterator cbegin() const noexcept;

        /**
         * @brief 获取末尾迭代器
         * @return 末尾迭代器
         */
        iterator end() noexcept;

        /**
         * @brief 获取末尾常量迭代器
         * @return 末尾常量迭代器
         */
        const_iterator end() const noexcept;

        /**
         * @brief 获取末尾常量迭代器（const 版本）
         * @return 末尾常量迭代器
         */
        const_iterator cend() const noexcept;

    private:
        size_type _size = 0; ///< 当前元素数量
        size_type _capacity = N; ///< 当前容量
        T *_data = nullptr;  ///< 堆上数据指针（栈上时为 nullptr）
        allocator_type _alloc{}; ///< 分配器实例

        alignas(T) unsigned char _stack_storage[sizeof(T) * N]; ///< 栈上存储空间

        /**
         * @brief 获取栈上数据指针
         * @return 栈上数据指针
         */
        T *_stack_ptr() noexcept { return reinterpret_cast<T *>(_stack_storage); }

        /**
         * @brief 获取栈上数据常量指针
         * @return 栈上数据常量指针
         */
        const T *_stack_ptr() const noexcept { return reinterpret_cast<const T *>(_stack_storage); }

        /**
         * @brief 判断当前是否使用栈上存储
         * @return 使用栈上存储返回 true，否则返回 false
         */
        bool using_stack() const noexcept { return _data == nullptr; }
    };

}
