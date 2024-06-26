#pragma once
#ifndef __lib_aveth_utils_hpp
#define __lib_aveth_utils_hpp
//  ALIB: A C-Style programming utility header for tedious tasks.
//
//  Any instance of array creation or object creation is handled using the C-style malloc()/free() functions!
//
//



/** - https://github.com/avetharun/avetharun/blob/main/ave_libs.hpp
 @copyright Avetharun 2021-2022 - GNU-GPL3-a
 Let's keep this simple. It follows the GNU GPL3 license, with very few modifications and exceptions.
 @ You can:
  - Use this commercially
  - Distribute modified versions
  - Modify this file and/or projects
  - Patent projects that use this file
  - Use this file and/or project privately, ie. for use in an internal server (See limitations & Exceptions)
  - Use freely under the following conditions
 @b Conditions:
  - You must disclose the source file and/or author when distributing this file
  - A copy of this license must be attached to the file, in verbatim
  - The same (or similar) license must be used if you modify and license your version
  - Changes must be listed. ie. modifying source code must have your name and change in the file.
	- To follow this it must either be
	  A: Beside the change (on top or on bottom, in a comment)
	  B: In the AUTHORs section of the file/project
	  C: in any changelog that references the file's changes

 @b Limitations
  - The user (you) are not liable for the harmful actions this program or file may cause, if any.
  - Keep code closed source (*See exceptions)
 @b Exceptions
  - If code is closed source, it must be in a private setting. Examples are as follows:
	EXA: A server used to host and/or distribute files
	EXB: Used as a base for porting and/or in microcontrollers
 */


 // Avetharun : 4-14-22 : added byte modification utilities
 // Avetharun : 4-15-22 : added beginswith function, and renamed readFileBytes to alib_file_read
 // Avetharun : 4-17-22 : added "copy sign" function
 // Avetharun : 4-27-22 : added json utilities & fixed  issue, causing things that must be declared seperately to not work.
 // Avetharun : 4-29-22 : modify the way function pointer types are created, now uses a macro. Formatted as follows:
	  // d_typedef_func_ty(ret_val, ty_name, ty_arg_types_variadic)
	  // Note: semicolon is NOT needed, as if it's put at the end, it will produce an intellisense warning. Apparently it's by design. Ignore it if it happens.
	  // Original impl: https://github.com/avetharun/avetharun/blob/bf49a022c7021fb3200231722f7975f167e1cf9f/ave_libs.hpp#L308
					   // Also added assert handling
#pragma warning (push)
#pragma warning (disable : 4996)
#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <format>
#include <stdarg.h>

#ifdef _WIN32
#define ALIB_WIN

//#include <windows.h>
//#include <shellapi.h>
#endif



struct alib_string : std::string {

	operator const char* ();
	operator char* ();
	alib_string(std::string other);
	alib_string(const char* buf);
};
template<typename K, typename V>
std::map<V, K> alib_reverse_map(const std::map<K, V>& m);
template<typename K, typename V>
std::unordered_map<V, K> alib_reverse_hmap(const std::unordered_map<K, V>& m);
template <typename k, typename v>
bool alib_VectorWithPairContains(std::vector<std::pair<k, v>> vec, k key);
template <typename v>
size_t alib_FindValueInVector(std::vector<v> vec, v val);


template <typename k, typename v>
size_t alib_FindOffsetOfPairInVector(std::vector<std::pair<k, v>> vec, k key);


#define alib_get_error_c() ___nn_alib_error_charp_str.c_str()
#define alib_get_error_s() ___nn_alib_error_charp_str

#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_180   0.01745329251            // pi/180
#define M_PIx180   565.486677646            // pi*180
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

constexpr float alib_1rad = 180 / M_PI;

float alib_rad2deg(float rad);
float alib_deg2rad(float deg);

// Note: ignore any "function definition for typedef_func_ty" or "Y is not defined" errors. They're temporary.
#define d_typedef_func_ty(return_z, name, ...) typedef return_z (*name)(__VA_ARGS__);
// Copy of ImGui's IMVector class, slimmed to fit the need of this library.
// Only 255 stack objects can be pushed at once. This is because THIS ISNT A REPLACEMENT OF STD::STACK

template<typename T>
struct ALStack
{
	int                 Size;
	int                 Capacity;
	T* Data;

	// Provide standard typedefs but we don't use them ourselves.
	typedef T                   value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;

