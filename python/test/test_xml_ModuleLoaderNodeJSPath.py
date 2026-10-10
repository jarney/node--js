from nodejs.xml import ModuleLoaderNodeJSPath
from nodejs.xml import NODEJS_PATH_DELIMITER_CHARACTER
from nodejs.xml import NODEJS_PATH_DELIMITER_STRING
from nodejs.xml import NODEJS_PATH_ESCAPE_CHARACTER
from nodejs.xml import NODEJS_PATH_ESCAPE_STRING

def doTest(given_path, expected):
    moduleLoader = ModuleLoaderNodeJSPath()
    moduleLoader.setNODEJS_PATH(given_path)
    path = moduleLoader.getNODEJS_PATH()
    assert(path == expected)

def test_xml_ModuleLoaderNodeJSPath_path_splitting_ordinary():
    doTest(
	"one" + NODEJS_PATH_DELIMITER_STRING + 
	"two" + NODEJS_PATH_DELIMITER_STRING + 
	"three" + NODEJS_PATH_DELIMITER_STRING + 
	"four",
	["one", "two", "three", "four"]
	)

def test_xml_ModuleLoaderNodeJSPath_path_splitting_escape_semicolon():
    doTest(
	"one" + NODEJS_PATH_DELIMITER_STRING + 
	"two" + NODEJS_PATH_DELIMITER_STRING + 
	"thre" + NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_DELIMITER_STRING + "e" + NODEJS_PATH_DELIMITER_STRING + 
	"four",
	["one", "two", "thre" + NODEJS_PATH_DELIMITER_STRING + "e", "four"]
        )


def test_xml_ModuleLoaderNodeJSPath_path_splitting_Just_a_stray_escape():
    doTest(NODEJS_PATH_ESCAPE_STRING, [NODEJS_PATH_ESCAPE_STRING])

def test_xml_ModuleLoaderNodeJSPath_path_splitting_double_escape_backslash():
    doTest(NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING, [NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING])
    doTest(
	"a" + NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING + "b",
	["a" + NODEJS_PATH_ESCAPE_STRING + NODEJS_PATH_ESCAPE_STRING + "b"],
    )

def test_xml_ModuleLoaderNodeJSPath_path_splitting_escape_other_things():
    doTest(
	NODEJS_PATH_ESCAPE_STRING + "a",
	[
	    NODEJS_PATH_ESCAPE_STRING + "a"
	]
	)
    doTest(
	NODEJS_PATH_ESCAPE_STRING + "a" +
	NODEJS_PATH_DELIMITER_STRING +
	NODEJS_PATH_ESCAPE_STRING + "c",
	[
	    NODEJS_PATH_ESCAPE_STRING + "a",
	    NODEJS_PATH_ESCAPE_STRING + "c"
	]
	)

def test_xml_ModuleLoaderNodeJSPath_path_splitting_empty():
    doTest("", [])

def test_xml_ModuleLoaderNodeJSPath_new_module_is_same():
    loader = ModuleLoaderNodeJSPath()

    newModule = loader.newModule("unique-module-name")

    nextModule = loader.newModule("unique-module-name")

    # Module names are unique.  If we ask for a new one, we
    # should expect to get the same one.
    assert(newModule == nextModule)

