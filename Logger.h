///////////////////////////////////////////////////////////////////////////////
/// @file Util\Logger.h
/// @brief Logger class declaration
/// @internal
///
/// 03March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////
#ifndef NIK_LOGGER_HEADER
#define NIK_LOGGER_HEADER

#include <fstream>
#include <string>
#include <sstream>
#include <Util\Utility.h>
#include <Util\ScopeLock.h>

namespace nik{

///////////////////////////////////////////////////////////////////////////////
/// @class Logger <Util\Logger.h>
/// @brief Allows for the logging of data to file or command line
/// @details This class is designed to give an entire project access to the
///     same out file for output.  This class creates its own thread to do
///     the writing to file.  
/// @attention The SetFile function should be called before using the object.
/// @warning This class is thread safe to a point.  Seperate calls of the
///     stream operator << may not appear together.
/// @note Logger is implemented by an impl class.
///////////////////////////////////////////////////////////////////////////////
class Logger
{
public:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @attention Call SetFile(const char* const) before using this object.
    /// @note Sets up the mutex and impl object on creation
    ///////////////////////////////////////////////////////////////////////////
    Logger();//const char* const fileName = 0);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    ///////////////////////////////////////////////////////////////////////////
    ~Logger();

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Sets the file for output
    /// @details Opens the file for output.  If a file is already open, then
    ///     that file will be closed before openning the new one.
    /// @note This function will spawn the logger thread
    /// @param[in] fileName The name of the file to open
    /// @return @arg true - The file opened successfully
    ///         @arg false - An error occured while trying to open the file
    ///////////////////////////////////////////////////////////////////////////
    bool SetFile(const char* const fileName);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Outputs some data to file
    /// @details Call this function to write output to the out file.
    /// @tparam T The type of the data being outputed
    /// @param val The data to output
    /// @return A reference to the Logger object
    ///////////////////////////////////////////////////////////////////////////
    template <class T>
    Logger& operator<<(const T& val)
    {
        ScopeLock al(m_Lock); // create auto lock
        m_Os << val;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Typedef for a function to manipulate the logger stream
    ///////////////////////////////////////////////////////////////////////////
    typedef Logger& (*ManipFunc_t)(Logger&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Stream manipulator handler
    /// @details Thils function accepts a string manipulator and calls it
    ///     on this object.
    /// @param[in] op The stream manipulator
    /// @return A refernce to the Logger object
    ///////////////////////////////////////////////////////////////////////////
    Logger& operator<<(ManipFunc_t op)
    {
        ScopeLock al(m_Lock); // create auto lock
        return op(*this);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Print an error
    /// @details This function assists in printing out an error message.  The
    ///     given message is printed, surrounded by a two rows of = characters.
    /// @param[in] msg The error message to print
    ///////////////////////////////////////////////////////////////////////////
    void PrintError(const std::string& msg);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Forces the stream out to file
    ///////////////////////////////////////////////////////////////////////////
    void Flush();

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Buffer for storing output before writing it
    ///////////////////////////////////////////////////////////////////////////
    std::ostringstream m_Os;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The implemenation class for Logger
    ///////////////////////////////////////////////////////////////////////////
    friend class LogImpl;
    class LogImpl;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The implementation object for this Logger
    ///////////////////////////////////////////////////////////////////////////
    LogImpl* m_LogImpl;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Lock for the output buffer
    ///////////////////////////////////////////////////////////////////////////
    Mutex* m_Lock;

}; // end class Logger


extern Logger log;

///////////////////////////////////////////////////////////////////////////
/// @brief Flushes the Logger stream
/// @param[in] logger The logger object to modify
/// @return A reference to the Logger object
///////////////////////////////////////////////////////////////////////////
inline Logger& endl(Logger& logger)
{
    logger << "\n";
    logger.Flush();
    return logger;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Thread safe logger
/// @details This logger class is designed to write safely in an environment 
///     with multiple threads running.  It is designed to work with Logger
///     to do the actually writing.  TLogger does not flush the stream to the
///     Logger object until Flush() is called or the TLogger object's 
///     destructor is called.
///////////////////////////////////////////////////////////////////////////////
template <bool USE_LOG>
class TLogger
{
public:
    
    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @attention This constructor is only valid when the template parameter
    ///     USE_LOG is set to true to use the global log object for logging.
    ///     Use TLogger(Logger&) to use TLogger with other loggers.
    /// @note Only implemented for USE_LOG == true
    ///////////////////////////////////////////////////////////////////////////
    TLogger(); 

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Constructor
    /// @details Create a TLogger that will %log through the given Logger 
    ///     object.
    /// @attention Only use when the template parameter USE_LOG is false.
    ///////////////////////////////////////////////////////////////////////////
    TLogger(Logger& logger);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Destructor
    /// @details Flush the stream on destruction.
    ///////////////////////////////////////////////////////////////////////////
    ~TLogger()
    {
        Flush();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Puts the given value into the stream
    /// @attention Use endl(TLogger&) to flush the stream
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline TLogger& operator<<(const T& val)
    {
        m_Os << val;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Typedef for a function that manipulates the TLogger object
    ///////////////////////////////////////////////////////////////////////////
    typedef TLogger& (*ManipFunc_t)(TLogger&);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Calls the given function to manipulate the TLogger object
    /// @details Enables stream manipulators like std::endl.
    ///////////////////////////////////////////////////////////////////////////
    TLogger& operator<<(ManipFunc_t op)
    {
        return op(*this);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Flushes the TLogger stream
    /// @details This function flushes the stream stored in this object to
    ///     the corresponding Logger object.
    ///////////////////////////////////////////////////////////////////////////
    void Flush()
    {
        *m_Log << m_Os.str();
        m_Log->Flush();
        m_Os.str("");
    }

private:

    ///////////////////////////////////////////////////////////////////////////
    /// @brief Storage for the stream data
    /// @details This acts as a buffer until the data is flushed to the 
    ///     Logger object.
    ///////////////////////////////////////////////////////////////////////////
    std::ostringstream m_Os;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief The logger to use for this object
    ///////////////////////////////////////////////////////////////////////////
    Logger* m_Log;
}; // endl class TLogger

///////////////////////////////////////////////////////////////////////////////
/// @brief Specialization for using the global log Logger
/// @details The TLogger object will write to the log object.
///////////////////////////////////////////////////////////////////////////////
template<>
TLogger<true>::TLogger();

///////////////////////////////////////////////////////////////////////////////
/// @brief for using TLogger with the global log logger.
///////////////////////////////////////////////////////////////////////////////
typedef TLogger<true> TLog;

///////////////////////////////////////////////////////////////////////////////
/// @brief Specialization for using a specified Logger object
/// @details The TLogger object will write to the given Logger object.  The
///     logger object must be stay valid for the entire existence of this
///     TLogger object.
///////////////////////////////////////////////////////////////////////////////
template<>
TLogger<false>::TLogger(Logger& logger);

///////////////////////////////////////////////////////////////////////////////
/// @brief Flushes the TLogger object
/// @details Calls TLogger::Flush() on the given TLogger object.  
///////////////////////////////////////////////////////////////////////////////
template<bool USE_LOG>
inline TLogger<USE_LOG>& endl(TLogger<USE_LOG>& logger)
{
    logger << "\n";
    logger.Flush();
    return logger;
}

} // end namespace nik

#endif 

////////////////////////End-of-File////////////////////////////////////////////