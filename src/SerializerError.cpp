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
    SerializerErrorCode error_code,
    unsigned long lineno,
    std::string file_context,
    std::string error
    )
{
    SerializerError error_record(error_code, lineno, file_context, error);
    mErrorsByCode[error_code].push_back(error_record);
}
bool
SerializerErrorReporterByCode::hasErrorCode(SerializerErrorCode error_code) const
{
    return mErrorsByCode.find(error_code) != mErrorsByCode.end();
}
bool
SerializerErrorReporterByCode::hasError() const
{
    return mErrorsByCode.size() != 0;
}

void
SerializerErrorReporterByCode::dump() const
{
    for (const auto & it : mErrorsByCode) {
	for (const auto & itErr : it.second) {
	    fprintf(stderr, "Code: %d\n", itErr.error_code);
	    fprintf(stderr, "Context: %s:%ld\n", itErr.file_context.c_str(), itErr.lineno);
	    fprintf(stderr, "Message: %s\n", itErr.error.c_str());
	}
    }
std::map<SerializerErrorCode, std::vector<SerializerError>> mErrorsByCode;
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
