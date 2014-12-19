#include "def.h"
#include "washer.h"
#include "washer_commands.h"

#include "washer_hw.h"

extern WASHER_t washer;
extern IO_INTERFACE_t washer_io;

static WASHER_ANSWER_t answer;

BOOL_t
WASHER_dumb_command(uint8_t *data, SIZE_t cnt)
{
	washer_io.log("DUMB COMMAND!\r\n");
	return FALSE;
}

BOOL_t
WASHER_doorlock_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_DOORLOCK] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_water_heater_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_WATER_HEATER] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_main_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_MAIN_VALVE] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_pre_valve_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_PRE_VALVE] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_motor_dir_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_RELAY] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

uint16_t
WASHER_timer_value(uint8_t pwm)
{
	switch (pwm)
	{
		case 0: return 63240U; break;
		case 1: return 62992U; break;
		case 2: return 62744U; break;
		case 3: return 62496U; break;
		case 4: return 62248U; break;
		case 5: return 62000U; break;
		case 6: return 61752U; break;
		case 7: return 61504U; break;
		case 8: return 61256U; break;
		case 9: return 61008U; break;
		case 10: return 60760U; break;
		case 11: return 60512U; break;
		case 12: return 60264U; break;
		case 13: return 60016U; break;
		case 14: return 59768U; break;
		case 15: return 59520U; break;
		case 16: return 59272U; break;
		case 17: return 59024U; break;
		case 18: return 58776U; break;
		case 19: return 58528U; break;
		case 20: return 58280U; break;
		case 21: return 58032U; break;
		case 22: return 57784U; break;
		case 23: return 57536U; break;
		case 24: return 57288U; break;
		case 25: return 57040U; break;
		case 26: return 56792U; break;
		case 27: return 56544U; break;
		case 28: return 56296U; break;
		case 29: return 56048U; break;
		case 30: return 55800U; break;
		case 31: return 55552U; break;
		case 32: return 55304U; break;
		case 33: return 55056U; break;
		case 34: return 54808U; break;
		case 35: return 54560U; break;
		case 36: return 54312U; break;
		case 37: return 54064U; break;
		case 38: return 53816U; break;
		case 39: return 53568U; break;
		case 40: return 53320U; break;
		case 41: return 53072U; break;
		case 42: return 52824U; break;
		case 43: return 52576U; break;
		case 44: return 52328U; break;
		case 45: return 52080U; break;
		case 46: return 51832U; break;
		case 47: return 51584U; break;
		case 48: return 51336U; break;
		case 49: return 51088U; break;
		case 50: return 50840U; break;
		case 51: return 50592U; break;
		case 52: return 50344U; break;
		case 53: return 50096U; break;
		case 54: return 49848U; break;
		case 55: return 49600U; break;
		case 56: return 49352U; break;
		case 57: return 49104U; break;
		case 58: return 48856U; break;
		case 59: return 48608U; break;
		case 60: return 48360U; break;
		case 61: return 48112U; break;
		case 62: return 47864U; break;
		case 63: return 47616U; break;
		case 64: return 47368U; break;
		case 65: return 47120U; break;
		case 66: return 46872U; break;
		case 67: return 46624U; break;
		case 68: return 46376U; break;
		case 69: return 46128U; break;
		case 70: return 45880U; break;
		case 71: return 45632U; break;
		case 72: return 45384U; break;
		case 73: return 45136U; break;
		case 74: return 44888U; break;
		case 75: return 44640U; break;
		case 76: return 44392U; break;
		case 77: return 44144U; break;
		case 78: return 43896U; break;
		case 79: return 43648U; break;
		case 80: return 43400U; break;
		case 81: return 43152U; break;
		case 82: return 42904U; break;
		case 83: return 42656U; break;
		case 84: return 42408U; break;
		case 85: return 42160U; break;
		case 86: return 41912U; break;
		case 87: return 41664U; break;
		case 88: return 41416U; break;
		case 89: return 41168U; break;
		case 90: return 40920U; break;
		case 91: return 40672U; break;
		case 92: return 40424U; break;
		case 93: return 40176U; break;
		case 94: return 39928U; break;
		case 95: return 39680U; break;
		case 96: return 39432U; break;
		case 97: return 39184U; break;
		case 98: return 38936U; break;
		case 99: return 38688U; break;
		case 100: return 38440U; break;
		case 101: return 38192U; break;
		case 102: return 37944U; break;
		case 103: return 37696U; break;
		case 104: return 37448U; break;
		case 105: return 37200U; break;
		case 106: return 36952U; break;
		case 107: return 36704U; break;
		case 108: return 36456U; break;
		case 109: return 36208U; break;
		case 110: return 35960U; break;
		case 111: return 35712U; break;
		case 112: return 35464U; break;
		case 113: return 35216U; break;
		case 114: return 34968U; break;
		case 115: return 34720U; break;
		case 116: return 34472U; break;
		case 117: return 34224U; break;
		case 118: return 33976U; break;
		case 119: return 33728U; break;
		case 120: return 33480U; break;
		case 121: return 33232U; break;
		case 122: return 32984U; break;
		case 123: return 32736U; break;
		case 124: return 32488U; break;
		case 125: return 32240U; break;
		case 126: return 31992U; break;
		case 127: return 31744U; break;
		case 128: return 31496U; break;
		case 129: return 31248U; break;
		case 130: return 31000U; break;
		case 131: return 30752U; break;
		case 132: return 30504U; break;
		case 133: return 30256U; break;
		case 134: return 30008U; break;
		case 135: return 29760U; break;
		case 136: return 29512U; break;
		case 137: return 29264U; break;
		case 138: return 29016U; break;
		case 139: return 28768U; break;
		case 140: return 28520U; break;
		case 141: return 28272U; break;
		case 142: return 28024U; break;
		case 143: return 27776U; break;
		case 144: return 27528U; break;
		case 145: return 27280U; break;
		case 146: return 27032U; break;
		case 147: return 26784U; break;
		case 148: return 26536U; break;
		case 149: return 26288U; break;
		case 150: return 26040U; break;
		case 151: return 25792U; break;
		case 152: return 25544U; break;
		case 153: return 25296U; break;
		case 154: return 25048U; break;
		case 155: return 24800U; break;
		case 156: return 24552U; break;
		case 157: return 24304U; break;
		case 158: return 24056U; break;
		case 159: return 23808U; break;
		case 160: return 23560U; break;
		case 161: return 23312U; break;
		case 162: return 23064U; break;
		case 163: return 22816U; break;
		case 164: return 22568U; break;
		case 165: return 22320U; break;
		case 166: return 22072U; break;
		case 167: return 21824U; break;
		case 168: return 21576U; break;
		case 169: return 21328U; break;
		case 170: return 21080U; break;
		case 171: return 20832U; break;
		case 172: return 20584U; break;
		case 173: return 20336U; break;
		case 174: return 20088U; break;
		case 175: return 19840U; break;
		case 176: return 19592U; break;
		case 177: return 19344U; break;
		case 178: return 19096U; break;
		case 179: return 18848U; break;
		case 180: return 18600U; break;
		case 181: return 18352U; break;
		case 182: return 18104U; break;
		case 183: return 17856U; break;
		case 184: return 17608U; break;
		case 185: return 17360U; break;
		case 186: return 17112U; break;
		case 187: return 16864U; break;
		case 188: return 16616U; break;
		case 189: return 16368U; break;
		case 190: return 16120U; break;
		case 191: return 15872U; break;
		case 192: return 15624U; break;
		case 193: return 15376U; break;
		case 194: return 15128U; break;
		case 195: return 14880U; break;
		case 196: return 14632U; break;
		case 197: return 14384U; break;
		case 198: return 14136U; break;
		case 199: return 13888U; break;
		case 200: return 13640U; break;
		case 201: return 13392U; break;
		case 202: return 13144U; break;
		case 203: return 12896U; break;
		case 204: return 12648U; break;
		case 205: return 12400U; break;
		case 206: return 12152U; break;
		case 207: return 11904U; break;
		case 208: return 11656U; break;
		case 209: return 11408U; break;
		case 210: return 11160U; break;
		case 211: return 10912U; break;
		case 212: return 10664U; break;
		case 213: return 10416U; break;
		case 214: return 10168U; break;
		case 215: return 9920U; break;
		case 216: return 9672U; break;
		case 217: return 9424U; break;
		case 218: return 9176U; break;
		case 219: return 8928U; break;
		case 220: return 8680U; break;
		case 221: return 8432U; break;
		case 222: return 8184U; break;
		case 223: return 7936U; break;
		case 224: return 7688U; break;
		case 225: return 7440U; break;
		case 226: return 7192U; break;
		case 227: return 6944U; break;
		case 228: return 6696U; break;
		case 229: return 6448U; break;
		case 230: return 6200U; break;
		case 231: return 5952U; break;
		case 232: return 5704U; break;
		case 233: return 5456U; break;
		case 234: return 5208U; break;
		case 235: return 4960U; break;
		case 236: return 4712U; break;
		case 237: return 4464U; break;
		case 238: return 4216U; break;
		case 239: return 3968U; break;
		case 240: return 3720U; break;
		case 241: return 3472U; break;
		case 242: return 3224U; break;
		case 243: return 2976U; break;
		case 244: return 2728U; break;
		case 245: return 2480U; break;
		case 246: return 2232U; break;
		case 247: return 1984U; break;
		case 248: return 1736U; break;
		case 249: return 1488U; break;
		case 250: return 1240U; break;
		case 251: return 992U; break;
		case 252: return 744U; break;
		case 253: return 496U; break;
		case 254: return 248U; break;
		case 255: return 0U; break;
	}
}