	// Constructors, destructor
	inline ALStack();
	inline ALStack(const ALStack<T>& src);
	inline ALStack<T>& operator=(const ALStack<T>& src);
	inline ~ALStack(); // Important: does not destruct anything

	inline void         clear();  // Important: does not destruct anything
	inline void         clear_delete();     // Important: never called automatically! always explicit.
	inline void         clear_destruct();           // Important: never called automatically! always explicit.

	inline bool         empty() const;
	inline int          size() const;
	inline int          size_in_bytes() const;
	inline int          max_size() const;
	inline int          capacity() const;

	inline T*			begin();
	inline const T*		begin() const;
	inline T*			end();
	inline const T*		end() const;
	inline T&			front();
	inline const T&		front() const;
	inline T&			back();
	inline T&			top();
	inline const T&		back() const;
	inline const T&		top() const;
	inline T&			at(int amt);
	inline void         swap(ALStack<T>& rhs);
	inline T*			insert(const T* it, const T& v);
	inline bool         contains(const T& v) const;

	inline int          _grow_capacity(int sz) const;
	inline void         resize(int new_size);
	inline void         resize(int new_size, const T& v);
	inline void         shrink(int new_size); // Resize a vector to a smaller size, guaranteed not to cause a reallocation
	inline void         reserve(int new_capacity);

	// NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the ImVector data itself! e.g. v.push_back(v[10]) is forbidden.
	inline void         push_back(const T& v);
	inline void         pop_back();
	inline void         pop();
	inline void         push(const T& v);
	inline void         push_front(const T& v);
};
#define AlibStack ALStack
#define alib_stack ALStack
#include <exception>
#define alib_noerr(CODE_TO_RUN) \
	try{\
		CODE_TO_RUN;\
	}catch(std::exception ignored){}

bool alib_atob(const char* buffer);
char* alib_itoa(int num, char* buffer, int base);
char* alib_ftoa(char* buf, size_t len, float f);
#ifndef NDEBUG
// Production builds should set NDEBUG=1
#define NDEBUG false
#endif
#define alib_malloca(TYPE, length) (TYPE*)malloc(length)
#define alib_malloct(TYPE) (TYPE*)malloc(sizeof(TYPE))
#ifdef WIN32
bool alib_can_reach_mem(void* ptr);
#endif
#ifndef ALIB_DEBUG_BUILD
#define ALIB_DEBUG_BUILD !NDEBUG
#endif

#if defined(ALIB_FORCE_BINARY) || (!defined(ALIB_NO_BINARY))
// 
//      Binary & bit manipulation utilities
//    - Avetharun
// 


/* Helper macros. Unless you know what you're doing, don't run these. */

// Sourced from https://bytes.com/topic/c/answers/219656-literal-binary
// Binary decoding to unsigned long v
#define HEX__(n) 0x##n##LU
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)
// Binary encoding into hex (internal+user-runnable)
#define B8(d) ((unsigned char)B8__(HEX__(d)))
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))

#define B64(dmsb,db2,db3,db4,db5,db6,db7,dlsb) \
(\
	((unsigned long long int)B32(dmsb,db2,db3,db4)<<32  ) \
+	((unsigned long long int)B32(db5,db6,db7,dlsb)	   ) \
)

/** \todo B64(v,v,v,v,v,v,v,v)
 * but that'll be a mess...
 */

 // Internal bit manipulation
#define imp_bitenb(val,n)   ( ( val ) |=  (1<<(n))) // Sets bit n to 1
#define imp_bitdis(val,n)   ( ( val ) &= ~(1<<(n))) // Sets bit n to 0
#define imp_flipbit(val,n)  ( ( val ) ^=  (1<<(n))) // Inverts bit n
#define imp_getbitv(val,n)   ( ( val ) &   (1<<(n))) // Gets bit n from data val





/*---------------------------------------*/
/* User macros */

// Binary number creation to unsigned long


// Bit manipulation 
//
// Sets bit nbit to value bv in variable var
#define setbitv(var,nbit,val) (val > 0) ? imp_bitenb(var,nbit) : imp_bitdis(var,nbit)
#define setbitsv(var,val,amt) for(int nbit=0; nbit < amt; nbit++) { setbitv(var,nbit,val); }
#define setbitssv(var,start,val,amt) for(int nbit=start; nbit < start+amt; nbit++) { setbitv(var,nbit,val); }
// Sets bit nbit to TRUE/1
#define bitenablev(var,nbit)              imp_bitenb(var,nbit)
// Sets bit nbit to FALSE/0
#define bitdisablev(var,nbit)             imp_bitdis(var,nbit)
// Get bit nbit from value var
#define getbitv(var,nbit)                 imp_getbitv(var,nbit)
#define flipbitv(var, nbit)               imp_flipbit(var,nbit)

