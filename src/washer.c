#include "def.h"
#include "washer.h"

#include "washer_commands.h"
#include "washer_hw.h"

WASHER_t washer;

IO_INTERFACE_t washer_io;

void
WASHER_init_washer(void)
{
	uint8_t i;

	washer.id = WASHER_ID;
	washer.sonar_fq = 0;
	washer.tacho_fq = 0;
	washer.temperature = 0;

	washer.motor_power = 0;
	washer.pwm_offset = 0;

	for(i = 0; i < WASHER_PERIPHERAL_COUNT; i++)
		washer.is_on[i] = FALSE;
}

void
WASHER_init(void)
{
	WASHER_HW_init();

	WASHER_init_washer();
}

void
WASHER_startup(void)
{
	WASHER_HW_startup();
}

BOOL_t
WASHER_parse(uint8_t *data, SIZE_t cnt)
{
	switch ( data[0] )
	{
		case WASHER_COMMAND_DOORLOCK_SET:
			return WASHER_doorlock_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_WATER_HEATER_SET:
			return WASHER_water_heater_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_MAIN_VALVE_SET:
			return WASHER_main_valve_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_PRE_VALVE_SET:
			return WASHER_pre_valve_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_MOTOR_DIR_SET:
			return WASHER_motor_dir_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_MOTOR_POWER_SET:
			return WASHER_motor_power_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_WATER_PUMP_SET:
			return WASHER_water_pump_set_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_TACHO_GET:
			return WASHER_tacho_get_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_SONAR_GET:
			return WASHER_sonar_get_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_DOORLOCK_GET:
			return WASHER_doorlock_get_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_ID_GET:
			return WASHER_id_get_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_TEMPERATURE_GET:
			return WASHER_temperature_get_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_RESET:
			return WASHER_reset_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_ZEROCROSS_GET:
			return WASHER_zerocross_get_command(&data[1], cnt - 1);
			break;

		case WASHER_COMMAND_PWM_OFFSET_SET:
			return WASHER_pwm_offset_set_command(&data[1], cnt - 1);
			break;

		default:
			return FALSE;
	}
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
