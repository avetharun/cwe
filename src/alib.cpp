#include "alib.h"

std::string ___nn_alib_error_charp_str;
float alib_rad2deg(float rad) {
	return rad * 180 / M_PI;
}

float alib_deg2rad(float deg) {
    return deg * M_PI / 180;
}


#ifdef __GNUC__
#define VSCPRINTF "implemented"




int vscprintf(const char* format, va_list ap)
{
	va_list ap_copy;
	va_copy(ap_copy, ap);
	int retval = vsnprintf(NULL, 0, format, ap_copy);
	va_end(ap_copy);
	return retval;
}
#endif


#ifdef _MSC_VER


int vasprintf(char** strp, const char* format, va_list ap)
{
	int len = vscprintf(format, ap);
	if (len == -1)
		return -1;
	char* str = (char*)malloc((size_t)len + 1);
	if (!str)
		return -1;
	int retval = vsnprintf(str, len + 1, format, ap);
	if (retval == -1) {
		free(str);
		return -1;
	}
	*strp = str;
	return retval;
}
#endif

int asprintf(char** strp, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	int retval = vasprintf(strp, format, ap);
	va_end(ap);
	return retval;
}

size_t alib_2d_ar_pos(size_t pitch, size_t x, size_t y, size_t bytes_per_step) {
	return y * pitch + x * bytes_per_step;
}

size_t alib_va_arg_length(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t bufsz = snprintf(NULL, 0, fmt, args);
	va_end(args);
	return bufsz;
}

size_t alib_va_arg_length(const char* fmt, va_list args) {
	size_t bufsz = vsnprintf(NULL, 0, fmt, args);
	return bufsz;
}

// Opens a URL externally
void alib_urlhttpopen_ext(alib_string url) {
#ifdef ALIB_WIN
	//ShellExecuteA(0, 0, url, 0, 0, SW_SHOW);
#endif
}

alib_string alib_urlhttpget(alib_string url) {
#ifdef ALIB_WIN


#endif
	return "";
}

alib_string alib_strfmtv(const char* fmt, va_list args) {
	size_t bufsz = vsnprintf(NULL, 0, fmt, args);
	alib_string m_out = std::string("\0", bufsz);
	vsnprintf(m_out.data(), bufsz + 1, fmt, args);
	return m_out.data();
}

alib_string alib_strfmt(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t bufsz = 0;
	bufsz = vsnprintf(NULL, 0, fmt, args);
	alib_string m_out = std::string("\0", bufsz);
	vsnprintf(m_out.data(), bufsz + 1, fmt, args);
	va_end(args);
	return m_out;
}

float alib_lerp_low(float a, float b, float f)
{
	return a + f * (b - a);
}

float alib_lerp(float a, float b, float f) {
	return (a * (1.0f - f)) + (b * f);
}

float alib_clerp(float a, float b, float f) {
	float o = (a * (1.0f - f)) + (b * f);
	if (o < a) { return a; }
	if (o > b) { return b; }
	return o;
}

const char* alib_chrrepl(const char* in, char match, char repl_value) {
	while (*(in++)) {
		if (*in == match) {
			(*(char*)in) = repl_value;
		}
	}
	return in;
}

// Copies the sign-ed ness of A into B
void alib_copy_signed(signed int a, signed int* b) {
	*b = (a < 0) ? -*b : (*b < 0) ? -*b : *b;
}
std::wstring convert_wstr(std::string str) {
	std::wstring str2(str.length(), L' '); // Make room for characters
	std::copy(str.begin(), str.end(), str2.begin());
	return str2;
}

void alib_split(std::string arr, std::string del, std::vector<std::string>* out)
{
	size_t start = 0;
	size_t end = arr.find(del);
	while (end != -1) {
		out->push_back(arr.substr(start, end - start));
		start = end + del.size();
		end = arr.find(del, start);
	}
	out->push_back(arr.substr(start, end - start));
}

void alib_split_quoted(std::string arr, std::vector<std::string>* out)
{
	size_t start = 0;
	size_t end = arr.find("\"");
	while (end != -1) {
		out->push_back(arr.substr(start, end - start));
		start = end + 1;
		end = arr.find("\"", start);
	}
	out->push_back(arr.substr(start, end - start));
}

std::vector<std::string> alib_split(const std::string& s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		//elems.push_back(item);
		elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}

