#include <iostream>
#include <catch2/catch_session.hpp>
#include <fstream>
#include <filesystem>

namespace NodeJS {
    namespace test {
	std::string test_data_directory;

	std::string testDirectory(void)
	{
	    return test_data_directory;
	}
	
	std::string testFilename(std::string basename)
	{
	    return (std::filesystem::path(test_data_directory) / basename).string();
	}
	
    }
}

using namespace NodeJS::test;
using namespace Catch::Clara;

int main( int argc, char* argv[] )
{
  Catch::Session session; // There must be exactly one instance

  // Build a new parser on top of Catch2's
  auto cli
    = session.cli()           // Get Catch2's command line parser
    | Opt( test_data_directory, "data-directory" ) // bind variable to a new option, with a hint string
      ["--data-directory"]
        ("Directory to use for test data files.");        // description string for the help output
   
  // Now pass the new composite back to Catch2 so it uses that
  session.cli( cli );

  // Let Catch2 (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) // Indicates a command line error
      return returnCode;

  return session.run();
}

