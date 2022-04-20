#pragma once

#include "assert.h"
#include <string>
#include <cmath>

template <typename T>
struct Vec2T {
	union {
		struct { T x, y; };
	};

	Vec2T(T X = 0, T Y = 0): x(X), y(Y) {}

	std::string GetString() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ") ";
	}
	
	Vec2T<T> normalize() const {
		float mag = std::sqrt(x * x + y * y);
		return Vec2T<T>(x/mag, y/mag);
	}

	bool operator!=(const Vec2T &) const;
	bool operator==(const Vec2T &) const;
	bool operator>(const Vec2T &) const;
	bool operator<(const Vec2T &) const;
	bool operator>=(const Vec2T &) const;
	bool operator<=(const Vec2T &) const;
	
	Vec2T<T> operator+(const Vec2T &) const;
	Vec2T<T> operator-(const Vec2T &) const;
	Vec2T<T> operator*(const Vec2T &) const;
	Vec2T<T> operator/(const Vec2T &) const;
	Vec2T<T> operator&(const Vec2T &) const;
	Vec2T<T> operator|(const Vec2T &) const;
	Vec2T<T> operator^(const Vec2T &) const;

	void operator=(const Vec2T &);
	void operator+=(const Vec2T &);
	void operator-=(const Vec2T &);
	void operator*=(const Vec2T &);
	void operator/=(const Vec2T &);
	void operator&=(const Vec2T &);
	void operator|=(const Vec2T &);
	void operator^=(const Vec2T &);
	
	template <typename R> bool operator!=(const R &) const;
	template <typename R> bool operator==(const R &) const;
	template <typename R> bool operator>(const R &) const;
	template <typename R> bool operator<(const R &) const;
	template <typename R> bool operator>=(const R &) const;
	template <typename R> bool operator<=(const R &) const;
	
	template <typename R> Vec2T<T> operator+(const R &) const;
	template <typename R> Vec2T<T> operator-(const R &) const;
	template <typename R> Vec2T<T> operator*(const R &) const;
	template <typename R> Vec2T<T> operator/(const R &) const;
	template <typename R> Vec2T<T> operator&(const R &) const;
	template <typename R> Vec2T<T> operator|(const R &) const;
	template <typename R> Vec2T<T> operator^(const R &) const;

	void operator=(const T &);
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
inline T& Vec2T<T>::operator[](int i) {
	assert(0 <= i && i < 2);

	switch(i) {
		case 0:
			return x;
		case 1:
			return y;
	}
}

template <typename T> inline void Vec2T<T>::operator=(const Vec2T &tmp) {
	x = tmp.x;
	y = tmp.y;
}

template <typename T> void Vec2T<T>::operator+=(const Vec2T &tmp) {
	x += tmp.x;
	y += tmp.y;
}
template <typename T> void Vec2T<T>::operator-=(const Vec2T &tmp) {
	x -= tmp.x;
	y -= tmp.y;
}
template <typename T> void Vec2T<T>::operator*=(const Vec2T &tmp) {
	x *= tmp.x;
	y *= tmp.y;
}
template <typename T> void Vec2T<T>::operator/=(const Vec2T &tmp) {
	x /= tmp.x;
	y /= tmp.y;
}
template <typename T> void Vec2T<T>::operator&=(const Vec2T &tmp) {
	x &= tmp.x;
	y &= tmp.y;
}
template <typename T> void Vec2T<T>::operator|=(const Vec2T &tmp) {
	x |= tmp.x;
	y |= tmp.y;
}
template <typename T> void Vec2T<T>::operator^=(const Vec2T &tmp) {
	x ^= tmp.x;
	y ^= tmp.y;
}

template <typename T> bool Vec2T<T>::operator!=(const Vec2T &tmp) const {
	return (x != tmp.x && y != tmp.y);
}
template <typename T> bool Vec2T<T>::operator==(const Vec2T &tmp) const {
	return (x == tmp.x && y == tmp.y);
}
template <typename T> bool Vec2T<T>::operator>(const Vec2T &tmp) const {
	return (x > tmp.x && y > tmp.y);
}
template <typename T> bool Vec2T<T>::operator<(const Vec2T &tmp) const {
	return (x < tmp.x && y < tmp.y);
}
template <typename T> bool Vec2T<T>::operator>=(const Vec2T &tmp) const {
	return (x >= tmp.x && y >= tmp.y);
}
template <typename T> bool Vec2T<T>::operator<=(const Vec2T &tmp) const {
	return (x <= tmp.x && y <= tmp.y);
}

