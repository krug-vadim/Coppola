#include "def.h"
#include "washer.h"
#include "washer_commands.h"

#include "washer_hw.h"

extern WASHER_t washer;
extern IO_INTERFACE_t washer_io;

static WASHER_ANSWER_t answer;

BOOL_t
WASHER_dumb_command(uint8_t *data, SIZE_t cnt)
{
	washer_io.log("DUMB COMMAND!\r\n");
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
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_RELAY] = data[0];

	return TRUE;
}

BOOL_t
WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt < sizeof(washer.motor_power) )
		return FALSE;

	washer.motor_power = (((uint16_t)data[0]) << 0)
	                   | (((uint16_t)data[1]) << 8);

	answer.type = WASHER_ANSWER_TYPE_TACHO;
	answer.data = washer.motor_power;
	washer_io.write((uint8_t *)&answer, sizeof(answer));

	return TRUE;
}

BOOL_t
WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_WATER_PUMP] = data[0];

	return TRUE;
}

BOOL_t
WASHER_tacho_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_ANSWER_TYPE_TACHO;
	answer.data = washer.tacho_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_sonar_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_ANSWER_TYPE_SONAR;
	answer.data = washer.sonar_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_door_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_ANSWER_TYPE_DOOR;
	answer.data = washer.is_on[WASHER_PERIPHERAL_DOOR];
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_id_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_ANSWER_TYPE_ID;
	answer.data = washer.id;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}