// Bit creation
// 

/**
 * Re-implementation of line @e 66 because compiler complains if I do the following:
 * #define Binary8(b8v) B8(b8v)
 * > Too many/little values in constant
*/
// Convert 8 bits into an unsigned char
#define Binary8(d) ((unsigned char)B8__(HEX__(d))) 
// Convert 16 bits (Each byte is seperated!) into an unsigned short
#define Binary16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
// Convert 32 bits (Each byte is seperated!) into an unsigned long
#define Binary32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
// Convert 64 bits (Each byte is seperated!) into an unsigned long long
#define Binary64(dmsb,db2,db3,db4,db5,db6,db7,dlsb) \
(\
	((unsigned long long int)B32(dmsb,db2,db3,db4)<<32  ) \
+	((unsigned long long int)B32(db5,db6,db7,dlsb)	   ) \
)
// Todo: Binary64 to T<long long>

/**
 * @todo @b Binary_Reading. Effectively reversing the above functions.
 * Planned implementation: Eventually.
 */



#endif // ALIB_NO_BINARY

#if defined(ALIB_FORCE_FILE) || (!defined(ALIB_NO_FILE))

#include <iosfwd>
#include <fstream>
#include <sstream>
#include <string>
 // If size != 0, leave size as is
void __alib_internal_reqlen__f_impl(size_t* sz, alib_string arr);
/* We want POSIX.1-2008 + XSI, i.e. SuSv4, features */
#define _XOPEN_SOURCE 700

/* Added on 2017-06-25:
   If the C library can support 64-bit file sizes
   and offsets, using the standard names,
   these defines tell the C library to do so. */
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64 

#include <stdlib.h>
#ifdef __linux__
#include <unistd.h>
#endif
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

   /* POSIX.1 says each process has at least 20 file descriptors.
	* Three of those belong to the standard streams.
	* Here, we use a conservative estimate of 15 available;
	* assuming we use at most two for other uses in this program,
	* we should never run into any problems.
	* Most trees are shallower than that, so it is efficient.
	* Deeper trees are traversed fine, just a bit slower.
	* (Linux allows typically hundreds to thousands of open files,
	*  so you'll probably never see any issues even if you used
	*  a much higher value, say a couple of hundred, but
	*  15 is a safe, reasonable value.)
   */
#ifndef USE_FDS
#define USE_FDS 15
#endif


const char* __alib_strfmt_file(const char* fmt, ...);

bool alib_file_exists(const char* name);
size_t alib_file_length(FILE* file);

size_t alib_file_length(const char* fname);

std::string alib_file_read(std::ifstream& file);
std::string alib_file_read(const char* fname);


void alib_file_read(const char* fname, const char* out_, unsigned long long* size_);
void alib_file_read(const char* fname, const char* out_, unsigned long long size_);

void alib_file_write(const char* filen, alib_string d, size_t l = 0);
// Note: does NOT close file after!
void alib_file_write(std::ostream& file, const char* d, size_t l = 0);

#ifdef _WIN32
#include "direct.h"
#define PATH_SEP '\\'
#define GETCWD _getcwd
#define CHDIR _chdir
#else
#include "unistd.h"
#define PATH_SEP '/'
#define GETCWD getcwd
#define CHDIR chdir
#endif
bool alib_scwd(const char* dir);

char* alib_gcwd();

#endif // ALIB_NO_FILE_UTILS


#if defined(ALIB_FORCE_CONCAT) || (!defined(ALIB_NO_CONCAT))

#define __concat_internal3(a, b, c) a##b##c
#define __concat_internal2(a, b) a##b
#define __concat4__internal(a,b,c,d) __concat_internal2(a,b)##__concat_internal2(c,d)
#define concat4(a,b,c,d) __concat4__internal(a,b,c,d)
#define concat3(a, b, c) __concat_internal3(a, b, c)
#define concat2(a, b) __concat_internal2(a, b)
#endif // ALIB_NO_CONCAT





