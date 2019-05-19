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


//#define LARGESET_CLEAR(SET, ECNT)			memset(SET, 0, ((ECNT) / 8) * sizeof(char))
//#define LARGESET_INCLUDE(SET, EIDX)		((SET)[(EIDX) / 8]) |= (0x80 >> ((EIDX) % 8))
//#define LARGESET_EXCLUDE(SET, EIDX)		((SET)[(EIDX) / 8]) &= ~(0x80 >> ((EIDX) % 8))
//#define LARGESET_CONTAINS(SET, EIDX)		(0 != (((SET)[(EIDX) / 8]) & (0x80 >> ((EIDX) % 8))))


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


#define STL_MAP_KEY_EXISTS(MAP, KEY)	   ((MAP).find(KEY) != (MAP).end())
#define RAND_RANGE(MAXVAL)					(((MAXVAL) * rand()) / RAND_MAX)


// ******************************************************************
// plattform-specific stuff (windows, linux, vusialc++, borlandc++, atmel, beck, ...
// ******************************************************************

/*

#define eprintf(format, ...)		fprintf(stderr, format, __VA_ARGS__)

*/




#ifndef __cplusplus
	typedef unsigned char bool_t;
	typedef bool_t bool;
	#define FALSE 0
	#define TRUE (!FALSE)
	#define false FALSE
	#define true  TRUE
#endif

// windows/visualcpp
#ifdef _MSC_VER
	#define VISUALCPP
#endif

