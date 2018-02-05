#include "Math.h"



Math::Math()
{
}


Math::~Math()
{
}

float Math::qsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking（对浮点数的邪恶位元hack）
	i = 0x5f3759df - (i >> 1);               // what the fuck?（这他妈的是怎么回事？）
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration （第一次迭代）
										   //      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）

	return y;
}