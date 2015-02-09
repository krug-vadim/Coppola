#ifndef WASHER_COMMANDS_H
#define WASHER_COMMANDS_H

typedef enum
{
	WASHER_COMMAND_DOORLOCK_SET     = 0x01,
	WASHER_COMMAND_WATER_HEATER_SET = 0x02,
	WASHER_COMMAND_MAIN_VALVE_SET   = 0x03,
	WASHER_COMMAND_PRE_VALVE_SET    = 0x04,
	WASHER_COMMAND_MOTOR_DIR_SET    = 0x05,
	WASHER_COMMAND_MOTOR_POWER_SET  = 0x06,
	WASHER_COMMAND_WATER_PUMP_SET   = 0x07,
	WASHER_COMMAND_TACHO_GET        = 0x11,
	WASHER_COMMAND_SONAR_GET        = 0x12,
	WASHER_COMMAND_DOORLOCK_GET     = 0x13,
	WASHER_COMMAND_ID_GET           = 0x14,
	WASHER_COMMAND_RESET            = 0x15,
	WASHER_COMMAND_TEMPERATURE_GET  = 0x16,
	WASHER_COMMAND_ZEROCROSS_GET    = 0x17,
	WASHER_COMMAND_PWM_OFFSET_SET   = 0x20,
	WASHER_COMMAND_BOOT_STRAP_LOADER= 0x21,

	WASHER_COMMAND_TACHO_ANSWER       = 0xF1,
	WASHER_COMMAND_SONAR_ANSWER       = 0xF2,
	WASHER_COMMAND_DOORLOCK_ANSWER    = 0xF3,
	WASHER_COMMAND_ID_ANSWER          = 0xF4,
	WASHER_COMMAND_TEMPERATURE_ANSWER = 0xF5,
	WASHER_COMMAND_ZEROCROSS_ANSWER   = 0xF6
}
WASHER_COMMAND_t;

typedef struct __attribute__((packed))
{
	uint8_t  type;
	uint32_t data;
}
WASHER_ANSWER_t;

BOOL_t WASHER_dumb_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_doorlock_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_water_heater_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_main_valve_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_pre_valve_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_motor_dir_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_pwm_offset_set_command(uint8_t *data, SIZE_t cnt);

BOOL_t WASHER_tacho_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_sonar_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_temperature_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_zerocross_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_doorlock_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_id_get_command(uint8_t *data, SIZE_t cnt);

BOOL_t WASHER_reset_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_boot_strap_loader_command(uint8_t *data, SIZE_t cnt);

#endif
