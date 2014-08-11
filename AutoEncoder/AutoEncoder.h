#include <string>

#ifdef _DEBUG
#pragma comment(lib, "..\\iconv_debug.lib")
#pragma comment(lib, "..\\libiconv_debug.lib")
#else
#pragma comment(lib, "..\\iconv.lib")
#pragma comment(lib, "..\\libiconv.lib")
#endif

namespace AutoEncoder {
	bool readfileEncodingAuto(const wchar_t *path, std::wstring &out);
	bool convertEncodingAuto(const char *in, std::wstring &outw);
	std::string detectEncoding(const char* data);
};