#if defined(ALIB_FORCE_NONAMES) || (!defined(ALIB_NO_NONAMES))
// This may require some explaining. I use these for creating "unnamed" functions and variables, at compile time.
// Obviously, since they're unnamed, they're single use.
// The easiest way for me to use this, is for unnamed struct initializing lambda functions, which uses a struct similar to this:
/*
#include <functional>
struct inline_initializer{
	inline_initializer(std::function<void()> i) {
	  i();
	}
};
inline_initializer _nn {
	[&] () {
		std::cout << "This runs before main()!";
	}
};
*/
// Since the variable is single use, as is obvious,

// What it should output is obviously based upon compiler, but what it'll look like on GCC is roughly this:
// ____34_unl__noname_line_34_LINEPOS_34_un_
// If you want to get the noname as a variable, do _nng(LINE_NUM)
// This is the first instance of _nn (), on line 23 of any file. At the moment, GCC doesn't have a "remove . " pragma, 
// so file-specific nonames that are on the same line are impossible.

#define ___nn_internal_concat__(a, b, c) a##b##c
#define ___nn_internal_concat(a, b, c) ___nn_internal_concat__(a, b, c)
#define _1_nn_internal__(a) ___nn_internal_concat(a, _LINEPOS_, __LINE__)
#define _1_nn_internal__2__ _1_nn_internal__(___nn_internal_concat(_noname, _line_, __LINE__))
#define _1_nn_internal__2_1_ ___nn_internal_concat(_, ___nn_internal_concat(__, __LINE__, _unl), _)
#define _1_nn_internal__3__ ___nn_internal_concat(_1_nn_internal__2_1_, _1_nn_internal__2__, _un)


#define _g_nn_internal__(a, line) ___nn_internal_concat(a, _LINEPOS_, line)
#define _g_nn_internal__2__(line) _g_nn_internal__(___nn_internal_concat(_noname, _line_, line), line)
#define _g_nn_internal__2_1_(line) ___nn_internal_concat(_, ___nn_internal_concat(__, line, _unl), _)
#define _g_nn_internal__3__(line) ___nn_internal_concat(_g_nn_internal__2_1_(line), _g_nn_internal__2__(line), _un)


#define _nn_impl ___nn_internal_concat(_, _1_nn_internal__3__, _) // Create no-name object
#define _nn _nn_impl
#define _nng(line) ___nn_internal_concat(_, _g_nn_internal__3__(line), _) // Get no-name object at file line (LINE)

#endif // ALIB_NO_NONAMES

#if defined(ALIB_FORCE_INLINE_RUNNERS) || (!defined(ALIB_NO_INLINE_RUNNERS)) 

#include <functional>
// lambda-based runner function. Runs when program initializes all variables.
struct alib_inline_run {
	alib_inline_run(std::function<void()> initFunc);
};


#endif // ALIB_NO_INLINE_RUNNERS


#if (defined(ALIB_FORCE_WIN_UTILS) && defined(_WIN32)) && (!defined(ALIB_NO_WIN_UTILS)) 
#include <Windows.h>
void alib_hide_console()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void alib_show_console()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool alib_console_visible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}


#endif

#if defined(ALIB_FORCE_FUNCPTR) || (!defined(ALIB_NO_FUNCPTR))
#define noop (void)0
d_typedef_func_ty(int, int_2i_f, int, int)
d_typedef_func_ty(int, int_1i_f, int)
d_typedef_func_ty(int, int_0a_f)
d_typedef_func_ty(void, int_1i_1p_f, int, void*)
d_typedef_func_ty(void, void_0a_f)
d_typedef_func_ty(void, void_1i_f, int)
d_typedef_func_ty(void, void_2i_f, int, int)
d_typedef_func_ty(void, void_1pc_1i32_f, const char*, uint32_t)
#endif // ALIB_NO_FUNCPTR


#if defined(ALIB_FORCE_BYTE_UTILS) || (!defined(ALIB_NO_BYTE_UTILS))
#include <cmath>
unsigned long alib_fnull(const char* arr);
unsigned long alib_n_fnull(const char* arr, int limit);
size_t alib_nulpos(const char* arr);
size_t alib_nulposn(const char* arr, int limit);
#define __alib_haslogm__
int alib_log(int base, int n);
#define __alib_haswrange__
bool alib_wrange(int min, int max, int val);
// Taken from python's usage of math.isclose()
bool alib_fisclose(float first, float second, float rel_tol = 1e-09, float abs_tol = 0.0);
// Taken from python's usage of math.isclose()
bool alib_disclose(double first, double second, double rel_tol = 1e-09, double abs_tol = 0.0);
// Check if the difference between [0] and [1] are within [amount]
bool alib_iswithin(float first, float second, float amount);


