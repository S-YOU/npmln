#ifndef BASE_H_
#define BASE_H_

#define MAXGROUP 0xff
#define MAXREPEAT 0xffff
#define MAXMODE 0xff
#define MAXREGEXITEM 0xfff
#define MAXREGEXSOURCE 0xfff
#define MAXDEPTH 0xf

#ifndef NDEBUG
#include <stdio.h>
#include <time.h>
#define LOG_TIME_INIT double start = clock();
#define LOG_TIME_REPEAT(M, ...) printf("Took: %8.3fms - " M "\n", (clock() - start) / 1000, ##__VA_ARGS__); start = clock();
#define log(M, ...) printf("" M, ##__VA_ARGS__);
#define PUTCH(ch) \
	if (ch <= 0x1f) { log("\\x%02x", ch);}\
	else if (ch < 0x7f) putchar(ch);\
	else if (ch <= 0xff) { log("\\x%02x", ch); }\
	else log("\\u%04x", ch);
#else
#define LOG_TIME_INIT
#define LOG_TIME_REPEAT(M, ...)
//#define printf(...)
#define putchar(...)
#define log(M, ...)
#define PUTCH(ch)
#endif

typedef unsigned char chr;
typedef unsigned char* str;

typedef enum {
	FLAG_DEFAULT = 0,
	IGNORECASE = 1,
	CLASS_NL = 2,
	DOT_NL = 4,
	NON_GREEDY = 8,
	FIX_LENGTH = 16,
	PARSE_INT = 32,
	PARSE_HEX = 64
} FLAGS;

typedef enum {
	OpLiteral,
	OpCharClass,
//	OpAnyCharNotNL,
	OpAnyChar,

	OpBeginLine,
	OpEndLine,
	OpBeginText,
	OpEndText,

	OpWordBoundary,
//	OpNoWordBoundary,
	OpAlternate,

	OpBackRef,
	OpGroupStart,
	OpGroupEnd,
} OP;

typedef enum {
	GrpCapture,
	GrpNonCapture,
	GrpAtomic,
	GrpBranchReset,
	GrpPosLookAhead,
	GrpNegLookAhead,
	GrpPosLookBehind,
	GrpNegLookBehind,
	GrpRef, //or expand automatically to back ref like \1
	GrpDefine,
} GRP;

struct _char_class {
	unsigned st;
	unsigned en;
	unsigned rev;
} char_class;
typedef struct _char_class* CharClass;

struct _regex_item {
	OP op;
	unsigned ch;
	CharClass *cls;
	unsigned cls_rev;
	unsigned cls_len;
	unsigned cls_id;
	unsigned cls_type;
	unsigned min, max;
	unsigned min_pos;
	unsigned min_sz;
	GRP grp_type;
	unsigned grp_id;
	unsigned cap_id;
	unsigned ref_id;
	unsigned cur_id;
	unsigned alt_id;
	unsigned lvl;
	FLAGS flags;
} regex_item;
typedef struct _regex_item* RegexItem;

#endif
