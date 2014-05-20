
typedef enum
{
	COMMAND_DOOR_SET         = 0x01,
	COMMAND_WATER_HEATER_SET = 0x02,
	COMMAND_MAIN_VALVE_SET   = 0x03,
	COMMAND_PRE_VALVE_SET    = 0x04,
	COMMAND_MOTOR_DIR_SET    = 0x05,
	COMMAND_MOTOR_POWER_SET  = 0x06,
	COMMAND_WATER_PUMP_SET   = 0x07,
	COMMAND_TAHO_GET         = 0x11,
	COMMAND_SONAR_GET        = 0x12,
	COMMAND_DOOR_GET         = 0x13,
	COMMAND_ID_GET           = 0x14,
}
COMMAND_t;

void
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
}