#include "def.h"
#include "washer.h"
#include "washer_commands.h"

#include "washer_hw.h"

static IO_FUNC_ptr commands[WASHER_COMMANDS_COUNT];

static WASHER_VALUE_t sonar_fq_current;
static WASHER_VALUE_t tacho_fq_current;

static uint8_t sonar_last_state;
static uint8_t tacho_last_state;

static uint8_t zerocross_last_state;

static BOOL_t  relay_is_on;
static BOOL_t  need_toggle_relay;

WASHER_t washer;

IO_INTERFACE_t washer_io;

void
WASHER_init_pins(void)
{
	PIN_MODE_INPUT(ZEROCROSS_PIN);
	PIN_MODE_INPUT(HEATER_TEMP_PIN);

	PIN_MODE_INPUT(SONAR_SENSE_PIN);
	PIN_MODE_INPUT(TACHO_SENSE_PIN);

	/* set all outputs to LOW */

	PIN_SET_LOW(RELAY_PIN);
	PIN_SET_LOW(DOORLOCK_PIN);
	PIN_SET_LOW(MOTOR_PIN);
	PIN_SET_LOW(HEATER_PIN);

	PIN_SET_LOW(DRAIN_PUMP_PIN);
	PIN_SET_LOW(VALVE_MAIN_PIN);
	PIN_SET_LOW(VALVE_PRE_PIN);


	/* and make them output */

	PIN_MODE_OUTPUT(RELAY_PIN);
	PIN_MODE_OUTPUT(DOORLOCK_PIN);
	PIN_MODE_OUTPUT(MOTOR_PIN);
	PIN_MODE_OUTPUT(HEATER_PIN);

	PIN_MODE_OUTPUT(DRAIN_PUMP_PIN);
	PIN_MODE_OUTPUT(VALVE_MAIN_PIN);
	PIN_MODE_OUTPUT(VALVE_PRE_PIN);
}

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
	WASHER_init_pins();
	WASHER_init_commands();
	WASHER_init_washer();

	sonar_fq_current = 0;
	tacho_fq_current = 0;

	sonar_last_state = 0;
	tacho_last_state = 0;

	zerocross_last_state = 0;

	relay_is_on = FALSE;
	need_toggle_relay = FALSE;
}

void
WASHER_startup(void)
{
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

void delay_us(uint16_t time)
{
	for(; time != 0; time--)
		asm("NOP");/*NOP();__delay_cycles(16);*/
}

void
WASHER_process(void)
{
	uint8_t pin_state;

	pin_state = PIN_VALUE(SONAR_SENSE_PIN);
	if ( sonar_last_state != pin_state )
	{
		sonar_fq_current++;
		sonar_last_state = pin_state;
	}

	pin_state = PIN_VALUE(TACHO_SENSE_PIN);
	if ( tacho_last_state != pin_state )
	{
		tacho_fq_current++;
		tacho_last_state = pin_state;
	}

	pin_state = PIN_VALUE(ZEROCROSS_PIN);
	if( zerocross_last_state != pin_state )
	{
		zerocross_last_state = pin_state;

		/*if ( washer.motor_power < 0 )
			PIN_SET_HIGH(RELAY_PIN);
		else
			PIN_SET_LOW(RELAY_PIN);*/

		if ( washer.motor_power )
		{
			/* XXX: calculate */
			delay_us(washer.motor_power*40);

			PIN_SET_HIGH(MOTOR_PIN);
			delay_us(100); /* XXX: why 100? dunno */
			PIN_SET_LOW(MOTOR_PIN);
		}
	}
}

void
WASHER_one_second_tick(void)
{
	washer.sonar_fq = sonar_fq_current;
	sonar_fq_current = 0;

	washer.tacho_fq = tacho_fq_current;
	tacho_fq_current = 0;
}

void
WASHER_io_set(IO_INTERFACE_t *io)
{
	washer_io = *io;
}