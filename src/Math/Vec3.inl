#pragma once

#include "assert.h"
#include <string>
#include <cmath>

#define VEC3_STRING(vec) "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ") "

template <typename T = float>
struct Vec3T {
	union {
		struct { T x, y, z; };
	};

	Vec3T(T X = 0, T Y = 0, T Z = 0): x(X), y(Y), z(Z) {}
	
	std::string GetString() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ") ";
	}
	
	Vec3T<T> normalize() const {
		float mag = std::sqrt(x * x + y * y + z * z);
		return Vec3T<T>(x/mag, y/mag, z/mag);
	}

	Vec3T<T> cross(const Vec3T &) const;

	bool operator!=(const Vec3T &) const;
	bool operator==(const Vec3T &) const;
	bool operator>(const Vec3T &) const;
	bool operator<(const Vec3T &) const;
	bool operator>=(const Vec3T &) const;
	bool operator<=(const Vec3T &) const;
	
	Vec3T<T> operator+(const Vec3T &) const;
	Vec3T<T> operator-(const Vec3T &) const;
	Vec3T<T> operator*(const Vec3T &) const;
	Vec3T<T> operator/(const Vec3T &) const;
	Vec3T<T> operator&(const Vec3T &) const;
	Vec3T<T> operator|(const Vec3T &) const;
	Vec3T<T> operator^(const Vec3T &) const;

	void operator=(const Vec3T &);
	void operator+=(const Vec3T &);
	void operator-=(const Vec3T &);
	void operator*=(const Vec3T &);
	void operator/=(const Vec3T &);
	void operator&=(const Vec3T &);
	void operator|=(const Vec3T &);
	void operator^=(const Vec3T &);
	
	template <typename R> bool operator!=(const R &) const;
	template <typename R> bool operator==(const R &) const;
	template <typename R> bool operator>(const R &) const;
	template <typename R> bool operator<(const R &) const;
	template <typename R> bool operator>=(const R &) const;
	template <typename R> bool operator<=(const R &) const;
	
	template <typename R> Vec3T<T> operator+(const R &) const;
	template <typename R> Vec3T<T> operator-(const R &) const;
	template <typename R> Vec3T<T> operator*(const R &) const;
	template <typename R> Vec3T<T> operator/(const R &) const;
	template <typename R> Vec3T<T> operator&(const R &) const;
	template <typename R> Vec3T<T> operator|(const R &) const;
	template <typename R> Vec3T<T> operator^(const R &) const;

	template <typename R> void operator=(const R &);
	template <typename R> void operator+=(const R &);
	template <typename R> void operator-=(const R &);
	template <typename R> void operator*=(const R &);
	template <typename R> void operator/=(const R &);
	template <typename R> void operator&=(const R &);
	template <typename R> void operator|=(const R &);
	template <typename R> void operator^=(const R &);

	T& operator[](int i);
};

template <typename T>
inline T& Vec3T<T>::operator[](int i) {
	assert(0 <= i && i < 3);

	switch(i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			assert(false);
	}
}

template <typename T> inline void Vec3T<T>::operator=(const Vec3T &tmp) {
	x = tmp.x;
	y = tmp.y;
	z = tmp.z;
}

template <typename T> void Vec3T<T>::operator+=(const Vec3T &tmp) {
	x += tmp.x;
	y += tmp.y;
	z += tmp.z;
}
template <typename T> void Vec3T<T>::operator-=(const Vec3T &tmp) {
	x -= tmp.x;
	y -= tmp.y;
	z -= tmp.z;
}
template <typename T> void Vec3T<T>::operator*=(const Vec3T &tmp) {
	x *= tmp.x;
	y *= tmp.y;
	z *= tmp.z;
}
template <typename T> void Vec3T<T>::operator/=(const Vec3T &tmp) {
	x /= tmp.x;
	y /= tmp.y;
	z /= tmp.z;
}
template <typename T> void Vec3T<T>::operator&=(const Vec3T &tmp) {
	x &= tmp.x;
	y &= tmp.y;
	z &= tmp.z;
}
template <typename T> void Vec3T<T>::operator|=(const Vec3T &tmp) {
	x |= tmp.x;
	y |= tmp.y;
	z |= tmp.z;
}
template <typename T> void Vec3T<T>::operator^=(const Vec3T &tmp) {
	x ^= tmp.x;
	y ^= tmp.y;
	z ^= tmp.z;
}

template <typename T> inline Vec3T<T> Vec3T<T>::cross(const Vec3T &tmp) const {
	return Vec3T(y * tmp.z - z * tmp.y, z * tmp.x - x * tmp.z, x * tmp.y - y * tmp.x);
}

