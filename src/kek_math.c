#include "kek.h"
#include <float.h>

Vec2 vec2_zero()
{
	return (Vec2){ 0.0f, 0.0f };
}

Vec3 vec3_zero()
{
	return (Vec3){ 0.0f, 0.0f, 0.0f };
}

Vec4 vec4_zero()
{
	return (Vec4){ 0.0f, 0.0f, 0.0f, 0.0f };
}

Vec2 vec2_identity()
{
	return (Vec2){ 1.0f, 1.0f };
}

Vec3 vec3_identity()
{
	return (Vec3){ 1.0f, 1.0f, 1.0f };
}

Vec4 vec4_identity()
{
	return (Vec4){ 1.0f, 1.0f, 1.0f, 1.0f };
}

Vec2 vec2(float x, float y)
{
	return (Vec2){ x, y };
}

Vec3 vec3(float x, float y, float z)
{
	return (Vec3){ x, y, z };
}

Vec4 vec4(float x, float y, float z, float w)
{
	return (Vec4){ x, y, z, w };
}

Vec2 vec3to2(Vec3 v)
{
	return (Vec2){ v.x, v.y };
}

Vec2 vec4to2(Vec4 v)
{
	return (Vec2){ v.x, v.y };
}

Vec3 vec2to3(Vec2 v)
{
	return (Vec3){ v.x, v.y, 0 };
}

Vec3 vec4to3(Vec4 v)
{
	return (Vec3){ v.x, v.y, v.z };
}

Vec4 vec2to4(Vec2 v)
{
	return (Vec4){ v.x, v.y, 0.0f, 0.0f };
}

Vec4 vec3to4(Vec3 v)
{
	return (Vec4){ v.x, v.y, v.z, 0.0f };
}

