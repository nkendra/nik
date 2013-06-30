///////////////////////////////////////////////////////////////////////////////
/// @file Util\Logger.cpp
/// @brief Logger class implementation
/// @internal
///
/// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#include "Logger.h"
#include "ScopeLock.h"
#include "Mutex.h"
#include "Thread.h"
#include <cassert>
//----------------------Constants--------------------------------------------//
const size_t Wait_Time_glob = 10; // ms
// Error strings
const char* const Error_Block_cstr = 
"=================================================";

//----------------------Free-Function-Prototypes-----------------------------//

namespace nik {
//----------------------Log-Object-Definition--------------------------------//
Logger log;

//----------------------LogImpl-Declaration----------------------------------//
///////////////////////////////////////////////////////////////////////////////
/// @class Logger Logger.cpp <Util\Logger.cpp>
/// @brief Implements the logging behavior
///////////////////////////////////////////////////////////////////////////////
class Logger::LogImpl
{
public:
    
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @attention The object isn't fully initialized until StartThread() is 
    ///     called.
    /// @param[in] log The Logger object
    ///////////////////////////////////////////////////////////////////////////
    LogImpl(Logger* log);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @note Cleans up the thread and mutex resources
    ///////////////////////////////////////////////////////////////////////////
    ~LogImpl();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Starts the logger thread
    /// @attention Multiple calls to this function are ignored.
    /// @note Sets up the thread and mutex resources
    ///////////////////////////////////////////////////////////////////////////
    void StartThread();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Opens the file for output
    /// @attention If a file was already open, the previous file is closed 
    ///     before opening the new file.
    /// @param[in] fileName The name of the file
    /// @return @arg true - Success
    ///         @arg false - Failure
    ///////////////////////////////////////////////////////////////////////////
    bool OpenFile(const char* const fileName);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Close the current output file
    /// @return @arg true - The file was closed
    ///         @arg false - The file was not closed
    ///////////////////////////////////////////////////////////////////////////
    bool CloseFile();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Force a write of the buffered output to file
    /// @return @arg true - Success
    ///         @arg false - Failure
    ///////////////////////////////////////////////////////////////////////////
    bool ForceWrite();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Attempt to write buffered output to file
    /// @details Calling this function will attempt to get the write lock
    ///     to output what is in the given output stream.  If the wait for the
    ///     lock times out, the data is not written and the function returns
    ///     false.
    /// @param[in] os The output to write
    /// @return @arg true - The data was written
    ///         @arg false - The data was not written
    ///////////////////////////////////////////////////////////////////////////
    bool Write(const std::ostringstream& os);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Thread function to call
    /// @details This is the function that is passed to the thread to call.
    ///     The function continually attempts to get the buffered output and
    ///     write it to file.  The function returns when the continue flag
    ///     is set to false in the Logger object.
    /// @param[in] logger Pointer to the LogImpl object
    /// @return Always 0
    ///////////////////////////////////////////////////////////////////////////
    static ThreadFuncReturnType_t NIK_API LogThreadFunc(ThreadFuncArgType_t logger);

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The Logger object
    ///////////////////////////////////////////////////////////////////////////
    Logger* m_Log;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The output file stream
    ///////////////////////////////////////////////////////////////////////////
    std::ofstream m_Of;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The output buffer
    ///////////////////////////////////////////////////////////////////////////
    std::ostringstream m_Buffer;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The output thread
    ///////////////////////////////////////////////////////////////////////////
    Thread* m_ThreadHandle;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Lock for the output buffer
    ///////////////////////////////////////////////////////////////////////////
    Mutex* m_BuffMutex;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Flag for continueing the output thread
    /// @details @li true - Continue output thread
    ///          @li false - End output thread
    ///////////////////////////////////////////////////////////////////////////
    bool m_Continue;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Indicates if the thread has started
    ///////////////////////////////////////////////////////////////////////////
    bool m_ThreadStarted;

}; // end class Logger::LogImpl

//----------------------Logger-Imlementation---------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
Logger::Logger
    (
    )
:
m_Lock(0)
{
    m_Lock = Mutex::Create();
    assert(m_Lock);
    m_LogImpl = new LogImpl(this);

} // end Logger::Logger

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
Logger::~Logger()
{
    // todo delete stuff
    delete m_Lock;
    delete m_LogImpl;
} // end Logger::~Logger

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
bool Logger::SetFile
    (
    const char* const fileName
    )
{
    // OpenFile() will close the current file if it is open
    if( !m_LogImpl->OpenFile(fileName))
    {
        assert(0);
        return false;
    }

    m_LogImpl->StartThread();

    return true;
} // end Logger::SetFile

///////////////////////////////////////////////////////////////////////////////
// 02April2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
void Logger::PrintError
    (
    const std::string& msg
    )
{
    *this << "!" << Error_Block_cstr << "\n";
    *this << msg << "\n";
    *this << Error_Block_cstr << "!\n";
} // end Logger::PrintError

//----------------------Private-Implementation-------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 02April2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
void Logger::Flush()
{
    ScopeLock al(m_Lock); // create auto lock
    // Attempt to write the buffer
    if(m_LogImpl->Write(m_Os))
    {
        // buffer was written so clear the stream
        m_Os.str("");
    }

} // end Logger::Flush

//----------------------LogImpl-Implementation-------------------------------//
//----------------------Static-Members---------------------------------------//

//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
Logger::LogImpl::LogImpl
    (
    Logger* log
    )
    :
m_Log(log),
m_ThreadHandle(0),
m_BuffMutex(0),
m_Continue(false),
m_ThreadStarted(false)
{
} // end Logger::LogImpl::LogImpl

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
nik::Logger::LogImpl::~LogImpl()
{
    if(!m_ThreadStarted)
    {
        // the logger was never setup,
        // just quit
        return;
    }
    // shutdown thread
    m_Continue = false;
    Sleep(50);
    // todo setup event on thread quit
    {
        ScopeLock al(m_BuffMutex);        
        // flush everything still in buffer
        ForceWrite();
    }

    // todo delete stuff
    delete m_ThreadHandle;
    delete m_BuffMutex;

    CloseFile();
} // end LogImpl::~LogImpl

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
void Logger::LogImpl::StartThread()
{
    // Check if the thread is already started
    if(m_ThreadStarted)
    {
        return;
    }
    // Mark the thread as started
    m_ThreadStarted = true;
    m_BuffMutex = Mutex::Create();
    if(!m_BuffMutex)
    {
        assert(0);
        DWORD errCode = GetLastError();
        std::ostringstream msg;
        msg << "Error: StartThread->CreateMutex failed with code: ";
        msg << errCode << std::endl;
        throw Error(msg.str());
    }

    // Setup thread
    m_Continue = true;

    m_ThreadHandle = Thread::Create();
    assert(m_ThreadHandle);
    m_ThreadHandle->StartThread(LogThreadFunc, this);

} // end Logger::LogImpl::StartThread

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
ThreadFuncReturnType_t NIK_API Logger::LogImpl::LogThreadFunc( ThreadFuncArgType_t logger )
{ 
    LogImpl* log = static_cast<LogImpl*>(logger);

    std::ostringstream outBuffer;
    while(log->m_Continue)
    {
        {
            ScopeLock al(log->m_BuffMutex);
            // Get a copy of the out buffer, then release mutex
            outBuffer << log->m_Buffer.str();
            log->m_Buffer.str("");
        }
        // now write out to file and clear outBuffer
        log->m_Of << outBuffer.str();
        outBuffer.str("");
        log->m_Of.flush();
        Sleep(25);
    } // end while(continue)
    
    // todo send end thread event

    return 0; 
} // end Logger::LogImpl::LogThreadFunc

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
bool Logger::LogImpl::OpenFile
    (
    const char* const fileName
    )
{
    if(m_Of.is_open())
    {
        // close file, then open new one
        CloseFile();
    }

    m_Of.open(fileName);

    if(!m_Of)
    {
        // todo
        return false;
    }

    return true;
} // end Logger::LogImpl::OpenFile

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
bool Logger::LogImpl::CloseFile()
{
    if(!m_Of.is_open())
    {
        return false;
    }

    m_Of.close();

    return !m_Of;
} // end Logger::LogImpl::CloseFile

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
bool Logger::LogImpl::ForceWrite()
{
    m_Buffer << m_Log->m_Os.str();
    m_Log->m_Os.str("");
    m_Of << m_Buffer.str();
    m_Buffer.str("");
    return true;
} // end Logger::LogImpl::ForceWrite

///////////////////////////////////////////////////////////////////////////////
// 19March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
bool Logger::LogImpl::Write
    (
    const std::ostringstream& os
    )
{
    if( !m_BuffMutex->Lock(Wait_Time_glob))
    {
        // Timed out, signal that the buffer was not saved off
        return false;
    }
    // save off the data and signal success
    m_Buffer << os.str();
    m_BuffMutex->Unlock();
    return true;
} // end Logger::LogImpl::Write

//----------------------TLogger-Implementation-------------------------------//
//----------------------Public-Implementation--------------------------------//
///////////////////////////////////////////////////////////////////////////////
// 28May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
template<>
TLogger<true>::TLogger()
:
m_Log(&log)
{}

///////////////////////////////////////////////////////////////////////////////
// 28May2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
template<>
TLogger<false>::TLogger(Logger& logger)
:
m_Log(&logger)
{}

} // end namespace nik



//----------------------Free-Function-Implementations------------------------//

////////////////////////End-of-File////////////////////////////////////////////