int alib_digitsInNum(long n, int base = 10);

// Get digits of [num], formatted as ASCII (by default, pass false to disable)
// arr[0] is the amount of digits in the array
// Note: malloc 
char* alib_getDigitsOfNumber(int num, bool ascii = true);


// If size != 0, leave size as is, otherwise size = strlen(arr)
void alib_reqlen(size_t* sz, const char* arr);

void alib_print_byte(uint8_t byte);

int alib_endswith(const char* str, const char* suffix);

int alib_beginswith(const char* str, const char* prefix);
// Get string between two strings
int alib_substr2(const char* input, size_t input_len);
// if string begins with X and the character afterward is NOT the last character of the prefix.
// ie alib_beginswith_anddoesntfollow("abc123", "a"); -> true
// ie alib_beginswith_anddoesntfollow("a", "a"); -> false
int alib_beginswith_anddoesntfollow(const char* str, const char* prefix);
// get position of char 
int alib_getchrpos(const char* src, char c, size_t len = 0);
// char array contains
bool alib_costr(std::string src, const char* match);
int alib_chreq(const char* src, const char* match);
int alib_streq(std::string src, const char* match);
int alib_streqn(std::string src, const char* match, const size_t len);
// Occurances of char `c` in `src`
size_t alib_chrocc(const char* src, char c, size_t len = 0);
#include <vector>
const char* alib_rmocc(const char* src, char c, size_t len = 0);

// get byte at offset of array 
char alib_get_byte(void* data, int offset);
// get byte at array[0]
char alib_get_byte(void* data);
// set byte at offset of array 
void alib_set_byte(void* data, char byte, int offset);
// set byte at array[0]
void alib_set_byte(void* data, char byte);

#ifdef _MSC_VER
#define VSCPRINTF "native (symlinked)"
#define vscprintf _vscprintf
#endif

#ifdef __GNUC__
#define VSCPRINTF "implemented"
int vscprintf(const char* format, va_list ap);
#endif

/*
 * asprintf, vasprintf:
 * MSVC does not implement these, thus we implement them here
 * GNU-C-compatible compilers implement these with the same names, thus we
 * don't have to do anything
 */
#ifdef _MSC_VER
#define VASPRINTF "implemented"
#define ASPRINTF "implemented"
int vasprintf(char** strp, const char* format, va_list ap);

int asprintf(char** strp, const char* format, ...);
#endif

#ifdef __GNUC__
#define VASPRINTF "native"
#define ASPRINTF "native"
#endif


size_t alib_2d_ar_pos(size_t pitch, size_t x, size_t y, size_t bytes_per_step = 4);
size_t alib_va_arg_length(const char* fmt, ...);
size_t alib_va_arg_length(const char* fmt, va_list args);
// Opens a URL externally
void alib_urlhttpopen_ext(alib_string url);

alib_string alib_urlhttpget(alib_string url);

alib_string alib_strfmtv(const char* fmt, va_list args);
alib_string alib_strfmt(const char* fmt, ...);
#define alib_strfmts alib_strfmt
#define alib_strfmtsv alib_strfmtv
float alib_lerp_low(float a, float b, float f);

float alib_lerp(float a, float b, float f);
float alib_clerp(float a, float b, float f);
const char* alib_chrrepl(const char* in, char match, char repl_value);
// Copies the sign-ed ness of A into B
void alib_copy_signed(signed int a, signed int* b);
template <typename T>
void alib_clampptr(T* out, T lower, T upper) {
	*out =
		(*out <= lower) ? lower : // out <= lower : return lower
		(*out <= upper) ? *out :  // out <= upper : return out
		upper;                     // out >  upper : return upper
}
template <typename T>
T alib_clamp(T n, T lower, T upper);
#include <codecvt>
#include <locale>
std::wstring convert_wstr(std::string str);

// Begin std::string specific utilities

#include <string>
#include <regex>		// regex, sregex_token_iterator
#include <algorithm>    // copy
#include <iterator>     // back_inserter
#include <iomanip>

void alib_split(std::string arr, std::string del, std::vector<std::string>* out);
void alib_split_quoted(std::string arr, std::vector<std::string>* out);

std::vector<std::string> alib_split(const std::string& s, char delim);
void alib_strsplit(std::string& str, char delim, std::vector<std::string>& out);
std::string alib_str_hex(std::string s);
std::string alib_int_hex(long l);

#include <string>
#include <algorithm>

//  Lowercases string
std::string alib_lower(const char* s);
std::string alib_lowers(std::string s2);


