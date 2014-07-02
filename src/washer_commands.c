#include "def.h"
#include "washer.h"
#include "washer_commands.h"

#include "washer_hw.h"

extern WASHER_t washer;

BOOL_t
WASHER_dumb_command(uint8_t *data, SIZE_t cnt)
{
	/* XXX: io.log("DUMB COMMAND!\r\n"); */
	return FALSE;
}

BOOL_t
WASHER_door_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_DOOR] = data[0];

	return TRUE;
}

BOOL_t
WASHER_water_heater_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_WATER_HEATER] = data[0];

	return TRUE;
}

BOOL_t
WASHER_main_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_MAIN_VALVE] = data[0];

	return TRUE;
}

BOOL_t
WASHER_pre_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_PRE_VALVE] = data[0];

	return TRUE;
}

BOOL_t
WASHER_motor_dir_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.motor_power = data[0];

	return TRUE;
}

BOOL_t
WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt)
{
	return FALSE;
}

BOOL_t
WASHER_tacho_get_command(uint8_t *data, SIZE_t cnt)
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