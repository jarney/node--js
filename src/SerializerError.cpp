#include "node--js/SerializerError.hpp"
#include <ostream>

using namespace NodeJS::core;

////////////////////////////////
// Send to output stream.
////////////////////////////////
SerializerErrorReporterStream::SerializerErrorReporterStream(std::ostream & aOstream)
    : mOstream(aOstream)
{}

void
SerializerErrorReporterStream::reportError(
	SerializerErrorCode error_code,
	unsigned long lineno,
	std::string file_context,
	std::string error
    )
{
    mOstream << "Line " << lineno << ": " << file_context << ": " << error << std::endl;
}

////////////////////////////////
// By error code
////////////////////////////////
unsigned long
SerializerErrorReporterByCode::size() const
{
    return mErrorsByCode.size();
}

const std::vector<SerializerError> &
SerializerErrorReporterByCode::getErrors(SerializerErrorCode code)
{
    return mErrorsByCode[code];
}

void
SerializerErrorReporterByCode::reportError(
	unsigned long error_code,
	unsigned long lineno,
	std::string file_context,
	std::string error
    )
{
    SerializerError error_record(error_code, lineno, file_context, error);
    mErrorsByCode[error_code].push_back(error_record);
}

SerializerError::SerializerError(
				 SerializerErrorCode aError_code,
				 unsigned long aLineno,
				 std::string aFile_context,
				 std::string aError
				)
  : error_code(aError_code)
  , lineno(aLineno)
  , file_context(aFile_context)
  , error(aError)
{}
