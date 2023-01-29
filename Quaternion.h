#pragma once
#include "Vectors.h"

class Quaternion
{
public:
	Quaternion(
		float _x = 0, 
		float _y = 0, 
		float _z = 0, 
		float _w = 1) : 
		x(_x), y(_y), z(_z), w(_w) {}

	float x;
	float y;
	float z;
	float w;

	static Quaternion FromEuler(Vector3 v);
	static Quaternion FromEuler(float x, float y, float z);
	static Quaternion LookRotation(Vector3 forward, Vector3 up);

	Vector3 ToEuler();
};

Vector3 operator * (Quaternion q, Vector3 v);
