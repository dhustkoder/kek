#include "kek.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M 397 /* changes to STATE_VECTOR_LENGTH also require changes to this */
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000

static unsigned int seed = 0xDEADBEEF;
static unsigned long mt[STATE_VECTOR_LENGTH];
static int rand_index = 0;

void random_init(void)
{
	mt[0] = seed & 0xffffffff;

	for (rand_index = 1; rand_index < STATE_VECTOR_LENGTH; rand_index++) {
		mt[rand_index] = (6069 * mt[rand_index - 1]) & 0xffffffff;
	}
}

int random_get_rangei(int min, int max)
{
	assert(min <= max);
	unsigned int value = random_getui();
	unsigned int range = (unsigned int)(max - min) + 1;
	value = value % range;
	return (int)value + min;
}

float random_get_rangef(float min, float max)
{
	assert(min <= max);
	float value = (float)random_getui(); //domain: [0min -> 0xFFFFFFFF]
	float percent = value / (float)0xFFFFFFFF;
	float range = (max - min) * percent + min;
	return range;
}
Vec2 random_get_range_v2(Vec2 min, Vec2 max)
{
	Vec2 val;
	
	val.x = random_get_rangef(min.x, max.x);
	val.y = random_get_rangef(min.x, max.y);

	return val;
	
}

Vec3 random_get_range_v3(Vec3 min, Vec3 max)
{
	Vec3 val;
	
	val.x = random_get_rangef(min.x, max.x);
	val.y = random_get_rangef(min.x, max.y);
	val.z = random_get_rangef(min.z, max.z);

	return val;
}

Vec4 random_get_range_v4(Vec4 min, Vec4 max)
{
	Vec4 val;
	
	val.x = random_get_rangef(min.x, max.x);
	val.y = random_get_rangef(min.x, max.y);
	val.z = random_get_rangef(min.z, max.z);
	val.w = random_get_rangef(min.w, max.w);

	return val;
}

unsigned int random_getui(void)
{
	unsigned long y;
	static unsigned long mag[2] = {
		0x0, 0x9908b0df
	}; /* mag[x] = x * 0x9908b0df for x = 0,1 */

	if (rand_index >= STATE_VECTOR_LENGTH || rand_index < 0) {
		/* generate STATE_VECTOR_LENGTH words at a time */
		int kk;
		if (rand_index >= STATE_VECTOR_LENGTH + 1 || rand_index < 0) {
			seed = 4357;
		}
		for (kk = 0; kk < STATE_VECTOR_LENGTH - STATE_VECTOR_M; kk++) {
			y = (mt[kk] & UPPER_MASK) |
			    (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + STATE_VECTOR_M] ^ (y >> 1) ^
				    mag[y & 0x1];
		}
		for (; kk < STATE_VECTOR_LENGTH - 1; kk++) {
			y = (mt[kk] & UPPER_MASK) |
			    (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + (STATE_VECTOR_M -
						STATE_VECTOR_LENGTH)] ^
				    (y >> 1) ^ mag[y & 0x1];
		}
		y = (mt[STATE_VECTOR_LENGTH - 1] & UPPER_MASK) |
		    (mt[0] & LOWER_MASK);
		mt[STATE_VECTOR_LENGTH - 1] =
			mt[STATE_VECTOR_M - 1] ^ (y >> 1) ^ mag[y & 0x1];
		rand_index = 0;
	}

	y = mt[rand_index++];
	y ^= (y >> 11);
	y ^= (y << 7) & TEMPERING_MASK_B;
	y ^= (y << 15) & TEMPERING_MASK_C;
	y ^= (y >> 18);
	return y;
}

float random_getf(void)
{
	return ((float)random_getui() / (unsigned int)0xffffffff);
}
