
export enum SerializerErrorCode {
  
  // Package-level errors
  PACKAGE_VERSION_MISSING = 1000, //
  PACKAGE_VERSION_UNSUPPORTED = 1001, //
  PACKAGE_DUPLICATE = 1002, //
  PACKAGE_MISSING = 1003, //
  PACKAGE_ID_MISSING = 1004, //
  PACKAGE_ID_INCONSISTENT = 1005, //
  
  // Data-type level errors
  DATA_TYPE_ID_MISSING = 2000, //
  DATA_TYPE_ID_DUPLICATE = 2001, //

  // Node type level errors
  NODE_TYPE_ID_MISSING = 3000, //
  NODE_TYPE_ID_DUPLICATE = 3001, //
  NODE_TYPE_VISIBILITY_MISSING = 3002, //
  NODE_TYPE_VISIBILITY_INVALID = 3003, //
  NODE_TYPE_TYPE_MISSING = 3004, //
  NODE_TYPE_TYPE_INVALID = 3005, //

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
  METADATA_KEY_DUPLICATE = 8003 //
};

export interface SerializerErrorReporter {
  reportError(
    error_code: SerializerErrorCode,
    lineno: number,
    file_context: string,
    error: string
  ) : void;
}

class SerializerError {
  public mErrorCode : SerializerErrorCode;
  public lineno: number;
  public file_context: string;
  public error: string;
  constructor(
    error_code: SerializerErrorCode,
    lineno: number,
    file_context: string,
    error: string
  ) {
    this.mErrorCode = error_code;
    this.lineno = lineno;
    this.file_context = file_context;
    this.error = error;
  }
}

export class SerializerErrorReporterStream implements SerializerErrorReporter {
  constructor() {
  }
  
  reportError(
    error_code: SerializerErrorCode,
    lineno: number,
    file_context: string,
    error: string
  ) : void {
    var err = new SerializerError(error_code, lineno, file_context, error);
    console.log(err);
  }
}

export class SerializerErrorReporterByCode implements SerializerErrorReporter {
  mErrorsByCode: { [key: string] : SerializerError[] };
  constructor() {
    this.mErrorsByCode = {};
  }
  public reportError(
    error_code: SerializerErrorCode,
    lineno: number,
    file_context: string,
    error: string
  ) : void {
    var err = new SerializerError(error_code, lineno, file_context, error);
    if (error_code in this.mErrorsByCode) {
      this.mErrorsByCode[error_code]!.push(err);
    }
    else {
      this.mErrorsByCode[error_code] = [ err ];
    }
  }
  public hasErrorCode(code: SerializerErrorCode) : boolean {
    return (code in this.mErrorsByCode);
  }
  public dump() : void {
    console.log(this.mErrorsByCode);
  }
  public hasError() : boolean {
    return Object.keys(this.mErrorsByCode).length > 0;
  }
}

export class SerializerErrorReporterNil implements SerializerErrorReporter {
  reportError(
    error_code: SerializerErrorCode,
    lineno: number,
    file_context: string,
    error: string
  ) : void {
    // Nil reporter does nothing.
  }
}
