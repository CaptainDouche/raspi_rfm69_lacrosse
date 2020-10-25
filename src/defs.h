#ifndef _INCLUDE_DEFS_H_
#define _INCLUDE_DEFS_H_

// generic macros:

#define COMPILE_TIMESTAMP					__DATE__ "/" __TIME__

#define	ELEMSIZE(A)							(sizeof((A)[0]))
#define	ELEMCNT(A)							(sizeof(A) / ELEMSIZE(A))
#define	OFFSETOF(TYPE, ELEM)				((size_t)((char *)&((TYPE *) 0)->ELEM - (char*)((TYPE*)NULL)))
#define CMP(X,Y)							(((X) == (Y)) ? 0 : (((X) < (Y)) ? -1 : +1))
#define LITERAL_STRLEN(S)					(sizeof(S) - sizeof(char))
#define LEN(S)								LITERAL_STRLEN(S)
#define LENGTH(S)							LITERAL_STRLEN(S)
#define LEN_L(S)							LITERAL_STRLEN(S)
#define LENGTH_L(S)							LITERAL_STRLEN(S)
#define LLEN(S)								LITERAL_STRLEN(S)

#define ARRAY(TYPE, PTR, INDEX)				(((TYPE*)(PTR))[INDEX])

#define INDEX_TO_MASK(INDEX)				(1 << (INDEX))
#define BV(INDEX)							INDEX_TO_MASK(INDEX)

#define BIT_GET(VAR, INDEX)					((VAR) & INDEX_TO_MASK(INDEX))
#define BIT_SET(VAR, INDEX)					(VAR) |= INDEX_TO_MASK(INDEX)
#define BIT_TOG(VAR, INDEX)					(VAR) ^= INDEX_TO_MASK(INDEX)
#define BIT_CLR(VAR, INDEX)					(VAR) &= ~INDEX_TO_MASK(INDEX)
#define BIT_CLEAR(VAR, INDEX)				BIT_CLR(VAR, INDEX)
#define BIT_ASSIGNFROM(VAL, VAR, INDEX)		{ if (VAL) { BIT_SET(VAR, INDEX); } else { BIT_CLR(VAR, INDEX); } } while (0) // http://gcc.gnu.org/onlinedocs/cpp/Swallowing-the-Semicolon.html

#define SET_CLEAR(SET)						(SET) = 0
#define SET_INCLUDE(SET, ELEMENTINDEX)		(SET) |= (1 << (ELEMENTINDEX))
#define SET_EXCLUDE(SET, ELEMENTINDEX)		(SET) &= ~(1 << (ELEMENTINDEX))
#define SET_CONTAINS(SET, ELEMENTINDEX)		(0 != ((SET) & (1 << (ELEMENTINDEX))))



#define MASK_LOWERBITS_8(WIDTH)				(~(0xff       << (WIDTH)))
#define MASK_LOWERBITS_16(WIDTH)			(~(0xffff     << (WIDTH)))
#define MASK_LOWERBITS_32(WIDTH)			(~(0xffffffff << (WIDTH)))

#define MASK_MIDBITS_8(OFFSET, WIDTH)		(MASK_LOWERBITS_8(WIDTH) << (OFFSET))
#define MASK_MIDBITS_16(OFFSET, WIDTH)		(MASK_LOWERBITS_16(WIDTH) << (OFFSET))
#define MASK_MIDBITS_32(OFFSET, WIDTH)		(MASK_LOWERBITS_32(WIDTH) << (OFFSET))

#define CRLF								"\r\n"


#define IN_RANGE(MIN, TEST, MAX)			(((MIN) <= (TEST)) && ((TEST) <= (MAX)))
#define OUTOF_RANGE(MIN, TEST, MAX)			(((MIN) > (TEST)) || ((TEST) > (MAX)))
#define TOLOWER(C)							((C) | 0x20)
#define TOUPPER(C)							((C) & ~0x20)
#define CHRTOINT(C)							((C) - (char)('0'))
#define CHRXTOINT(C)						(IN_RANGE('0', (C), '9') ? ((C) - (char)('0')) : (TOLOWER(C) - 'a' + 0x0a) )

#define STREQU(STR1, STR2)					(0 == strcmp(STR1, STR2))

#ifdef _LINUX
	#define STRIEQU(STR1, STR2)				(0 == strcasecmp(STR1, STR2))
#else
	#define STRIEQU(STR1, STR2)				(0 == stricmp(STR1, STR2))
#endif

#define STRSTARTS(STR1, LIT)				(0 == strncmp(STR1, LIT, strlen(LIT)))
#define STRSTARTS_C(STR1, LIT)				(0 == strncmp(STR1, LIT, LITERAL_STRLEN(LIT)))


#define BYTE1_16(I16)						((uint8_t)((I16) >> 8))
#define BYTE0_16(I16)						((uint8_t)((I16) & 0xff))
#define HIBYTE_16(I16)						BYTE1_16(I16) // just a synonym
#define LOBYTE_16(I16)						BYTE0_16(I16) // just a synonym
#define HIBYTE(I16)							BYTE1_16(I16) // just a synonym
#define LOBYTE(I16)							BYTE0_16(I16) // just a synonym
#define MKINT16_88(HI8, LO8)				((uint16_t)(((HI8) << 8) | (LO8)))
#define MKINT16(HI8, LO8)					MKINT16_88(HI8, LO8) // just a synonym	
#define MKINT32_8888(B3,B2,B1,B0)			( \
											(((uint32_t)(B3)) << 24) | \
											(((uint32_t)(B2)) << 16) | \
											(((uint32_t)(B1)) <<  8) | \
											(((uint32_t)(B0))      )   \
											)
