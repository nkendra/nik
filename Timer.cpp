///////////////////////////////////////////////////////////////////////////////
/// @file Timer.h
/// @brief Contains the implementation of the Timer class.
/// @internal
///
/// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////

#include "Timer.h"



namespace nik
{

//----------------------Timer-Implemenation----------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
Timer::Timer
    (
    bool start
    )
:
m_IsRunning(false), // Start() requires this to be false
m_TicksStart(0),
m_StoredTicks(0)
{
    // Start the timer if flagged to start.
    if(start)
    {
        Start();
    }
} // end Timer::Timer

///////////////////////////////////////////////////////////////////////////////
// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////  
unsigned Timer::GetTimeMs()
{
    // Add the number of stored clock ticks to the current running number of
    // clock ticks.
    clock_t runningTicks = m_StoredTicks;

    // Only add the running clock ticks if the timer is actually running
    if(m_IsRunning)
    {
        runningTicks += clock()-m_TicksStart;
    }
    // convert to milliseconds
    return (1000*runningTicks) / CLOCKS_PER_SEC;
} // end Timer::GetTimeMs

///////////////////////////////////////////////////////////////////////////////
// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
void Timer::Start()
{
    // If the timer is not currently running, save the start point
    if( !m_IsRunning)
    {
        m_TicksStart = clock();
        m_IsRunning = true;
    }
} // end Timer::Start

///////////////////////////////////////////////////////////////////////////////
// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
void Timer::Stop()
{
    if(m_IsRunning)
    {
        // Save the number of clock ticks since last start
        m_StoredTicks += clock() - m_TicksStart;
        m_IsRunning = false;
    }
} // end Timer::Stop

///////////////////////////////////////////////////////////////////////////////
// 15July2010, nik: initial
///////////////////////////////////////////////////////////////////////////////  
void Timer::Reset()
{
    // Clear the running flag and dump the stored clock ticks
    m_IsRunning = false;
    m_StoredTicks = 0;
} // end Timer::Reset

} // end namespace nik

////////////////////////End-of-File////////////////////////////////////////////
