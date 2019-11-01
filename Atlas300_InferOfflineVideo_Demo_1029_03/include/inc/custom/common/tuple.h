/* Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef DOMI_COMMON_TUPLE_H_
#define DOMI_COMMON_TUPLE_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace domi
{

template < typename ValueType > class Tuple
{
public:
    /*! \brief default constructor */
    Tuple() = default;
    /*! \brief destructor */
    inline ~Tuple()
    {
        delete[] data_heap_;
        data_heap_ = nullptr;
    }
    /*!
     * \brief copy constructor from another tuple
     * \param s the source tuple
     */
    inline Tuple(const Tuple< ValueType >& s)
    {
        this->assign(s.begin(), s.end());
    }
    /*!
     * \brief constructor from initializer list
     * \param init the initializer_list
     */
    inline Tuple(std::initializer_list< ValueType > init)
    {
        this->assign(init.begin(), init.end());
    }
    /*!
     * \brief constructor from vector
     * \param init the vector
     */
    inline Tuple(std::vector< ValueType > init)
    {  // NOLINT(runtime/explicit)
        this->assign(init.begin(), init.end());
    }
    /*!
     * \brief move constructor from Tuple
     * \param src the source shape
     */

    inline Tuple(Tuple< ValueType >&& src)
    {  // NOLINT(runtime/explicit)
        this->swap(src);
    }
    /*!
     * \brief construct the Tuple from content of iterator
     * \param begin the beginning of iterator
     * \param end end the end of the iterator
     * \tparam RandomAccessIterator iterator type
     */
    template < typename RandomAccessIterator >
    inline Tuple(RandomAccessIterator begin, RandomAccessIterator end)
    {
        this->assign(begin, end);
    }
    /*!
     * \brief Assign content to tuple from iterator.
     * \param begin the beginning of iterator
     * \param end end the end of the iterator
     * \tparam RandomAccessIterator iterator type
     */
    template < typename RandomAccessIterator >
    inline void assign(RandomAccessIterator begin, RandomAccessIterator end)
    {
        this->SetDim(end - begin);
        std::copy(begin, end, this->begin());
    }
    /*!
     * \brief Swap current object with other
     * \param other another object to be swapped.
     */
    inline void swap(Tuple< ValueType >& other)
    {  // NOLINT(*)
        std::swap(ndim_, other.ndim_);
        std::swap(num_heap_allocated_, other.num_heap_allocated_);
        std::swap(data_stack_, other.data_stack_);
        std::swap(data_heap_, other.data_heap_);
    }
    /*!
     * \brief assignment from another tuple.
     * \param src source tuple
     * \return reference of self
     */
    inline Tuple< ValueType >& operator=(const Tuple< ValueType >& src)
    {
        this->assign(src.begin(), src.end());
        return *this;
    }
    /*!
     * \brief assignment from rvalue of another tuple.
     * \param src source tuple
     * \return reference of self
     */
    inline Tuple< ValueType >& operator=(Tuple< ValueType >&& src)
    {
        Tuple< ValueType >(std::move(src)).swap(*this);
        return *this;
    }
    /*!
     * \brief assignment from initializer list
     * \param init the source initializer list
     * \return reference of self
     */
    inline Tuple< ValueType >& operator=(std::initializer_list< ValueType > init)
    {
        this->assign(init.begin(), init.end());
        return *this;
    }
    /*!
     * \return whether two tuple equals
     * \param s the tuple to compare against
     */
    inline bool operator==(const Tuple< ValueType >& s) const
    {
        if(ndim_ != s.ndim_)
            return false;
        return std::equal(begin(), end(), s.begin());
    }
    /*!
     * \return whether two tuple not equal
     * \param s the tuple to compare against
     */
    inline bool operator!=(const Tuple< ValueType >& s) const
    {
        return !(*this == s);
    }
    /*! \return the begin data pointer to content of the tuple */
    inline const ValueType* begin() const
    {
        return ndim_ <= STACK_CACHE_NUM ? data_stack_ : data_heap_;
    }
    /*! \return the begin data pointer to content of the tuple */
    inline ValueType* begin()
    {
        return ndim_ <= STACK_CACHE_NUM ? data_stack_ : data_heap_;
    }
    /*! \return the data pointer to end of the tuple */
    inline const ValueType* end() const
    {
        return ndim_ <= STACK_CACHE_NUM ? (data_stack_ + ndim_) : (data_heap_ + ndim_);
    }
    /*! \return the data pointer to end the tuple */
    inline ValueType* end()
    {
        return ndim_ <= STACK_CACHE_NUM ? (data_stack_ + ndim_) : (data_heap_ + ndim_);
    }
    /*! \return number of dimension of the tuple */
    inline uint32_t ndim() const
    {
        return ndim_;
    }
    /*!
     * \brief get corresponding index
     * \param i dimension index
     * \return the corresponding dimension size
     */
    inline ValueType& operator[](size_t i)
    {
        return begin()[ i ];
    }
    /*!
     * \brief get corresponding index
     * \param i dimension index
     * \return the corresponding dimension size
     */
    inline const ValueType& operator[](size_t i) const
    {
        return begin()[ i ];
    }
    /*!
     * \brief allow output string of tuple to ostream
     * \param os the output stream
     * \param t the tuple
     * \return the ostream
     */
    friend std::ostream& operator<<(std::ostream& os, const Tuple< ValueType >& t)
    {
        os << '[';
        const ValueType* begin = t.begin();
        const ValueType* end   = t.end();
        for(const ValueType* it = begin; it != end; ++it)
        {
            if(it != begin)
                os << ',';
            os << *it;
        }
        os << ']';
        return os;
    }
    /*!
     * \brief read tuple from the istream
     * \param is the input stream
     * \param t The tuple
     * \return the istream
     */
    friend std::istream& operator>>(std::istream& is, Tuple< ValueType >& t)
    {
        // get (
        while(true)
        {
            char ch = is.peek();
            if(isdigit(ch) || ch == '-')
            {
                ValueType idx;
                if(is >> idx)
                {
                    t.assign(&idx, &idx + 1);
                }
                return is;
            }
            is.get();
            if(ch == '(' || ch == '[')
                break;
            if(!isspace(ch))
            {
                is.setstate(std::ios::failbit);
                return is;
            }
        }
        // Handle empty tuple
        while(isspace(is.peek()))
        {
            is.get();
        }
        if(is.peek() == ')' || is.peek() == ']')
        {
            is.get();
            return is;
        }
        // Handle non-empty tuple
        ValueType                idx;
        std::vector< ValueType > tmp;
        while(is >> idx)
        {
            tmp.push_back(idx);
            char ch;
            do
            {
                ch = is.get();
            } while(isspace(ch));
            if(std::is_integral< ValueType >::value && ch == 'L')
            {
                ch = is.get();
            }
            if(ch == ',')
            {
                while(true)
                {
                    ch = is.peek();
                    if(isspace(ch))
                    {
                        is.get();
                        continue;
                    }
                    if(ch == ')' || ch == ']')
                    {
                        is.get();
                        break;
                    }
                    break;
                }
                if(ch == ')' || ch == ']')
                    break;
            }
            else if(ch == ')' || ch == ']')
            {
                break;
            }
            else
            {
                is.setstate(std::ios::failbit);
                return is;
            }
        }
        t.assign(tmp.begin(), tmp.end());
        return is;
    }
public:
    // stack cache size
    static const uint32_t STACK_CACHE_NUM = 4;
    /*! \brief in stack space used to store shape when it is small */
    ValueType data_stack_[ STACK_CACHE_NUM ];
    /*! \brief space to store shape when dimension is big*/
    ValueType* data_heap_{ nullptr };

protected:
    /*! \brief number of dimension of the tuple */
    uint32_t ndim_{ 0 };
    /*! \brief number of cells allocated in data_heap_ */
    uint32_t num_heap_allocated_{ 0 };

    // internal function to change the dimension
    inline void SetDim(uint32_t ndim)
    {
        if(ndim > STACK_CACHE_NUM && ndim > num_heap_allocated_)
        {
            delete[] data_heap_;
            data_heap_ = nullptr;
            data_heap_          = new (std::nothrow) ValueType[ ndim ]();
            num_heap_allocated_ = ndim;
        }
        ndim_ = ndim;
    }
};

using UintTuple = Tuple<uint32_t>;
using IntTuple = Tuple<int64_t>;
using FloatTuple = Tuple<float>;
using BoolTuple = Tuple<bool>;
using StringTuple = Tuple<std::string>;

}  // namespace domi

#endif  // DOMI_COMMON_TUPLE_H_
