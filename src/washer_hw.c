#include "def.h"

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