template <typename T> Vec2T<T> Vec2T<T>::operator+(const Vec2T &tmp) const {
	return Vec2T(x + tmp.x, y + tmp.y);
}
template <typename T> Vec2T<T> Vec2T<T>::operator-(const Vec2T &tmp) const {
	return Vec2T(x - tmp.x, y - tmp.y);
}
template <typename T> Vec2T<T> Vec2T<T>::operator*(const Vec2T &tmp) const {
	return Vec2T(x * tmp.x, y * tmp.y);
}
template <typename T> Vec2T<T> Vec2T<T>::operator/(const Vec2T &tmp) const {
	return Vec2T(x / tmp.x, y / tmp.y);
}
template <typename T> Vec2T<T> Vec2T<T>::operator&(const Vec2T &tmp) const {
	return Vec2T(x & tmp.x, y & tmp.y);
}
template <typename T> Vec2T<T> Vec2T<T>::operator|(const Vec2T &tmp) const {
	return Vec2T(x | tmp.x, y | tmp.y);
}
template <typename T> Vec2T<T> Vec2T<T>::operator^(const Vec2T &tmp) const {
	return Vec2T(x ^ tmp.x, y ^ tmp.y);
}

template <typename T> void Vec2T<T>::operator=(const T &tmp) {
	x = tmp;
	y = tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator+=(const R &tmp) {
	x += tmp;
	y += tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator-=(const R &tmp) {
	x -= tmp;
	y -= tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator*=(const R &tmp) {
	x *= tmp;
	y *= tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator/=(const R &tmp) {
	x /= tmp;
	y /= tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator&=(const R &tmp) {
	x &= tmp;
	y &= tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator|=(const R &tmp) {
	x |= tmp;
	y |= tmp;
}
template <typename T> template <typename R> void Vec2T<T>::operator^=(const R &tmp) {
	x ^= tmp;
	y ^= tmp;
}

template <typename T> template <typename R> bool Vec2T<T>::operator!=(const R &tmp) const {
	return (x != tmp && y != tmp);
}
template <typename T> template <typename R> bool Vec2T<T>::operator==(const R &tmp) const {
	return (x == tmp && y == tmp);
}
template <typename T> template <typename R> bool Vec2T<T>::operator>(const R &tmp) const {
	return (x > tmp && y > tmp);
}
template <typename T> template <typename R> bool Vec2T<T>::operator<(const R &tmp) const {
	return (x < tmp && y < tmp);
}
template <typename T> template <typename R> bool Vec2T<T>::operator>=(const R &tmp) const {
	return (x >= tmp && y >= tmp);
}
template <typename T> template <typename R> bool Vec2T<T>::operator<=(const R &tmp) const {
	return (x <= tmp && y <= tmp);
}

template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator+(const R &tmp) const {
	return Vec2T(x + tmp, y + tmp);
}
template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator-(const R &tmp) const {
	return Vec2T(x - tmp, y - tmp);
}
template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator*(const R &tmp) const {
	return Vec2T(x * tmp, y * tmp);
}
template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator/(const R &tmp) const {
	return Vec2T(x / tmp, y / tmp);
}
template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator&(const R &tmp) const {
	return Vec2T(x & tmp, y & tmp);
}
template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator|(const R &tmp) const {
	return Vec2T(x | tmp, y | tmp);
}
template <typename T> template <typename R> Vec2T<T> Vec2T<T>::operator^(const R &tmp) const {
	return Vec2T(x ^ tmp, y ^ tmp);
}

using Vec2 = Vec2T<float>;
using Vec2d = Vec2T<double>;
using IVec2 = Vec2T<int>;
using Vec2ui = Vec2T<unsigned int>;