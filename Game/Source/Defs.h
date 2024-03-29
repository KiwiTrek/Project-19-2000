#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdio.h>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != nullptr )		\
		{					  \
	  delete x;			\
	  x = nullptr;			  \
		}					  \
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != nullptr )			  \
		{							\
	  delete[] x;				\
	  x = nullptr;					\
		}							\
							  \
	}

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Bits
// Sets the indexed bit to 1
inline unsigned char SetBit(unsigned char bitfield, uint index)
{
	bitfield = bitfield | (1 << index);
	return bitfield;
}

// Clears the indexed bit to 0
inline unsigned char ClearBit(unsigned char bitfield, uint index)
{
	bitfield = bitfield & ~(1 << index);
	return bitfield;
}

// Switches the indexed bit to the other state
inline unsigned char ToggleBit(unsigned char bitfield, uint index)
{
	bitfield = bitfield ^ 1 << index;
	return bitfield;

}

// Standard string size
#define SHORT_STR	 32
#define MID_STR		255
#define HUGE_STR   8192
#define TEXT_LEN	512

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) //LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

// Maths calculations
#define PI 3.14159265359f
#define DEG_2_RAD(a) a * PI / 180.0f
#define RAD_2_DEG(a) a * 180.0f / PI

#endif	// __DEFS_H__