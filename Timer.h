///////////////////////////////////////////////////////////////////////////////
/// @file Timer.h
/// @brief Contains the declaration of the Timer class.
/// @internal
///
/// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////

#ifndef NIK_TIMER_HEADER
#define NIK_TIMER_HEADER

#include <ctime>

namespace nik 
{

///////////////////////////////////////////////////////////////////////////////
/// @class Timer Timer.h <Util\Timer.h>
/// @brief Timing object
/// @details This class has similiar behavior to that of a stopwatch.  The 
///     timer can be started, stopped, and reset.  Also, the current running
///     time can be aquired at any point.
///////////////////////////////////////////////////////////////////////////////
class Timer
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Constructs a timer object that can be started on construction.
    ///     Pass true as the start parameter to start the timer on 
    ///     construction.
    /// @param[in] start @li true - Start the timer on construction
    ///                  @li false - Do not start the timer (default)
    ///////////////////////////////////////////////////////////////////////////
    Timer(bool start=false);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Copy constructor
    /// @note Deep copy the implementation object
    /// @param[in] org The original Timer object to copy from
    ///////////////////////////////////////////////////////////////////////////
    Timer(const Timer& org);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Copy constructor
    /// @note Deep copy the implementation object
    /// @param[in] rhs The right-hand-side object of the assignment operator
    ///////////////////////////////////////////////////////////////////////////
    Timer& operator=(const Timer& rhs);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Get the time elapsed (ms)
    /// @details This function gets the time that has elapsed from the time
    ///     this object was started.  
    /// @attention
    ///     @li If the timer has not been started, this function returns 0.
    ///     @li If the timer is currently running, this function returns the
    ///         current running time.
    ///     @li If the timer has been stopped, this function returns the time
    ///         between the start and stop calls.
    /// @return The elapsed time in milliseconds.
    ///////////////////////////////////////////////////////////////////////////
    unsigned GetTimeMs();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Starts the timer
    /// @details This function should be called to start the timer when it
    ///     is currently stopped.  This will not reset the time, time will begin
    ///     to be added onto the current running time.
    /// @warning 
    ///     @li Calling this function does not reset the timer, call Reset().
    ///////////////////////////////////////////////////////////////////////////
    void Start();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Stop the timer
    /// @details Calling stop will halt timer and the current running time will
    ///     be retained.
    /// @warning This function will not reset the timer, call Reset().  If 
    ///     Reset() is not called, the next call to Start() will cause the
    ///     additional time to be added to the current running time.
    ///////////////////////////////////////////////////////////////////////////
    void Stop();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Reset the timer
    /// @details Calling this function wipes out the current running time of 
    ///     the timer.  If the timer is currently running, the timer will be
    ///     stopped and no further time will be recorded until Start() is 
    ///     called.
    ///////////////////////////////////////////////////////////////////////////
    void Reset();

private:

    ///////////////////////////////////////////////////////////////////////////
    // @brief Flags if the timer is currently running
    ///////////////////////////////////////////////////////////////////////////
    bool m_IsRunning;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The starting number of clock ticks
    /// @details Records the clock ticks when the timer was started.  When the
    ///     timer is stopped and started again, this value is updated to the
    ///     most recent start.
    ///////////////////////////////////////////////////////////////////////////
    clock_t m_TicksStart;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Stored clock ticks
    /// @details This variable stores the number of clock ticks between the
    ///     start and stop actions.  In this way, the number of clock ticks
    ///     is saved so that if the timer is started up again, the current 
    ///     running time is saved.
    ///////////////////////////////////////////////////////////////////////////
    clock_t m_StoredTicks;
};

} // end namespace nik

#endif
////////////////////////End-of-File////////////////////////////////////////////
