#pragma once

#include <string>
#include <vector>
#include <map>

namespace NodeJS {
    namespace core {

	typedef unsigned long SerializerErrorCode;
	
	/**
	 * Serializer errors are reported by the specific
	 * implementations and are dependent on the type of
	 * serializer in use.  Therefore the error details
	 * depend heavily on which serializer was used.
	 */
	class SerializerErrorReporter {
	public:
	    SerializerErrorReporter() = default;
	    virtual ~SerializerErrorReporter() = default;
	    /**
	     * This method is used by a serializer to report an error.
	     * The error code's interpretation differs based on which
	     * serializer is in use.
	     */
	    virtual void reportError(
		SerializerErrorCode errror_code,
		unsigned long lineno,
		std::string file_context,
		std::string error
		) = 0;
	};

	/**
	 * This object represents all of the information
	 * about a serializer error.
	 */
	class SerializerError {
	public:
  	    SerializerError();
  	    SerializerError(
			    SerializerErrorCode aError_code,
			    unsigned long aLineno,
			    std::string aFile_context,
			    std::string aError
			   );
  	    SerializerError(const SerializerError & other) = default;
	    ~SerializerError() = default;
	    SerializerErrorCode error_code;
	    unsigned long lineno;
	    std::string file_context;
	    std::string error;
	    
	};

	/**
	 * This provides an error reporter that simply
	 * formats the error to an output stream
	 * for display or on a console.
	 */
	class SerializerErrorReporterStream : public SerializerErrorReporter {
	public:
	    SerializerErrorReporterStream(std::ostream & os);
	    virtual ~SerializerErrorReporterStream() = default;
	    virtual void reportError(
		SerializerErrorCode error_code,
		unsigned long lineno,
		std::string file_context,
		std::string error
		);
	private:
	    std::ostream & mOstream;
	};
	/**
	 * This provides an error reporter that stores
	 * the errors in a map by error code so that they
	 * can be retrieved by type.  This is useful in
	 * things like unit tests where we expect specific errors.
	 */
	class SerializerErrorReporterByCode : public SerializerErrorReporter {
	public:
	    SerializerErrorReporterByCode() = default;
	    virtual ~SerializerErrorReporterByCode() = default;
	    virtual void reportError(
		SerializerErrorCode error_code,
		unsigned long lineno,
		std::string file_context,
		std::string error
		);
	    size_t size() const;
	    const std::vector<SerializerError> & getErrors(SerializerErrorCode code);
	private:
	    std::map<SerializerErrorCode, std::vector<SerializerError>> mErrorsByCode;
	};
	
    }

}
