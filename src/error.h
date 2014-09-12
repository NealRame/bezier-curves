#pragma once

#include <exception>
#include <string>

namespace nealrame
{
class Error : public std::exception {
	std::string msg_;
public:
	Error() noexcept
	{ }
	
	Error(const char *msg) noexcept :
		msg_(msg)
	{ }

	Error(const std::string &msg) noexcept :
		msg_(msg)
	{ }

	virtual ~Error()
	{ }
	
	virtual const char * what() const noexcept
	{
		return msg_.c_str();
	}
};	
}
