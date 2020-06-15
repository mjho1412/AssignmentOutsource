#include "dbLib.h"

template <typename T>
class MyLink {
public:
	T data;
	MyLink<T>* next;
	MyLink<T>() {}
	MyLink<T>(const MyLink<T> &n = nullptr) { next = n; }
	MyLink<T>(const T& ele, const MyLink<T> &n = nullptr) {
		data = ele;
		next = n;
	}
};

template <typename T>
class MyList
{
    // Return: The number of elements in the list.
    virtual int length() const = 0;
    virtual void insert(const T& ele) = 0;
};

template <typename T>
class MyLList : public MyList<T>
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

class BidAndAsk : Node<int> {

};

class CurrencyPairInfoTree : AVLTree<BidAndAsk>
{

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



