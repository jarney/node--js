
from nodejs.NodeModule import NodeModule
from nodejs.xml import Serializer

import os

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
        nodejs_path = os.environ.get("NODEJS_PATH")
        if not nodejs_path == None:
            self.mPath = split_path(nodejs_path)
        else:
            self.mPath = ["."]
        self.mLoadedModules = {}

    def setNODEJS_PATH(self, path):
        self.mPath = split_path(path)

    def getNODEJS_PATH(self):
        return self.mPath

    def loadModule(self, moduleName, err):
        if moduleName in self.mLoadedModules:
            return self.mLoadedModules[moduleName]

        loadedModule = None
        for directory in self.mPath:
            filename = os.path.join(directory, moduleName + ".xml");
            ser = Serializer.instance()
            loadedModule = NodeModule(self)
            try:
                with open(filename, "r") as stream:
                    ser.read(loadedModule, stream, err)
                    break
                loadedModule = None
            except FileNotFoundError as x:
                print(x)
                loadedModule = None

        self.mLoadedModules[moduleName] = loadedModule
        return loadedModule

    def newModule(self, moduleName):
        if moduleName in self.mLoadedModules:
            return self.mLoadedModules[moduleName]
        module = NodeModule(self)
        self.mLoadedModules[moduleName] = module
        return module

