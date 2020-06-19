#include "dbLib.h"

template <typename T>
class Link {
public:
	T data;
	Link<T>* next;
	Link<T>() {}
	Link<T>(const Link<T>* n) { next = n; }
	Link<T>(const T& ele, Link<T>* n) {
		data = ele;
		next = n;
	}
};

template <typename T>
class List {
	// Return: The number of elements in the list.
	virtual int length() const = 0;
	virtual void insert(const T& ele) = 0;
	virtual void insertAtEnd(const T& ele) = 0;
	virtual void moveNextNode() = 0;
	virtual T* getFirstNode() = 0;
	virtual T* getNextNode() = 0;
	virtual T* getCurrentNode() = 0;
	virtual void removeAll() = 0;
	virtual void removeHeadNode() = 0;
	virtual void removeNextNode() = 0;
};

template <typename T>
class LList : public List<T> {
private:
	Link<T>* head;
	Link<T>* last;
	Link<T>* curr;
	int cnt;
public:
	~LList() {
		removeAll();
	}
	LList() { head = last = curr = nullptr; cnt = 0; }
	int length() const { return cnt; }
	void insert(const T& ele) {
		curr->next = new Link<T>(ele, curr->next);
		if (last == curr)
		{
			last = curr->next;
		}
		cnt++;
	}
	void insertAtHead(const T& ele) {
		if (head == nullptr)
		{
			head = last = curr = new Link<T>(ele, nullptr);
		}
		else
		{
			head = new Link<T>(ele, head);
		}
		cnt++;
	}
	void insertAtEnd(const T& ele)
	{
		if (last == nullptr)
		{
			head = last = curr = new Link<T>(ele, nullptr);
		}
		else
		{
			last->next = new Link<T>(ele, nullptr);
			last = last->next;
		}
		cnt++;
	}
	void moveNextNode()
	{
		curr = curr->next;
	}
	T* getFirstNode()
	{
		curr = head;
		if (curr == nullptr)
		{
			return nullptr;
		}
		return &(curr->data);
	}
	T* getNextNode()
	{
		if (curr == nullptr)
		{
			return nullptr;
		}
		return &(curr->next->data);
	}

	T* getCurrentNode()
	{
		if (curr == nullptr)
		{
			return nullptr;
		}
		return &(curr->data);
	}
	void removeAll()
	{
		curr = head;
		while (curr != nullptr)
		{
			head = curr->next;
			delete curr;
			curr = head;
		}
		delete head;
		//if(last != nullptr) delete last;
		curr = head = last = nullptr;
		cnt = 0;



		delete curr;
	}
	void removeHeadNode()
	{
		curr = head;
		if (curr == nullptr)
		{
			return;
		}
		if (head == last)
		{
			head = last = nullptr;
		}
		else
		{
			head = curr->next;
		}
		delete curr;
		curr = head;
		cnt--;
	}
	void removeNextNode()
	{
		if (curr == nullptr)
		{
			return;
		}
		else if (curr->next != nullptr)
		{
			if (curr->next == last)
			{
				last = curr;
			}
			Link<T>* deletedNode = curr->next;
			curr->next = deletedNode->next;
			delete deletedNode;
			deletedNode = NULL;
			cnt--;
		}
	}
};



class ProcessData
{
private:
	LList<CurrencyPairInfoTree> *data;
	static const int MAXSIZECODE = 8;
	static int split(string line, string *&sp);
	static int count_space(string s);
public:
	ProcessData();
	~ProcessData();
	int process(string line);
	int insert(const string *sp, const int n);
	int remove(const string* sp, const int n);
	int update(const string* sp, const int n);
	CurrencyPairInfoTree* findTreeOfPair(string baseCurrency, string quoteCurrency);
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