BOOL_t
WASHER_motor_power_set_command(uint8_t *data, SIZE_t cnt)
{
	uint16_t temp;

	if ( cnt < sizeof(washer.motor_power) )
		return FALSE;

	temp = (((uint16_t)data[0]) << 0)
	     | (((uint16_t)data[1]) << 8);

	if ( temp == 0 )
		washer.motor_power = 0;
	else
		washer.motor_power = WASHER_timer_value(temp);

	return TRUE;
}

BOOL_t
WASHER_water_pump_set_command(uint8_t *data, SIZE_t cnt)
{
	if ( cnt == 0 )
		return FALSE;

	washer.is_on[WASHER_PERIPHERAL_WATER_PUMP] = (data[0] == 0x01) ? TRUE : FALSE;

	return TRUE;
}

BOOL_t
WASHER_tacho_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_TACHO_ANSWER;
	answer.data = washer.tacho_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_sonar_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_SONAR_ANSWER;
	answer.data = washer.sonar_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_zerocross_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_ZEROCROSS_ANSWER;
	answer.data = washer.zerocross_fq;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_temperature_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_TEMPERATURE_ANSWER;
	answer.data = washer.temperature;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_doorlock_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_DOORLOCK_ANSWER;
	answer.data = (washer.is_on[WASHER_PERIPHERAL_DOORLOCK]) ? 0x01 : 0xFF;
	return washer_io.write((uint8_t *)&answer, sizeof(answer));
}

BOOL_t
WASHER_id_get_command(uint8_t *data, SIZE_t cnt)
{
	answer.type = WASHER_COMMAND_ID_ANSWER;
	answer.data = washer.id;
	return washer_io.write((uint8_t *)&answer.type, sizeof(answer.type));
}

BOOL_t
WASHER_reset_command(uint8_t *data, SIZE_t cnt)
{
	/*asm("RST;");*/
	return TRUE;
}
