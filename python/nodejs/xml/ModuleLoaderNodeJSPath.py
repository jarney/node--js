from nodejs.NodeModule import NodeModule

NODEJS_PATH_DELIMITER_CHARACTER = ';'
NODEJS_PATH_DELIMITER_STRING = ";"

NODEJS_PATH_ESCAPE_CHARACTER = '\\'
NODEJS_PATH_ESCAPE_STRING = "\\"


NORMAL = 0
ESCAPE = 1

def split_path(pathstr):
    path = []
    os = ""
    
    state = NORMAL
    for c in pathstr:
        if (state == NORMAL):
            if (c == NODEJS_PATH_ESCAPE_CHARACTER):
                state = ESCAPE
            elif (c == NODEJS_PATH_DELIMITER_CHARACTER):
                path.append(os)
                os = ""
                state = NORMAL;
            else:
                os = os + c
        else: #///*if (state == ESCAPE)*/ {
            if (c == NODEJS_PATH_DELIMITER_CHARACTER):
                os = os + c
                state = NORMAL
            elif (c == NODEJS_PATH_ESCAPE_CHARACTER):
                os = os + NODEJS_PATH_ESCAPE_CHARACTER + NODEJS_PATH_ESCAPE_CHARACTER
            else:
                os = os + NODEJS_PATH_ESCAPE_CHARACTER + c
            state = NORMAL;
    if (state == ESCAPE):
        os = os + NODEJS_PATH_ESCAPE_CHARACTER
    if len(os) > 0:
        path.append(os)
    return path;


class ModuleLoaderNodeJSPath:
    def __init__(self):
        #const char *nodejs_path = getenv("NODEJS_PATH");
        #if (nodejs_path != nullptr):
        #    mPath = split_path(std::string(nodejs_path));
        #else:
        self.mPath = ["."]


    def setNODEJS_PATH(self, path):
        self.mPath = split_path(path)

    def getNODEJS_PATH(self):
        return self.mPath

    def newModule(self, moduleName):
        module = NodeModule(self)
        return module

