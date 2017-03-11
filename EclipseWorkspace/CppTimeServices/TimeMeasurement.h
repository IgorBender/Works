#ifndef TIMEMEASUREMENT_H_
#define TIMEMEASUREMENT_H_

#include <chrono>

/*!
 * \section intro_sec Introduction
 *
 *
 * <b>TimeMeasurement</b> is designed in aim to be a cross-platform utility.
 * Cross-platform ability is reachet through using of standard C++11 chrono package.
 *
 */

/// Time measuremet class.
/// Requres C++11.
class TimeMeasurement
{
public:
    /// Constructor.
    /// Default constructor.
    TimeMeasurement() : m_Started(false)
    {}
    /// Destructor.
    virtual ~TimeMeasurement()
    {}

    /// Start measurement - set begin measurement time-point.
    void startMeasure()
    {
        m_Started = true;
        m_BeginTimePoint = std::chrono::high_resolution_clock::now();
    }
    /// Get elapset time in milliseconds.
    /// \return milliseconds from begin time-point (-1 if begin time-point is not set).
    double getElapsedTimeMsec()
    {
        if(!m_Started)
            return -1.0;
        std::chrono::duration<double, std::milli> Measured = std::chrono::high_resolution_clock::now() - m_BeginTimePoint;
        return Measured.count();
    }
    /// Get elapset time in microseconds.
    /// \return microseconds from begin time-point (-1 if begin time-point is not set).
    double getElapsedTimeUsec()
    {
        if(!m_Started)
            return -1.0;
        std::chrono::duration<double, std::micro> Measured = std::chrono::high_resolution_clock::now() - m_BeginTimePoint;
        return Measured.count();
    }

protected:
    bool m_Started; /// Indicates if begin time-point is set.
    std::chrono::time_point<std::chrono::high_resolution_clock> m_BeginTimePoint; /// Begin time-point.
};

#endif /* TIMEMEASUREMENT_H_ */

