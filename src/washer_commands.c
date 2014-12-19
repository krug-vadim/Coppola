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
		case 0: return 16320U; break;
		case 1: return 16256U; break;
		case 2: return 16192U; break;
		case 3: return 16128U; break;
		case 4: return 16064U; break;
		case 5: return 16000U; break;
		case 6: return 15936U; break;
		case 7: return 15872U; break;
		case 8: return 15808U; break;
		case 9: return 15744U; break;
		case 10: return 15680U; break;
		case 11: return 15616U; break;
		case 12: return 15552U; break;
		case 13: return 15488U; break;
		case 14: return 15424U; break;
		case 15: return 15360U; break;
		case 16: return 15296U; break;
		case 17: return 15232U; break;
		case 18: return 15168U; break;
		case 19: return 15104U; break;
		case 20: return 15040U; break;
		case 21: return 14976U; break;
		case 22: return 14912U; break;
		case 23: return 14848U; break;
		case 24: return 14784U; break;
		case 25: return 14720U; break;
		case 26: return 14656U; break;
		case 27: return 14592U; break;
		case 28: return 14528U; break;
		case 29: return 14464U; break;
		case 30: return 14400U; break;
		case 31: return 14336U; break;
		case 32: return 14272U; break;
		case 33: return 14208U; break;
		case 34: return 14144U; break;
		case 35: return 14080U; break;
		case 36: return 14016U; break;
		case 37: return 13952U; break;
		case 38: return 13888U; break;
		case 39: return 13824U; break;
		case 40: return 13760U; break;
		case 41: return 13696U; break;
		case 42: return 13632U; break;
		case 43: return 13568U; break;
		case 44: return 13504U; break;
		case 45: return 13440U; break;
		case 46: return 13376U; break;
		case 47: return 13312U; break;
		case 48: return 13248U; break;
		case 49: return 13184U; break;
		case 50: return 13120U; break;
		case 51: return 13056U; break;
		case 52: return 12992U; break;
		case 53: return 12928U; break;
		case 54: return 12864U; break;
		case 55: return 12800U; break;
		case 56: return 12736U; break;
		case 57: return 12672U; break;
		case 58: return 12608U; break;
		case 59: return 12544U; break;
		case 60: return 12480U; break;
		case 61: return 12416U; break;
		case 62: return 12352U; break;
		case 63: return 12288U; break;
		case 64: return 12224U; break;
		case 65: return 12160U; break;
		case 66: return 12096U; break;
		case 67: return 12032U; break;
		case 68: return 11968U; break;
		case 69: return 11904U; break;
		case 70: return 11840U; break;
		case 71: return 11776U; break;
		case 72: return 11712U; break;
		case 73: return 11648U; break;
		case 74: return 11584U; break;
		case 75: return 11520U; break;
		case 76: return 11456U; break;
		case 77: return 11392U; break;
		case 78: return 11328U; break;
		case 79: return 11264U; break;
		case 80: return 11200U; break;
		case 81: return 11136U; break;
		case 82: return 11072U; break;
		case 83: return 11008U; break;
		case 84: return 10944U; break;
		case 85: return 10880U; break;
		case 86: return 10816U; break;
		case 87: return 10752U; break;
		case 88: return 10688U; break;
		case 89: return 10624U; break;
		case 90: return 10560U; break;
		case 91: return 10496U; break;
		case 92: return 10432U; break;
		case 93: return 10368U; break;
		case 94: return 10304U; break;
		case 95: return 10240U; break;
		case 96: return 10176U; break;
		case 97: return 10112U; break;
		case 98: return 10048U; break;
		case 99: return 9984U; break;
		case 100: return 9920U; break;
		case 101: return 9856U; break;
		case 102: return 9792U; break;
		case 103: return 9728U; break;
		case 104: return 9664U; break;
		case 105: return 9600U; break;
		case 106: return 9536U; break;
		case 107: return 9472U; break;
		case 108: return 9408U; break;
		case 109: return 9344U; break;
		case 110: return 9280U; break;
		case 111: return 9216U; break;
		case 112: return 9152U; break;
		case 113: return 9088U; break;
		case 114: return 9024U; break;
		case 115: return 8960U; break;
		case 116: return 8896U; break;
		case 117: return 8832U; break;
		case 118: return 8768U; break;
		case 119: return 8704U; break;
		case 120: return 8640U; break;
		case 121: return 8576U; break;
		case 122: return 8512U; break;
		case 123: return 8448U; break;
		case 124: return 8384U; break;
		case 125: return 8320U; break;
		case 126: return 8256U; break;
		case 127: return 8192U; break;
		case 128: return 8128U; break;
		case 129: return 8064U; break;
		case 130: return 8000U; break;
		case 131: return 7936U; break;
		case 132: return 7872U; break;
		case 133: return 7808U; break;
		case 134: return 7744U; break;
		case 135: return 7680U; break;
		case 136: return 7616U; break;
		case 137: return 7552U; break;
		case 138: return 7488U; break;
		case 139: return 7424U; break;
		case 140: return 7360U; break;
		case 141: return 7296U; break;
		case 142: return 7232U; break;
		case 143: return 7168U; break;
		case 144: return 7104U; break;
		case 145: return 7040U; break;
		case 146: return 6976U; break;
		case 147: return 6912U; break;
		case 148: return 6848U; break;
		case 149: return 6784U; break;
		case 150: return 6720U; break;
		case 151: return 6656U; break;
		case 152: return 6592U; break;
		case 153: return 6528U; break;
		case 154: return 6464U; break;
		case 155: return 6400U; break;
		case 156: return 6336U; break;
		case 157: return 6272U; break;
		case 158: return 6208U; break;
		case 159: return 6144U; break;
		case 160: return 6080U; break;
		case 161: return 6016U; break;
		case 162: return 5952U; break;
		case 163: return 5888U; break;
		case 164: return 5824U; break;
		case 165: return 5760U; break;
		case 166: return 5696U; break;
		case 167: return 5632U; break;
		case 168: return 5568U; break;
		case 169: return 5504U; break;
		case 170: return 5440U; break;
		case 171: return 5376U; break;
		case 172: return 5312U; break;
		case 173: return 5248U; break;
		case 174: return 5184U; break;
		case 175: return 5120U; break;
		case 176: return 5056U; break;
		case 177: return 4992U; break;
		case 178: return 4928U; break;
		case 179: return 4864U; break;
		case 180: return 4800U; break;
		case 181: return 4736U; break;
		case 182: return 4672U; break;
		case 183: return 4608U; break;
		case 184: return 4544U; break;
		case 185: return 4480U; break;
		case 186: return 4416U; break;
		case 187: return 4352U; break;
		case 188: return 4288U; break;
		case 189: return 4224U; break;
		case 190: return 4160U; break;
		case 191: return 4096U; break;
		case 192: return 4032U; break;
		case 193: return 3968U; break;
		case 194: return 3904U; break;
		case 195: return 3840U; break;
		case 196: return 3776U; break;
		case 197: return 3712U; break;
		case 198: return 3648U; break;
		case 199: return 3584U; break;
		case 200: return 3520U; break;
		case 201: return 3456U; break;
		case 202: return 3392U; break;
		case 203: return 3328U; break;
		case 204: return 3264U; break;
		case 205: return 3200U; break;
		case 206: return 3136U; break;
		case 207: return 3072U; break;
		case 208: return 3008U; break;
		case 209: return 2944U; break;
		case 210: return 2880U; break;
		case 211: return 2816U; break;
		case 212: return 2752U; break;
		case 213: return 2688U; break;
		case 214: return 2624U; break;
		case 215: return 2560U; break;
		case 216: return 2496U; break;
		case 217: return 2432U; break;
		case 218: return 2368U; break;
		case 219: return 2304U; break;
		case 220: return 2240U; break;
		case 221: return 2176U; break;
		case 222: return 2112U; break;
		case 223: return 2048U; break;
		case 224: return 1984U; break;
		case 225: return 1920U; break;
		case 226: return 1856U; break;
		case 227: return 1792U; break;
		case 228: return 1728U; break;
		case 229: return 1664U; break;
		case 230: return 1600U; break;
		case 231: return 1536U; break;
		case 232: return 1472U; break;
		case 233: return 1408U; break;
		case 234: return 1344U; break;
		case 235: return 1280U; break;
		case 236: return 1216U; break;
		case 237: return 1152U; break;
		case 238: return 1088U; break;
		case 239: return 1024U; break;
		case 240: return 960U; break;
		case 241: return 896U; break;
		case 242: return 832U; break;
		case 243: return 768U; break;
		case 244: return 704U; break;
		case 245: return 640U; break;
		case 246: return 576U; break;
		case 247: return 512U; break;
		case 248: return 448U; break;
		case 249: return 384U; break;
		case 250: return 320U; break;
		case 251: return 256U; break;
		case 252: return 192U; break;
		case 253: return 128U; break;
		case 254: return 64U; break;
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
