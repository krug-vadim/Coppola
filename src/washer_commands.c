#include "def.h"
#include "washer_commands.h"

#include "washer_hw.h"

BOOL_t
WASHER_dumb_command(uint8_t *data, SIZE_t cnt)
{
	/* XXX: io.log("DUMB COMMAND!\r\n"); */
	return FALSE;
}

BOOL_t
WASHER_door_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_water_heater_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_main_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_pre_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_motor_dir_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_taho_get_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_sonar_get_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_door_get_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_id_get_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}