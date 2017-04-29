/* 
 * File:   CyclicBuffer.h
 * Author: igor
 *
 * Created on April 15, 2011, 5:08 PM
 */

#ifndef _CIRCULARBUFFER_H
#define	_CIRCULARBUFFER_H

#include <array>
#include <exception>
#include <iostream>

#if (_WIN32 && _MSC_VER == 1500)
using namespace std::tr1;
#endif

/*
 * CyclicBuffer template implements cyclic buffer of instances with possibility
 * of navigation with cyclic iterators.
 *
 */

template <typename Tn, int Dim> class CircularBuffer : public std::array<Tn, Dim>
{
public:

    explicit CircularBuffer() : m_Begin(std::array<Tn, Dim>::begin()), m_End(std::array<Tn, Dim>::end())
    {}

    /*
     * The buffer may be filled till its size with push_back method and
     * after that each call of push_back causes wrap-around the buffer.
     */
    void push_back(const Tn& t)
    {
        if(std::array<Tn, Dim>::size() == size()) // Full buffer
        {
            if (++m_Begin == std::array<Tn, Dim>::end())
            {
                m_Begin = std::array<Tn, Dim>::begin();
            }
        }
        if(!empty())
        {
            if(std::array<Tn, Dim>::end() == ++m_End)
            {
                m_End = std::array<Tn, Dim>::begin();
            }
        }
        else
        {
            m_End = std::array<Tn, Dim>::begin();
        }
        *m_End = t;
    }

    void pop_front()
    {
        if(empty())
        {
            return;
        }
        if(m_Begin == m_End) // Empty buffer
        {
            m_Begin = std::array<Tn, Dim>::begin();
            m_End = std::array<Tn, Dim>::end();
            return;
        }
        if(++m_Begin == std::array<Tn, Dim>::end())
        {
            m_Begin = std::array<Tn, Dim>::begin();
        }
    }

    bool empty() const
    {
        return std::array<Tn, Dim>::begin() == m_Begin && std::array<Tn, Dim>::end() == m_End;
    }

    const typename std::array<Tn, Dim>::size_type size() const
    {
        if(empty())
        {
            return 0;
        }
        if(0 > std::distance(m_Begin, m_End))
        {
            return std::array<Tn, Dim>::size() + std::distance(m_Begin, m_End) + 1;
        }
        else
        {
            return std::distance(m_Begin, m_End) + 1;
        }
    }

    Tn& operator[](typename std::array<Tn, Dim>::size_type i)
    {
        if(i >= std::array<Tn, Dim>::size())
        {
            std::out_of_range e("out_of_range");
        	throw(e);
        }
        typename std::array<Tn, Dim>::size_type Index = (m_Begin - std::array<Tn, Dim>::begin() + i) % size();
        return std::array<Tn, Dim>::at(Index);
    }

    const Tn& operator[](typename std::array<Tn, Dim>::size_type i) const
    {
        if(i >= size())
        {
            std::out_of_range e("out_of_range");
        	throw(e);
        }
        typename std::array<Tn, Dim>::size_type Index = (m_Begin - std::array<Tn, Dim>::begin() + i) % size();
        return std::array<Tn, Dim>::at(Index);
    }

    Tn& at(typename std::array<Tn, Dim>::size_type i)
    {
        if(i >= size())
        {
            std::out_of_range e("out_of_range");
            throw(e);
        }
        typename std::array<Tn, Dim>::size_type Index = (m_Begin - std::array<Tn, Dim>::begin() + i) % size();
        return std::array<Tn, Dim>::at(Index);
    }

    const Tn& at(typename std::array<Tn, Dim>::size_type i) const
    {
        if(i >= size())
        {
            std::out_of_range e("out_of_range");
            throw(e);
        }
        typename std::array<Tn, Dim>::size_type Index = (m_Begin - std::array<Tn, Dim>::begin() + i) % size();
        return std::array<Tn, Dim>::at(Index);
    }

    /*
     * Clears the buffer to its initial state.
     */
    void clear()
    {
        m_Begin = std::array<Tn, Dim>::begin();
        m_End = std::array<Tn, Dim>::end();
    }

    class circular_iterator
    {
    public:
		// Iterator traits
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef Tn value_type;
		typedef int difference_type;
		typedef Tn* pointer;
		typedef Tn& reference;
		
        circular_iterator() // default
        {}

        circular_iterator(typename std::array<Tn, Dim>::iterator i,
            typename std::array<Tn, Dim>::iterator Begin,
            typename std::array<Tn, Dim>::iterator End,
            typename std::array<Tn, Dim>::iterator First,
            typename std::array<Tn, Dim>::iterator Last,
            typename std::array<Tn, Dim>::size_type Size = 0) :
            m_Curr(i), m_Begin(Begin), m_End(End), m_First(First), m_Last(Last), m_Size(Size), m_Count(Size)
        {}

        circular_iterator & operator=(const circular_iterator& i)
        {
            m_Begin = i.m_Begin;
            m_End = i.m_End;
            m_Curr = i.m_Curr;
            m_First = i.m_First;
            m_Last = i.m_Last;
            m_Size = i.m_Size;
            m_Count = i.m_Count;
            return *this;
        }

        Tn & operator*()
        {
            return *m_Curr;
        }

        typename std::array<Tn, Dim>::iterator operator++() // Prefix
        {
            if(0 >= --m_Count)
            {
                m_Curr = m_Last;
                m_Count = 0;
                return m_Curr;
            }
            ++(m_Curr);
            if (m_Curr == m_Last)
            {
                m_Curr = m_First;
                return m_Curr;
            }
            return m_Curr;
        }

        typename std::array<Tn, Dim>::iterator operator++(int) // Postfix
        {
        	typename std::array<Tn, Dim>::iterator Tmp = m_Curr;
            if(0 >= --m_Count)
            {
                m_Curr = m_Last;
//                 m_Count = 0;
                return Tmp;
            }
            if (++m_Curr == m_Last)
            {
                m_Curr = m_First;
            }
            return Tmp;
        }

        typename std::array<Tn, Dim>::iterator operator--() // Prefix
        {
            if (m_Size == ++m_Count)
            {
                m_Curr = m_Begin;
                return m_Curr;
            }
            if (m_Last == m_Curr)
            {
                m_Curr = m_End;
                if(m_Curr < m_First)
                {
                    m_Curr = m_Last;
                    --m_Curr;
                }
                return m_Curr;
            }
            if(m_Curr == m_First)
            {
                m_Curr = m_Last;
                --m_Curr;
                return m_Curr;
            }
            --(m_Curr);
            return m_Curr;
        }

        typename std::array<Tn, Dim>::iterator operator--(int) // Postfix
        {
        	typename std::array<Tn, Dim>::iterator Tmp = m_Curr;
            if(m_Last == m_Curr)
            {
                m_Curr = m_End;
 //               m_Count = 1;
                return Tmp;
            }
            if(m_Size == m_Count++)
            {
                m_Curr = m_Begin;
//                 m_Count = m_Size;
                return m_Curr;
            }
            if (m_Curr == m_First)
            {
                m_Curr = m_Last - 1;
                return Tmp;
            }
            --(m_Curr);
            return Tmp;
        }

        bool operator==(const circular_iterator& i)
        {
            return m_Curr == i.m_Curr;
        }

        bool operator !=(const circular_iterator& i)
        {
            return m_Curr != i.m_Curr;
        }

        typename std::array<Tn, Dim>::iterator m_Curr;
        typename std::array<Tn, Dim>::iterator m_Begin;
        typename std::array<Tn, Dim>::iterator m_End;
        typename std::array<Tn, Dim>::iterator m_First;
        typename std::array<Tn, Dim>::iterator m_Last;
        typename std::array<Tn, Dim>::size_type m_Size;
        typename std::array<Tn, Dim>::size_type m_Count;
    };

    class reverse_circular_iterator
    {
    public:
		// Iterator traits
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef Tn value_type;
		typedef int difference_type;
		typedef Tn* pointer;
		typedef Tn& reference;

        reverse_circular_iterator() // default
        {}

        reverse_circular_iterator(typename std::array<Tn, Dim>::reverse_iterator i,
        		typename std::array<Tn, Dim>::reverse_iterator Begin,
				typename std::array<Tn, Dim>::reverse_iterator End,
				typename std::array<Tn, Dim>::reverse_iterator First,
				typename std::array<Tn, Dim>::reverse_iterator Last,
				typename std::array<Tn, Dim>::size_type Size = 0) :
            m_Curr(i), m_Begin(Begin), m_End(End), m_First(First), m_Last(Last), m_Size(Size), m_Count(Size)
        {}

        reverse_circular_iterator & operator=(const reverse_circular_iterator& i)
        {
            m_Begin = i.m_Begin;
            m_End = i.m_End;
            m_Curr = i.m_Curr;
            m_First = i.m_First;
            m_Last = i.m_Last;
            m_Size = i.m_Size;
            m_Count = i.m_Count;
            return *this;
        }

        Tn & operator*()
        {
            return *m_Curr;
        }

        typename std::array<Tn, Dim>::reverse_iterator operator++() // Prefix
        {
            if(0 >= --m_Count)
            {
                m_Curr = m_Last;
                m_Count = 0;
                return m_Curr;
            }
            ++(m_Curr);
            if (m_Curr == m_Last)
            {
                m_Curr = m_First;
            }
            return m_Curr;
        }
        
        typename std::array<Tn, Dim>::reverse_iterator operator++(int) // Postfix
        {
            typename std::array<Tn, Dim>::reverse_iterator Tmp = m_Curr;
            if (0 >= --m_Count)
            {
                m_Curr = m_Last;
                m_Count = 0;
                return Tmp;
            }
            if(++m_Curr == m_Last)
            {
                m_Curr = m_First;
            }
            return Tmp;
        }

        typename std::array<Tn, Dim>::reverse_iterator operator--() // Prefix
        {
            if(m_Last == m_Curr)
            {
                m_Curr = m_Begin;
                m_Count = 0;
                return m_Curr;
            }
            if(m_Size == m_Count++)
            {
                m_Curr = m_End;
                m_Count = m_Size;
                return m_Curr;
            }
            --(m_Curr);
            if(m_Curr == m_First - 1)
            {
                m_Curr = m_Last - 1;
                return m_Curr;
            }
            return m_Curr;
        }

        typename std::array<Tn, Dim>::reverse_iterator operator--(int) // Postfix
        {
        	typename std::array<Tn, Dim>::reverse_iterator Tmp = m_Curr;
            if(m_Last == m_Curr)
            {
                m_Curr = m_Begin - 1;
                m_Count = 0;
                return Tmp;
            }
            if(m_Size == m_Count++)
            {
                m_Curr = m_End;
                m_Count = m_Size;
                return m_Curr;
            }
            if(m_Curr == m_First)
            {
                m_Curr = m_Last - 1;
                return Tmp;
            }
            --(m_Curr);
            return Tmp;
        }

        bool operator==(const reverse_circular_iterator& i)
        {
            return m_Curr == i.m_Curr;
        }

        bool operator !=(const reverse_circular_iterator& i)
        {
            return m_Curr != i.m_Curr;
        }

        typename std::array<Tn, Dim>::reverse_iterator m_Curr;
        typename std::array<Tn, Dim>::reverse_iterator m_Begin;
        typename std::array<Tn, Dim>::reverse_iterator m_End;
        typename std::array<Tn, Dim>::reverse_iterator m_First;
        typename std::array<Tn, Dim>::reverse_iterator m_Last;
        typename std::array<Tn, Dim>::size_type m_Size;
        typename std::array<Tn, Dim>::size_type m_Count;
    };

    circular_iterator begin()
    {
        circular_iterator i(m_Begin, m_Begin, m_End, std::array<Tn, Dim>::begin(), std::array<Tn, Dim>::end(), size());
        return i;
    }

    circular_iterator end()
    {
        circular_iterator i(std::array<Tn, Dim>::end(), m_Begin, m_End, std::array<Tn, Dim>::begin(), std::array<Tn, Dim>::end(), size());
        return i;
    }

    reverse_circular_iterator rbegin()
    {
        reverse_circular_iterator i(typename std::array<Tn, Dim>::reverse_iterator(m_End + 1),
        		typename std::array<Tn, Dim>::reverse_iterator(m_Begin),
				typename std::array<Tn, Dim>::reverse_iterator(m_End),
				std::array<Tn, Dim>::rbegin(), std::array<Tn, Dim>::rend(), size());
        return i;
    }
    reverse_circular_iterator rend()
    {
        reverse_circular_iterator i(std::array<Tn, Dim>::rend(),
        		typename std::array<Tn, Dim>::reverse_iterator(m_Begin),
				typename std::array<Tn, Dim>::reverse_iterator(m_End),
				std::array<Tn, Dim>::rbegin(), std::array<Tn, Dim>::rend(), size());
        return i;
    }

    Tn& front() = delete;
    const Tn& front() const = delete;
    Tn& back() = delete;
    const Tn& back() const = delete;
    void assign(const Tn& value) = delete;
    void fill(const Tn& value) = delete;

private:
    typename std::array<Tn, Dim>::iterator m_Begin; // The iterator points on first element of circular buffer.
    typename std::array<Tn, Dim>::iterator m_End; // The iterator points on last element.

};

#endif	/* _CIRCULARBUFFER_H */

