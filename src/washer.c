#include "def.h"
#include "washer.h"

BOOL_t
WASHER_command(WASHER_COMMAND_t cmd)
{
	switch ( cmd )
	{
		case WASHER_COMMAND_DOOR_SET:
			/*WASHER_peripheral_set(WASHER_PERIPHERAL_DOOR, state);*/
			break;

		case WASHER_COMMAND_WATER_HEATER_SET:
			/*WASHER_peripheral_set(WASHER_PERIPHERAL_WATER_HEATER, state);*/
			break;

		case WASHER_COMMAND_MAIN_VALVE_SET:
			/*WASHER_peripheral_set(WASHER_PERIPHERAL_MAIN_VALVE, state);*/
			break;

		case WASHER_COMMAND_PRE_VALVE_SET:
			/*WASHER_peripheral_set(WASHER_PERIPHERAL_PRE_VALVE, state);*/
			break;

		case WASHER_COMMAND_WATER_PUMP_SET:
			/*WASHER_peripheral_set(WASHER_PERIPHERAL_WATER_PUMP, state);*/
			break;

		case WASHER_COMMAND_MOTOR_DIR_SET:
			break;

		case WASHER_COMMAND_MOTOR_POWER_SET:
			break;

		case WASHER_COMMAND_TAHO_GET:
			break;

		case WASHER_COMMAND_SONAR_GET:
			break;

		case WASHER_COMMAND_DOOR_GET:
			break;

		case WASHER_COMMAND_ID_GET:
			break;

		default:
			return FALSE;
			/* XXX: error handling*/
			break;
	}

	return TRUE;
}

BOOL_t
WASHER_parse(uint8_t *data, SIZE_t cnt)
{
	if ( cnt < 1 )
		return FALSE;

	return WASHER_command(data[0]);
}
