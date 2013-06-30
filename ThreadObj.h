///////////////////////////////////////////////////////////////////////////////
/// @file Util\ThreadObj.h
/// @brief Contains declaration for the thread classes
/// @details Declarations for the following classes:
///     @li SimpleThreadCoord
///     @li ThreadObj
/// @internal
///
/// 24May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_THREAD_OBJ_HEADER
#define NIK_THREAD_OBJ_HEADER

#include <Util\Utility.h>
#include <Util\Logger.h>
#include <cassert>
#include <Util\Event.h>
#include <Util\Thread.h>

namespace nik {

///////////////////////////////////////////////////////////////////////////////
/// @class SimpleThreadCoord ThreadObj.h <Util\ThreadObj.h>
/// @brief Manages client code running in a thread
/// @details SimpleThreadCoord works in conjunction with ThreadObj to run client
///     code in a seperate thread.  SimpleThreadCoord is responsible for 
///     calling the client code and determining when the thread has been 
///     requested to stop.  
///
///     SimpleThreadCoord runs a loop in the thread.  In every loop, the client
///     code is called.  If the client code signals for the thread to end, 
///     SimpleThreadCoord exits the thread.  Otherwise, SimpleThreadCoord checks
///     if ThreadObj has signaled for this thread to stop.  If the thread has
///     has been signaled to stop, on the next call to the user code 
///     SimpleThreadCoord notifies the client code that the thread has been 
///     signaled to stop and will not be called again. When the client code
///     returns, SimpleThreadCoord stops the thread.
/// @note Thread events are only created on the call to Run()
/// @see ThreadObj
///////////////////////////////////////////////////////////////////////////////
class SimpleThreadCoord
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The prototype of the user function to call in the thread
    ///////////////////////////////////////////////////////////////////////////
    typedef bool (*UserFunc_t)(bool);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Sets up the object with the user function to call in the
    ///     thread.
    ///////////////////////////////////////////////////////////////////////////
    SimpleThreadCoord(UserFunc_t userFunc);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Default constructor
    /// @details Sets up the object.  This constructor is only provided so that
    ///     a SimpleThreadCoord can be created and assigned to from another SimpleThreadCoord.
    ///////////////////////////////////////////////////////////////////////////
    SimpleThreadCoord();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Assignment operator
    /// @details This function copies the user function from the original
    ///     object.
    /// @note The event objects are not copied from the rhs object.
    ///////////////////////////////////////////////////////////////////////////
    SimpleThreadCoord& operator=(const SimpleThreadCoord& rhs);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details This function waits for the thread to stop then cleans up
    ///     the events.
    ///////////////////////////////////////////////////////////////////////////
    ~SimpleThreadCoord();

    // Use default copy construction
    //SimpleThreadCoord(const SimpleThreadCoord& orig);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Runs the thread and starts the calls to client code
    /// @details This function runs calls the client function continuously.  
    ///     If the client code or the stop event is signaled, then this 
    ///     function returns.
    ///////////////////////////////////////////////////////////////////////////
    void Run();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Notifies the thread that it has been signaled to stop
    ///////////////////////////////////////////////////////////////////////////
    void SignalStop();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Waits until the thread function in Run() signals that it has
    ///     stopped.
    ///////////////////////////////////////////////////////////////////////////
    void WaitForStop();

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Event that indicates the stop signal has been set
    ///////////////////////////////////////////////////////////////////////////
    Event* m_StopEvent;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Event that indicates the thread function has ended
    ///////////////////////////////////////////////////////////////////////////
    Event* m_ThreadStoppedEvent;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The client code to call
    ///////////////////////////////////////////////////////////////////////////
    UserFunc_t m_UserFunc;
}; // end class SimpleThreadCoord

