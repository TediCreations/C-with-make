#include "console/console.h"

int main( void )
{
	console_t cmd = console_build();
	cmd.puts( "Give input:\n" );
	char* input = cmd.gets( &cmd );
	cmd.puts( "You entered: " );
	cmd.puts( input );
	cmd.puts( "\n" );

	return 0;
}
