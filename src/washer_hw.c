#include "def.h"

#include "washer.h"
#include "washer_hw.h"

static WASHER_VALUE_t sonar_fq_current;
static WASHER_VALUE_t tacho_fq_current;
static WASHER_VALUE_t zerocross_fq_current;

static uint8_t sonar_last_state;
static uint8_t tacho_last_state;

static BOOL_t zerocrossing;

extern WASHER_t washer;
extern IO_INTERFACE_t washer_io;

static MOTOR_STATE_t motor_state;

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
WASHER_HW_zerocross_interrupt_setup(void)
{
	/* change to some HAL */
	PIN_MODE_INPUT(ZEROCROSS_PIN);
	P2IES |= (1 << (ZEROCROSS_PIN-8));
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
	WASHER_HW_zerocross_interrupt_setup();

	motor_state = MOTOR_STATE_OFF;

	/* Timer A1 SMCLK, UP 1 uS */
	TA1CCR0 = 0;
	TA1CCTL0 = 0x10;
	TA1CTL = TASSEL_2
	       | ID_0
	       | MC_1
	       ;
}

void
WASHER_HW_startup(void)
{
	sonar_fq_current = 0;
	tacho_fq_current = 0;

	sonar_last_state = 0;
	tacho_last_state = 0;

	zerocrossing = FALSE;

	/* adc startup */
	ADC10CTL0 |= ENC + ADC10SC;

	P2IE  |=  (1 << (ZEROCROSS_PIN - 8));
	P2IFG &= ~(1 << (ZEROCROSS_PIN - 8));
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

	if ( zerocrossing )
	{
		WASHER_HW_peripherals_set();
		zerocrossing = FALSE;
	}
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
void __attribute__((interrupt(TIMER1_A0_VECTOR))) motor_pwm(void)
{
	PIN_SET_LOW(MOTOR_PIN);

	if ( motor_state == MOTOR_STATE_WAIT )
	{
		TA1CCR0 = MOTOR_IMPULSE_LENGTH;
		motor_state = MOTOR_STATE_IMPULSE;
		PIN_SET_HIGH(MOTOR_PIN);
	}
	else
	{
		TA1CCR0 = 0;
		motor_state = MOTOR_STATE_OFF;
	}
}

void __attribute__((interrupt(ADC10_VECTOR))) ADC10_ISR(void)
{
	washer.temperature = ADC10MEM;
}

void __attribute__((interrupt(PORT2_VECTOR))) ZEROCROSS_PORT(void)
{
	PIN_SET_LOW(MOTOR_PIN);

	zerocrossing = TRUE;
	zerocross_fq_current++;

	motor_state = MOTOR_STATE_WAIT;
	TA1CCR0 = washer.motor_power;

	P2IES ^= (1 << (ZEROCROSS_PIN - 8));
	P2IFG &= ~(1 << (ZEROCROSS_PIN - 8));
}
