#ifndef WASHER_H
#define WASHER_H

typedef enum
{
	WASHER_PERIPHERAL_DOOR = 0x00,

	WASHER_PERIPHERAL_WATER_PUMP = 0x01,
	WASHER_PERIPHERAL_WATER_HEATER = 0x02,

	WASHER_PERIPHERAL_PRE_VALVE = 0x03,
	WASHER_PERIPHERAL_MAIN_VALVE = 0x04,

	WASHER_PERIPHERAL_RELAY = 0x05
}
WASHER_PERIPHERAL_t;

typedef BOOL_t WASHER_PERIPHERAL_STATE;

#define WASHER_PERIPHERAL_COUNT (6U)

typedef enum
{
	WASHER_COMMAND_DOOR_SET         = 0x01,
	WASHER_COMMAND_WATER_HEATER_SET = 0x02,
	WASHER_COMMAND_MAIN_VALVE_SET   = 0x03,
	WASHER_COMMAND_PRE_VALVE_SET    = 0x04,
	WASHER_COMMAND_MOTOR_DIR_SET    = 0x05,
	WASHER_COMMAND_MOTOR_POWER_SET  = 0x06,
	WASHER_COMMAND_WATER_PUMP_SET   = 0x07,
	WASHER_COMMAND_TACHO_GET        = 0x11,
	WASHER_COMMAND_SONAR_GET        = 0x12,
	WASHER_COMMAND_DOOR_GET         = 0x13,
	WASHER_COMMAND_ID_GET           = 0x14
}
WASHER_COMMAND_t;

#define WASHER_COMMANDS_COUNT (0x15U)

typedef int16_t  WASHER_POWER_t;
typedef uint16_t WASHER_VALUE_t;
typedef uint16_t WASHER_ID_t;

#define WASHER_ID 0x01

typedef struct
{
	WASHER_ID_t    id;

	WASHER_VALUE_t sonar_fq;
	WASHER_VALUE_t tacho_fq;
	WASHER_VALUE_t temperature;

	WASHER_POWER_t motor_power;

	WASHER_PERIPHERAL_STATE is_on[WASHER_PERIPHERAL_COUNT];
}
WASHER_t;

void WASHER_init(void);
void WASHER_startup(void);
void WASHER_process(void);

void WASHER_one_second_tick(void);

BOOL_t WASHER_parse(uint8_t *data, SIZE_t cnt);

void WASHER_io_set(IO_INTERFACE_t *io);

#endif
