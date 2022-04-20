#pragma once

#include "assert.h"

template <typename T = float>
struct Vec4T {
	union {
		struct { T x, y, z, w; };
	};

	Vec4T(T X = 0, T Y = 0, T Z = 0, T W = 0): x(X), y(Y), z(Z), w(W) {}
	
	bool operator!=(const Vec4T &) const;
	bool operator==(const Vec4T &) const;
	bool operator>(const Vec4T &) const;
	bool operator<(const Vec4T &) const;
	bool operator>=(const Vec4T &) const;
	bool operator<=(const Vec4T &) const;
	
	Vec4T<T> operator+(const Vec4T &) const;
	Vec4T<T> operator-(const Vec4T &) const;
	Vec4T<T> operator*(const Vec4T &) const;
	Vec4T<T> operator/(const Vec4T &) const;
	Vec4T<T> operator&(const Vec4T &) const;
	Vec4T<T> operator|(const Vec4T &) const;
	Vec4T<T> operator^(const Vec4T &) const;

	void operator=(const Vec4T &);

	void operator+=(const Vec4T &);
	void operator-=(const Vec4T &);
	void operator*=(const Vec4T &);
	void operator/=(const Vec4T &);
	void operator&=(const Vec4T &);
	void operator|=(const Vec4T &);
	void operator^=(const Vec4T &);

	T& operator[](int i);
};

template <typename T>
inline T& Vec4T<T>::operator[](int i) {
	assert(0 <= i && i < 4);

	switch(i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
	}
}

template <typename T> inline void Vec4T<T>::operator=(const Vec4T &tmp) {
	x = tmp.x;
	y = tmp.y;
	z = tmp.z;
	w = tmp.w;
}

template <typename T> void Vec4T<T>::operator+=(const Vec4T &tmp) {
	x += tmp.x;
	y += tmp.y;
	z += tmp.z;
	w += tmp.w;
}
template <typename T> void Vec4T<T>::operator-=(const Vec4T &tmp) {
	x -= tmp.x;
	y -= tmp.y;
	z -= tmp.z;
	w -= tmp.w;
}
template <typename T> void Vec4T<T>::operator*=(const Vec4T &tmp) {
	x *= tmp.x;
	y *= tmp.y;
	z *= tmp.z;
	w *= tmp.w;
}
template <typename T> void Vec4T<T>::operator/=(const Vec4T &tmp) {
	x /= tmp.x;
	y /= tmp.y;
	z /= tmp.z;
	w /= tmp.w;
}
template <typename T> void Vec4T<T>::operator&=(const Vec4T &tmp) {
	x &= tmp.x;
	y &= tmp.y;
	z &= tmp.z;
	w &= tmp.w;
}
template <typename T> void Vec4T<T>::operator|=(const Vec4T &tmp) {
	x |= tmp.x;
	y |= tmp.y;
	z |= tmp.z;
	w |= tmp.w;
}
template <typename T> void Vec4T<T>::operator^=(const Vec4T &tmp) {
	x ^= tmp.x;
	y ^= tmp.y;
	z ^= tmp.z;
	w ^= tmp.w;
}

template <typename T> bool Vec4T<T>::operator!=(const Vec4T &tmp) const {
	return (x != tmp.x && y != tmp.y && z != tmp.z && w != tmp.w);
}
template <typename T> bool Vec4T<T>::operator==(const Vec4T &tmp) const {
	return (x == tmp.x && y == tmp.y && z == tmp.z && w == tmp.w);
}
template <typename T> bool Vec4T<T>::operator>(const Vec4T &tmp) const {
	return (x > tmp.x && y > tmp.y && z > tmp.z && w > tmp.w);
}
template <typename T> bool Vec4T<T>::operator<(const Vec4T &tmp) const {
	return (x < tmp.x && y < tmp.y && z < tmp.z && w < tmp.w);
}
template <typename T> bool Vec4T<T>::operator>=(const Vec4T &tmp) const {
	return (x >= tmp.x && y >= tmp.y && z >= tmp.z && w >= tmp.w);
}
template <typename T> bool Vec4T<T>::operator<=(const Vec4T &tmp) const {
	return (x <= tmp.x && y <= tmp.y && z <= tmp.z && w <= tmp.w);
}

template <typename T> Vec4T<T> Vec4T<T>::operator+(const Vec4T &tmp) const {
	return Vec4T(x + tmp.x, y + tmp.y, z + tmp.z, w + tmp.w);
}
template <typename T> Vec4T<T> Vec4T<T>::operator-(const Vec4T &tmp) const {
	return Vec4T(x - tmp.x, y - tmp.y, z - tmp.z, w - tmp.w);
}
template <typename T> Vec4T<T> Vec4T<T>::operator*(const Vec4T &tmp) const {
	return Vec4T(x * tmp.x, y * tmp.y, z * tmp.z, w * tmp.w);
}
template <typename T> Vec4T<T> Vec4T<T>::operator/(const Vec4T &tmp) const {
	return Vec4T(x / tmp.x, y / tmp.y, z / tmp.z, w / tmp.w);
}
template <typename T> Vec4T<T> Vec4T<T>::operator&(const Vec4T &tmp) const {
	return Vec4T(x & tmp.x, y & tmp.y, z & tmp.z, w & tmp.w);
}
template <typename T> Vec4T<T> Vec4T<T>::operator|(const Vec4T &tmp) const {
	return Vec4T(x | tmp.x, y | tmp.y, z | tmp.z, w | tmp.w);
}
template <typename T> Vec4T<T> Vec4T<T>::operator^(const Vec4T &tmp) const {
	return Vec4T(x ^ tmp.x, y ^ tmp.y, z ^ tmp.z, w ^ tmp.w);
}

using Vec4 = Vec4T<float>;
using Vec4d = Vec4T<double>;
using Vec4i = Vec4T<int>;
using Vec4ui = Vec4T<unsigned int>;