template <typename T> bool Vec3T<T>::operator!=(const Vec3T &tmp) const {
	return (x != tmp.x && y != tmp.y && z != tmp.z);
}
template <typename T> bool Vec3T<T>::operator==(const Vec3T &tmp) const {
	return (x == tmp.x && y == tmp.y && z == tmp.z);
}
template <typename T> bool Vec3T<T>::operator>(const Vec3T &tmp) const {
	return (x > tmp.x && y > tmp.y && z > tmp.z);
}
template <typename T> bool Vec3T<T>::operator<(const Vec3T &tmp) const {
	return (x < tmp.x && y < tmp.y && z < tmp.z);
}
template <typename T> bool Vec3T<T>::operator>=(const Vec3T &tmp) const {
	return (x >= tmp.x && y >= tmp.y && z >= tmp.z);
}
template <typename T> bool Vec3T<T>::operator<=(const Vec3T &tmp) const {
	return (x <= tmp.x && y <= tmp.y && z <= tmp.z);
}

template <typename T> Vec3T<T> Vec3T<T>::operator+(const Vec3T &tmp) const {
	return Vec3T(x + tmp.x, y + tmp.y, z + tmp.z);
}
template <typename T> Vec3T<T> Vec3T<T>::operator-(const Vec3T &tmp) const {
	return Vec3T(x - tmp.x, y - tmp.y, z - tmp.z);
}
template <typename T> Vec3T<T> Vec3T<T>::operator*(const Vec3T &tmp) const {
	return Vec3T(x * tmp.x, y * tmp.y, z * tmp.z);
}
template <typename T> Vec3T<T> Vec3T<T>::operator/(const Vec3T &tmp) const {
	return Vec3T(x / tmp.x, y / tmp.y, z / tmp.z);
}
template <typename T> Vec3T<T> Vec3T<T>::operator&(const Vec3T &tmp) const {
	return Vec3T(x & tmp.x, y & tmp.y, z & tmp.z);
}
template <typename T> Vec3T<T> Vec3T<T>::operator|(const Vec3T &tmp) const {
	return Vec3T(x | tmp.x, y | tmp.y, z | tmp.z);
}
template <typename T> Vec3T<T> Vec3T<T>::operator^(const Vec3T &tmp) const {
	return Vec3T(x ^ tmp.x, y ^ tmp.y, z ^ tmp.z);
}

template <typename T> template <typename R> void Vec3T<T>::operator=(const R &tmp) {
	x = tmp;
	y = tmp;
	z = tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator+=(const R &tmp) {
	x += tmp;
	y += tmp;
	z += tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator-=(const R &tmp) {
	x -= tmp;
	y -= tmp;
	z -= tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator*=(const R &tmp) {
	x *= tmp;
	y *= tmp;
	z *= tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator/=(const R &tmp) {
	x /= tmp;
	y /= tmp;
	z /= tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator&=(const R &tmp) {
	x &= tmp;
	y &= tmp;
	z &= tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator|=(const R &tmp) {
	x |= tmp;
	y |= tmp;
	z |= tmp;
}
template <typename T> template <typename R> void Vec3T<T>::operator^=(const R &tmp) {
	x ^= tmp;
	y ^= tmp;
	z ^= tmp;
}

template <typename T> template <typename R> bool Vec3T<T>::operator!=(const R &tmp) const {
	return (x != tmp && y != tmp && z != tmp);
}
template <typename T> template <typename R> bool Vec3T<T>::operator==(const R &tmp) const {
	return (x == tmp && y == tmp && z == tmp);
}
template <typename T> template <typename R> bool Vec3T<T>::operator>(const R &tmp) const {
	return (x > tmp && y > tmp && z > tmp);
}
template <typename T> template <typename R> bool Vec3T<T>::operator<(const R &tmp) const {
	return (x < tmp && y < tmp && z < tmp);
}
template <typename T> template <typename R> bool Vec3T<T>::operator>=(const R &tmp) const {
	return (x >= tmp && y >= tmp && z >= tmp);
}
template <typename T> template <typename R> bool Vec3T<T>::operator<=(const R &tmp) const {
	return (x <= tmp && y <= tmp && z <= tmp);
}

template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator+(const R &tmp) const {
	return Vec3T(x + tmp, y + tmp, z + tmp);
}
template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator-(const R &tmp) const {
	return Vec3T(x - tmp, y - tmp, z - tmp);
}
template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator*(const R &tmp) const {
	return Vec3T(x * tmp, y * tmp, z * tmp);
}
template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator/(const R &tmp) const {
	return Vec3T(x / tmp, y / tmp, z / tmp);
}
template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator&(const R &tmp) const {
	return Vec3T(x & tmp, y & tmp, z & tmp);
}
template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator|(const R &tmp) const {
	return Vec3T(x | tmp, y | tmp, z | tmp);
}
template <typename T> template <typename R> Vec3T<T> Vec3T<T>::operator^(const R &tmp) const {
	return Vec3T(x ^ tmp, y ^ tmp, z ^ tmp);
}

using Vec3 = Vec3T<float>;
using Vec3d = Vec3T<double>;
using IVec3 = Vec3T<int>;
using Vec3ui = Vec3T<unsigned int>;
