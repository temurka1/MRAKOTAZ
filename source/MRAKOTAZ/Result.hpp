#ifndef RESULT_H
#define RESULT_H

#include <string>

namespace MRAKOTAZ
{
	namespace core
	{	
		/// <summary>
		/// Class encapsulating a return status of an operation with an optional error message.
		/// </summary>
		class Result
		{
		public:
			enum StatusCode
			{
				OK = 0,
				NOT_OK, // I promise
				ERROR = 1,
			};

			Result() : _code(StatusCode::OK)
			{
			}

			explicit Result(StatusCode code) : _code(code)
			{
			}

			Result(StatusCode code, std::string& message) : _code(code), _message(message)
			{
			}

			Result(const Result& status) = default;
			Result(Result&& status) = default;

			StatusCode Code() const
			{
				return _code;
			}

			const std::string& Error() const
			{
				return _message;
			}

			bool Ok() const
			{
				return _code == StatusCode::OK;
			}

			bool operator==(StatusCode code) const
			{
				return code == _code;
			}

			Result& operator=(const Result&) = default;
		private:
			StatusCode _code;
			std::string _message;
		};

#define RETURN_IF_ERROR(expression)								\
    const MRAKOTAZ::core::Result _local_status = (expression);	\
    if (!_local_status.Ok())									\
	{															\
		return _local_status;									\
    }															\
  
	}
}

#endif 

