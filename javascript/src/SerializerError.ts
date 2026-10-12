
export enum SerializerErrorCode {
  // Package-level errors
  PACKAGE_DUPLICATE = 1000,
  PACKAGE_MISSING = 1001,
  PACKAGE_ID_MISSING = 1002,
  PACKAGE_ID_INCONSISTENT = 1003,
  
  // Data-type level errors
  BAD_DATA_TYPE = 2000,

  // Node type level errors
  BAD_NODE_TYPE = 3000,

  // Graph level errors
  BAD_GRAPH = 4000,

  // Node-level errors
  BAD_NODE = 5000,

  // Edge-level errors
  BAD_EDGE = 6000
  
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
