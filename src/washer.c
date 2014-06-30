#include "def.h"
#include "washer.h"
#include "washer_commands.h"

IO_FUNC_ptr commands[WASHER_COMMANDS_COUNT];

void
WASHER_init(void)
{
	SIZE_t i;
	for(i = 0; i < WASHER_COMMANDS_COUNT; i++)
		commands[i] = WASHER_dumb_command;

	commands[0x01] = WASHER_door_set_command;
	commands[0x02] = WASHER_water_heater_set_command;
	commands[0x03] = WASHER_main_valve_set_command;
	commands[0x04] = WASHER_pre_valve_set_command;
	commands[0x05] = WASHER_motor_dir_set_command;
	commands[0x06] = WASHER_motor_power_set_command;
	commands[0x07] = WASHER_water_pump_set_command;
	commands[0x11] = WASHER_taho_get_command;
	commands[0x12] = WASHER_sonar_get_command;
	commands[0x13] = WASHER_door_get_command;
	commands[0x14] = WASHER_id_get_command;
}

BOOL_t
WASHER_command(WASHER_COMMAND_t cmd, uint8_t *data, SIZE_t cnt)
{
	if ( data[0] < WASHER_COMMANDS_COUNT )
		return commands[data[0]](&data[1], cnt - 1);
	return FALSE;
}

BOOL_t
WASHER_parse(uint8_t *data, SIZE_t cnt)
{
	if ( cnt < 1 )
		return FALSE;

	return WASHER_command(data[0], &data[1], cnt - 1);
}
