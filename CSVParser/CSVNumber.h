class CSVNumber {
public:
	static int getNumber(int num);
	static void setNumber(int num, int val);
	static void increaseNumber(int num);
	static void decreaseNumber(int num);
private:
	static int data[1000];
};