// windows/turbocpp
#if ((defined __TURBOC__) || (defined __BORLANDC__ )) && (defined _WIN32)

	#define STRSAFE_NO_DEPRECATE

	#define BORLANDCPP
	#define TURBOCPP

	// #define _WIN32

    #ifndef eprintf
    	#define eprintf(...)		fprintf(stderr, __VA_ARGS__)
    #endif

    #ifndef dprintf
    	#define dprintf(...)		fprintf(stdout, __VA_ARGS__)
    #endif

	#define AS(STR)							AnsiString(STR)
	#define ASP(...)						AnsiString().sprintf(__VA_ARGS__)
	#define AS_STARTSWITH(STR, HEAD)		((STR).Pos(HEAD) == 1)
	#define AS_CONTAINS(STR, SUBSTR)		((STR).Pos(SUBSTR) > 0)
	#define AS_ENDSWITH(STR, TAIL)			((STR).Pos(TAIL) == ((STR).Length() - AS(TAIL).Length() + 1))
	#define AS_REPLACE(STR, OLDS, NEWS)     StringReplace(STR, OLDS, NEWS, TReplaceFlags() << rfReplaceAll)

	#define AS_LEFTFROM(STR, DELIM)			(STR).SubString(1, (STR).Pos(DELIM) - 1)
	#define AS_RIGHTFROM(STR, DELIM)		(STR).SubString((STR).Pos(DELIM) + AS(DELIM).Length(), (STR).Length())
	#define AS_FIRSTCHAR(STR)				((STR)[0])
	#define AS_LASTCHAR(STR)				((STR)[(STR).Length()])
	#define AS_LEFTMOST(STR, LEN)			(STR).SubString(1, LEN)
	#define AS_RIGHTMOST(STR, LEN)          ACHTUNG_UNGETESTET !!! (STR).SubString((STR).Length() - (LEN), LEN)

	#define TCOMP(COMP)						((TComponent*)(COMP))
	#define TAG(COMP)						(TCOMP(COMP)->Tag)
	#define FINDCOMPONENT(TYPE, NAME)		((TYPE*)(this->FindComponent(NAME)))
	#define FINDCOMPONENT_I(TYPE, FMT, I)	FINDCOMPONENT(TYPE, ASP(FMT, I))
	#define STRINGLISTLINE(STRINGLIST, I)	((STRINGLIST)->Strings[I])
	#define MEMOLINE(MEMO, I)				STRINGLISTLINE((MEMO)->Lines, I)
	#define LISTBOXLINE(LISTBOX)			STRINGLISTLINE((LISTBOX)->Items, (LISTBOX)->ItemIndex)



	#define MSGBOX_INFO(STR) 				MessageDlg(STR, mtInformation, TMsgDlgButtons() << mbOK, 0)
	#define MSGBOX_WARN(STR) 				MessageDlg(STR, mtWarning, TMsgDlgButtons() << mbOK, 0)
	#define MSGBOX_ERROR(STR) 				MessageDlg(STR, mtError, TMsgDlgButtons() << mbOK, 0)
	#define MSGBOX_ERR(STR)             	MSGBOX_ERROR(STR)
	#define QUESTIONBOX_INFO(STR) 			(MessageDlg(STR, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	#define QUESTIONBOX_WARN(STR) 			(MessageDlg(STR, mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	#define QUESTIONBOX_ERROR(STR) 			(MessageDlg(STR, mtError, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	#define QUESTIONBOX_ERR(STR)          	QUESTIONBOX_ERROR(STR)

	#define MSGBOX_INFO_NOPARENT(STR)		MessageBox(NULL, STR, "Information", MB_OK | MB_ICONINFORMATION)
	#define MSGBOX_WARN_NOPARENT(STR)		MessageBox(NULL, STR, "Warning", MB_OK | MB_ICONEXCLAMATION)
	#define MSGBOX_ERROR_NOPARENT(STR)		MessageBox(NULL, STR, "Error", MB_OK | MB_ICONERROR)

	#define QUESTIONBOX_INFO_NOPARENT(STR)	(IDYES == MessageBox(NULL, STR, "Information", MB_YESNO | MB_ICONINFORMATION))
	#define QUESTIONBOX_WARN_NOPARENT(STR)	(IDYES == MessageBox(NULL, STR, "Warning", MB_YESNO | MB_ICONEXCLAMATION))
	#define QUESTIONBOX_ERROR_NOPARENT(STR)	(IDYES == MessageBox(NULL, STR, "Error", MB_YESNO | MB_ICONERROR))

    #define ROUND(F) 						floor((F)+0.5)
    #define STR2FLOAT_SAVE(STR)        		StrToFloat(AS_REPLACE(STR, ",", "."))

	#define YESNOCANCELBOX_INFO(STR) 		MessageDlg(STR, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0)
	#define YESNOCANCELBOX_WARN(STR) 		MessageDlg(STR, mtWarning,      TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0)

	#define QUESTIONBOX(STR)                QUESTIONBOX_INFO(STR)

	#define DISABLE_REDRAW(COMPONENT)		SendMessage((COMPONENT)->Handle, WM_SETREDRAW, WPARAM(false), 0)
	#define ENABLE_REDRAW(COMPONENT)		SendMessage((COMPONENT)->Handle, WM_SETREDRAW, WPARAM(true), 0)


	#define PROP(TYPE, NAME, READ, WRITE) 	public: \
												__property TYPE NAME = \
												{ read = READ, write = WRITE };

	#define PROP_VM(TYPE, NAME) 			protected: \
												TYPE m ## NAME; \
												void Set ## NAME(TYPE Value); \
											public: \
												__property TYPE NAME = \
												{ read = m ## NAME, write = Set ## NAME };

	#define PROP_VV(TYPE, NAME) 			protected: \
												TYPE m ## NAME; \
											public: \
												__property TYPE NAME = \
												{ read = m ## NAME, write = m ## NAME };

	#define PROP_MM(TYPE, NAME) 			protected: \
												TYPE Get ## NAME(void); \
												void Set ## NAME(TYPE Value); \
											public: \
												__property TYPE NAME = \
												{ read = Get ## NAME, write = Set ## NAME };




	#define PROP_V(TYPE, NAME) 				protected: \
												TYPE m ## NAME; \
											public: \
												__property TYPE NAME = \
												{ read = m ## NAME };

	#define PROP_M(TYPE, NAME) 				protected: \
												TYPE Get ## NAME(void); \
											public: \
												__property TYPE NAME = \
												{ read = Get ## NAME };

#endif

// windows general
#ifdef _WIN32
	#define DLLEXPORT_H				__stdcall //__declspec(dllexport) __stdcall
	#define DLLEXPORT_C 			__stdcall //__declspec(dllexport) __stdcall
	#define	SIGHANDLER_CALLTYPE		__cdecl

	#ifndef PATH_MAX
		#define PATH_MAX			MAX_PATH
	#endif

#endif

// atmel
#ifdef __AVR__
	// die STRXXX()-makros nicht bei atmel verwenden, stattdessen folgende verwenden:
	// STRXXX_L (Literal -> ProgramSpace) oder STRXXX_R (RAM)
	#undef STREQU
	#undef STRSTARTS
	#define STREQU_R(STR1, STR2)			(0 == strcmp(STR1, STR2))
	#define STREQU_L(STR1, LIT)				(0 == strcmp_P(STR1, PSTR(LIT)))
	#define STRSTARTS_L(STR1, LIT)			(0 == strncmp_P(STR1, PSTR(LIT), LITERAL_STRLEN(LIT)))
	#define STRSTARTS_R(STR1, STR2)			(0 == strncmp(STR1, STR2, strlen(STR2)))

	#define sprintf_L(STR, FMT, ...)		sprintf_P(STR, PSTR(FMT), ##__VA_ARGS__)
	#define strcpy_L(DST, LIT)				strcpy_P(DST, PSTR(LIT))
	#define strncpy_L(DST, LIT, LEN)		strcpy_P(DST, PSTR(LIT), LEN)



	
	#define DDR_OUT(DDR, BITPOS)			BIT_SET(DDR, BITPOS)
	#define DDR_IN(DDR, BITPOS)				BIT_CLR(DDR, BITPOS)
	#define PULLUP_ENABLE(PORT, BITPOS)		BIT_SET(PORT, BITPOS)
	#define PULLUP_DISABLE(PORT, BITPOS)	BIT_CLR(PORT, BITPOS)

	#define _HI(NAME)						BIT_SET(NAME ## _PORT, NAME ## _BITPOS)
	#define _LO(NAME)						BIT_CLR(NAME ## _PORT, NAME ## _BITPOS)
	#define _TOG(NAME)						BIT_TOG(NAME ## _PORT, NAME ## _BITPOS)
	#define _GET(NAME)						BIT_GET(NAME ## _PIN,  NAME ## _BITPOS)
	#define _SET(VALUE, NAME)				{ if (VALUE) _HI(NAME); else _LO(NAME); }
	#define _NSET(VALUE, NAME)				{ if (VALUE) _LO(NAME); else _HI(NAME); }
	#define _DDR_OUT(NAME)					BIT_SET(NAME ## _DDR,  NAME ## _BITPOS)
	#define _DDR_IN(NAME)					BIT_CLR(NAME ## _DDR,  NAME ## _BITPOS)
	#define _PULLUP_ENABLE(NAME)			BIT_SET(NAME ## _PORT, NAME ## _BITPOS)
	#define _PULLUP_DISABLE(NAME)			BIT_CLR(NAME ## _PORT, NAME ## _BITPOS)

	#define _SET(VALUE, NAME)				{ if (VALUE) _HI(NAME); else _LO(NAME); }
	#define _NSET(VALUE, NAME)				{ if (VALUE) _LO(NAME); else _HI(NAME); }

	#define SREG_CLI()						uint8_t __sreg = SREG; cli();
	#define SREG_RESTORE()					SREG = __sreg;	
	#define LONGDELAY_MS(MS)				{wdt_reset(); EXTWDOG_TOG(); uint16_t _i; for (_i=0; _i<(MS/100); _i++) { _delay_ms(100); wdt_reset(); EXTWDG_TOG(); } }
	#define SHORTDELAY_MS(MS)				{ uint16_t _i; for (_i=0; _i<(MS); _i++) { _delay_ms(1); ; } }
	#define CHECK_AND_RESET(FLAG)			if (FLAG) { FLAG = false; return true; } else { return false; }		
	
	#define WDOG_REBOOT()					{ cli(); wdt_enable(WDTO_15MS); while (true); }

	#define NOP()							asm volatile ("nop")

	#define MKREG_8BIT(B7,B6,B5,B4,B3,B2,B1,B0)	 ( _BV(B7) | _BV(B6) | _BV(B5) | _BV(B4) | _BV(B3) | _BV(B2) | _BV(B1) | _BV(B0) ) 
	#define MKREG(B7,B6,B5,B4,B3,B2,B1,B0)	MKREG_8BIT(B7,B6,B5,B4,B3,B2,B1,B0)

	#define F_CPU_18						18432000ul
	#define F_CPU_14						14745600ul
	#define F_CPU_11						11059200ul

	#define BAUD_115						115200ul //28800L //  temporär geändert!!!
	
#else
	#define CHECK_AND_RESET(FLAG)			if (FLAG) { FLAG = false; return true; } else { return false; }		
	#define prog_char						char
	#define PROGMEM
	#define PGM_VOID_P						void*
	#define EEMEM
	#define PSTR(STR)						STR
	#define pgm_read_byte(p)				DEREF_U8(p)
	#define pgm_read_word(p)				DEREF_U16(p)
	#define pgm_read_dword(p)				DEREF_U32(p)
	#define eeprom_read_byte(p)				DEREF_U8(p)
	#define eeprom_read_word(p)				DEREF_U16(p)
	#define eeprom_read_dword(p)			DEREF_U32(p)
	#define eeprom_read_float(p)			DEREF(p, float)
	#define eeprom_read_block(d, s, n)		memcpy(d, s, n)
	#define eeprom_write_byte(p, v)			DEREF_U8(p)  = (v)
	#define eeprom_write_word(p, v)			DEREF_U16(p) = (v)
	#define eeprom_write_dword(p, v)		DEREF_U32(p) = (v)
	#define eeprom_write_float(p, v)		DEREF(p, float) = (v)
	#define eeprom_write_block(s, d, n)		memcpy(d, s, n)
	#define eeprom_update_byte(p, v)		eeprom_write_byte(p, v)
	#define eeprom_update_word(p, v)		eeprom_write_word(p, v)
	#define eeprom_update_dword(p, v)		eeprom_write_dword(p, v)
	#define eeprom_update_float(p, v)		eeprom_write_float(p, v)
	#define eeprom_update_block(s, d, n)	eeprom_write_block(s, d, n)
	#define strncmp_P(STR1, STR2, N)		strncmp(STR1, STR2, N)
	#define strcmp_P(STR1, STR2)			strcmp(STR1, STR2)
	#define memcpy_P(s, d, l)				memcpy(s, d, l)
	#define STREQU_R(STR1, STR2)			(0 == strcmp(STR1, STR2))
	#define STREQU_L(STR1, LIT)				(0 == strcmp_P(STR1, PSTR(LIT)))
	#define STRSTARTS_L(STR1, LIT)			(0 == strncmp_P(STR1, PSTR(LIT), LITERAL_STRLEN(LIT)))
	#define STRSTARTS_R(STR1, STR2)			(0 == strncmp(STR1, STR2, strlen(STR2)))

#endif

//#ifdef _LINUX
#if 0
	#define CONSOLE_COLOR_BRIGHTGREEN		"\x1b[32m"
	#define CONSOLE_COLOR_BRIGHTRED			"\x1b[31m"
	#define CONSOLE_COLOR_BRIGHTBLUE		"\x1b[34m"
	#define CONSOLE_COLOR_YELLOW			"\x1b[33m"
	#define CONSOLE_COLOR_NORMAL			"\x1b[0m"

	#define printf_red(format, ...)			printf(CONSOLE_COLOR_BRIGHTRED 		format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	#define printf_grn(format, ...)			printf(CONSOLE_COLOR_BRIGHTGREEN 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	#define printf_green(format, ...)		printf(CONSOLE_COLOR_BRIGHTGREEN 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	#define printf_blu(format, ...)			printf(CONSOLE_COLOR_BRIGHTBLUE 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	#define printf_blue(format, ...)		printf(CONSOLE_COLOR_BRIGHTBLUE 	format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	#define printf_yel(format, ...)			printf(CONSOLE_COLOR_YELLOW 		format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
	#define printf_yellow(format, ...)		printf(CONSOLE_COLOR_YELLOW 		format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)

	#define eprintf(format, ...)			fprintf (stderr, CONSOLE_COLOR_BRIGHTRED format CONSOLE_COLOR_NORMAL, ##__VA_ARGS__)
#endif

#ifdef _LINUX

    #ifndef eprintf
    	#define eprintf(format, ...)		fprintf(stderr, format, ##__VA_ARGS__)
    #endif

	#define _BV(INDEX)							(1 << (INDEX))

	/*
    #ifndef dprintf
    	#define dprintf(format, ...)		fprintf(stdout, format, ##__VA_ARGS__)
    #endif
	*/

#endif







#endif // _INCLUDE_DEFS_H_
