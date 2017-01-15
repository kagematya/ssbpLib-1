﻿#pragma once
#include <algorithm>
#include <assert.h>

namespace ss{
/**
 * 定数
 */

static const double SS_PI = 3.14159265358979323846;
static const double SS_2PI = SS_PI * 2;
double SSRadToDeg(double rad);
double SSDegToRad(double deg);

void DebugPrintToConsole(const char *filename, int line, const char *format, ...);


/** 配列のサイズを返す */
template<class TYPE, size_t N>
size_t lengthof(const TYPE(&ar)[N]){
	return N;
}

/** valを[min:max]の範囲にしたものを返す */
template<class T>
T clamp(T val, T minVal, T maxVal){
	assert(minVal <= maxVal);
	return std::min(std::max(val, minVal), maxVal);
}

/** [minVal:maxVal)の範囲でループさせる。整数値向け */
template<class T>
T wrap(T val, T minVal, T maxVal){
	assert(minVal < maxVal);
	int n = (val - minVal) % (maxVal - minVal);
	return (n >= 0) ? (n + minVal) : (n + maxVal);
}

/** [minVal:maxVal)の範囲でループさせる。浮動小数点数向け */
template<class T>
T fwrap(T val, T minVal, T maxVal){
	assert(minVal < maxVal);
	double n = fmod(val - minVal, maxVal - minVal);
	return (n >= 0) ? (n + minVal) : (n + maxVal);
}


/** 線形補間 t[0:1] */
template <typename T>
T lerp(const T &from, const T &to, double t){
	T diff = to - from;
	return from + diff*t;
}


} //namespace ss



#define SS_SAFE_DELETE(p)           { delete (p); (p) = nullptr; }
#define SS_SAFE_DELETE_ARRAY(p)     { delete[] (p); (p) = nullptr; }


#ifdef _DEBUG
#define SS_LOG(...)					ss::DebugPrintToConsole(__FILE__, __LINE__, __VA_ARGS__)
#define SS_ASSERT(cond)				assert(cond)
#define SS_ASSERT_LOG(cond, ...)	{ SS_LOG(__VA_ARGS__); SS_ASSERT(cond); }
#else
#define SS_LOG(...)
#define SS_ASSERT(cond)				
#define SS_ASSERT_LOG(cond, ...)	
#endif