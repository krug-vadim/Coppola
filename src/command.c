#include "def.h"
#include "command.h"

BOOL_t
COMMAND_execute(COMMAND_t cmd)
{
	switch ( cmd )
	{
		case COMMAND_DOOR_SET:
			WASHER_peripheral_set(WASHER_PERIPHERAL_DOOR, state);
			break;

		case COMMAND_WATER_HEATER_SET:
			WASHER_peripheral_set(WASHER_PERIPHERAL_WATER_HEATER, state);
			break;

		case COMMAND_MAIN_VALVE_SET:
			WASHER_peripheral_set(WASHER_PERIPHERAL_MAIN_VALVE, state);
			break;

		case COMMAND_PRE_VALVE_SET:
			WASHER_peripheral_set(WASHER_PERIPHERAL_PRE_VALVE, state);
			break;

		case COMMAND_WATER_PUMP_SET:
			WASHER_peripheral_set(WASHER_PERIPHERAL_WATER_PUMP, state);
			break;

		case COMMAND_MOTOR_DIR_SET:
			break;

		case COMMAND_MOTOR_POWER_SET:
			break;

		case COMMAND_TAHO_GET:
			break;

		case COMMAND_SONAR_GET:
			break;

		case COMMAND_DOOR_GET:
			break;

		case COMMAND_ID_GET:
			break;

		default:
			// XXX: error handling
			break;
	}

	return TRUE;
}

BOOL_t
COMMAND_parse(uint8_t *data, SIZE_t cnt)
{
	if ( cnt < 1 )
		return FALSE;

	return COMMAND_execute(data[0]);
}
