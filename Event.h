///////////////////////////////////////////////////////////////////////////////
/// @file Util\Event.h
/// @brief Contains declaration for the Event class
/// @internal
///
/// 26May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_EVENT_HEADER
#define NIK_EVENT_HEADER


namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class Event Event.h <Util\Event.h>
/// @brief Class for setting and checking events
/// @details Event object with signal event and wait for event functionality.
///     The event object can only be created with the Create() function, which
///     returns an Event object created with new.
/// @note Event currently only supports Windows
///////////////////////////////////////////////////////////////////////////////
class Event
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Event object
    /// @details Event object can only be created through the Create function.
    ///     This function returns a pointer to an Event object created with
    ///     new.
    /// @attention Remember to call delete when finished with the object.
    /// @attention Returns 0 when the creation of the event fails
    /// @return @arg 0 - Creating the event failed
    ///         @arg A pointer to the new event
    ///////////////////////////////////////////////////////////////////////////
    static Event* Create();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Cleans up the event and any resources it holds
    ///////////////////////////////////////////////////////////////////////////
    ~Event();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Set the event as signaled
    /// @return @arg true - Success
    ///         @arg false - Failure
    ///////////////////////////////////////////////////////////////////////////
    bool SetEvent();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Clear the event signal
    /// @return @arg true - Success
    ///         @arg false - Failure
    ///////////////////////////////////////////////////////////////////////////
    bool ClearEvent();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Waits for the given event to be set
    /// @details This function waits for the event to be signaled.  This
    ///     implementation can handle the case of the event being signaled before
    ///     the wait for event function is called.
    /// @param[in] waitTime The time to wait for the event in milliseconds. 
    ///     Use FOREVER as the time to ignore the timeout and wait until the
    ///     event is signaled.
    /// @return The wait for event result:
    ///     @li WAIT_SIGNALED
    ///     @li WAIT_TIMEDOUT
    ///     @li Anthing else is an error
    ///////////////////////////////////////////////////////////////////////////
    size_t WaitForEvent(size_t waitTime);

    static size_t FOREVER; ///< Flags the WaitForEvent(size_t) to wait forever

    static size_t WAIT_SIGNALED; ///< Wait returned due to the event being signaled
    static size_t WAIT_TIMEDOUT;  ///< Wait returned due to timeout

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent construction outside of this class
    /// @details This function sets up the event on creation.
    /// @attention Use Create() to construct a new Event object
    ///////////////////////////////////////////////////////////////////////////
    Event();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent copy construction
    /// @attention Use Create() to construct a new Event object
    ///////////////////////////////////////////////////////////////////////////
    Event(const Event&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Prevent copy through assignment
    ///////////////////////////////////////////////////////////////////////////
    Event& operator=(const Event&);

    class EventImpl; // Implementation

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Pointer to the imlementation
    /// @details The Event class uses an implementation class to handle the
    ///     behavior and functionality of Event.
    ///////////////////////////////////////////////////////////////////////////
    EventImpl* m_Impl;
}; // end class Event

} // end namespace nik

#endif 

////////////////////////End-of-File////////////////////////////////////////////
