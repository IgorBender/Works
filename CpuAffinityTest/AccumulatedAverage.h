#ifndef ACUMULATEDAVERAGE_H
#define ACUMULATEDAVERAGE_H

#include <stdint.h>

template<typename T, typename NumType = uint32_t> class AccumulatedAverage
{
public:
    AccumulatedAverage(NumType Threshold = NumType(0)) : m_Num(NumType(0)),
        m_Threshold(Threshold) {}
    void init(const T& Initial, NumType First)
    {
        m_Average = Initial;
        m_Num = First;
    }

    T& accumulate(const T& Value)
    {
        if(m_Threshold && m_Num >= m_Threshold)
            return m_Average;
        NumType Prev = m_Num;
        ++m_Num;
        m_Average = (m_Average * Prev + Value) / m_Num;
        return m_Average;
    }
    T& getAverage()
    {
        return m_Average;
    }
    NumType getNumber()
    {
        return m_Num;
    }

protected:
    T m_Average;
    NumType m_Num;
    NumType m_Threshold;
};

#endif /*ACUMULATEDAVERAGE_H*/

