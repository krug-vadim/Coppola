#ifndef WASHER_COMMANDS_H
#define WASHER_COMMANDS_H

typedef enum
{
	WASHER_ANSWER_TYPE_TACHO = 0xF1,
	WASHER_ANSWER_TYPE_SONAR = 0xF2,
	WASHER_ANSWER_TYPE_DOOR  = 0xF3,
	WASHER_ANSWER_TYPE_ID    = 0xF4
}
WASHER_ANSWER_TYPE_t;

typedef struct __attribute__((packed))
{
	uint8_t  type;
	uint32_t data;
}
WASHER_ANSWER_t;

BOOL_t WASHER_dumb_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_door_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_water_heater_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_main_valve_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_pre_valve_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_motor_dir_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_tacho_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_sonar_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_door_get_command(uint8_t *data, SIZE_t cnt);
BOOL_t WASHER_id_get_command(uint8_t *data, SIZE_t cnt);

#endif
