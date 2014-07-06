#include "def.h"

#include "washer.h"
#include "washer_hw.h"

PIN_REGS_t pins_hw[PIN_COUNT] =
{
	/* PORT 1 */
	{ &P1OUT, &P1DIR, &P1IN, BIT0}, /* P1_0 */
	{ &P1OUT, &P1DIR, &P1IN, BIT1}, /* P1_1 */
	{ &P1OUT, &P1DIR, &P1IN, BIT2}, /* P1_2 */
	{ &P1OUT, &P1DIR, &P1IN, BIT3}, /* P1_3 */
	{ &P1OUT, &P1DIR, &P1IN, BIT4}, /* P1_4 */
	{ &P1OUT, &P1DIR, &P1IN, BIT5}, /* P1_5 */
	{ &P1OUT, &P1DIR, &P1IN, BIT6}, /* P1_6 */
	{ &P1OUT, &P1DIR, &P1IN, BIT7}, /* P1_7 */
	/* PORT 2 */
	{ &P2OUT, &P2DIR, &P2IN, BIT0}, /* P2_0 */
	{ &P2OUT, &P2DIR, &P2IN, BIT1}, /* P2_1 */
	{ &P2OUT, &P2DIR, &P2IN, BIT2}, /* P2_2 */
	{ &P2OUT, &P2DIR, &P2IN, BIT3}, /* P2_3 */
	{ &P2OUT, &P2DIR, &P2IN, BIT4}, /* P2_4 */
	{ &P2OUT, &P2DIR, &P2IN, BIT5}, /* P2_5 */
	{ &P2OUT, &P2DIR, &P2IN, BIT6}, /* P2_6 */
	{ &P2OUT, &P2DIR, &P2IN, BIT7}  /* P2_7 */
};

#define PIN_MODE_INPUT(p)  (*(pins_hw[p].dir) |=  (1 << pins_hw[p].pin))
#define PIN_MODE_OUTPUT(p) (*(pins_hw[p].dir) &= ~(1 << pins_hw[p].pin))

#define PIN_SET_LOW(p)     (*(pins_hw[p].out) &= ~(1 << pins_hw[p].pin))
#define PIN_SET_HIGH(p)    (*(pins_hw[p].out) |=  (1 << pins_hw[p].pin))
#define PIN_TOGGLE(p)      (*(pins_hw[p].out) ^=  (1 << pins_hw[p].pin))
#define PIN_VALUE(p)       ((*(pins_hw[p].in)) & (1 << pins_hw[p].pin))

static WASHER_VALUE_t sonar_fq_current;
static WASHER_VALUE_t tacho_fq_current;

static uint8_t sonar_last_state;
static uint8_t tacho_last_state;

static uint8_t zerocross_last_state;

static BOOL_t  relay_is_on;
static BOOL_t  need_toggle_relay;

extern WASHER_t washer;
extern IO_INTERFACE_t washer_io;

void delay_us(uint16_t time)
{
	for(; time != 0; time--)
		asm("NOP");/*NOP();__delay_cycles(16);*/
}

void
WASHER_HW_init(void)
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
WASHER_HW_startup(void)
{
	sonar_fq_current = 0;
	tacho_fq_current = 0;

	sonar_last_state = 0;
	tacho_last_state = 0;

	zerocross_last_state = 0;

	relay_is_on = FALSE;
	need_toggle_relay = FALSE;
}

void
WASHER_HW_process(void)
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
WASHER_HW_one_second_tick(void)
{
	washer.sonar_fq = sonar_fq_current;
	sonar_fq_current = 0;

	washer.tacho_fq = tacho_fq_current;
	tacho_fq_current = 0;
}
