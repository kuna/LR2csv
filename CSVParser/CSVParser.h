#include <tchar.h>
#include <vector>
#include <string>

/*******************************
 * class CSVParser
 *******************************
 * read & write csv file.
 * optimized for LR2 csv data file.
 * 
 * providing method:
 * [public]
 * ReadCSVFile(wchar_t) : read csv file
 * WriteCSVFile(wchar_t) : write csv file
 * resetpos() : reset pos
 * next() : select next object. not found, return FALSE (index wont change)
 * nextSRC() : select next SRC object. (if filtered, #SRC_ is automatically attached)
 * - find same-named #SRC one.
 * - when not found, return FALSE.
 * nextDST() : select next DST object. 
 * - (if filtered, #DST_ is automatically attached)
 * - if current one is #SRC, then find paired #DST one.
 * - when not found, return FALSE
 * select(int) : select [n]th object
 * getindex() : get current object index
 * insert(a, b) : 
 * const std::wstring& get(int) : from the selected object, get [n]th column data.
 * int get(int) : same as mentioned. (return type int)
 * double get(int) : same as mentioned. (return type int)
 * void set(int, int)
 * void set(int, const std::wstring&)
 * void set(int, double)
 * void setFilter(const std::wstring&) : set filter for next() method. need to call resetpos()
 * void clearFilter() : clear filter. same as setFilter()
 * void selectFiltered(const std::wstring&) : calls internally resetpos, setfilter, next. only for single data selection.
 * 
 * [private]
 * readFile()
 * splitLine()
 * removeLineComment()
 *
 *
 *
 * [parsing example]
 * 1. csv skin (play)
 *
 * while (next()) 
 *     (copy #SRC data)
 *     if (nextDST()) {
 *         ab
 *         ab
 *     }
 * }
 *
 * 2. music data csv
 * 
 * if (selectFiltered(_T("#PATH"))) {
       get(1)
	   ...
 * }
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

class CSVParser {
private:
	std::vector<std::wstring> lines;
	int currentIndex;

public:
	bool ReadCSVFile(const wchar_t *path);
};