// Uppercases string
std::string alib_upper(const char* s);
std::string alib_uppers(std::string s2);
// Uppies!!! YAYYYYY
std::string alib_uppies(std::string s2);

double alib_percent(long double num, double percent);
float alib_percentf(float num, float percent);

int alib_percents(int base, std::string percent_str);

#include <cmath>
#define alib_max(a,b) (((a) > (b)) ? (a) : (b))
#define alib_min(a,b) (((a) < (b)) ? (a) : (b))
template<typename T> struct alib_is_shared_ptr : std::false_type {};
template<typename T> struct alib_is_shared_ptr<std::shared_ptr<T>> : std::true_type {};


template <typename T>
void alib_remove_any_of(std::vector<T> _v, T vy);

template <typename T>
bool alib_contains_any_of(std::vector<T> _v, T vy);

#endif // ALIB_NO_BYTE_UTILS

#if defined(ALIB_FORCE_ASSERT) || (!defined(ALIB_NO_ASSERT))
#include <assert.h>
#include <stdio.h>
#include <conio.h>
#define alib_assert(condition, fmt, ...) if (condition) { fprintf(stderr, fmt, __VA_ARGS__); fprintf(stderr, "\nPress any key to exit.\n"); while (!_getch()) {} exit(-1); }
#define alib_asserti(condition, fmt, ...) if (condition) { \
	fprintf(stderr, fmt, __VA_ARGS__); fprintf(stderr, "\nIgnore? Y/N"); \
	int k = _getch();\
	if (tolower(k)== 'y') {} else if (tolower(k) == 'n') {\
		exit(-1);\
	} else {fprintf(stderr, "Not a valid input. Please type Y/N");}\
}
//#if defined (_WIN32)
//#include <Windows.h>
//#define alib_assert_p(condition,fmt, ...) if( condition ){ MessageBox(0, convert_wstr(alib_strfmt(fmt, __VA_ARGS__)).c_str(), L"Assertion failed", MB_OK); exit(-1); }
//#define alib_assert_pi(condition, fmt, ...) if (condition) { 
//        int state = MessageBox(0, convert_wstr(alib_strfmt(fmt, __VA_ARGS__)).c_str(), L"Assertion failed. Ignore?", MB_YESNO); 
//        switch (state){
//        case IDNO:
//            exit(1);
//            break;
//        };
//}
//#endif
#endif // ALIB_FORCE_ASSERT
#include <vector>
#include <map>
template <typename T>
void alib_invalidatev(std::vector<T> __v);
template <typename K = std::string, typename V>
void alib_invalidatem(std::map<K, V> m);
template <typename V_T>
void alib_remove_if(std::vector<V_T> _vec, std::function<bool(V_T)> _p);
#include <chrono>
#include <thread>
#define alib_sleep_micros(micros) std::this_thread::sleep_for(std::chrono::microseconds(micros));
#define alib_sleep_millis(millis) std::this_thread::sleep_for(std::chrono::milliseconds(millis));
#define alib_sleep_second(seconds) std::this_thread::sleep_for(std::chrono::seconds(seconds));
// T should be a numeric or a numeric-like object.
// Average of a numeric type. Defaults to the average of 2 objects.
template <typename T, int max_elements = 2> struct alib_average {
	int length = max_elements;
	ALStack<T> m_stack;
	void add(T element);
	T get();
};

#endif // __lib_aveth_utils_hpp


// begin preprocessor defs that need to be explicitly defined
/**
 * @paragraph TODO: move this into the .cpp file!
 * @paragraph TODO: The engine does not support android at the moment! Planned compatibility once engine base is finished.
 */
#if defined(ALIB_ANDROID) && !defined(alib_android_logging_helper__)
#define alib_android_logging_helper__
#if !defined(ALIB_PROJECT_NAME)
#define ALIB_PROJECT_NAME "UnnamedAndroidProject (DEFINE USING #define ALIB_PROJECT_NAME '')"
#endif
int android_fread(void* cookie, char* buf, int size) {
	return AAsset_read((AAsset*)cookie, buf, size);
}

int android_fwrite(void* cookie, const char* buf, int size) {
	return EACCES; // can't provide write access to the apk
}

