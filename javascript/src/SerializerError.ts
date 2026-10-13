
export enum SerializerErrorCode {
  // Package-level errors
  PACKAGE_DUPLICATE = 1000,
  PACKAGE_MISSING = 1001,
  PACKAGE_ID_MISSING = 1002,
  PACKAGE_ID_INCONSISTENT = 1003,
  
  // Data-type level errors
  DATA_TYPE_ID_MISSING = 2000,
  DATA_TYPE_ID_DUPLICATE = 2001,

  // Node type level errors
  BAD_NODE_TYPE = 3000,
  NODE_TYPE_ID_MISSING = 3001,
  NODE_TYPE_ID_DUPLICATE = 3002,
  NODE_TYPE_VISIBILITY_MISSING = 3003,
  NODE_TYPE_VISIBILITY_INVALID = 3004,
  NODE_TYPE_TYPE_MISSING = 3005,
  NODE_TYPE_TYPE_INVALID = 3006,

  PORT_ID_MISSING = 4001,
  PORT_ID_DUPLICATE = 4002,
  PORT_DATA_TYPE_MISSING = 4003,
  PORT_DATA_TYPE_INVALID = 4004,
  PORT_CONNECTION_POLICY_MISSING = 4005,
  PORT_CONNECTION_POLICY_INVALID = 4006,

  // Graph level errors
  GRAPH_ID_MISSING = 5000,
  GRAPH_ID_DUPLICATE = 5001,

  // Node-level errors
  NODE_NODE_ID_MISSING = 6000,
  NODE_NODE_ID_DUPLICATE = 6001,
  NODE_NODE_TYPE_MISSING = 6002,
  NODE_NODE_TYPE_INVALID = 6003,

  // Edge-level errors
  BAD_EDGE = 7000,


    // Generic metadata formatting errors
  METADATA_MISSING_NAMESPACE = 8000,
  METADATA_DUPLICATE_NAMESPACE = 8001,
  METADATA_MISSING_KEY = 8002,
  METADATA_DUPLICATE_KEY = 8003
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