void alib_strsplit(std::string& str, char delim, std::vector<std::string>& out)
{
	size_t start;
	size_t end = 0;
	// Doesn't contain 'delim', so return the base string itself.
	if ((str.find_first_not_of(delim, end)) == std::string::npos) {
		out.push_back(str);
		return;
	}
	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

std::string alib_str_hex(std::string s) {

	std::stringstream s1;
	for (int i = 0; i < s.length(); i++) {
		s1 << std::hex << (int)s.at(i);
	}

	return s1.str();
}

std::string alib_int_hex(long l) {
	std::stringstream stream;
	stream << std::hex << l;
	return std::string(stream.str());
}

//  Lowercases string
std::string alib_lower(const char* s)
{
	std::string s2 = s;
	std::transform(s2.begin(), s2.end(), s2.begin(), tolower);
	return s2;
}

std::string alib_lowers(std::string s2)
{
	std::transform(s2.begin(), s2.end(), s2.begin(), tolower);
	return s2;
}

// Uppercases string
std::string alib_upper(const char* s)
{
	std::string s2 = s;
	std::transform(s2.begin(), s2.end(), s2.begin(), toupper);
	return s2;
}

std::string alib_uppers(std::string s2)
{
	std::transform(s2.begin(), s2.end(), s2.begin(), toupper);
	return s2;
}
std::string alib_uppies(std::string s2) { return alib_uppers(s2); }
double alib_percent(long double num, double percent) {
	long double _n_d100 = (num / 100);
	return lroundl(_n_d100 * percent);
}
float alib_percentf(float num, float percent) {
	float _n_d100 = (num * 0.01);
	return lroundf(_n_d100 * percent);
}
int alib_percents(int base, std::string percent_str) {
	if ((alib_endswith(percent_str.c_str(), "%"))) percent_str.erase(percent_str.end());
	int percent = atoi(percent_str.c_str());
	return alib_percent(base, percent);
}








static const char* alib_bit_rep[16] = {
	"0000","0001","0010","0011",
	"0100","0101","0110","0111",
	"1000","1001","1010","1011",
	"1100","1101","1110","1111",
};



unsigned long alib_fnull(const char* arr) {
	int n_t_offset = 0;
	while (*arr++ != '\0') { n_t_offset++; }
	return n_t_offset;
}



bool alib_atob(const char* buffer) {
	size_t l = strlen(buffer);
	if (l >= 1) {
		if (buffer[0] == 't' || buffer[0] == '1') {
			return true;
		}
		if (buffer[0] == 'f' || buffer[0] == '0') {
			return false;
		}
	}
	return false;
}

char* alib_itoa(int num, char* buffer, int base) {
	int curr = 0;

	if (num == 0) {
		// Base case
		buffer[curr++] = '0';
		buffer[curr] = '\0';
		return buffer;
	}

	int num_digits = 0;

	if (num < 0) {
		if (base == 10) {
			num_digits++;
			buffer[curr] = '-';
			curr++;
			// Make it positive and finally add the minus sign
			num *= -1;
		}
		else
			// Unsupported base. Return NULL
			return NULL;
	}

	num_digits += (int)floor(log(num) / log(base)) + 1;

	// Go through the digits one by one
	// from left to right
	while (curr < num_digits) {
		// Get the base value. For example, 10^2 = 1000, for the third digit
		int base_val = (int)pow(base, num_digits - 1 - curr);

		// Get the numerical value
		int num_val = num / base_val;

		char value = num_val + '0';
		buffer[curr] = value;

		curr++;
		num -= base_val * num_val;
	}
	buffer[curr] = '\0';
	return buffer;
}

char* alib_ftoa(char* buf, size_t len, float f)
{
	char* cp = buf;
	unsigned long    l, rem;

	if (f < 0) {
		*cp++ = '-';
		f = -f;
	}
	l = (unsigned long)f;
	f -= (float)l;
	rem = (unsigned long)(f * 1e6);
	snprintf(cp, len, "%lu.%6.6lu", l, rem);
	return buf;
}


#ifdef WIN32

bool alib_can_reach_mem(void* ptr) {
	__try {
		char prefix = *(((char*)ptr)); //Get the first byte. If this doesn't cause an error, then we can reach this.
		return true;
	}
	__except (true) { //Catch all unique exceptions (Windows exceptions) 
		return false; //Can't reach this memory
	}
}
#endif

// If size != 0, leave size as is
void __alib_internal_reqlen__f_impl(size_t* sz, alib_string arr) {
	if ((*sz) == 0) {
		*sz = arr.size();
	}
}

const char* __alib_strfmt_file(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t bufsz = 0;
	bufsz = snprintf(NULL, 0, fmt, args);
	const char* _buf = (const char*)malloc(bufsz);
	vsnprintf((char*)_buf, bufsz, fmt, args);
	va_end(args);
	return _buf;
}

bool alib_file_exists(const char* name) {
	if (FILE* file = fopen(name, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

#ifdef __unix__
size_t alib_file_length(FILE* file, stat* file_stat) {
	fstat(_fileno(file), file_stat);
	size_t sz = file_stat->st_size;
	return sz;
}

#endif
size_t alib_file_length(FILE* file) {
	struct stat file_stat;
	fstat(_fileno(file), &file_stat);
	size_t sz = file_stat.st_size;
	return sz;
}

size_t alib_file_length(const char* fname) {
	if (!alib_file_exists(fname)) {
		return 0;
	}
	FILE* file = fopen(fname, "rb");
	size_t sz = alib_file_length(file);
	fclose(file);
	return sz;
}

std::string alib_file_read(std::ifstream& file) {
	std::ostringstream buf;
	buf << file.rdbuf();
	return buf.str();
}

std::string alib_file_read(const char* fname) {
	std::ostringstream buf;
	std::ifstream file(fname, std::ios::binary);
	buf << file.rdbuf();
	file.close();
	return buf.str();
}

void alib_file_read(const char* fname, const char* out_, unsigned long long* size_) {
	if (!alib_file_exists(fname)) {
		return;
	}
	FILE* file = fopen(fname, "rb");
	size_t len = alib_file_length(file);
	*size_ = len;
	if (!out_) {
		out_ = alib_malloca(char, len);
	}
	fread((void*)out_, len, 1, file);
	fclose(file);
}

void alib_file_write(const char* filen, alib_string d, size_t l) {
	std::ofstream file(filen, std::ios::out | std::ios::binary);
	__alib_internal_reqlen__f_impl(&l, d);
	file.write(d, l);
	file.close();
}

// Note: does NOT close file after!
void alib_file_write(std::ostream& file, const char* d, size_t l) {
	__alib_internal_reqlen__f_impl(&l, d);
	file.write(d, l);
}

bool alib_scwd(const char* dir) { return CHDIR(dir) == 0; }

char* alib_gcwd() {
	return GETCWD(0, 0);
}

unsigned long alib_n_fnull(const char* arr, int limit) {
	for (int i = 0; i < limit; i++) {
		if (arr[i] == '\0') {
			return i;
		}
	}
	return 0;
}

size_t alib_nulpos(const char* arr) {
	return alib_fnull(arr);
}

size_t alib_nulposn(const char* arr, int limit) {
	return alib_n_fnull(arr, limit);
}

int alib_log(int base, int n) {
#ifndef _CMATH_
	// 75% as fast as cmath, if using recursion. If we have cmath avalible, use that instead.
	return (n > base - 1)
		? 1 + alib_log(n / base, base)
		: 0;
#else
	return (int)(log(n) / log(base));
#endif
}

bool alib_wrange(int min, int max, int val) {
	return ((val - max) * (val - min) <= 0);
}

// Taken from python's usage of math.isclose()
bool alib_fisclose(float first, float second, float rel_tol, float abs_tol) {
	return abs(first - second) <= alib_max(rel_tol * alib_max(abs(first), abs(second)), abs_tol);
}

// Taken from python's usage of math.isclose()
bool alib_disclose(double first, double second, double rel_tol, double abs_tol) {
	return abs(first - second) <= alib_max(rel_tol * alib_max(abs(first), abs(second)), abs_tol);
}

// Check if the difference between [0] and [1] are within [amount]
bool alib_iswithin(float first, float second, float amount) {
	float a = abs(first - second);
	return (a < amount);
}

int alib_digitsInNum(long n, int base)
{
	if (n == 0) { return 1; }
	if (n < 0) {
		n *= -1;
	}
	// log using base, then
	// taking it to the lowest int then add 1
	return (int)floor(alib_log(base, n)) + 1;
}

// Get digits of [num], formatted as ASCII (by default, pass false to disable)
// arr[0] is the amount of digits in the array
// Note: malloc 
char* alib_getDigitsOfNumber(int num, bool ascii) {
	int amt_digits = alib_digitsInNum(num, 10);
	if (amt_digits <= 0) {
		amt_digits = 1;
	}
	char* digits = (char*)malloc(static_cast<size_t>(amt_digits) + 1);
	int i = 0;
	while (num != 0 && i < amt_digits)
	{
		// What even is math?
		const int least_significant_digit = num % 10;
		digits[i] = (least_significant_digit + ((ascii) ? 48 : 0));
		num /= 10;
		i++;
	}

	return digits;
}

// If size != 0, leave size as is, otherwise size = strlen(arr)
void alib_reqlen(size_t* sz, const char* arr) {
	if ((*sz) == 0) {
		(*sz) = strlen(arr);
	}
}

void alib_print_byte(uint8_t byte)
{
	printf("%s%s", alib_bit_rep[byte >> 4], alib_bit_rep[byte & 0x0F]);
}

int alib_endswith(const char* str, const char* suffix)
{
	if (!str || !suffix)
		return 0;
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if (lensuffix > lenstr)
		return 0;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int alib_beginswith(const char* str, const char* prefix)
{
	if (!str || !prefix)
		return 0;
	size_t lenstr = strlen(str);
	size_t lenprefix = strlen(prefix);
	if (lenprefix > lenstr)
		return 0;
	return strncmp(str, prefix, lenprefix) == 0;
}

// Get string between two strings
int alib_substr2(const char* input, size_t input_len) {
	return 0;
}

// if string begins with X and the character afterward is NOT the last character of the prefix.
int alib_beginswith_anddoesntfollow(const char* str, const char* prefix)
{
	if (!str || !prefix)
		return 0;
	size_t lenstr = strlen(str);
	size_t lenprefix = strlen(prefix);
	if (lenprefix > lenstr)
		return 0;
	// No character after
	if (lenstr < lenprefix + 1) { return strncmp(str, prefix, lenprefix) == 0; }
	return strncmp(str, prefix, lenprefix) == 0 && str[lenprefix] != prefix[lenprefix - 1];
}

// get position of char 
int alib_getchrpos(const char* src, char c, size_t len)
{
	alib_reqlen(&len, src);
	for (size_t i = 0; i < len; i++)
	{
		if (src[i] == c)
			return (int)i;
	}
	return -1;
}

// char array contains
bool alib_costr(std::string src, const char* match) {
	if (src.find(match) != std::string::npos) {
		return true;
	}
	return false;
}

int alib_chreq(const char* src, const char* match) {
	size_t sl = strlen(src);
	size_t ml = strlen(match);
	if (ml > sl) { return false; }
	return strncmp(src, match, ml);
}

int alib_streq(std::string src, const char* match) {
	size_t sl = src.size();
	size_t ml = strlen(match);
	if (ml > sl) { return false; }
	return (src.compare(match) == 0);
}

int alib_streqn(std::string src, const char* match, const size_t len) {
	return strncmp(src.c_str(), match, len) == 0;
}

// Occurances of char `c` in `src`
size_t alib_chrocc(const char* src, char c, size_t len) {
	alib_reqlen(&len, src);
	size_t occ = 0;
	for (size_t i = 0; i < len; i++) {
		if (src[i] == c) { occ++; }
	}
	return occ;
}

const char* alib_rmocc(const char* src, char c, size_t len) {
	alib_reqlen(&len, src);
	std::vector<char> src_copy = *(std::make_unique<std::vector<char>>().get());
	for (int i = 0; i < len; i++) {
		if (src[i] == c) { continue; }
		src_copy.push_back(src[i]);
	}
	src_copy.push_back('\0');
	return src_copy.data();
}

// get byte at offset of array 
char alib_get_byte(void* data, int offset) {
	return ((char*)data)[offset];
}

// get byte at array[0]
char alib_get_byte(void* data) {
	return ((char*)data)[0];
}

// set byte at offset of array 
void alib_set_byte(void* data, char byte, int offset) {
	reinterpret_cast<char*>(data)[offset] = byte;
}

// set byte at array[0]
void alib_set_byte(void* data, char byte) {
	reinterpret_cast<char*>(data)[0] = byte;
}

inline alib_inline_run::alib_inline_run(std::function<void()> initFunc) {
	initFunc();
}

inline alib_string::alib_string(std::string other) {
	this->swap(other);
}

inline alib_string::alib_string(const char* buf) {
	this->append(buf);
}

inline alib_string::operator const char* () {
	return this->c_str();
}
inline alib_string::operator char* () {
	return (char*)this->c_str();
}