fpos_t android_fseek(void* cookie, fpos_t offset, int whence) {
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

int android_fclose(void* cookie) {
	AAsset_close((AAsset*)cookie);
	return 0;
}
long android_ftell(void* cookie) {
	return AAsset_getLength64((AAsset*)cookie);
}
FILE* android_fopen(const char* fname, AAssetManager* _mgr) {
	AAsset* asset = AAssetManager_open(_mgr, fname, 0);
	if (!asset) return NULL;

	return funopen(asset, android_fread, android_fwrite, android_fseek, android_fclose);
}
#define alib_LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, ALIB_PROJECT_NAME, __VA_ARGS__))
#define alib_LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, ALIB_PROJECT_NAME, __VA_ARGS__))
#define alib_LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, ALIB_PROJECT_NAME, __VA_ARGS__))
#define alib_LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, ALIB_PROJECT_NAME, __VA_ARGS__))
#define alib_LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, ALIB_PROJECT_NAME, __VA_ARGS__))
#endif

// Create macros to emulate "public T N = V" used in other languages
#if defined(ALIB_VARIABLE_MANAGER) && !defined(alib_visibility_helper__)
#define alib_visibility_helper__

// These macros assume that it's to invert a variable's visibility, so don't use them if you don't want that!

#define public(var) public: var; private:
#define private(var) private: var; public:

#endif


#if defined(ALIB_INSTANCER) && !defined(alib_instance_management_helper__)
#define alib_instance_management_helper__

#define __private_internal_(var) private: var; public:
// Creates a private(static TYPE* instance)
#define genInstance(TYPE) \
	__private_internal_(static TYPE* instance); \
	static TYPE* GetInstance() { return (instance == nullptr) ? new TYPE() : instance; }
#define genInstanceDef(TYPE) \
	TYPE* TYPE::instance = nullptr;

#endif


//
// nlohmann/json utilities
//
#if defined(INCLUDE_NLOHMANN_JSON_HPP_) && (!defined(alib_json_utilities__included_))
#define alib_json_utilities__included_
// nlohhman/json included, or alib_force_json defined use these utilities

using ___alib__json = nlohmann::json;
#define JSONREF ___alib__json &

#undef JSONREF

#endif

template<typename K, typename V>
std::map<V, K> alib_reverse_map(const std::map<K, V>& m) {
	std::map<V, K> r;
	for (const auto& kv : m) {
		r[kv.second] = kv.first;
	}
	return r;
}
template<typename K, typename V>
std::unordered_map<V, K> alib_reverse_hmap(const std::unordered_map<K, V>& m) {
	std::unordered_map<V, K> r;
	for (const auto& kv : m) {
		r[kv.second] = kv.first;
	}
	return r;
}
template <typename k, typename v>
bool alib_VectorWithPairContains(std::vector<std::pair<k, v>> vec, k key) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec.at(i).first == key) {
			return true;
		}
	}
	return false;
}
template <typename v>
size_t alib_FindValueInVector(std::vector<v> vec, v val) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec.at(i) == val) {
			return i;
		}
	}
	return vec.size();
}


template <typename k, typename v>
size_t alib_FindOffsetOfPairInVector(std::vector<std::pair<k, v>> vec, k key) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec.at(i).first == key) {
			return i;
		}
	}
	return vec.size();
}

template<typename T>
T alib_clamp(T n, T lower, T upper) {
	return n <= lower ? lower : n <= upper ? n : upper;
}

template <typename T>
void alib_remove_any_of(std::vector<T> _v, T vy) {
	auto idx = _v.begin();
	while ((idx = std::find(_v.begin(), _v.end(), vy)) != _v.end()) {
		_v.erase(idx);
		_v.shrink_to_fit();
	}
}
template <typename T>
bool alib_contains_any_of(std::vector<T> _v, T vy) {
	auto& it = std::find(_v.begin(), _v.end(), vy);
	return it != _v.end();
}
template <typename T>
void alib_invalidatev(std::vector<T> __v) {
	for (int i = 0; i < __v.size(); i++) {
		delete& __v.at(i);
	}
	__v.clear();
}

template <typename K, typename V>
void alib_invalidatem(std::map<K, V> m) {
	for (const auto& kv : m) {
		delete kv.second;
	}
	m.clear();
}

template <typename V_T>
void alib_remove_if(std::vector<V_T> _vec, std::function<bool(V_T)> _p) {
	for (int i = 0; i < _vec.size(); i++) {
		if (_p(_vec.at(i))) {
			_vec.erase(_vec.begin() + i);
		}
	}
}





template<typename T, int max_elements>
inline void alib_average<T, max_elements>::add(T element) {
	if (m_stack.size() >= length) {
		m_stack.pop_back();
	}
	m_stack.push_front(element);
}

