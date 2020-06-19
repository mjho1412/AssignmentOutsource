#include "dbLib.h"

template <typename T>
class MyLink {
public:
	T data;
	MyLink<T>* next;
    //~MyLink() { delete next; }
	MyLink<T>( MyLink<T> *n = nullptr) { next = n; }
	MyLink<T>(const T& ele,  MyLink<T> *n = nullptr) {
        data = ele;
		next = n;
	}
};



template <typename T>
class MyLList
{
private:
    MyLink<T>* head;
    MyLink<T>* last;
    MyLink<T>* curr;
    int cnt;

public:
    MyLList() { head = last = curr = new MyLink<T>(); }
    ~MyLList() {
        last = curr = nullptr;
        while (head != NULL) {
            MyLink<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }
    int length() const { return cnt; }
    void insert(const T& ele)
    {
        if (last == nullptr)
        {
            head = last = curr = new MyLink<T>(ele);
        }
        else {
            last->next = new MyLink<T>(ele);
            last = last->next;
        }
        cnt++;
        //curr->next = new Link<T>(ele, curr->next);
    }
};



class ProcessData
{
private:
	MyLList<CurrencyPairInfoTree> *data;
	static const int MAXSIZECODE = 8;
	static int split(string line, string *&sp);
	static int count_space(string s);
public:
	ProcessData();
	~ProcessData();
	int process(string line);
	int insert(const string *sp, const int n);
};

class Util {
public:
	static bool StrToInt(string str, int& n) {
		if (isValidNumber(str, false))
		{
			n = atoi(str.c_str());
			return true;
		}
		return false;
	}
	static bool StrToFloat(string str, float& n)
	{
		try
		{
			if (!isValidNumber(str, true))
			{
				return false;
			}
			else
			{
				n = stof(str);
				return true;
			}
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
	static bool isValidNumber(string str, bool isFloat)
	{
		if (str.length() == 0)
		{
			return false;
		}
		bool hasPoint = false;
		for (int i = 0; i < str.length(); i++)
		{
			bool isdit = isdigit(str[i]);
			if (!isdit)
			{
				if (str[i] == '.' && isFloat)
				{
					if (!hasPoint)
					{
						hasPoint = true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}
};


