///////////////////////////////////////////////////////////////////////////////
/// @file Util\ThreadObj.cpp
/// @brief Contains implementation for the SimpleThreadCoord class
/// @internal
///
/// 24May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////

#include "ThreadObj.h"
#include "Utility.h"
#include <Windows.h>
#include <cassert>


namespace nik {

//----------------------SimpleThreadCoord-Implementation---------------------//
//----------------------Static-Members---------------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
SimpleThreadCoord::SimpleThreadCoord
    (
    UserFunc_t userFunc
    )
:
m_StopEvent(0),
m_ThreadStoppedEvent(0),
m_UserFunc(userFunc)
{
   // The events are setup on Run.  This allows copying a ThreadObj
   // to behave nicely before calling Run.
} // end SimpleThreadCoord::SimpleThreadCoord

///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
SimpleThreadCoord::SimpleThreadCoord()
:
m_StopEvent(0),
m_ThreadStoppedEvent(0),
m_UserFunc(0)
{}

///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
SimpleThreadCoord& SimpleThreadCoord::operator=
    (
    const SimpleThreadCoord& rhs
    )
{
    // Do not copy the events
    m_UserFunc = rhs.m_UserFunc;
    return *this;
} // end assignment operator


///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
SimpleThreadCoord::~SimpleThreadCoord()
{
    // Only cleanup if the events have been created from Run()
    if(m_StopEvent || m_ThreadStoppedEvent)
    {
        WaitForStop();
        delete m_StopEvent;
        delete m_ThreadStoppedEvent;
    }
} // end SimpleThreadCoord::~SimpleThreadCoord

///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
void SimpleThreadCoord::Run
    (
    )
{
 
    assert(0 == m_StopEvent);
    assert(0 == m_ThreadStoppedEvent);

    // Neither event should be setup 
    if(m_StopEvent || m_ThreadStoppedEvent)
    {
        throw nik::Error("SimpleThreadCoord::Run->Events have already been created");
    }

    // Setup the events
    m_StopEvent = Event::Create();
    assert(m_StopEvent);
    m_ThreadStoppedEvent = Event::Create();
    assert(m_ThreadStoppedEvent);

    // Begin execution
    nik::log << "Run-> Loop start...\n";
    // Call the users function, if it returns false then quit the 
    // execution of the thread.  If true, then check for the quit
    // event being set.
    bool continueThread = true; // Flags if the thread will continue\quit
    while(m_UserFunc(continueThread) && continueThread) // Note order of evaluation
    {
        // Check for the quit event
        DWORD waitResult = m_StopEvent->WaitForEvent(0); // Don't wait
        if( waitResult == Event::WAIT_SIGNALED)
        {
            nik::log << "Received stop event" << nik::endl;
            continueThread = false; // Flag the quit event, while loop will break
        }
    } // end while()

    m_ThreadStoppedEvent->SetEvent(); // Flag shutdown complete
} // end SimpleThreadCoord::Run

///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
void SimpleThreadCoord::SignalStop()
{
    // Set the stop event
    assert(m_StopEvent);
    m_StopEvent->SetEvent();
} // end SimpleThreadCoord::SignalStop

///////////////////////////////////////////////////////////////////////////////
// 24May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
void SimpleThreadCoord::WaitForStop()
{
    assert(m_StopEvent);
    m_StopEvent->SetEvent(); // Set again just in case

    assert(m_ThreadStoppedEvent);
    m_ThreadStoppedEvent->WaitForEvent(Event::FOREVER);
} // end SimpleThreadCoord::WaitForStop

} // end namespace nik

////////////////////////End-of-File////////////////////////////////////////////
