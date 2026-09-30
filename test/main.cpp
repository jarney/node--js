#include <iostream>
#include <catch2/catch_session.hpp>

int main( int argc, char* argv[] )
{
  Catch::Session session; // There must be exactly one instance

  std::string data_directory(".");

  // Build a new parser on top of Catch2's
  using namespace Catch::Clara;
  auto cli
    = session.cli()           // Get Catch2's command line parser
    | Arg( data_directory, "data-directory" ) // bind variable to a new option, with a hint string
        ("Directory to use for test data files.");        // description string for the help output

  // Now pass the new composite back to Catch2 so it uses that
  session.cli( cli );

  // Let Catch2 (using Clara) parse the command line
  int returnCode = session.applyCommandLine( argc, argv );
  if( returnCode != 0 ) // Indicates a command line error
      return returnCode;

  std::cout << data_directory << std::endl;

  return session.run();
}
