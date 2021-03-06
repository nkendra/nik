///////////////////////////////////////////////////////////////////////////////
/// @file Util\Event.cpp
/// @brief Contains implementation for the Event class
/// @internal
///
/// 26May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////

#include "Event.h"
#include "Utility.h"
#include <Windows.h>
#include <cassert>


namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class Event::EventImpl Event.cpp <Util\Event.cpp>
/// @brief The implementation of the Event class
/// @details Defines the behavior of the Event class.
/// @note Event currently only supports Windows
///////////////////////////////////////////////////////////////////////////////
class Event::EventImpl
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor
    /// @details Initializes the member variables.  Construction does not set
    ///     up the event resources.
    ///////////////////////////////////////////////////////////////////////////
    EventImpl();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Frees up the event resources
    ///////////////////////////////////////////////////////////////////////////
    ~EventImpl();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Sets up the event resources
    /// @return @arg true - Success
    ///         @arg false - Required resources could not be obtained
    ///////////////////////////////////////////////////////////////////////////
    bool Init();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Signals the event
    /// @return @arg true - Set event succeeded
    ///         @arg false - Setting the event failed
    ///////////////////////////////////////////////////////////////////////////
    bool SetEvent();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Clears the event signal
    /// @return @arg true - Success 
    ///         @arg false - Failure
    ///////////////////////////////////////////////////////////////////////////
    bool ClearEvent();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Waits for the event to signal
    /// @param[in] waitTime The time to wait for the event in milliseconds.  
    /// @attention Event::FOREVER will cause this function to wait without 
    ///     timeout for the event to be signaled.
    /// @return The wait result:
    ///     @arg Event::WAIT_SIGNALED - The event was signaled
    ///     @arg Event::WAIT_TIMEDOUT - Timed out
    ///////////////////////////////////////////////////////////////////////////
    size_t WaitForEvent(size_t waitTime);

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent copy construction
    ///////////////////////////////////////////////////////////////////////////
    EventImpl(const EventImpl&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent copying through assignment
    ///////////////////////////////////////////////////////////////////////////
    EventImpl& operator=(const EventImpl&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The event resource
    /// @note Only works with windows now
    ///////////////////////////////////////////////////////////////////////////
    HANDLE m_Event;
    
}; // end class Event::EventImpl

//----------------------Event-Implementation---------------------------------//
//----------------------Static-Members---------------------------------------//
size_t Event::FOREVER = INFINITE; // Use windows definition for now...
size_t Event::WAIT_SIGNALED = WAIT_OBJECT_0; // Windows definition
size_t Event::WAIT_TIMEDOUT = WAIT_TIMEOUT; // Windows defintion
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Event* Event::Create()
{
    try{
    return new Event;
    }
    catch(nik::Error&)
    {
        // For now, just return 0
        return 0;
    }
} // end Event::Create

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Event::~Event()
{
    delete m_Impl;
} // end Event::~Event

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Event::SetEvent()
{
    return m_Impl->SetEvent();
} // end Event::SetEvent

///////////////////////////////////////////////////////////////////////////////
// 15June2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Event::ClearEvent()
{
    return m_Impl->ClearEvent();
} // end Event::ClearEvent

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
size_t Event::WaitForEvent(size_t waitTime)
{
    return m_Impl->WaitForEvent(waitTime);
} // end Event::WaitForEvent

//----------------------Private-Implementation-------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Event::Event()
{
    m_Impl = new EventImpl;
    // Setup the resources
    if( !m_Impl->Init())
    {
        delete m_Impl;
        throw nik::Error("nik::Event initialization failed");
    }
    
} // end Event::Event

//----------------------Event::EventImpl-Implementation----------------------//
//----------------------Static-Members---------------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
Event::EventImpl::EventImpl()
:
m_Event(0)
{
} // end EventImpl::EventImpl

Event::EventImpl::~EventImpl()
{
    if(m_Event)
    {
        CloseHandle(m_Event);
        m_Event = 0;
    }

} // end EventImpl::~EventImpl

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Event::EventImpl::Init()
{
    m_Event = CreateEvent(
                    0,      // Default security parameters
                    true,   // Require manual reset of the state
                    false,  // Start the initial state to not signaled
                    0       // Create event with no name
                    );
    assert(m_Event);
    return 0 != m_Event;
} // end EventImpl::Init

///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Event::EventImpl::SetEvent()
{
    return ::SetEvent(m_Event) != 0; // Returns non-zero on success
} // end EventImple::SetEvent

///////////////////////////////////////////////////////////////////////////////
// 15June2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
bool Event::EventImpl::ClearEvent()
{
    return ::ResetEvent(m_Event) != 0; // Returns non-zero on success
} // end EventImple::SetEvent


///////////////////////////////////////////////////////////////////////////////
// 26May2010: nik, initial
///////////////////////////////////////////////////////////////////////////////
size_t Event::EventImpl::WaitForEvent(size_t waitTime)
{
    return WaitForSingleObject(m_Event, waitTime);
} // end EventImpl::SetEvent

} // end namespace nik

////////////////////////End-of-File////////////////////////////////////////////
