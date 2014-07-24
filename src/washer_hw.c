#include "def.h"

#include "washer.h"
#include "washer_hw.h"

static WASHER_VALUE_t sonar_fq_current;
static WASHER_VALUE_t tacho_fq_current;
static WASHER_VALUE_t zerocross_fq_current;

static uint8_t sonar_last_state;
static uint8_t tacho_last_state;
static uint8_t zerocross_last_state;

static BOOL_t  relay_is_on;
static BOOL_t  need_toggle_relay;

extern WASHER_t washer;
extern IO_INTERFACE_t washer_io;

static MOTOR_STATE_t motor_state;

void delay_us(uint16_t time)
{
	for(; time != 0; time--)
		asm("NOP");/*NOP();__delay_cycles(16);*/
}

void
WASHER_HW_adc_init(void)
{
	ADC10CTL0 = SREF_0
	          | ADC10SHT_3
	          | ADC10SR
	          | MSC
	          | REF2_5V
	          | REFON
	          | ADC10ON
	          | ADC10IE
	          ;

	ADC10CTL1 = HEATER_TEMP_ANALOG
	          | SHS_0
	          | ADC10DIV_7
	          | ADC10SSEL_0
	          | CONSEQ_2
	          ;

	/* XXX: change to MACROSS selectable */
	ADC10AE0 |= BIT5;
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

	WASHER_HW_adc_init();

	/* Timer A1 ACLK/1, UP 1 second */
	TA1CCR0 = 0;
	TA1CCTL0 = 0x10;
	TA1CTL = TASSEL_1
	       | ID_0
	       | MC_1
	       ;

	motor_state = MOTOR_STATE_OFF;
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

	/* adc startup */
	ADC10CTL0 |= ENC + ADC10SC;
}

void
WASHER_HW_peripherals_set(void)
{
	if ( washer.is_on[WASHER_PERIPHERAL_DOORLOCK] )
		PIN_SET_HIGH(DOORLOCK_PIN);
	else
		PIN_SET_LOW(DOORLOCK_PIN);

	if ( washer.is_on[WASHER_PERIPHERAL_WATER_PUMP] )
		PIN_SET_HIGH(DRAIN_PUMP_PIN);
	else
		PIN_SET_LOW(DRAIN_PUMP_PIN);

	if ( washer.is_on[WASHER_PERIPHERAL_WATER_HEATER] )
		PIN_SET_HIGH(HEATER_PIN);
	else
		PIN_SET_LOW(HEATER_PIN);

	if ( washer.is_on[WASHER_PERIPHERAL_PRE_VALVE] )
		PIN_SET_HIGH(VALVE_PRE_PIN);
	else
		PIN_SET_LOW(VALVE_PRE_PIN);

	if ( washer.is_on[WASHER_PERIPHERAL_MAIN_VALVE] )
		PIN_SET_HIGH(VALVE_MAIN_PIN);
	else
		PIN_SET_LOW(VALVE_MAIN_PIN);

	if ( washer.is_on[WASHER_PERIPHERAL_RELAY] )
		PIN_SET_HIGH(RELAY_PIN);
	else
		PIN_SET_LOW(RELAY_PIN);
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
		zerocross_fq_current++;
		zerocross_last_state = pin_state;

		if ( washer.motor_power && motor_state == MOTOR_STATE_OFF )
		{
			if ( washer.motor_power >= MOTOR_POWER_MAX )
			{
				motor_state = MOTOR_STATE_IMPULSE;
				TA1CCR0 = MOTOR_IMPULSE_LENGTH;
			}
			else
			{
				motor_state = MOTOR_STATE_WAIT;
				TA1CCR0 = washer.motor_power;
			}
		}
	}

	WASHER_HW_peripherals_set();
}

void
WASHER_HW_one_second_tick(void)
{
	washer.sonar_fq = sonar_fq_current;
	sonar_fq_current = 0;

	washer.tacho_fq = tacho_fq_current;
	tacho_fq_current = 0;

	washer.zerocross_fq = zerocross_fq_current;
	zerocross_fq_current = 0;
}

/* TIMERA1 interrupt service routine */
void __attribute__((interrupt(TIMER1_A0_VECTOR))) zerocrossing(void)
{
	switch ( motor_state )
	{
		case MOTOR_STATE_OFF:
			TA1CCR0 = 0;
			PIN_SET_LOW(MOTOR_PIN);
			motor_state = MOTOR_STATE_OFF;
			break;

		case MOTOR_STATE_WAIT:
			TA1CCR0 = MOTOR_IMPULSE_LENGTH;
			PIN_SET_HIGH(MOTOR_PIN);
			motor_state = MOTOR_STATE_IMPULSE;
			break;

		case MOTOR_STATE_IMPULSE:
			TA1CCR0 = 0;
			PIN_SET_LOW(MOTOR_PIN);
			motor_state = MOTOR_STATE_OFF;
			break;

		default:
			TA1CCR0 = 0;
			PIN_SET_LOW(MOTOR_PIN);
			motor_state = MOTOR_STATE_OFF;
			break;
	}
}

void __attribute__((interrupt(ADC10_VECTOR))) ADC10_ISR(void)
{
	washer.temperature = ADC10MEM;
}
