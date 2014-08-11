// uses uchardet library & iconv library.
// by kuna. MIT License.

#include "AutoEncoder.h"
#include "uchardet.h"
#include "iconv.h"

bool AutoEncoder::readfileEncodingAuto(const wchar_t *path, std::wstring &out) {
	FILE *fp = _wfopen(path, L"rb");
	if (!fp)
		return false;
	
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *outbuf = (char*)malloc(fileSize + 10);
	fread(outbuf, 1, fileSize, fp);
	fclose(fp);

	if (convertEncodingAuto(outbuf, out)) {
		free(outbuf);
		return true;
	} else {
		free(outbuf);
		return false;
	}
}

bool AutoEncoder::convertEncodingAuto(const char *in, std::wstring &outw) {
	std::string encode = detectEncoding(in);
	if (encode == "error" || encode == "unknown") {
		// just copy
		iconv_t cd;
		if ((cd = iconv_open("UTF-16LE", "ASCII")) == (iconv_t) -1) {
			iconv_close(cd);
		} else {
			size_t in_bytes = strlen(in);
			size_t out_bytes = (in_bytes+1) *4;
			char *out = (char *) calloc(out_bytes, sizeof(char));
			char *outp = out;

			iconv(cd, &in, &in_bytes, &outp, &out_bytes);

			outw = (wchar_t*)out;

			iconv_close(cd);
			if (out) free(out);
		}

		return false;
	} else {
		iconv_t cd;
		if ((cd = iconv_open("UTF-16LE", encode.c_str())) == (iconv_t) -1) {
			iconv_close(cd);
			return false;
		} else {
			size_t in_bytes = strlen(in);
			size_t out_bytes = (in_bytes+1) *4;
			char *out = (char *) calloc(out_bytes, sizeof(char));
			char *outp = out;

			if(iconv(cd, &in, &in_bytes, &outp, &out_bytes)== -1) {
				char *sterr = strerror(errno);
				if (strcmp(sterr, "E2BIG")==0
					|| strcmp(sterr, "EILSEQ")==0
					|| strcmp(sterr, "EINVAL")==0) {
					iconv_close(cd);
					if (out) free(out);
					return false;
				}
			}

			outw = (wchar_t*)out;

			iconv_close(cd);
			if (out) free(out);
			return true;
		}
	}
}

std::string AutoEncoder::detectEncoding(const char* data) {
	uchardet_t detector = uchardet_new();
	int r = uchardet_handle_data(detector, data, strlen(data));
	if (r) {
		uchardet_delete(detector);
		return "error";
	}
	uchardet_data_end(detector);
	const char* charset = uchardet_get_charset(detector);
	std::string result;
	if (*charset) {
		result = charset;
	} else {
		result = "unknown";
	}
	uchardet_delete(detector);
	return result;
}