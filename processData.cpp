#include "processData.h"

const int AMOUNT_EACH_LOT = 100000;

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
	LList<CurrencyPairInfoTree> *list = new LList<CurrencyPairInfoTree>();
	data = list;
	LList<OrderInfoTree>* list2 = new LList<OrderInfoTree>();
	orderData = list2;
	LList<string>* list3 = new LList<string>();
	orderIdList = list3;
}
ProcessData::~ProcessData()
{
	delete data;
	delete orderData;
	delete orderIdList;
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
	//cout << line << endl;

	string *p;
	int n = ProcessData::split(line, p);
	//cout << p[0] << endl;
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
			res = this->createOrAdjustMarginAccount(p, n);
			break;
		case cdCode:
			res = this->checkMarginAccount(p, n);
			break;
		case slCode:
			res = this->setMarginPercent(p, n);
			break;
		case insCode:
			res = this->insert(p, n);
			break;
		case updCode:
			res = this->update(p, n);
			break;
		case delCode:
			res = this->remove(p, n);
			break;
		case obCode:
			if (accountBalance < 0) {
				res = -1;
			}
			else {
				res = this->addNewOrder(p, n);
			}	
			break;
		case cbCode:
			if (accountBalance < 0) {
				res = -1;
			}
			else {
				res = this->closeOrder(p, n);
			}
			break;
		case osCode:
			if (accountBalance < 0) {
				res = -1;
			}
			else {
				res = this->addNewOrder(p, n);
			}
			break;
		case csCode:
			if (accountBalance < 0) {
				res = -1;
			}
			else {
				res = this->closeOrder(p, n);
			}
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


CurrencyPairInfoTree* ProcessData::findTreeOfPair(string baseCurrency, string quoteCurrency) {
	CurrencyPairInfoTree* exchange = ((LList<CurrencyPairInfoTree>*)data)->getFirstNode();
	while (exchange != NULL && !exchange->isEqual(baseCurrency, quoteCurrency))
	{
		((LList<CurrencyPairInfoTree>*)data)->moveNextNode();
		exchange = ((LList<CurrencyPairInfoTree>*)data)->getCurrentNode();
	}
	return exchange;
}

OrderInfoTree* ProcessData::findOrderTree(string baseCurrency, string quoteCurrency) {
	OrderInfoTree* exchange = ((LList<OrderInfoTree>*)orderData)->getFirstNode();
	while (exchange != NULL && !exchange->isEqual(baseCurrency, quoteCurrency))
	{
		((LList<OrderInfoTree>*)orderData)->moveNextNode();
		exchange = ((LList<OrderInfoTree>*)orderData)->getCurrentNode();
	}
	return exchange;
}

int ProcessData::insert(const string* sp, const int n)
{
	if (n != 6) return -1;

	string baseCurrency = sp[1].c_str();
	string quoteCurrency = sp[2].c_str();
	int time = atoi(sp[3].c_str());
	float bidPrice = stof(sp[4]);
	float askPrice = stof(sp[5]);
	BidAndAsk mData = BidAndAsk(bidPrice, askPrice);
	BaseData<BidAndAsk> mDataToInsert = BaseData<BidAndAsk>(time, mData);

	CurrencyPairInfoTree* treePos = findTreeOfPair(baseCurrency, quoteCurrency);
	if (treePos == NULL) {
		CurrencyPairInfoTree* newTree = new CurrencyPairInfoTree(baseCurrency, quoteCurrency);
		((LList<CurrencyPairInfoTree>*)data)->insertAtEnd(*newTree);
	}
	treePos = findTreeOfPair(baseCurrency, quoteCurrency);
	int result = treePos->insert(mDataToInsert, false);

	cout << "After add : -- " << baseCurrency << " -- and -- " << quoteCurrency << " -- :" << endl;
	treePos->printTreeStructure();
	treePos = nullptr;
	cout << endl << endl;

	return result;
}

int ProcessData::remove(const string* sp, const int n)
{
	if (n != 4 && n!=5) {
		return -1;
	}

	string baseCurrency = sp[1].c_str();
	string quoteCurrency = sp[2].c_str();
	int startTime = atoi(sp[3].c_str());
	int endTime;
	if (n == 4) {
		endTime = startTime;
	}
	else {
		endTime = atoi(sp[4].c_str());
	}

	CurrencyPairInfoTree* treePos = findTreeOfPair(baseCurrency, quoteCurrency);
	if (treePos == NULL) {
		treePos = nullptr;
		return -1;
	}
	else {
		int result = treePos->removeByRange(startTime, endTime);
		
		cout << "After remove : -- " << baseCurrency << " -- and -- " << quoteCurrency << " -- :" << endl;
		treePos->printTreeStructure();
		treePos = nullptr;
		cout << endl << endl;
		return result;
	}
}

int ProcessData::update(const string* sp, const int n)
{
	if (n != 6) {
		return -1;
	}

	string baseCurrency = sp[1].c_str();
	string quoteCurrency = sp[2].c_str();
	int time = atoi(sp[3].c_str());
	float bidPrice = stof(sp[4]);
	float askPrice = stof(sp[5]);
	BidAndAsk mData = BidAndAsk(bidPrice, askPrice);
	BaseData<BidAndAsk> mDataToUpdate = BaseData<BidAndAsk>(time, mData);

	CurrencyPairInfoTree* treePos = findTreeOfPair(baseCurrency, quoteCurrency);
	if (treePos == NULL) {
		treePos = nullptr;
		return -1;
	}
	else {
		int result = treePos->updateNode(mDataToUpdate);
		
		cout << "After update : -- " << baseCurrency << " -- and -- " << quoteCurrency << " -- :" << endl;
		treePos->printTreeStructure();
		treePos = nullptr;
		cout << endl << endl;
		return result;
	}
}

int ProcessData::createOrAdjustMarginAccount(const string* sp, const int n) {
	if (n != 2) {
		return -1;
	}

	int newBalance = stof(sp[1]);
	has_opened_margin_account = true;
	accountBalance = newBalance;
	return 1;

}

int ProcessData::checkMarginAccount(const string* sp, const int n) {
	if (n != 1) {
		return -1;
	}
	if (has_opened_margin_account) {
		return Util::getIntPart(accountBalance);
	}
	else {
		return -1;
	}

	
}

int ProcessData::setMarginPercent(const string* sp, const int n) {
	if (n != 2) {
		return -1;
	}

	if (has_opened_margin_account) {
		return Util::getIntPart(accountBalance * marginPercent);
	}
	else {
		return -1;
	}
}

int ProcessData::checkIfClosestPairExist(string baseCurr, string quoteCurr, int time, double& u, bool isSelling) {
	CurrencyPairInfoTree* currencyTreePos = findTreeOfPair(baseCurr, quoteCurr);
	BidAndAsk closestPairData;
	if (currencyTreePos == NULL) {
		return -1;
	}
	else {
		if (currencyTreePos->findClosestNode(time) == NULL) {
			return -1;
		}
		else {
			closestPairData = currencyTreePos->findClosestNode(time)->data.baseData;
		}
	}

	if (isSelling) {
		u = closestPairData.askPrice;
	}
	else {
		u = closestPairData.bidPrice;
	}
	
	
	return 1;
}

template <typename T>
static bool stringCpr(T string1, T string2) {
	return static_cast<string>(string1) == static_cast<string>(string2);
}

int ProcessData::addNewOrder(const string* sp, const int n) {
	if (n != 6) {
		return -1;
	}

	string baseCurrency = sp[1].c_str();
	string quoteCurrency = sp[2].c_str();
	int time = atoi(sp[3].c_str());
	int lotAmount = atoi(sp[4].c_str());
	string orderId = sp[5].c_str();

	bool isSell;
	if (sp[0].c_str() == "OB") {
		isSell = false;
	}
	else {
		isSell = true;
	}
	// Check for validations
	if (baseCurrency != "USD" && quoteCurrency != "USD") {
		return -1;
	}

	if (((LList<string>*)orderIdList)->checkForDuplicate(orderId, stringCpr)) {
		return -1;
	}

	if (lotAmount * AMOUNT_EACH_LOT > accountBalance * marginPercent) {
		return -1;
	}

	// Check if currency pair info of this pair exist
	double u1;
	if (checkIfClosestPairExist(baseCurrency, quoteCurrency, time, u1, isSell) == -1) {
		return -1;
	}
	
	Order mData = Order(orderId, lotAmount, isSell, u1);
	BaseData<Order> mDataToInsert = BaseData<Order>(time, mData);

	OrderInfoTree* treePos = findOrderTree(baseCurrency, quoteCurrency);
	if (treePos == NULL) {
		OrderInfoTree* newTree = new OrderInfoTree(baseCurrency, quoteCurrency);
		((LList<OrderInfoTree>*)orderData)->insertAtEnd(*newTree);
	}
	treePos = findOrderTree(baseCurrency, quoteCurrency);

	treePos->insert(mDataToInsert, true);
	((LList<string>*)orderIdList)->insertAtEnd(orderId);

	cout << "After add open order : -- " << baseCurrency << " -- and -- " << quoteCurrency << " -- :" << endl;
	treePos->printTreeStructure();
	treePos = nullptr;
	cout << endl << endl;

	return Util::getIntPart( u1*lotAmount*AMOUNT_EACH_LOT);
}

int forwarder(void* context, int a, string b, bool c) {
	return static_cast<ProcessData*>(context)->closeSpecificOrder(a, b, c);
	
};

int ProcessData::closeOrder(const string* sp, const int n) {
	if (n != 3) {
		return -1;
	}

	int time = atoi(sp[1].c_str());
	string orderId = sp[2].c_str();
	string command = sp[0].c_str();
	bool isSell;
	if (command == "CS") {
		isSell = true;
	}
	else {
		isSell = false;
	}
	int result = closeSpecificOrder(time, orderId, isSell);
	// After closing 1 order, check for account neagtive, if is negative than mass closing 
	if (accountBalance < 0) {
		cout << "het tien";
		Link<OrderInfoTree>* tempOrderTree = ((LList<OrderInfoTree>*)orderData)->head;
		while (tempOrderTree != NULL)
		{
			//((LList<string>*)orderIdList)->printAll();
			tempOrderTree->data.massRemove(&forwarder, this);
			tempOrderTree = tempOrderTree->next;
		}
	}
	return result;
}



int ProcessData::closeSpecificOrder(int time, string orderId, bool isSell) {
	// If there isn't any order that has this id 
	//((LList<string>*)orderIdList)->printAll(); 
	//cout << "dasd : " << orderId<<endl;
	if (!((LList<string>*)orderIdList)->checkForDuplicate(orderId, stringCpr)) {
		return -1;
	}
	// Find the existing order with this id and time
	Node<Order>* closingOrder = NULL;
	Link<OrderInfoTree>* tempOrderTree = ((LList<OrderInfoTree>*)orderData)->head;
	while (tempOrderTree != NULL )
	{
		closingOrder = tempOrderTree->data.searchByIdAndTime(time, orderId, isSell);
		//tempOrderTree->data.printTreeStructure();
		if (closingOrder != NULL) {
			break;
		}
		else {
			tempOrderTree = tempOrderTree->next;
		}
		
	}

	if (closingOrder == NULL) {
		return -1;
	}
	else {
		// Check if currency pair info of this pair exist
		double  u2;
		if (checkIfClosestPairExist(tempOrderTree->data.baseCurrency, tempOrderTree->data.quoteCurrency, time, u2, !isSell) == -1) {
			return -1;
		}
		
		// Get profit base on each cases
		double  u1 = closingOrder->data.baseData.firstUsdPrice;
		double  profit;
		if (closingOrder->data.baseData.isSell) {
			
			profit = (u2 - u1) * closingOrder->data.baseData.lotAmount * AMOUNT_EACH_LOT;
		}
		else {
			profit = (u1 - u2) * closingOrder->data.baseData.lotAmount * AMOUNT_EACH_LOT;
		}
		// Update account balance and order
		closingOrder->data.baseData.isClosed = true;
		accountBalance += profit;
		BaseData<Order> dataToDel= BaseData<Order>(closingOrder->data.time, closingOrder->data.baseData);
		tempOrderTree->data.removeOrder(dataToDel);

		cout << "After remove order : -- " << tempOrderTree->data.baseCurrency << " -- and -- " << tempOrderTree->data.quoteCurrency << " -- :" << endl;
		tempOrderTree->data.printTreeStructure();
		cout << endl << endl;

		return Util::getIntPart(profit);
	}
};


