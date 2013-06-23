/*
 * Exception.h
 *
 *  Created on: Jun 18, 2013
 *      Author: nick
 */

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>
#include <exception>
#include <sstream>

class Exception : public std::exception
{
public:
	Exception(const std::string& desc)
	:
	m_Desc(desc)
	{}

	virtual ~Exception() throw()
	{}

	virtual const char* what() const throw()
	{
		return m_Desc.c_str();
	}

private:

	std::string m_Desc;
};

#define RAISE_EXCEPTION(STR_DESC) std::ostringstream os; os << __FILE__ << ": " << __LINE__ << "\n" \
	<< __PRETTY_FUNCTION__ << ": " << STR_DESC; \
	throw Exception(os.str())

#endif
