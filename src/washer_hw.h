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

#define HEATER_TEMP_ANALOG (INCH_5)

#define MOTOR_IMPULSE_LENGTH (2U)   /* 2 * 80uS = 160 uS */
#define MOTOR_POWER_MAX      (255U)
#define MOTOR_POWER_50HZ     (117U)
#define MOTOR_MAX_WAIT       (116U) /* 250 * 80uS = 20 mS */

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

typedef enum
{
	MOTOR_STATE_OFF     = 0x00,
	MOTOR_STATE_WAIT    = 0x01,
	MOTOR_STATE_IMPULSE = 0x02
}
MOTOR_STATE_t;

#define PIN_MODE_OUTPUT(p) (p < 8) ? (P1DIR |=  (1 << p)) : (P2DIR |=  (1 << (p - 8)))
#define PIN_MODE_INPUT(p)  (p < 8) ? (P1DIR &= ~(1 << p)) : (P2DIR &= ~(1 << (p - 8)))

#define PIN_SET_LOW(p)     (p < 8) ? (P1OUT &= ~(1 << p)) : (P2OUT &= ~(1 << (p - 8)))
#define PIN_SET_HIGH(p)    (p < 8) ? (P1OUT |=  (1 << p)) : (P2OUT |=  (1 << (p - 8)))
#define PIN_TOGGLE(p)      (p < 8) ? (P1OUT ^=  (1 << p)) : (P2OUT ^=  (1 << (p - 8)))

#define PIN_VALUE(p)       ( (p < 8) ? (P1IN & (1 << p)) : (P2IN & (1 << (p - 8))) )

void WASHER_HW_init(void);
void WASHER_HW_startup(void);
void WASHER_HW_process(void);

void WASHER_HW_one_second_tick(void);

#endif
