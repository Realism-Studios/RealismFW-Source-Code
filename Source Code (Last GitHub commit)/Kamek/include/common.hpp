#ifndef __KAMEK_COMMON_H
#define __KAMEK_COMMON_H


#define GEKKO

/* Common Number Types */
typedef signed long long s64;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef float f32;
typedef double f64;

typedef volatile s64 vs64;
typedef volatile s32 vs32;
typedef volatile s16 vs16;
typedef volatile s8 vs8;

typedef volatile u64 vu64;
typedef volatile u32 vu32;
typedef volatile u16 vu16;
typedef volatile u8 vu8;

typedef volatile f64 vf64;
typedef volatile f32 vf32;

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef unsigned long size_t;

typedef char* Ptr; // DUMB

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define NULL 0

/* Structures */
typedef struct { f32 x, y; } VEC2, Vec2, *Vec2Ptr, Point2d, *Point2dPtr;
typedef struct { f32 x, y, z; } VEC3, Vec, Vec3, *VecPtr, Point3d, *Point3dPtr;
typedef struct { s16 x; s16 y; s16 z; }S16Vec, *S16VecPtr;

typedef struct { f32 frame, value, slope; } HermiteKey;

extern "C" const char * strrchr ( const char * str, int character );
extern "C" int strcmp ( const char * str1, const char * str2 );

extern "C" char* strstr(char* str1, const char* str2);
extern "C" int atoi(const char* str);

#include "sdk/mtx.hpp"


// Stop the auto completion from whining
#ifdef __CLANG
inline void *operator new(unsigned int size, void *ptr) { return ptr; }
float abs(float value);
double abs(double value);
#endif
#ifndef __CLANG
inline void *operator new(size_t size, void *ptr) { return ptr; }

inline float abs(float value) {
	return __fabs(value);
}
inline double abs(double value) {
	return __fabs(value);
}
#endif


struct tree_node {
	tree_node *parent, *child, *prev_sibling, *next_sibling;
	void *obj;
};

/* Virtual Function Helpers */
//#define VF_BEGIN(type, obj, id, vtable_offset) \
//	{ type __VFUNC = (((u32)(obj))+(vtable_offset));
#define VF_BEGIN(type, obj, id, vtable_offset) \
	{ type __VFUNC = ((type*)(*((void**)(((u32)(obj))+(vtable_offset)))))[(id)];

#define VF_CALL __VFUNC

#define VF_END }




/* Common Functions */

void PPCHalt();

void OSReport(const char *format, ...);
void OSPanic(const char *file, int line, const char *msg, ...);
u64 __OSGetSystemTime();

void OSReturnToDataManager();
void OSReturnToMenu();

void OSShutdownSystem();

int sprintf(char *buffer, const char *format, ...);
int snprintf(char *buffer, size_t buff_size, const char *format, ...);
char *strcat(char *destination, const char *source);
extern "C" void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);

void *AllocFromGameHeap1(u32 size);
void FreeFromGameHeap1(void *block);

float GetHermiteCurveValue(float current_frame, HermiteKey* keys, unsigned int key_count);

/* Archive */
/*#ifdef REGION_PAL
	#define ARC_TABLE ((*((void**)0x8042A318))+4)
	#define RAW_ARC_TABLE (*((void**)0x8042A318))
#endif

#ifdef REGION_NTSC
	#define ARC_TABLE ((*((void**)0x8042A038))+4)
	#define RAW_ARC_TABLE (*((void**)0x8042A038))
#endif

char *RetrieveFileFromArc(void *table, char *name, char *path);
char *RetrieveFileFromArcAlt(void *table, char *name, char *path);*/

extern void *ArchiveHeap; // PAL 0x8042A72C, NTSC 0x8042A44C

namespace nw4r { namespace math { float FrSqrt(float); }}
float sqrtf(float x) {
    return (x <= 0) ? 0.0f : x * nw4r::math::FrSqrt(x);
}

#ifdef __MWERKS__
	#define InfiniteLoop for (;;) { asm { nop } }
#else
	#define InfiniteLoop for (;;) { asm("nop"); }
#endif

extern "C" u32 CXGetUncompressedSize( const void *srcp );
extern "C" void CXUncompressLZ( const void *srcp, void *destp );

namespace nw4r { namespace db {
    void Exception_Printf_(const char *msg, ...);
	void* sException();
}}

// Pointer to data read out function
extern u8 (*GetCharPtr_)(const u8* buf);

// Actual function
extern u8 GetCharOnMem_(const u8* buf);

// Convert hex string to u32
extern u32 XStrToU32_(const u8* str);

#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))

void __construct_array(void *array, void *ctor, void *dtor, u32 size, u32 num); // 0x802DCC90


// hey Mr. Ushort-phobic, take a look at what i am...
struct OSFontHeader {
	u16 fontType;
	u16 firstChar, lastChar;
	u16 invalidChar;

	u16 ascent, descent;
	u16 width, leading;
	u16 cellWidth, cellHeight;

	u32 sheetSize;
	u16 sheetFormat; // GXTexFmt
	u16 sheetColumnCount, sheetRowCount;
	u16 sheetWidth, sheetHeight;

	u16 widthTableOffset;
	u32 sheetImageOffset;
	u32 sheetFullSize;

	u8 colorTable[4]; // GXColor?
};

void *OSAllocFromMEM1ArenaLo(u32 size, u32 align);

void *MEM1ArenaLo; // 0x804297F8
u32 OSLoadFont(OSFontHeader *fontData, void *MEM1ArenaLo); // NOT FOR FUCKING GAME FONTS

// you might wanna just do 640, 480 for this...
void ConfigureVideo(u16 width, u16 height);

void DCFlushRange(void *startAddr, u32 length);

void VIInit();
void VISetBlack(int black);
u32 VIGetRetraceCount();
void VIFlush();

#endif
