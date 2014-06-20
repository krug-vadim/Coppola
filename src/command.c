#include "def.h"
#include "command.h"



BOOL_t
COMMAND_parse(uint8_t *data, SIZE_t cnt)
{
	if ( cnt < 1 )
		return FALSE;

	return COMMAND_execute(data[0]);
}