///////////////////////////////////////////////////////////////////////////////
/// @class ThreadObj ThreadObj.h <Util\ThreadObj.h>
/// @brief Manages a thread running client code
/// @details ThreadObj contains functionality for starting and stopping client
///     code running in a thread.  ThreadObj works in conjunction with 
///     THREAD_COORD class that handles the client code running in the loop.
///     See SimpleThreadCoord for the functionality that must be provided for
///     this template parameter.
/// @attention ThreadObj is tied to the life time of the thread.  Deleting ThreadObj
///     will cause the corresponding thread to close.
/// @see SimpleThreadCoord
///////////////////////////////////////////////////////////////////////////////
template <typename THREAD_COORD>
class ThreadObj
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Construction does not start the thread.
    ///////////////////////////////////////////////////////////////////////////
    ThreadObj()
    :
    m_Thread(0)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Destruction will halt the thread
    ///////////////////////////////////////////////////////////////////////////
    ~ThreadObj()
    {
        if( m_Thread)
        {
            m_ThreadCoord.WaitForStop();
        }
        delete m_Thread;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Starts the thread
    /// @details This function creates a new thread and executes the given 
    ///     function specified in the argument list.  The function returns
    ///     true if the thread could be created, false otherwise.
    /// @param[in] userObj The thread coordinator that handles running the client code
    /// @return @arg true - A new thread was started
    ///         @arg false - Failed to create the new thread
    ///////////////////////////////////////////////////////////////////////////
    bool Run(THREAD_COORD userObj)
    {
        assert( !m_Thread);

        m_ThreadCoord = userObj;
        m_Thread = Thread::Create();

        assert(m_Thread);
        if( !m_Thread)
        {
            return false;
        }

        bool success = m_Thread->StartThread(MyThreadFunction, this);
        assert(success);
        return success;
    } // end Run

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Stops the execution of the thread
    ///////////////////////////////////////////////////////////////////////////
    void Stop()
    {
        m_ThreadCoord.SignalStop();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Waits for the thread to stop
    ///////////////////////////////////////////////////////////////////////////
    void WaitForStop()
    {
        m_ThreadCoord.WaitForStop();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Indicates if the thread is running
    /// @return @arg true - The thread is running
    ///         @arg false - The thread is not running
    ///////////////////////////////////////////////////////////////////////////
    bool IsRunning() const
    {
        return m_ThreadCoord.IsRunning();
    } // end IsRunning


private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Function called by the new thread
    /// @details This function calls the thread coordinator for the ThreadObj
    ///     passed in as the parameter. The thread coordinator function 
    ///     does not return until the thread is ready to quit.
    /// @param[in] lpParam Points to a ThreadObj instance
    /// @return Always 0
    ///////////////////////////////////////////////////////////////////////////
    static nik::ThreadFuncReturnType_t NIK_API MyThreadFunction
        (  
        nik::ThreadFuncArgType_t lpParam 
        )
    {
        ThreadObj* threadObj = static_cast<ThreadObj*>(lpParam);

        threadObj->m_ThreadCoord.Run();
        // cleanup the thread
        delete threadObj->m_Thread;
        threadObj->m_Thread = 0;
        return 0; /// @todo What to return for windows thread function
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Disallow copy construction
    ///////////////////////////////////////////////////////////////////////////
    ThreadObj(const ThreadObj&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Disallow assignment
    ///////////////////////////////////////////////////////////////////////////
    ThreadObj& operator=(const ThreadObj&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Object that handles running the client code
    ///////////////////////////////////////////////////////////////////////////
    THREAD_COORD m_ThreadCoord;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The thread controlled by this object
    ///////////////////////////////////////////////////////////////////////////
    Thread* m_Thread;

}; // end class ThreadObj

template <typename CLIENT_FUNC>
class ThreadCoord
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Sets up the object with the user function to call in the
    ///     thread.
    ///////////////////////////////////////////////////////////////////////////
    ThreadCoord(CLIENT_FUNC userFunc)
    :
    m_StopEvent(0),
    m_ThreadStoppedEvent(0),
    m_UserFunc(userFunc),
    m_IsRunning(false)
    {}

    ThreadCoord()
    :
    m_StopEvent(0),
    m_ThreadStoppedEvent(0),
    m_IsRunning(false)
    {}


    ThreadCoord(const ThreadCoord& orig)
    :
    m_StopEvent(0),
    m_ThreadStoppedEvent(0),
    m_UserFunc(orig.m_UserFunc),
    m_IsRunning(false)
    {}

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Assignment operator
    /// @details This function copies the user function from the original
    ///     object.
    /// @note The event objects are not copied from the rhs object.
    ///////////////////////////////////////////////////////////////////////////
    ThreadCoord& operator=(const ThreadCoord& rhs)
    {
        m_UserFunc = rhs.m_UserFunc;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details This function waits for the thread to stop then cleans up
    ///     the events.
    ///////////////////////////////////////////////////////////////////////////
    ~ThreadCoord()
    {
        // Only cleanup if the events have been created from Run()
        if(m_StopEvent || m_ThreadStoppedEvent)
        {
            WaitForStop();
            delete m_StopEvent;
            delete m_ThreadStoppedEvent;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Runs the thread and starts the calls to client code
    /// @details This function runs calls the client function continuously.  
    ///     If the client code or the stop event is signaled, then this 
    ///     function returns.
    ///////////////////////////////////////////////////////////////////////////
    void Run()
    {

        if( m_IsRunning)
        {
            nik::log << "Warning: Thread already running, attempted to call ThreadCoord::Run()\n"; 
            return;
        }
        assert( !m_IsRunning);
        m_IsRunning = true;

        if(!m_StopEvent)
        {
            // Setup the events
            m_StopEvent = Event::Create();
            assert(m_StopEvent);
            m_ThreadStoppedEvent = Event::Create();
            assert(m_ThreadStoppedEvent);
        }

        Reset();

        // Begin execution
        nik::log << "Run-> Loop start...\n";
        // Call the users function, if it returns false then quit the 
        // execution of the thread.  If true, then check for the quit
        // event being set.
        bool continueThread = true; // Flags if the thread will continue\quit
        while(m_UserFunc.Run(continueThread) && continueThread) // Note order of evaluation
        {
            // Check for the quit event
            DWORD waitResult = m_StopEvent->WaitForEvent(0); // Don't wait
            if( waitResult == Event::WAIT_SIGNALED)
            {
                nik::log << "Received stop event\n";
                continueThread = false; // Flag the quit event, while loop will break
            }
        } // end while()
        m_IsRunning = false;
        m_ThreadStoppedEvent->SetEvent(); // Flag shutdown complete
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Notifies the thread that it has been signaled to stop
    ///////////////////////////////////////////////////////////////////////////
    void SignalStop()
    {
        if( !m_IsRunning)
        {
            nik::log << "[ThreadCoord::SignalStop] Thread is not running, cannot signal stop\n";
            return;
        }
         // Set the stop event
        assert(m_StopEvent);
        m_StopEvent->SetEvent();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Waits until the thread function in Run() signals that it has
    ///     stopped.
    ///////////////////////////////////////////////////////////////////////////
    void WaitForStop()
    {
        if( !m_IsRunning)
        {
            nik::log << "[ThreadCoord::WaitForStop] Thread is not running, skipping wait.\n";
            return;
        }
        assert(m_StopEvent);
        bool result = m_StopEvent->SetEvent(); // Set again just in case
        assert(result);

        assert(m_ThreadStoppedEvent);
        m_ThreadStoppedEvent->WaitForEvent(Event::FOREVER);
    } // end WaitForStop

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Indicates if the thread is running
    /// @return @arg true - The thread is running
    ///         @arg false - The thread is not running
    ///////////////////////////////////////////////////////////////////////////
    bool IsRunning() const
    {
        return m_IsRunning;
    } // end IsRunning

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Resets events to the initial state
    ///////////////////////////////////////////////////////////////////////////
    void Reset()
    {
        m_StopEvent->ClearEvent();
        m_ThreadStoppedEvent->ClearEvent();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Event that indicates the stop signal has been set
    ///////////////////////////////////////////////////////////////////////////
    Event* m_StopEvent;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Event that indicates the thread function has ended
    ///////////////////////////////////////////////////////////////////////////
    Event* m_ThreadStoppedEvent;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The client code to call
    ///////////////////////////////////////////////////////////////////////////
    CLIENT_FUNC m_UserFunc;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Indicates if the thread is running
    ///////////////////////////////////////////////////////////////////////////
    bool m_IsRunning;

}; // end class ThreadCoord

} // end namespace nik

#endif

////////////////////////End-of-File////////////////////////////////////////////
