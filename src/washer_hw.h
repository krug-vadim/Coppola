#ifndef WASHER_HW_H
#define WASHER_HW_H

#define ZEROCROSS_PIN   (P2_0)

#define RELAY_PIN       (P2_1)
#define MOTOR_PIN       (P2_6)

#define HEATER_PIN      (P2_7)
#define DOORLOCK_PIN    (P1_7)
#define VALVE_MAIN_PIN  (P1_6)
#define DRAIN_PUMP_PIN  (P2_5)
#define VALVE_PRE_PIN   (P2_4)

#define SONAR_SENSE_PIN (P1_3)
#define TACHO_SENSE_PIN (P1_0)
#define HEATER_TEMP_PIN (P1_5)

#define HEATER_TEMP_ANALOG A5

typedef enum
{
	/* port one */
	P1_0 =  0,
	P1_1 =  1,
	P1_2 =  2,
	P1_3 =  3,
	P1_4 =  4,
	P1_5 =  5,
	P1_6 =  6,
	P1_7 =  7,

	/* port two */
	P2_0 =  8,
	P2_1 =  9,
	P2_2 = 10,
	P2_3 = 11,
	P2_4 = 12,
	P2_5 = 13,
	P2_6 = 14,
	P2_7 = 15
}
PIN_t;

#define PIN_COUNT 16

typedef enum
{
	PIN_STATE_LOW  = 0x00,
	PIN_STATE_HIGH = 0xFF
}
PIN_STATE_t;

typedef struct
{
	volatile       uint8_t *out;
	volatile       uint8_t *dir;
	volatile const uint8_t *in;
	volatile       uint8_t pin;
}
PIN_REGS_t;

void WASHER_HW_init(void);
void WASHER_HW_startup(void);
void WASHER_HW_process(void);

void WASHER_HW_one_second_tick(void);

#endif