float vec2_dot_product(Vec2 a, Vec2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float vec3_dot_product(Vec3 a, Vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float vec4_dot_product(Vec4 a, Vec4 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vec2 vec2_project(Vec2 u, Vec2 v)
{
	float uvdot = vec2_dot_product(u, v);
	float vvdot = vec2_dot_product(v, v);

	return vec2_mulf(v, uvdot / vvdot);
}

Vec3 vec3_project(Vec3 u, Vec3 v)
{
	float uvdot = vec3_dot_product(u, v);
	float vvdot = vec3_dot_product(v, v);

	return vec3_mulf(v, uvdot / vvdot);
}

Vec4 vec4_project(Vec4 u, Vec4 v)
{
	float uvdot = vec4_dot_product(u, v);
	float vvdot = vec4_dot_product(v, v);

	return vec4_mulf(v, uvdot / vvdot);
}

Vec3 cross_product(Vec3 v1, Vec3 vec2)
{
	Vec3 v;

	v.x = (v1.y * vec2.z) - (v1.z * vec2.y);
	v.y = (v1.z * vec2.x) - (v1.x * vec2.z);
	v.z = (v1.x * vec2.y) - (v1.y * vec2.x);

	return v;
}

float vec2_length2(Vec2 v)
{
	return vec2_dot_product(v, v);
}

float vec3_length2(Vec3 v)
{
	return vec3_dot_product(v, v);
}

float vec4_length2(Vec4 v)
{
	return vec4_dot_product(v, v);
}

float vec2_length(Vec2 v)
{
	return sqrtf(vec2_length2(v));
}

float vec3_length(Vec3 v)
{
	return sqrtf(vec3_length2(v));
}

float vec4_length(Vec4 v)
{
	return sqrtf(vec4_length2(v));
}

float signf(float v)
{
	return (v < 0) ? -1.f : 1.f;
}

int signi(int sign)
{
	if(sign == 0) return 0;
	if(sign < 0) return -1;
	if(sign > 0) return 1;

	return 0;
}

Vec2 vec2_sign(Vec2 v)
{
	v.x = signf(v.x);
	v.y = signf(v.y);

	return v;
}

Vec3 vec3_sign(Vec3 v)
{
	v.x = signf(v.x);
	v.y = signf(v.y);
	v.z = signf(v.z);

	return v;
}

Vec4 vec4_sign(Vec4 v)
{
	v.x = signf(v.x);
	v.y = signf(v.y);
	v.z = signf(v.z);
	v.w = signf(v.w);

	return v;
}

Vec2 vec2_normal(Vec2 v)
{
	return vec2_divf(v, vec2_length(v));
}

Vec3 vec3_normal(Vec3 v)
{
	return vec3_divf(v, vec3_length(v));
}

Vec4 vec4_normal(Vec4 v)
{
	return vec4_divf(v, vec4_length(v));
}

Vec2 vec2_negative(Vec2 v)
{
	return vec2_mulf(v, -1.f);
}

Vec3 vec3_negative(Vec3 v)
{
	return vec3_mulf(v, -1.f);
}

Vec4 vec4_negative(Vec4 v)
{
	return vec4_mulf(v, -1.f);
}

Vec2 vec2_abs(Vec2 v)
{
	v.x = fabsf(v.x);
	v.y = fabsf(v.y);
	return v;
}

Vec3 vec3_abs(Vec3 v)
{
	v.x = fabsf(v.x);
	v.y = fabsf(v.y);
	v.z = fabsf(v.z);
	return v;
}

Vec4 vec4_abs(Vec4 v)
{
	v.x = fabsf(v.x);
	v.y = fabsf(v.y);
	v.z = fabsf(v.z);
	v.w = fabsf(v.w);
	return v;
}

Vec2 vec2_clamp(Vec2 v, Vec2 min, Vec2 max)
{
	Vec2 value;
	value.x = KUT_MAX(v.x, min.x);
	value.x = KUT_MIN(v.x, max.x);

	value.y = KUT_MAX(v.y, min.y);
	value.y = KUT_MIN(v.y, max.y);

	return value;
}

Vec3 vec3_clamp(Vec3 v, Vec3 min, Vec3 max)
{
	Vec3 value;
	value.x = KUT_MAX(v.x, min.x);
	value.x = KUT_MIN(v.x, max.x);

	value.y = KUT_MAX(v.y, min.y);
	value.y = KUT_MIN(v.y, max.y);

	value.z = KUT_MAX(v.z, min.z);
	value.z = KUT_MIN(v.z, max.z);

	return value;
}

Vec4 vec4_clamp(Vec4 v, Vec4 min, Vec4 max)
{
	Vec4 value;
	value.x = KUT_MAX(v.x, min.x);
	value.x = KUT_MIN(v.x, max.x);

	value.y = KUT_MAX(v.y, min.y);
	value.y = KUT_MIN(v.y, max.y);

	value.z = KUT_MAX(v.z, min.z);
	value.z = KUT_MIN(v.z, max.z);

	value.w = KUT_MAX(v.w, min.w);
	value.w = KUT_MIN(v.w, max.w);

	return value;
}

Vec2 vec2_add(Vec2 a, Vec2 b)
{
	return (Vec2){ a.x + b.x, a.y + b.y };
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
	return (Vec2){ a.x - b.x, a.y - b.y };
}

Vec2 vec2_mul(Vec2 a, Vec2 b)
{
	return (Vec2){ a.x * b.x, a.y * b.y };
}

Vec2 vec2_div(Vec2 a, Vec2 b)
{
	return (Vec2){ a.x / b.x, a.y / b.y };
}

Vec2 vec2_addf(Vec2 a, float b)
{

	return (Vec2){ a.x + b, a.y + b };
}

Vec2 vec2_subf(Vec2 a, float b)
{
	return (Vec2){ a.x - b, a.y - b };
}

Vec2 vec2_mulf(Vec2 a, float b)
{
	return (Vec2){ a.x * b, a.y * b };
}

Vec2 vec2_divf(Vec2 a, float b)
{
	return (Vec2){ a.x / b, a.y / b };
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
	return (Vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
	return (Vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 vec3_mul(Vec3 a, Vec3 b)
{
	return (Vec3){ a.x * b.x, a.y * b.y, a.z * b.z };
}

Vec3 vec3_div(Vec3 a, Vec3 b)
{
	return (Vec3){ a.x / b.x, a.y / b.y, a.z / b.z };
}

Vec3 vec3_addf(Vec3 a, float b)
{
	return (Vec3){ a.x + b, a.y + b, a.z + b };
}

Vec3 vec3_subf(Vec3 a, float b)
{
	return (Vec3){ a.x - b, a.y - b, a.z - b };
}

Vec3 vec3_mulf(Vec3 a, float b)
{
	return (Vec3){ a.x * b, a.y * b, a.z * b };
}

Vec3 vec3_divf(Vec3 a, float b)
{
	return (Vec3){ a.x / b, a.y / b, a.z / b };
}

Vec4 vec4_add(Vec4 a, Vec4 b)
{
	return (Vec4){ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

Vec4 vec4_sub(Vec4 a, Vec4 b)
{
	return (Vec4){ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

Vec4 vec4_mul(Vec4 a, Vec4 b)
{
	return (Vec4){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

Vec4 vec4_div(Vec4 a, Vec4 b)
{
	return (Vec4){ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

Vec4 vec4_addf(Vec4 a, float b)
{
	return (Vec4){ a.x + b, a.y + b, a.z + b, a.w + b };
}

Vec4 vec4_subf(Vec4 a, float b)
{
	return (Vec4){ a.x - b, a.y - b, a.z - b, a.w - b };
}

Vec4 vec4_mulf(Vec4 a, float b)
{
	return (Vec4){ a.x * b, a.y * b, a.z * b, a.w * b };
}

Vec4 vec4_divf(Vec4 a, float b)
{
	return (Vec4){ a.x / b, a.y / b, a.z / b, a.w / b };
}

Mat4 mat4_mul(Mat4 a, Mat4 b)
{
	Mat4 r;

	for (int columns = 0; columns < 4; ++columns) {
		for (int rows = 0; rows < 4; ++rows) {
			float sum = 0;

			for (int matrice = 0; matrice < 4; ++matrice) {
				sum += a.m[matrice][rows] *
				       b.m[columns][matrice];
			}

			r.m[columns][rows] = sum;
		}
	}

	return r;
}

Vec4 mat4_mul_vec4(Mat4 m, Vec4 v)
{
	Vec4 res;
	res.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z +
		m.m[3][0] * v.w;
	res.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z +
		m.m[3][1] * v.w;
	res.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z +
		m.m[3][2] * v.w;
	res.w = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z +
		m.m[3][3] * v.w;
	return res;
}

Mat4 mat4_mulf(Mat4 m, float v)
{
	float *cell =  &m.m[0][0];

	for (int i = 0; i < 16; ++i) 
		cell[i] *= v;
		
	return m;
}


//todo: delete these comments
#define COLLISION_ABOVE 0x01
#define COLLISION_BELOW 0x02
#define COLLISION_LEFT  0x04
#define COLLISION_RIGHT 0x08
// collision masks are relative to "a"
unsigned int aabb2(Rect2 a, Rect2 b)
{
	float ax0 = a.position.x - a.size.x / 2;
	float ax1 = a.position.x + a.size.x / 2;
	float ay0 = a.position.y - a.size.y / 2;
	float ay1 = a.position.y + a.size.y / 2;
	float bx0 = b.position.x - b.size.x / 2;
	float bx1 = b.position.x + b.size.x / 2;
	float by0 = b.position.y - b.size.y / 2;
	float by1 = b.position.y + b.size.y / 2;
	
	int flags = 0;


	if(ax0 < bx1 && ax1 > bx0 && ay0 < by1 && ay1 > by0) {
		Vec2 d = vec2_sub(b.position, a.position);

		if(d.x >= 0.0f)
			flags |= COLLISION_RIGHT;

		if(d.x <= 0.0f)
			flags |= COLLISION_LEFT;

		if(d.y >= 0.0f)
			flags |= COLLISION_ABOVE;

		if(d.y <= 0.0f)
			flags |= COLLISION_BELOW;
	}

	return flags;
}
unsigned int aabb(Vec2 p0, Vec2 s0, Vec2 p1, Vec2 s1)
{
    Rect2 a;
    a.position = p0;
    a.size = s0;

    Rect2 b;
    b.position = p1;
    b.size = s1;

    return aabb2(a, b);
}

unsigned int aabb3(Cube a, Cube b)
{
	float ax0 = a.position.x - a.size.x / 2;
	float ax1 = a.position.x + a.size.x / 2;
	float ay0 = a.position.y - a.size.y / 2;
	float ay1 = a.position.y + a.size.y / 2;
	float az0 = a.position.z - a.size.z / 2;
	float az1 = a.position.z + a.size.z / 2;
	float bx0 = b.position.x - b.size.x / 2;
	float bx1 = b.position.x + b.size.x / 2;
	float by0 = b.position.y - b.size.y / 2;
	float by1 = b.position.y + b.size.y / 2;
	float bz0 = b.position.z - b.size.z / 2;
	float bz1 = b.position.z + b.size.z / 2;
	return (ax0 < bx1 && ax1 > bx0 && 
			ay0 < by1 && ay1 > by0 &&
			az0 < bz1 && az1 > bz0) ? 1 : 0;
}

Mat4 mat4_zero()
{
	Mat4 m;

	float *cell =  &m.m[0][0];

	for (int i = 0; i < 16; ++i)
		cell[i] = 0;

	return m;
}

Mat4 mat4_identity()
{
	Mat4 m = mat4_zero();
	float *cell = &m.m[0][0];

	for (int i = 0; i < 16; ++i)
		cell[i] = 0;

	m.m[0][0] = 1.f;
	m.m[1][1] = 1.f;
	m.m[2][2] = 1.f;
	m.m[3][3] = 1.f;

	return m;
}

int line_intersect(Vec3 p0, Vec3 v0, Vec3 p1, Vec3 v1, Vec3 *c)
{
	Vec3 q0 = vec3_add(p0, v0);
	Vec3 q1 = vec3_add(p1, v1);
	float x;
	float y;

	// special case: both lines are vertical
	if (v0.x == 0 && v1.x == 0) {
		if (p0.x != p1.x) {
			return 0;
		}
		if (p1.y <= p0.y && p0.y <= q1.y) {
			c->x = p0.x;
			c->y = p0.y;
			return 1;
		}
		if (p1.y <= q0.y && q0.y <= q1.y) {
			c->x = q0.x;
			c->y = q0.y;
			return 1;
		}
		if (p0.y <= p1.y && p1.y <= q0.y) {
			c->x = p1.x;
			c->y = p1.y;
			return 1;
		}
		if (p0.y <= q1.y && q1.y <= q0.y) {
			c->x = q1.x;
			c->y = q1.y;
			return 1;
		}
		return 0;
	}

	// line0 is vertical
	else if (v0.x == 0) {
		x = p0.x;
		float m1 = v1.y / v1.x;
		float b1 = p1.y - m1 * p1.x;
		y = m1 * x + b1;
	}

	// line1 is vertical
	else if (v1.x == 0) {
		x = p1.x;
		float m0 = v0.y / v0.x;
		float b0 = p0.y - m0 * p0.x;
		y = m0 * x + b0;
	}
	//todo handle cases where slopes are 0, b is invalid, need to apply similar range checks like above
	//except on a different axis

	// neither are vertial
	else {
		float m0 = v0.y / v0.x;
		float m1 = v1.y / v1.x;

		float b0 = p0.y - m0 * p0.x;
		float b1 = p1.y - m1 * p1.x;

		x = (b1 - b0) / (m0 - m1);
		y = m0 * x + b0;
	}

	// range test
	float r0xl = KUT_MIN(p0.x, q0.x);
	float r0xh = KUT_MAX(p0.x, q0.x);
	float r0yl = KUT_MIN(p0.y, q0.y);
	float r0yh = KUT_MAX(p0.y, q0.y);
	float r1xl = KUT_MIN(p1.x, q1.x);
	float r1xh = KUT_MAX(p1.x, q1.x);
	float r1yl = KUT_MIN(p1.y, q1.y);
	float r1yh = KUT_MAX(p1.y, q1.y);

	if (r0xl <= x && x <= r0xh && r0yl <= y && y <= r0yh && r1xl <= x &&
	    x <= r1xh && r1yl <= y && y <= r1yh) {
		c->x = x;
		c->y = y;

		return 1;
	}

	return 0;
}

float clampf(float value, float min, float max)
{
	value = KUT_MAX(value, min);
	value = KUT_MIN(value, max);

	return value;
}

float deg_to_rad(float deg)
{
	return (deg / 360.f) * 2.f * KUT_PI;
}

float rad_to_deg(float rad)
{
	return (rad / (2.f * KUT_PI)) * 360.f;
}

Mat4 mat4_translate(const Vec3 v)
{
	Mat4 m = mat4_identity();

	m.m[3][0] = v.x;
	m.m[3][1] = v.y;
	m.m[3][2] = v.z;

	return m;
}

Mat4 mat4_scale(const Vec3 v)
{
	Mat4 m = mat4_identity();

	m.m[0][0] = v.x;
	m.m[1][1] = v.y;
	m.m[2][2] = v.z;

	return m;
}

float magnitudef(float v)
{
	return (v < 0) ? v * -1.f : v;
}

Mat4 rotatex(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	Mat4 r;
	r.m[0][0] = 1;
	r.m[0][1] = 0;
	r.m[0][2] = 0;
	r.m[0][3] = 0;
	r.m[1][0] = 0;
	r.m[1][1] = c;
	r.m[1][2] = -s;
	r.m[1][3] = 0;
	r.m[2][0] = 0;
	r.m[2][1] = s;
	r.m[2][2] = c;
	r.m[2][3] = 0;
	r.m[3][0] = 0;
	r.m[3][1] = 0;
	r.m[3][2] = 0;
	r.m[3][3] = 1;
	return (r);
}

Mat4 rotatey(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	Mat4 r;
	r.m[0][0] = c;
	r.m[0][1] = 0;
	r.m[0][2] = s;
	r.m[0][3] = 0;
	r.m[1][0] = 0;
	r.m[1][1] = 1;
	r.m[1][2] = 0;
	r.m[1][3] = 0;
	r.m[2][0] = -s;
	r.m[2][1] = 0;
	r.m[2][2] = c;
	r.m[2][3] = 0;
	r.m[3][0] = 0;
	r.m[3][1] = 0;
	r.m[3][2] = 0;
	r.m[3][3] = 1;

	return (r);
}

Mat4 rotatez(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	Mat4 r;
	r.m[0][0] = c;
	r.m[0][1] = -s;
	r.m[0][2] = 0;
	r.m[0][3] = 0;
	r.m[1][0] = s;
	r.m[1][1] = c;
	r.m[1][2] = 0;
	r.m[1][3] = 0;
	r.m[2][0] = 0;
	r.m[2][1] = 0;
	r.m[2][2] = 1;
	r.m[2][3] = 0;
	r.m[3][0] = 0;
	r.m[3][1] = 0;
	r.m[3][2] = 0;
	r.m[3][3] = 1;
	return (r);
}

Mat4 ortho_projection(float left, float right, float bottom,
			float top, float near, float far)
{
	float rl, tb, fn;

	Mat4 m = mat4_zero();

	rl = 1.0f / (right - left);
	tb = 1.0f / (top - bottom);
	fn = -1.0f / (far - near);

	m.m[0][0] = 2.0f * rl;
	m.m[1][1] = 2.0f * tb;
	m.m[2][2] = 2.0f * fn;
	m.m[3][0] = -(right + left) * rl;
	m.m[3][1] = -(top + bottom) * tb;
	m.m[3][2] = (far + near) * fn;
	m.m[3][3] = 1.0f;

	return m;
}

Vec3 unproject(Vec3 pos, Mat4 viewproj, Vec4 viewport)
{
	//normalize to [-1,1]
	Vec4 v;
	v.x = 2.0f * (pos.x - viewport.x) / viewport.z - 1.0f;
	v.y = 2.0f * (pos.y - viewport.y) / viewport.w - 1.0f;
	v.z = 2.0f * pos.z - 1.0f;
	v.w = 1.0f;

	Mat4 inv = mat4_inverse(viewproj);

	v = mat4_mul_vec4(inv, v);

	v = vec4_mulf(v, 1.0f / v.w);

	return vec4to3(v);
}

Vec3 screen_to_world_coord(Mat4 projection,
                             Mat4 view,
                             Vec4 viewport,
                             Vec2 coord)
{
	Mat4 vp = mat4_mul(view, projection);
	Vec2 dimensions;
	dimensions.x = viewport.z - viewport.x;
	dimensions.y = viewport.w - viewport.y;
	
	Vec3 screen_pos0 = vec3(coord.x, dimensions.y - coord.y, 0.0f);
	Vec3 screen_pos1 = vec3(coord.x, dimensions.y - coord.y, 1.0f);
	
	Vec3 a = unproject(screen_pos0, vp, viewport);
	Vec3 b = unproject(screen_pos1, vp, viewport);
	Vec3 ray = vec3_sub(b, a);
	Vec3 raydir = vec3_normal(ray);
	// some linear algebra
	// x = ax + (bx - ax)*t
	// y = ay + (by - ay)*t
	// z = az + (bz - az)*t
	// where b - a = raydir
	// solve for t where z = 0
	// 0 = az + raydir*t
	// 0 = az + (bz - az)*t
	// -az/(bz-az) = t
	float t = -a.z / (raydir.z);
	
	return vec3_add(a, vec3_mulf(raydir, t));
}
Mat4 perspective_projection(float fovy, float aspect, float near,
			      float far)
{
	float f, fn;

	Mat4 m = mat4_zero();

	f = 1.0f / tanf(fovy * 0.5f);
	fn = 1.0f / (near - far);

	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = (near + far) * fn;
	m.m[2][3] = -1.0f;
	m.m[3][2] = 2.0f * near * far * fn;

	return m;
}

Mat4 look_at(Vec3 eye, Vec3 center, Vec3 up)
{
	Vec3 f;
	Vec3 s;
	Vec3 u;
	Vec3 c;

	f = vec3_sub(center, eye);
	f = vec3_normal(f);

	c = cross_product(f, up);
	s = vec3_normal(c);
	u = cross_product(s, f);

	Mat4 m;
	m.m[0][0] = s.x;
	m.m[0][1] = u.x;
	m.m[0][2] = -f.x;

	m.m[1][0] = s.y;
	m.m[1][1] = u.y;
	m.m[1][2] = -f.y;

	m.m[2][0] = s.z;
	m.m[2][1] = u.z;
	m.m[2][2] = -f.z;

	m.m[3][0] = -vec3_dot_product(s, eye);
	m.m[3][1] = -vec3_dot_product(u, eye);
	m.m[3][2] = vec3_dot_product(f, eye);

	m.m[0][3] = 0.0f;
	m.m[1][3] = 0.0f;
	m.m[2][3] = 0.0f;
	m.m[3][3] = 1.0f;
	return m;
}

Mat4 mat4_inverse(const Mat4 mat)
{
	Mat4 dest;
	float t[6];
	float det;
	float a = mat.m[0][0], b = mat.m[0][1], c = mat.m[0][2],
	      d = mat.m[0][3], e = mat.m[1][0], f = mat.m[1][1],
	      g = mat.m[1][2], h = mat.m[1][3], i = mat.m[2][0],
	      j = mat.m[2][1], k = mat.m[2][2], l = mat.m[2][3],
	      m = mat.m[3][0], n = mat.m[3][1], o = mat.m[3][2],
	      p = mat.m[3][3];

	t[0] = k * p - o * l;
	t[1] = j * p - n * l;
	t[2] = j * o - n * k;
	t[3] = i * p - m * l;
	t[4] = i * o - m * k;
	t[5] = i * n - m * j;

	dest.m[0][0] = f * t[0] - g * t[1] + h * t[2];
	dest.m[1][0] = -(e * t[0] - g * t[3] + h * t[4]);
	dest.m[2][0] = e * t[1] - f * t[3] + h * t[5];
	dest.m[3][0] = -(e * t[2] - f * t[4] + g * t[5]);

	dest.m[0][1] = -(b * t[0] - c * t[1] + d * t[2]);
	dest.m[1][1] = a * t[0] - c * t[3] + d * t[4];
	dest.m[2][1] = -(a * t[1] - b * t[3] + d * t[5]);
	dest.m[3][1] = a * t[2] - b * t[4] + c * t[5];

	t[0] = g * p - o * h;
	t[1] = f * p - n * h;
	t[2] = f * o - n * g;
	t[3] = e * p - m * h;
	t[4] = e * o - m * g;
	t[5] = e * n - m * f;

	dest.m[0][2] = b * t[0] - c * t[1] + d * t[2];
	dest.m[1][2] = -(a * t[0] - c * t[3] + d * t[4]);
	dest.m[2][2] = a * t[1] - b * t[3] + d * t[5];
	dest.m[3][2] = -(a * t[2] - b * t[4] + c * t[5]);

	t[0] = g * l - k * h;
	t[1] = f * l - j * h;
	t[2] = f * k - j * g;
	t[3] = e * l - i * h;
	t[4] = e * k - i * g;
	t[5] = e * j - i * f;

	dest.m[0][3] = -(b * t[0] - c * t[1] + d * t[2]);
	dest.m[1][3] = a * t[0] - c * t[3] + d * t[4];
	dest.m[2][3] = -(a * t[1] - b * t[3] + d * t[5]);
	dest.m[3][3] = a * t[2] - b * t[4] + c * t[5];

	det = 1.0f / (a * dest.m[0][0] + b * dest.m[1][0] + c * dest.m[2][0] +
		      d * dest.m[3][0]);

	dest = mat4_mulf(dest, det);

	return dest;
}

Mat4 mat4_transpose(Mat4 *m)
{
	Mat4 mat;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			mat.m[row][col] = m->m[col][row];
		}
	}

	return mat;
}

int sum(const int *list, int count)
{
	int sum = 0;

	for (int i = 0; i < count; ++i)
		sum += list[i];

	return sum;
}

void vec3_minmax(Vec3 *p0, Vec3 *p1)
{
	Vec3 t0 = *p0;
	Vec3 t1 = *p1;
	
	p0->x = KUT_MIN(t0.x, t1.x);
	p0->y = KUT_MIN(t0.y, t1.y);
	p0->z = KUT_MIN(t0.z, t1.z);

	p1->x = KUT_MAX(t0.x, t1.x);
	p1->y = KUT_MAX(t0.y, t1.y);
	p1->z = KUT_MAX(t0.z, t1.z);
}

void vec3_order(Vec3 *p0, Vec3 *p1)
{
	Vec3 t0 = *p0;
	Vec3 t1 = *p1;
	
	p0->x = KUT_MIN(t0.x, t1.x);
	p0->y = KUT_MIN(t0.y, t1.y);
	p0->z = KUT_MIN(t0.z, t1.z);

	p1->x = KUT_MAX(t0.x, t1.x);
	p1->y = KUT_MAX(t0.y, t1.y);
	p1->z = KUT_MAX(t0.z, t1.z);
}

Vec2 line_line_intersect_point(Vec2 p0, Vec2 p1, Vec2 q0, Vec2 q1)
{
	float pa = p1.x - p0.x;
	float pb = p1.y - p0.y;

	// using Y = MX + B to solve
	// MX - Y + B = 0
	
	// find M where M = (Y1-Y0)/(X1-X0)
	float mp = (p1.y - p0.y) / (p1.x - p0.x);
	float mq = (q1.y - q0.y) / (q1.x - q0.x);

	// find B, where B = Y - MX
	float bp = p1.y - mp * p1.x;
	float bq = q1.y - mq * q1.x;


	// solve for X when Yp = Yq
	// Y = Mp * X + Bp
	// Y = Mq * X + Bq
	// Mp * X + Bp = Mq *X + Bq
	// (Mp * X) - (Mq * X) = Bq - Bp
	// X * (Mp - Mq) = Bq - Bp
	// X = (Bq - Bp) / (Mp - Mq)
	float x = (mp - mq) / (bq - bp);
	
	// knowing x, solve Y
	// Y = Mp * X + Bp;
	float y = mp * x + bp;

	return vec2(x, y);

	//todo: upport for m == infinite
}

bool circle_circle_intersect(Vec2 p0, float r0, Vec2 p1, float r1)
{
	float dx = p1.x - p0.x;
	float dy = p1.y - p0.y;

	float d2 = dx * dx + dy * dy;
	float r2 = (r1 + r2) * (r1 + r2);

	return (d2 <= r2) ? true : false;
}


bool line_circle_collision(Vec2 p0, Vec2 p1, Vec2 c, float r, Vec2 *intersect)
{
    Vec2 a = vec2_sub(c, p0);
    Vec2 b = vec2_sub(p1, p0);
    float a_dot_b = vec2_dot_product(a, b);
    float b_dot_b = vec2_dot_product(b, b); // b^2

    // intersection point = P0 + B(A dot B)/ |B|^2
    Vec2 i = vec2_mulf(b, a_dot_b);
    i = vec2_divf(i, b_dot_b);
    i = vec2_add(p0, i);

    Vec2 dd = vec2_sub(c, i);

    float d2 = vec2_dot_product(dd, dd);
    float r2 = r * r;


    if(d2 < r2)
    {
        *intersect = i;
        return true;
        // collision
    }

    return false;
}

float line_slope(Vec2 p0, Vec2 p1)
{
	Vec2 p;
	p.x = p1.x - p0.x;
	p.y = p1.y - p0.y;

	// todo: check for infinite
	return p.y / p.x;
}

float line_normal_slope(Vec2 p0, Vec2 p1)
{
	Vec2 p;
	p.x = p1.x - p0.x;
	p.y = p1.y - p0.y;

	// todo: check for infinite
	
	return -p.x / p.y;
}

bool point_line_bounce(Vec2 c0, Vec2 vel, Vec2 p0, Vec2 p1, Vec2 *p_intersect, Vec2 *c_out, Vec2 *dir)
{
  Vec2 p = vec2_sub(p1, p0);
  Vec2 p_unit = vec2_normal(p);
  Vec2 c1 = vec2_add(c0, vel);
  Vec2 c = vec2_sub(c1, c0);
  Vec2 c_unit = vec2_normal(c);

  // find b points in y = mx + b
  // b = y - mx
  // todo: what if slopes are infinite
  
  float m_c = (c1.y - c0.y) / (c1.x - c0.x);
  float m_p = (p1.y - p0.y) / (p1.x - p0.x);
  
  float b_c = c0.y - m_c * c0.x;
  float b_p = p0.y - m_p * p0.x;

  // solve for intercept where 
  // y = m_c * x + b_c and y = m_p * x + b_p
  // x = (y_c - b_c) / m_c
  // plug in x into y = m_p + b_p
  // and you eventually end up with
  // y = ((m_p * b_c)/ m_c + b_p) / (1 - m_p / m_c)
  // set m_r as a ratio for m_p / m_c
  // y = (m_r * b_c + b_p) / (1 - m_r)
  float m_r = m_p / m_c;

  Vec2 i; // i is for intercept
  i.y = (m_r * b_c + b_p) / (1 - m_r);
  i.x = (i.y - b_c) / m_c;

  Vec2 cmin;
  Vec2 cmax;
  Vec2 pmin;
  Vec2 pmax;

  cmin.x = KUT_MIN(c0.x, c1.x);
  cmax.x = KUT_MAX(c0.x, c1.x);
  cmin.y = KUT_MIN(c0.y, c1.y);
  cmax.y = KUT_MAX(c0.y, c1.y);
  pmin.x = KUT_MIN(p0.x, p1.x);
  pmax.x = KUT_MAX(p0.x, p1.x);
  pmin.y = KUT_MIN(p0.y, p1.y);
  pmax.y = KUT_MAX(p0.y, p1.y);

  *c_out = c1;
  *dir = vec2_normal(vel); //todo: normalize

  if(i.x < cmin.x)
      return false;
  if(i.x > cmax.x)
      return false;
  if(i.x < pmin.x)
      return false;
  if(i.x > pmax.x)
      return false;

  if(i.y < cmin.y)
      return false;
  if(i.y > cmax.y)
      return false;
  if(i.y < pmin.y)
      return false;
  if(i.y > pmax.y)
      return false;


  Vec2 q = vec2_sub(c1, i);
  float l = vec2_dot_product(q, p_unit);
  Vec2 s = vec2_mulf(p_unit, l);
  Vec2 t = vec2_add(i, s);

  Vec2 a = vec2_sub(t, c1);
  Vec2 a2 = vec2_mulf(a, 2.0f);

  *c_out = vec2_add(c1, a2);
  *dir = vec2_normal(vec2_sub(*c_out, i));
  *p_intersect = i;

  //printf("c0:%f,%f, c1:%f,%f cu:%f,%f l:%f s:%f,%f, t:%f,%f a:%f,%f\n",
  //        c0.x, c0.y, c1.x, c1.y, c_unit.x, c_unit.y, l, s.x, s.y, t.x, t.y, a.x, a.y);

  return true;
}
