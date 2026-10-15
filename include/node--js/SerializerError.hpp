#pragma once

#include <string>
#include <vector>
#include <map>

namespace NodeJS {
    namespace core {

	typedef enum {
	    NO_LOADER_DEFINED = 1, // No loader plugin defined.
	    
	    // Package-level errors
	    PACKAGE_VERSION_MISSING = 1000, //
	    PACKAGE_VERSION_UNSUPPORTED = 1001, //
	    PACKAGE_DUPLICATE = 1002, //
	    PACKAGE_MISSING = 1003, //
	    PACKAGE_ID_MISSING = 1004, //
	    PACKAGE_ID_INCONSISTENT = 1005, //
	    PACKAGE_SCOPE_FAILED_TO_LOAD = 1006, //
	    
	    
	    // Data-type level errors
	    DATA_TYPE_ID_MISSING = 2000, //
	    DATA_TYPE_ID_DUPLICATE = 2001, //
	    DATA_TYPE_ID_INVALID = 2002,
	    
	    // Node type level errors
	    NODE_TYPE_ID_MISSING = 3000, //
	    NODE_TYPE_ID_DUPLICATE = 3001, //
	    NODE_TYPE_VISIBILITY_MISSING = 3002, //
	    NODE_TYPE_VISIBILITY_INVALID = 3003, //
	    NODE_TYPE_TYPE_MISSING = 3004, //
	    NODE_TYPE_TYPE_INVALID = 3005, //
	    NODE_TYPE_DATA_KEY_MISSING = 3006,
	    NODE_TYPE_DATA_KEY_DUPLICATE = 3007,
	    
	    PORT_ID_MISSING = 4001, //
	    PORT_ID_DUPLICATE = 4002, //
	    PORT_DATA_TYPE_MISSING = 4003, //
	    PORT_DATA_TYPE_INVALID = 4004, //
	    //PORT_CONNECTION_POLICY_MISSING = 4005, // NOT ENFORCED, defaults to 'one'.
	    PORT_CONNECTION_POLICY_INVALID = 4006,
	    
	    // Graph level errors
	    GRAPH_ID_MISSING = 5000,
	    GRAPH_ID_DUPLICATE = 5001,
	    GRAPH_CYCLE = 5002,
	    
	    // Node-level errors
	    NODE_NODE_ID_MISSING = 6000,
	    NODE_NODE_ID_DUPLICATE = 6001,
	    NODE_NODE_TYPE_MISSING = 6002,
	    NODE_NODE_TYPE_INVALID = 6003,
	    NODE_DATA_KEY_MISSING = 6004,
	    NODE_DATA_KEY_DUPLICATE = 6005,
	    NODE_NODE_TYPE_DOES_NOT_ALLOW_OVERRIDE = 6006,
	
	    // Edge-level errors
	    EDGE_DUPLICATE = 7001, //
	    EDGE_FROM_NODE_MISSING = 7002,
	    EDGE_FROM_PORT_MISSING = 7003,
	    EDGE_TO_NODE_MISSING = 7004,
	    EDGE_TO_PORT_MISSING = 7005,
	    EDGE_FROM_NODE_INVALID = 7006,
	    EDGE_FROM_PORT_INVALID = 7007,
	    EDGE_TO_NODE_INVALID = 7008,
	    EDGE_TO_PORT_INVALID = 7009,
	    EDGE_PORT_DATA_MISMATCH = 7010, //
	    EDGE_VIOLATES_POLICY = 7011,
	    
	    // Generic metadata formatting errors
	    METADATA_NAMESPACE_MISSING = 8000, //
	    METADATA_NAMESPACE_DUPLICATE = 8001, //
	    METADATA_KEY_MISSING = 8002, //
	    METADATA_KEY_DUPLICATE = 8003, //

	    LEGACY_ERROR_XML_PARSE = 9999
	    
	} SerializerErrorCode;
	
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
	    bool hasError() const;
	    bool hasErrorCode(SerializerErrorCode error_code) const;
	    void dump() const;
	private:
	    std::map<SerializerErrorCode, std::vector<SerializerError>> mErrorsByCode;
	};
	
    }

}
