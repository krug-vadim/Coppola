#include "def.h"
#include "washer.h"
#include "washer_commands.h"

static IO_FUNC_ptr commands[WASHER_COMMANDS_COUNT];

static WASHER_t washer;

static WASHER_VALUE_t sonar_fq_current;
static WASHER_VALUE_t taho_fq_current;

void
WASHER_init_pins(void)
{
	/*relay_state = LOW;
	need_switch_relay = 0;

	pinMode(ZEROCROSS_PIN, INPUT);
	pinMode(HEATER_TEMP_PIN, INPUT);

	pinMode(SONAR_SENSE_PIN, INPUT_PULLUP);
	pinMode(TACHO_SENSE_PIN, INPUT_PULLUP);

	// set all outputs to LOW

	digitalWrite(RELAY_PIN,  LOW);
	digitalWrite(DOORLOCK_PIN,  LOW);
	digitalWrite(MOTOR_PIN,  LOW);
	digitalWrite(HEATER_PIN,  LOW);

	digitalWrite(UNKNOWN1_PIN,  LOW);
	digitalWrite(UNKNOWN2_PIN,  LOW);
	digitalWrite(UNKNOWN3_PIN,  LOW);

	// and make them output

	pinMode(RELAY_PIN, OUTPUT);
	pinMode(DOORLOCK_PIN, OUTPUT);
	pinMode(MOTOR_PIN, OUTPUT);
	pinMode(HEATER_PIN, OUTPUT);

	pinMode(UNKNOWN1_PIN, OUTPUT);
	pinMode(UNKNOWN2_PIN, OUTPUT);
	pinMode(UNKNOWN3_PIN, OUTPUT);*/
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
	commands[0x11] = WASHER_taho_get_command;
	commands[0x12] = WASHER_sonar_get_command;
	commands[0x13] = WASHER_door_get_command;
	commands[0x14] = WASHER_id_get_command;
}

void
WASHER_init_washer(void)
{
	SIZE_t i;

	washer.sonar_fq = 0;
	washer.taho_fq = 0;
	washer.temperature = 0;

	washer.motor_power = 0;

	for(i = 0; i < WASHER_PERIPHERAL_COUNT; i++)
		washer.is_on[i] = FALSE;
}

void
WASHER_init(void)
{
	WASHER_init_pins();
	WASHER_init_commands();

	WASHER_init_washer();

	sonar_fq_current = 0;
	taho_fq_current = 0;
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
	return FALSE;
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
/*	pinState = digitalRead(SONAR_SENSE_PIN);
	if ( pinState != _sonarLastState )
	{
	_sonarCounter++;
	_sonarLastState = pinState;
	}

	pinState = digitalRead(TACHO_SENSE_PIN);
	if ( pinState != _tachoLastState )
	{
	_tachoCounter++;
	_tachoLastState = pinState;
	}

	_currentZeroCrossState = digitalRead(ZEROCROSS_PIN);

	if(_lastZeroCrossState != _currentZeroCrossState)
	{
		_lastZeroCrossState = _currentZeroCrossState;

		if ( need_switch_relay )
		{
			relay_state = (relay_state == HIGH) ? LOW : HIGH;
			digitalWrite(RELAY_PIN,  relay_state);
			need_switch_relay = 0x00;
		}

		if (_delay)
		{
			delay_us(_delay*40);

			digitalWrite(MOTOR_PIN,  HIGH);
			//digitalWrite(P1_0,  HIGH);

			//delay_us(_delay * 4);
			delay_us(100);

			digitalWrite(MOTOR_PIN,  LOW);
			//digitalWrite(P1_0,  LOW);
		}
	}*/
}

void
WASHER_one_second_tick(void)
{
	washer.sonar_fq = sonar_fq_current;
	sonar_fq_current = 0;

	washer.taho_fq = taho_fq_current;
	taho_fq_current = 0;
}