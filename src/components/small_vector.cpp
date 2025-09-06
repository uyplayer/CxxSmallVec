#include "CxxSmallVec/small_vector.h"
#include <utility>

namespace cxxsmallvec
{

template <typename T, std::size_t N, typename Alloc>
SmallVector<T, N, Alloc>::~SmallVector()
{
    std::destroy(begin(), end());

    if (!using_stack())
    {
        alloc_traits::deallocate(_alloc, _data, _capacity);
    }
}

template <typename T, std::size_t N, typename Alloc>
SmallVector<T, N, Alloc>::SmallVector(SmallVector &&other) noexcept
{
    if (other.using_stack())
    {
        std::uninitialized_move(other.begin(), other.end(), _stack_ptr());
        _size = other._size;
        std::destroy(other.begin(), other.end());
        other._size = 0;
    }
    else
    {
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        other._data = nullptr;
        other._size = 0;
        other._capacity = N;
    }
    if (alloc_traits::propagate_on_container_move_assignment::value)
    {
        _alloc = std::move(other._alloc);
    }
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::operator=(SmallVector &&other) noexcept -> SmallVector &
{
    if (this == &other)
    {
        return *this;
    }

    clear();
    if (!using_stack())
    {
        alloc_traits::deallocate(_alloc, _data, _capacity);
        _data = nullptr;
        _capacity = N;
    }

    if (other.using_stack())
    {
        std::uninitialized_move(other.begin(), other.end(), _stack_ptr());
        _size = other._size;
        std::destroy(other.begin(), other.end());
        other._size = 0;
    }
    else
    {
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        other._data = nullptr;
        other._size = 0;
        other._capacity = N;
    }

    if (alloc_traits::propagate_on_container_move_assignment::value)
    {
        _alloc = std::move(other._alloc);
    }

    return *this;
}

template <typename T, std::size_t N, typename Alloc>
[[nodiscard]] auto SmallVector<T, N, Alloc>::size() const noexcept -> size_type
{
    return _size;
}

template <typename T, std::size_t N, typename Alloc>
[[nodiscard]] auto SmallVector<T, N, Alloc>::capacity() const noexcept -> size_type
{
    return _capacity;
}

template <typename T, std::size_t N, typename Alloc>
[[nodiscard]] auto SmallVector<T, N, Alloc>::empty() const noexcept -> bool
{
    return _size == 0;
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::reserve(size_type new_cap)
{
    if (new_cap <= _capacity)
    {
        return;
    }

    auto *new_data = alloc_traits::allocate(_alloc, new_cap);

    try
    {
        T *old_data_ptr = data();
        std::uninitialized_move(old_data_ptr, old_data_ptr + _size, new_data);
    }
    catch (...)
    {
        alloc_traits::deallocate(_alloc, new_data, new_cap);
        throw;
    }

    std::destroy(begin(), end());

    if (!using_stack())
    {
        alloc_traits::deallocate(_alloc, _data, _capacity);
    }

    _data = new_data;
    _capacity = new_cap;
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::resize(size_type new_size)
{
    if (new_size > _size)
    {
        if (new_size > _capacity)
        {
            reserve(new_size);
        }
        for (size_type i = _size; i < new_size; ++i)
        {
            alloc_traits::construct(_alloc, data() + i);
        }
    }
    else if (new_size < _size)
    {
        for (size_type i = new_size; i < _size; ++i)
        {
            alloc_traits::destroy(_alloc, data() + i);
        }
    }
    _size = new_size;
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::shrink_to_fit_stack()
{
    if (using_stack() || _size > N)
    {
        return;
    }

    try
    {
        std::uninitialized_move(_data, _data + _size, _stack_ptr());
    }
    catch (...)
    {
        throw;
    }

    std::destroy(_data, _data + _size);
    alloc_traits::deallocate(_alloc, _data, _capacity);

    _data = nullptr;
    _capacity = N;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::operator[](size_type pos) -> reference
{
    return data()[pos];
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::operator[](size_type pos) const -> const_reference
{
    return data()[pos];
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::at(size_type pos) -> reference
{
    if (pos >= _size)
    {
        throw std::out_of_range("SmallVector::at: index out of range");
    }
    return data()[pos];
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::at(size_type pos) const -> const_reference
{
    if (pos >= _size)
    {
        throw std::out_of_range("SmallVector::at: index out of range");
    }
    return data()[pos];
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::front() -> reference
{
    return *begin();
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::front() const -> const_reference
{
    return *begin();
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::back() -> reference
{
    return *(end() - 1);
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::back() const -> const_reference
{
    return *(end() - 1);
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::data() noexcept -> T *
{
    return using_stack() ? _stack_ptr() : _data;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::data() const noexcept -> const T *
{
    return using_stack() ? _stack_ptr() : _data;
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::push_back(const T &value)
{
    if (_size == _capacity)
    {
        reserve(_capacity > 0 ? _capacity * 2 : 1);
    }
    alloc_traits::construct(_alloc, data() + _size, value);
    ++_size;
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::push_back(T &&value)
{
    if (_size == _capacity)
    {
        reserve(_capacity > 0 ? _capacity * 2 : 1);
    }
    alloc_traits::construct(_alloc, data() + _size, std::move(value));
    ++_size;
}

template <typename T, std::size_t N, typename Alloc>
template <class... Args>
auto SmallVector<T, N, Alloc>::emplace_back(Args &&...args) -> reference
{
    if (_size == _capacity)
    {
        reserve(_capacity > 0 ? _capacity * 2 : 1);
    }
    T *ptr = data() + _size;
    alloc_traits::construct(_alloc, ptr, std::forward<Args>(args)...);
    ++_size;
    return *ptr;
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::pop_back()
{
    if (_size > 0)
    {
        --_size;
        alloc_traits::destroy(_alloc, data() + _size);
    }
}

template <typename T, std::size_t N, typename Alloc>
void SmallVector<T, N, Alloc>::clear()
{
    std::destroy(begin(), end());
    _size = 0;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::insert(const_iterator pos, const T &value) -> iterator
{
    const auto offset = pos - cbegin();
    if (_size == _capacity)
    {
        reserve(_capacity > 0 ? _capacity * 2 : 1);
    }
    iterator insert_pos = begin() + offset;

    if (insert_pos < end())
    {
        alloc_traits::construct(_alloc, end(), std::move(*(end() - 1)));
        std::move_backward(insert_pos, end() - 1, end());
        *insert_pos = value;
    }
    else
    {
        alloc_traits::construct(_alloc, end(), value);
    }

    ++_size;
    return insert_pos;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::erase(const_iterator pos) -> iterator
{
    const auto offset = pos - cbegin();
    iterator erase_pos = begin() + offset;

    std::move(erase_pos + 1, end(), erase_pos);

    --_size;
    alloc_traits::destroy(_alloc, end());

    return erase_pos;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::begin() noexcept -> iterator
{
    return data();
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::begin() const noexcept -> const_iterator
{
    return data();
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::cbegin() const noexcept -> const_iterator
{
    return begin();
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::end() noexcept -> iterator
{
    return data() + _size;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::end() const noexcept -> const_iterator
{
    return data() + _size;
}

template <typename T, std::size_t N, typename Alloc>
auto SmallVector<T, N, Alloc>::cend() const noexcept -> const_iterator
{
    return end();
}

} // namespace cxxsmallvec