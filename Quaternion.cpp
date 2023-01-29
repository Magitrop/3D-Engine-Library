#include "pch.h"
#include "Quaternion.h"
#include <algorithm>
#include <iostream>

Vector3 operator * (Quaternion q, Vector3 v)
{
	float num = q.x * 2;
	float num2 = q.y * 2;
	float num3 = q.z * 2;
	float num4 = q.x * num;
	float num5 = q.y * num2;
	float num6 = q.z * num3;
	float num7 = q.x * num2;
	float num8 = q.x * num3;
	float num9 = q.y * num3;
	float num10 = q.w * num;
	float num11 = q.w * num2;
	float num12 = q.w * num3;

	Vector3 result;
	result.x = (1 - (num5 + num6)) * v.x + (num7 - num12) * v.y + (num8 + num11) * v.z;
	result.y = (num7 + num12) * v.x + (1 - (num4 + num6)) * v.y + (num9 - num10) * v.z;
	result.z = (num8 - num11) * v.x + (num9 + num10) * v.y + (1 - (num4 + num5)) * v.z;
    return result;
}

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
	double cr = cos(glm::radians(x) * 0.5);
	double sr = sin(glm::radians(x) * 0.5);
	double cp = cos(glm::radians(y) * 0.5);
	double sp = sin(glm::radians(y) * 0.5);
	double cy = cos(glm::radians(z) * 0.5);
	double sy = sin(glm::radians(z) * 0.5);

	double _w = cr * cp * cy + sr * sp * sy;
	double _x = sr * cp * cy - cr * sp * sy;
	double _y = cr * sp * cy + sr * cp * sy;
	double _z = cr * cp * sy - sr * sp * cy;
	return Quaternion(_x, _y, _z, _w);
}

Quaternion Quaternion::FromEuler(Vector3 v)
{
	double cr = cos(glm::radians(v.x) * 0.5);
	double sr = sin(glm::radians(v.x) * 0.5);
	double cp = cos(glm::radians(v.y) * 0.5);
	double sp = sin(glm::radians(v.y) * 0.5);
	double cy = cos(glm::radians(v.z) * 0.5);
	double sy = sin(glm::radians(v.z) * 0.5);

	double _w = cr * cp * cy + sr * sp * sy;
	double _x = sr * cp * cy - cr * sp * sy;
	double _y = cr * sp * cy + sr * cp * sy;
	double _z = cr * cp * sy - sr * sp * cy;
    return Quaternion(_x, _y, _z, _w);
}

Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 up)
{
    forward = glm::normalize(forward);

    Vector3 vector = forward;
    Vector3 vector2 = glm::normalize(Vectors::Cross(up, vector));
    Vector3 vector3 = Vectors::Cross(vector, vector2);
    float m00 = vector2.x;
    float m01 = vector2.y;
    float m02 = vector2.z;
    float m10 = vector3.x;
    float m11 = vector3.y;
    float m12 = vector3.z;
    float m20 = vector.x;
    float m21 = vector.y;
    float m22 = vector.z;
    float num8 = (m00 + m11) + m22;

    Quaternion quaternion;
    if (num8 > 0)
    {
        float num = sqrt(num8 + 1);
        quaternion.w = num * 0.5f;
        num = 0.5f / num;
        quaternion.x = (m12 - m21) * num;
        quaternion.y = (m20 - m02) * num;
        quaternion.z = (m01 - m10) * num;
        return quaternion;
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = sqrt(((1 + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        quaternion.x = 0.5f * num7;
        quaternion.y = (m01 + m10) * num4;
        quaternion.z = (m02 + m20) * num4;
        quaternion.w = (m12 - m21) * num4;
        return quaternion;
    }
    if (m11 > m22)
    {
        float num6 = sqrt(((1 + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        quaternion.x = (m10 + m01) * num3;
        quaternion.y = 0.5f * num6;
        quaternion.z = (m21 + m12) * num3;
        quaternion.w = (m20 - m02) * num3;
        return quaternion;
    }
    float num5 = sqrt(((1 + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    quaternion.x = (m20 + m02) * num2;
    quaternion.y = (m21 + m12) * num2;
    quaternion.z = 0.5f * num5;
    quaternion.w = (m01 - m10) * num2;
    return quaternion;
}

Vector3 Quaternion::ToEuler()
{
    Vector3 v(0);

    float t0 = 2.0f * (w * x + y * z);
    float t1 = 1.0f - 2.0f * (x * x + y * y);
    v.x = glm::degrees(atan2(t0, t1));

    float t2 = std::clamp(2.0f * (w * y - z * x), -1.f, 1.f);
    v.y = glm::degrees(asin(t2));

    float t3 = 2.0f * (w * z + x * y);
    float t4 = 1.0f - 2.0f * (y * y + z * z);
    v.z = glm::degrees(atan2(t3, t4));

    return v;
}
