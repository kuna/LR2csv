#include <string>

class CSVText {
private:
	static std::wstring data[200];

public:
	static std::wstring &GetText(int num);
	static void SetText(int num, std::wstring str);
};