template<typename T, int max_elements>
inline T alib_average<T, max_elements>::get() {
	T sum = 0;
	int amt = 0;
	for (int i = 0; i < length && i < m_stack.size(); i++) {
		sum += *(m_stack.Data + i);
		amt++;
	}
	if (amt)
		return sum / amt;
	return 0;
}







// Constructors, destructor
template<typename T>
inline ALStack<T>::ALStack() { Size = Capacity = 0; Data = NULL; }

template<typename T>
inline ALStack<T>::ALStack(const ALStack<T>& src) { Size = Capacity = 0; Data = NULL; operator=(src); }

template<typename T>
inline ALStack<T>& ALStack<T>::operator=(const ALStack<T>& src) { clear(); resize(src.Size); memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }

template<typename T>
inline ALStack<T>::~ALStack() { if (Data) free(Data); }

template<typename T>
inline void ALStack<T>::clear() { if (Data) { Size = Capacity = 0; free(Data); Data = NULL; } }

// Important: does not destruct anything
template<typename T>
inline void ALStack<T>::clear_delete() { for (int n = 0; n < Size; n++) Data[n].~T(); clear(); }

// Important: never called automatically! always explicit.
template<typename T>
inline void ALStack<T>::clear_destruct() { for (int n = 0; n < Size; n++) Data[n].~T(); clear(); }

template<typename T>
inline bool ALStack<T>::empty() const { return Size == 0; }

template<typename T>
inline int ALStack<T>::size() const { return Size; }

template<typename T>
inline int ALStack<T>::size_in_bytes() const { return Size * (int)sizeof(T); }

template<typename T>
inline int ALStack<T>::max_size() const { return 0xff / (int)sizeof(T); }

template<typename T>
inline int ALStack<T>::capacity() const { return Capacity; }

template<typename T>
inline T* ALStack<T>::begin() { return Data; }

template<typename T>
inline const T* ALStack<T>::begin() const { return Data; }

template<typename T>
inline T* ALStack<T>::end() { return Data + Size; }

template<typename T>
inline const T* ALStack<T>::end() const { return Data + Size; }

template<typename T>
inline T& ALStack<T>::front() { assert(Size > 0); return Data[0]; }

template<typename T>
inline const T& ALStack<T>::front() const { assert(Size > 0); return Data[0]; }

template<typename T>
inline T& ALStack<T>::back() { assert(Size > 0); return Data[Size - 1]; }

template<typename T>
inline T& ALStack<T>::top() { return back(); }

template<typename T>
inline const T& ALStack<T>::back() const { assert(Size > 0); return Data[Size - 1]; }

template<typename T>
inline const T& ALStack<T>::top() const { return back(); }

template<typename T>
inline T& ALStack<T>::at(int amt) { assert(amt < Size); return Data + amt; }

template<typename T>
inline void ALStack<T>::swap(ALStack<T>& rhs) { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

template<typename T>
inline T* ALStack<T>::insert(const T* it, const T& v) { assert(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }

template<typename T>
inline bool ALStack<T>::contains(const T& v) const { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }

template<typename T>
inline int ALStack<T>::_grow_capacity(int sz) const { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > sz ? new_capacity : sz; }

template<typename T>
inline void ALStack<T>::resize(int new_size) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }

template<typename T>
inline void ALStack<T>::resize(int new_size, const T& v) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }

template<typename T>
inline void ALStack<T>::shrink(int new_size) { assert(new_size <= Size); Size = new_size; }

// Resize a vector to a smaller size, guaranteed not to cause a reallocation
template<typename T>
inline void ALStack<T>::reserve(int new_capacity) { if (new_capacity <= Capacity) return; T* new_data = (T*)malloc((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); free(Data); } Data = new_data; Capacity = new_capacity; }

// NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the ImVector data itself! e.g. v.push_back(v[10]) is forbidden.
template<typename T>
inline void ALStack<T>::push_back(const T& v) { assert(Size == Capacity); reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }

template<typename T>
inline void ALStack<T>::pop_back() { assert(Size > 0); Size--; }

template<typename T>
inline void ALStack<T>::pop() { assert(Size > 0); Size--; }

template<typename T>
inline void ALStack<T>::push(const T& v) { if (Size == 0) push_back(v); else insert(Data, v); }

template<typename T>
inline void ALStack<T>::push_front(const T& v) { if (Size == 0) push_back(v); else insert(Data, v); }



#pragma warning (pop)