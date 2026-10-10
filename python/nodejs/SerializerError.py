
class SerializerErrorReporter:
    def __init__(self):
        pass

    def reportError(self, errorCode, lineno, file_context, error):
        # This is the abstract error reporter
        pass


class SerializerErrorReporterStream:
    def __init__(self, aStream):
        self.mStream = aStream
        pass

    def reportError(self, errorCode, lineno, file_context, error):
        msg = "Error: " + str(errorCode) + str(file_context) + ": " + str(lineno) + " : " + error + "\n"
        self.mStream.write(msg)
