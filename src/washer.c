#include "def.h"
#include "washer.h"

#include "washer_commands.h"
#include "washer_hw.h"

static IO_FUNC_ptr commands[WASHER_COMMANDS_COUNT];

WASHER_t washer;

IO_INTERFACE_t washer_io;

void
WASHER_init_washer(void)
{
	SIZE_t i;

	washer.id = WASHER_ID;
	washer.sonar_fq = 0;
	washer.tacho_fq = 0;
	washer.temperature = 0;

	washer.motor_power = 0;

	for(i = 0; i < WASHER_PERIPHERAL_COUNT; i++)
		washer.is_on[i] = FALSE;
}

void
WASHER_init_commands(void)
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
	commands[0x11] = WASHER_tacho_get_command;
	commands[0x12] = WASHER_sonar_get_command;
	commands[0x13] = WASHER_door_get_command;
	commands[0x14] = WASHER_id_get_command;
}

void
WASHER_init(void)
{
	WASHER_HW_init();

	WASHER_init_commands();
	WASHER_init_washer();
}

void
WASHER_startup(void)
{
	WASHER_HW_startup();
}

BOOL_t
WASHER_command(WASHER_COMMAND_t cmd, uint8_t *data, SIZE_t cnt)
{
	if ( data[0] < WASHER_COMMANDS_COUNT )
		return commands[data[0]](&data[1], cnt - 1);

	return TRUE;
}

BOOL_t
WASHER_parse(uint8_t *data, SIZE_t cnt)
{
	if ( cnt < 1 )
		return FALSE;

	return WASHER_command(data[0], &data[1], cnt - 1);
}

void
WASHER_process(void)
{
	WASHER_HW_process();
}

void
WASHER_one_second_tick(void)
{
	WASHER_HW_one_second_tick();
}

void
WASHER_io_set(IO_INTERFACE_t *io)
{
	washer_io = *io;
}