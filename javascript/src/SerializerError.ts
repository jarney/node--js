
type SerializerErrorCode = number;
	
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

class SerializerErrorReporterStream implements SerializerErrorReporter {
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

class SerializerErrorReporterByCode implements SerializerErrorReporter {
  mErrorsByCode: { [key: string] : SerializerError[] };
  constructor() {
    this.mErrorsByCode = {};
  }
  reportError(
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
}

export { SerializerErrorReporterStream, SerializerErrorReporterByCode };