#define MKINT32(B3,B2,B1,B0)				MKINT32_8888(B3,B2,B1,B0) // just a synonym							
#define HI16_32(I32)						((uint16_t)((I32) >> 16))
#define LO16_32(I32)						((uint16_t)((I32) & 0xffff))
#define MKINT32_1616(HI16, LO16)			((((uint32_t)(HI16)) << 16) | (uint32_t)(LO16))
#define GET_BYTE(I, N)						((uint8_t)(((I) >> (8*N)) & 0xff)) // GET_BYTE(0xaabbccdd, 2) => 0xbb 
#define BYTE3_32(I32)						GET_BYTE(I32, 3)
#define BYTE2_32(I32)						GET_BYTE(I32, 2)
#define BYTE1_32(I32)						GET_BYTE(I32, 1)
#define BYTE0_32(I32)						((uint8_t)((I32) & 0xff))

#define SWAPINT16(I16)						MKINT16(LOBYTE(I16), HIBYTE(I16))
#define SWAPINT32(I32)						MKINT32_1616(LOBYTE(I32), HIBYTE(I32))

#define MAX(A, B)							(((A) > (B)) ? (A) : (B))
#define MIN(A, B)							(((A) < (B)) ? (A) : (B))

#define HINIBBLE(I8)						((I8) >> 4) //(((I8) >> 4) & 0x0f)
#define LONIBBLE(I8)						((I8)  & 0x0f)

#define CHARS2UINT32(a,b,c,d)				( \
											(((uint32_t)(d)) << 24) | \
											(((uint32_t)(c)) << 16) | \
											(((uint32_t)(b)) <<  8) | \
											(((uint32_t)(a)) <<  0)   \
											)

#define CONCAT2(a, b)       				a ## b
#define CONCAT3(a, b, c)   					a ## b ## c
#define CONCAT4(a, b, c, d)   				a ## b ## c ## d
											
#define _S(MACRO)							#MACRO		// C preprocessor stringification
#define _SS(MACRO)							_S(MACRO) 	// http://gcc.gnu.org/onlinedocs/cpp/Stringification.html		

#define BOOL2ONOFF(B)						((B) ? "ON" : "OFF")
#define BOOL2STR(B)							((B) ? "true" : "false")
#define BOOL2CHAR(B)						((B) ? '1' : '0')
#define BOOL2CHR(B)							BOOL2CHAR(B)


//#define SKIPSPACES(P)						{ while ((*(P))++) { if ((*(P))!=' ') { (P)--; break; } } } while (0)

#define TYPECAST(TYPE, MYVAR)				((TYPE)(MYVAR))
#define U8(MYVAR)							TYPECAST(uint8_t,   MYVAR)
#define U16(MYVAR)							TYPECAST(uint16_t,  MYVAR)
#define U32(MYVAR)							TYPECAST(uint32_t,  MYVAR)
#define U64(MYVAR)							TYPECAST(uint64_t,  MYVAR)
#define I8(MYVAR)							TYPECAST(int8_t,    MYVAR)
#define I16(MYVAR)							TYPECAST(int16_t,   MYVAR)
#define I32(MYVAR)							TYPECAST(int32_t,   MYVAR)
#define I64(MYVAR)							TYPECAST(int64_t,   MYVAR)
#define VU8(MYVAR)							TYPECAST(volatile uint8_t,   MYVAR)
#define U8PTR(MYVAR)						TYPECAST(uint8_t*,  MYVAR)
#define U16PTR(MYVAR)						TYPECAST(uint16_t*, MYVAR)
#define U32PTR(MYVAR)						TYPECAST(uint32_t*, MYVAR)
#define U64PTR(MYVAR)						TYPECAST(uint64_t*, MYVAR)
#define I8PTR(MYVAR)						TYPECAST(int8_t*,   MYVAR)
#define I16PTR(MYVAR)						TYPECAST(int16_t*,  MYVAR)
#define I32PTR(MYVAR)						TYPECAST(int32_t*,  MYVAR)
#define I64PTR(MYVAR)						TYPECAST(int64_t*,  MYVAR)
#define VU8PTR(MYVAR)						TYPECAST(volatile uint8_t*,  MYVAR)
#define DEREF(TYPE, PTR)					(*((TYPE*)(PTR)))
#define DEREF_U8(PTR)						DEREF(uint8_t,  PTR)
#define DEREF_U16(PTR)						DEREF(uint16_t, PTR)
#define DEREF_U32(PTR)						DEREF(uint32_t, PTR)
#define DEREF_U64(PTR)						DEREF(uint64_t, PTR)
#define DEREF_I8(PTR)						DEREF(int8_t,   PTR)
#define DEREF_I16(PTR)						DEREF(int16_t,  PTR)
#define DEREF_I32(PTR)						DEREF(int32_t,  PTR)
#define DEREF_I64(PTR)						DEREF(int64_t,  PTR)

#define BITS2BYTE(b7,b6,b5,b4,b3,b2,b1,b0)	(	((b7)<<7) | \
												((b6)<<6) | \
												((b5)<<5) | \
												((b4)<<4) | \
												((b3)<<3) | \
												((b2)<<2) | \
												((b1)<<1) | \
												((b0)<<0)   \
											)







#ifdef _LINUX

    #ifndef eprintf
    	#define eprintf(format, ...)		fprintf(stderr, format, ##__VA_ARGS__)
    #endif
	
    // #ifndef dprintf
    	// #define dprintf(format, ...)		fprintf(stdout, format, ##__VA_ARGS__)
    // #endif	

	#define _BV(INDEX)							(1 << (INDEX))

#endif







#endif // _INCLUDE_DEFS_H_
