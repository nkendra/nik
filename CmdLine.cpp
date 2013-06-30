///////////////////////////////////////////////////////////////////////////////
/// @file CmdLine.cpp
/// @brief Implementation of Control class
/// @internal
///
/// 03March2010, nik: initial
///////////////////////////////////////////////////////////////////////////////

#include "CmdLine.h"

using std::string;
using std::make_pair;

//----------------------Free-Function-Prototypes-----------------------------//
bool IsFlag(const string& arg);
//----------------------CmdLine-Implementation-------------------------------//
//----------------------Public-Implementation--------------------------------//
namespace nik {
CmdLine::CmdLine()
{
} // end CmdLine::CmdLine

CmdLine::CmdLine
    (
    int argc, 
    char** argv
    )
{
    ParseCmdArg(argc, argv);
} // end CmdLine::CmdLine

    
void CmdLine::ParseCmdArg
    (
    int argc, 
    char** argv
    )
{
    m_Args.clear();
    string flag;
    string data;
    for(int i = 1; i < argc; ++i)
    {
        // check for flag
        if(IsFlag(argv[i]))
        {
            // Identified flag, save the previous flag and data
            if( !flag.empty())
            {
                m_Args.insert(make_pair(flag, data));
                flag.clear();
                data.clear();
            }
            // Identified flag, get the flag
            flag.assign(argv[i]+1);
        }
        else // Data
        {
            // Append spaces in between args
            if(!data.empty())
            {
                data.append(" ");
            }
            data.append(argv[i]);
        }
    }
    // Save the last flag/data pair
    if(!flag.empty())
    {
        m_Args.insert(make_pair(flag, data));
    }

} // end CmdLine::ParseCmdArg

const char* CmdLine::GetArg
    (
    char flag
    ) const
{
    ArgMap_t::const_iterator it = m_Args.find(string(1, flag));
    return it==m_Args.end() ? 0 : it->second.c_str();
} // end CmdLine::GetArg

const char* CmdLine::GetArg
    (
    const char* const flag
    ) const
{
    ArgMap_t::const_iterator it = m_Args.find(flag);
    return it==m_Args.end() ? 0 : it->second.c_str();
} // end CmdLine::GetArg

bool CmdLine::IsFlagSet
    (
    char flag
    ) const
{
    return GetArg(flag) != 0;
} // end CmdLine::IsFlagSet

bool CmdLine::IsFlagSet
    (
    const char* const flag
    ) const
{
    return GetArg(flag) != 0;
} // end CmdLine::IsFlagSet
} // end namespace nik
//----------------------Free-Function-Implementations------------------------//
bool IsFlag
    (
    const string& arg
    )
{
    return arg.size() > 1 && arg[0] == '-';
} // end IsFlag

////////////////////////End-of-File////////////////////////////////////////////
