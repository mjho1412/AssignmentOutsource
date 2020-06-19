#include "processData.h"

enum CodeValue
{
	sdCode,
	cdCode,
	slCode,
	insCode,
	delCode,
	updCode,
	obCode,
	cbCode,
	osCode,
	csCode
};

static map<string, CodeValue> s_mapCodeValues = {
	{"SD", sdCode},
	{"CD", cdCode},
	{"SL", slCode},
	{"INS", insCode},
	{"DEL", delCode},
	{"UPD", updCode},
	{"OB", obCode},
	{"CB", cbCode},
	{"OS", osCode},
	{"CS", csCode}};

ProcessData::ProcessData()
{
	MyLList<CurrencyPairInfoTree> *list = new MyLList<CurrencyPairInfoTree>();
	data = list;
}
ProcessData::~ProcessData()
{
	delete data;
}
/* 
	Split the command line into instruction and its parameters
	(This version is simplified to return just the instruction only; students should rewrite to make it return instruction's parameters)
	Input:
		line: the command line
		sp: pass-by-reference parameter used as an output: return the array of strings keeping instruction and its parameter
	Output: return the number of elements in array sp.	
*/
int ProcessData::split(string line, string *&sp)
{
	sp = new string[MAXSIZECODE];
	const string SEP = " ";
	int pos, lastpos = 0, idx = 0;
	pos = line.find(SEP, lastpos);
	while (pos > -1)
	{
		sp[idx] = line.substr(lastpos, pos - lastpos);
		lastpos = pos + 1;
		pos = line.find(SEP, lastpos);
		idx++;
		if (idx >= MAXSIZECODE)
		{
			return 0;
		}
	}
	if (lastpos < line.length())
	{
		sp[idx] = line.substr(lastpos, pos - lastpos);
		idx++;
	}
	if (idx <= ProcessData::count_space(line)) {
		return 0;
	}
	return idx;
}

int ProcessData::count_space(string s) {
	int count = 0;

	for (int i = 0; i < s.size(); i++)
		if (s[i] == ' ') count++;

	return count;
};

int ProcessData::process(string line)
{
	cout << line << endl;

	string *p;
	int n = ProcessData::split(line, p);
	cout << p[0] << endl;
	if (n <= 0)
	{
		delete[] p;
		return -1;
	}
	int res = -1;
	try
	{
		switch (s_mapCodeValues[p[0]])
		{
		case sdCode:
			break;
		case cdCode:
			break;
		case slCode:
			break;
		case insCode:
			res = this->insert(p, n);
			break;
		case updCode:
			break;
		case delCode:
			break;
		case obCode:
			break;
		case cbCode:
			break;
		case osCode:
			break;
		case csCode:
			break;
		default:
			res = -1;
		}
	}
	catch (invalid_argument iax)
	{
		delete[] p;
		return res;
	}
	delete[] p;
	return res;
}
/*
	To process insert instruction
	(This is the simplified version)
	Input:
		sp: pointer to string array keeping instruction and its parameters
		n: the number of elements in sp
	Output:
		return the result as required
*/
int ProcessData::insert(const string *sp, const int n)
{
	if (n != 6) {
		return -1;
	}

	BidAndAsk mData = BidAndAsk(atoi(sp[3].c_str()), stof(sp[4]), stof(sp[5]));

	//data->insert(mData);

	/*string baseCurrency = sp[1];
	string quoteCurrency = sp[2];*/

	return 1;
}
