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
WASHER_doorlock_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_DOORLOCK] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_water_heater_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_WATER_HEATER] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_main_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_MAIN_VALVE] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_pre_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_PRE_VALVE] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_motor_dir_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_RELAY] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt)
{
	uint16_t temp;

	if ( cnt < sizeof(washer.motor_power) )
		return FALSE;

	temp = (((uint16_t)data[0]) << 0)
	     | (((uint16_t)data[1]) << 8);

	temp >>= 1;

	if ( temp == 0 )
		temp = 0;
	else if ( temp <= MOTOR_IMPULSE_LENGTH )
		temp = MOTOR_MAX_WAIT;
	else if ( temp < MOTOR_MAX_WAIT )
		temp = MOTOR_MAX_WAIT - temp;
	else
		temp = 1;

	washer.motor_power = temp;

	return TRUE;
}

BOOL_t
WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_WATER_PUMP] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_tacho_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_TACHO_ANSWER;
	answer.data = washer.tacho_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_sonar_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_SONAR_ANSWER;
	answer.data = washer.sonar_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_zerocross_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_ZEROCROSS_ANSWER;
	answer.data = washer.zerocross_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_temperature_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_TEMPERATURE_ANSWER;
	answer.data = washer.temperature;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_doorlock_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_DOORLOCK_ANSWER;
	answer.data = (washer.is_on[WASHER_PERIPHERAL_DOORLOCK]) ? 0x01 : 0xFF;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_id_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_ID_ANSWER;
	answer.data = washer.id;
	return washer_io.write((uint8_t *)&answer.type, sizeof(answer.type));
}

BOOL_t
WASHER_reset_command(uint8_t *data, SIZE_t cnt)
{
	/*asm("RST;");*/
	return TRUE;
}
