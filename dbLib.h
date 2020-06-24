#include "main.h"
#include <queue>

//template <typename T>
//class Link
//{
//public:
//    T data;
//    Link<T> *next;
//    Link<T>(const Link<T> &n = nullptr) { next = n; }
//    Link<T>(const T &ele, const Link<T> &n = nullptr)
//    {
//        data = ele;
//        next = n;
//    }
//};
//template <typename T>
//class List
//{
//    // Return: The number of elements in the list.
//    virtual int length() const = 0;
//    virtual void insert(const T &ele) = 0;
//};
//template <typename T>
//class LList : public List<T>
//{
//private:
//    Link<T> *head;
//    Link<T> *last;
//    Link<T> *curr;
//    int cnt;
//
//public:
//    LList() { head = last = curr = new Link<T>(); }
//    int length() const { return cnt; }
//    void insert(const T &ele)
//    {
//        curr->next = new Link<T>(ele, curr->next);
//    }
//};

//void printInteger(int& n)
//{
//    cout << n << " ";
//}

class Order {
public:
    string id;
    int lotAmount;
    bool isClosed;
    bool isSell;
    double firstUsdPrice;
    Order(string mId, int mLotAmount, bool mIsSell, float mFirstUsdPrice) {
        id = mId;
        lotAmount = mLotAmount;
        isClosed = false;
        isSell = mIsSell;
        firstUsdPrice = mFirstUsdPrice;
    }
    Order(){}
};

class BidAndAsk {
public:
    double  bidPrice;
    double  askPrice;
    BidAndAsk(double  mBidPrice, double  mAskPrice) {
        bidPrice = mBidPrice;
        askPrice = mAskPrice;
    }
    BidAndAsk() {}
};

template <typename T>
class BaseData {
public:
    int time;
    T baseData;
    BaseData() {

    }
    BaseData(int mTime, T mData) {
        time = mTime;
        baseData = mData;
    }
};

template <typename T>
class Node
{
public:
    BaseData<T> data;
    Node* left;
    Node* right;

    Node(const BaseData<T>& ele)
    {
        data = ele;
        this->left = nullptr;
        this->right = nullptr;
    }

    ~Node() {
        delete left;
        delete right;
    }
};



template <typename T>
class AVLTree
{
private:
    
    

public:
    string baseCurrency;
    string quoteCurrency;
    Node<T>* root;
    AVLTree(string baseCurrency, string quoteCurrency)
    {
        this->baseCurrency = baseCurrency;
        this->quoteCurrency = quoteCurrency;
        this->root = nullptr;
    };
    ~AVLTree() {
        while (root != NULL) {
            remove(root->data);
        }
    };
    AVLTree() {

    };

    int showResult() {
        if (root == NULL) {
            return -1;
        }
        else {
            return root->data.time;
        }
    }

    int insert(const BaseData<T>& ele, bool isAllowDulicated) {
        root = insertRec(root, ele, isAllowDulicated);
        return showResult();
    }



    Node<T>* insertRec(Node<T>*& node, const  BaseData<T>& data, bool isAllowDulicated)
    {
        /* 1. Perform the normal BST insertion */
        if (node == NULL) {
            return new Node<T>(data);
        }


        if (data.time < node->data.time) {
            node->left = insertRec(node->left, data, isAllowDulicated);
        }

        else if (data.time > node->data.time) {
            node->right = insertRec(node->right, data, isAllowDulicated);
        }
        else // Equal keys will update ask price and bid price 
        {
            if (isAllowDulicated) {
                node->right = insertRec(node->right, data, isAllowDulicated);
            }
            else {
                node->data = data;
                return node;
            }
            
        }

        /* 3. Get the balance factor of this ancestor
            node to check whether this node became
            unbalanced */
            //int balance = getBalance(node);
        int balance;
        if (node == NULL)
            balance = 0;
        else {
            //cout << " : " << getHeightRec(node->left);// << " " << getHeightRec(node->right) << endl;
            balance = getHeightRec(node->left) - getHeightRec(node->right);
        }


        // If this node becomes unbalanced, then  
        // there are 4 cases  

        // Left Left Case  
        if (balance > 1 && data.time < node->left->data.time)
            return rightRotate(node);

        // Right Right Case  
        if (balance < -1 && data.time > node->right->data.time)
            return leftRotate(node);

        // Left Right Case  
        if (balance > 1 && data.time > node->left->data.time)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case  
        if (balance < -1 && data.time < node->right->data.time)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        /* return the (unchanged) node pointer */
        return node;
    }

    Node<T>* getLeftMost(Node<T>*& node)
    {

        Node<T>* current = node;

        /* loop down to find the leftmost leaf */
        while (current->right != NULL)
            current = current->right;

        return current;
    }

    int getBalance(Node<T>* N)
    {
        cout << "<<" << N->data.time;
        if (N == NULL)
            return 0;
        return getHeightRec(N->left) - getHeightRec(N->right);
    }

    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int getHeightRec(Node<T>* node)
    {
        if (node == NULL)
            return 0;
        int lh = this->getHeightRec(node->left);
        int rh = this->getHeightRec(node->right);
        return (lh > rh ? lh : rh) + 1;
    }

    Node<T>* rightRotate(Node<T>*& y)
    {
        Node<T>* x = y->left;
        Node<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    Node<T>* leftRotate(Node<T>*& x)
    {
        Node<T>* y = x->right;
        Node<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        return y;
    }

    Node<T>* removeRec(Node<T>*& node, const BaseData<T>& value)
    {
        if (node == NULL)
            return node;

        if (value.time < node->data.time)
            node->left = removeRec(node->left, value);
        else if (value.time > node->data.time)
            node->right = removeRec(node->right, value);
        else
        {
            if ((node->left == NULL) || (node->right == NULL))
            {
                Node<T>* temp = node->left ?
                    node->left :
                    node->right;

                if (temp == NULL)
                {
                    temp = node;
                    node = NULL;
                }
                else   
                    *node = *temp;
                free(temp);
            }
            else
            {
                Node<T>* temp = getLeftMost(node->left);
                node->data = temp->data;
                node->left = removeRec(node->left, temp->data);
            }
        }
        if (node == NULL)
            return node;

        int balance;
        if (node == NULL)
            balance = 0;
        else {
            balance = getHeightRec(node->left) - getHeightRec(node->right);
        }
        int leftBalance;
        if (node->left == NULL)
            leftBalance = 0;
        else {
            leftBalance = getHeightRec(node->left->left) - getHeightRec(node->left->right);
        }
        int rightBalance;
        if (node->right == NULL)
            rightBalance = 0;
        else {
            rightBalance = getHeightRec(node->right->left) - getHeightRec(node->right->right);
        }


        // Left Left Case  
        if (balance > 1 && leftBalance >= 0)
            return rightRotate(node);

        // Left Right Case  
        if (balance > 1 && leftBalance < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case  
        if (balance < -1 &&  rightBalance <= 0)
            return leftRotate(node);

        // Right Left Case  
        if (balance < -1 && rightBalance > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    int remove(const BaseData<T>& value)
    {
        root = removeRec(root, value);
        return showResult();
    }

    int removeByRange(int startTime, int endTime)
    {
        while (root != NULL) {
            if (root->data.time >= startTime && root->data.time <= endTime) {
                root = removeRec(root, root->data);
            }
            else {
                break;
            }
        }
        return showResult();
    }

    Node<T>* search(Node<T>* node, int time)
    {
        if (node == NULL)
            return NULL;

        if(node->data.time == time)
            return node;

        if (node->data.time < time)
            return search(node->right, time);

        return search(node->left, time);
    }

    int updateNode(BaseData<T> value) {
        Node<T>* nodeToUpdate = search(root, value.time);
        if (nodeToUpdate == NULL) {
            nodeToUpdate = nullptr;
            return 0;
        }
        else {
            nodeToUpdate->data = value;
            nodeToUpdate = nullptr;
            return showResult();
        }
       
    }

    void findClosestRec( Node<T>* ptr, int k, int& min_diff, Node<T>*& returnNode)
    {
        if (ptr == NULL)
            return;
        if (ptr->data.time == k)
        {
           // min_diff_key = k;
            returnNode = ptr;
            return;
        }

        if (min_diff > abs(ptr->data.time - k))
        {
            min_diff = abs(ptr->data.time - k);
            //min_diff_key = ptr->data.time;
            returnNode = ptr;
        }

        if (k < ptr->data.time)
            findClosestRec(ptr->left, k, min_diff, returnNode);
        else
            findClosestRec(ptr->right, k, min_diff, returnNode);
    }

    Node<T>* findClosestNode(int timeToFindClosest )
    {
        Node<T>* returnNode = NULL;
        int min_diff = INT_MAX;
        findClosestRec(root, timeToFindClosest, min_diff, returnNode);
        if (returnNode == NULL) {
            return NULL;
        }
        else {
            return returnNode;
        }
        
    }


    void printNSpace(int n)
    {
        for (int i = 0; i < n - 1; i++)
            cout << " ";
    }


    

    bool isEqual(string baseCurrency, string quoteCurrency)
    {
        return this->baseCurrency == baseCurrency && this->quoteCurrency == quoteCurrency;
    }

    
};

class CurrencyPairInfoTree : public  AVLTree<BidAndAsk> {
public:
    CurrencyPairInfoTree(string baseCurrency, string quoteCurrency):AVLTree<BidAndAsk>(baseCurrency, quoteCurrency){
    };
    CurrencyPairInfoTree() {};
    void printTreeStructure()
    {
        int height = this->getHeightRec(root);

        if (this->root == NULL)
        {
            cout << "NULL\n";
            return;
        }
        queue<Node<BidAndAsk>*> q;
        q.push(root);
        Node<BidAndAsk>* temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = pow(2, height);
        printNSpace(space / 2);
        while (!q.empty())
        {
            temp = q.front();
            q.pop();

            if (temp == NULL)
            {
                cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else
            {

                cout << temp->data.time <<"("<<temp->data.baseData.askPrice<<","<<temp->data.baseData.bidPrice<<")";
                //cout << temp->data.time;
                q.push(temp->left);
                q.push(temp->right);
            }
            printNSpace(space);
            count++;
            if (count == maxNode)
            {
                cout << endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height)
                return;
        }
    }
};

class OrderInfoTree : public  AVLTree<Order> {
public:
    OrderInfoTree(string baseCurrency, string quoteCurrency) :AVLTree<Order>(baseCurrency, quoteCurrency) {
    };
    OrderInfoTree() {};
    Node<Order>* searchByIdAndTimeRec(Node<Order>* node, int time, string orderId, bool isSell)
    {
        if (node == NULL)
            return NULL;

        if (
            //node->data.time == time && 
            node->data.baseData.id == orderId && 
            node->data.baseData.isSell == isSell &&
            node->data.baseData.isClosed == false
            )
            return node;

        if (node->data.time < time)
            return searchByIdAndTimeRec(node->right, time, orderId, isSell);

        return searchByIdAndTimeRec(node->left, time, orderId, isSell);
    }

    Node<Order>* searchByIdAndTime(int time, string orderId, bool isSell) {
        return searchByIdAndTimeRec(root, time, orderId, isSell);
    }

    void massRemove(int (*closeOrder)(void *,int, string, bool), void* classContext) {
        Node<Order>* returnNode = root;
        while (root != NULL) {
            BaseData<Order> rootData = root->data;
            int time = rootData.time;
            string orderId = rootData.baseData.id;
            bool isSell = rootData.baseData.isSell;
            int returnVal = closeOrder(classContext, time, orderId, isSell);
            root = removeRec(root, rootData);
        }
    }

    void printTreeStructure()
    {
        int height = this->getHeightRec(root);

        if (this->root == NULL)
        {
            cout << "NULL\n";
            return;
        }
        queue<Node<Order>*> q;
        q.push(root);
        Node<Order>* temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = pow(2, height);
        printNSpace(space / 2);
        while (!q.empty())
        {
            temp = q.front();
            q.pop();

            if (temp == NULL)
            {
                cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else
            {

                cout << temp->data.time <<"("<<temp->data.baseData.id<<","<<temp->data.baseData.lotAmount<<","<<temp->data.baseData.firstUsdPrice<<")";
                //cout << temp->data.time;
                //showData<T>(temp->data.baseData);
                q.push(temp->left);
                q.push(temp->right);
            }
            printNSpace(space);
            count++;
            if (count == maxNode)
            {
                cout << endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height)
                return;
        }
    }

    Node<Order>* removeOrderRec(Node<Order>*& node, const BaseData<Order>& value)
    {
        if (node == NULL)
            return node;

        if (value.time < node->data.time)
            node->left = removeOrderRec(node->left, value);
        else if (value.time > node->data.time)
            node->right = removeOrderRec(node->right, value);
        else if(value.baseData.id != value.baseData.id)
            node->right = removeOrderRec(node->right, value);
        else
        {
            if ((node->left == NULL) || (node->right == NULL))
            {
                Node<Order>* temp = node->left ?
                    node->left :
                    node->right;

                if (temp == NULL)
                {
                    temp = node;
                    node = NULL;
                }
                else
                    *node = *temp;
                free(temp);
            }
            else
            {
                Node<Order>* temp = getLeftMost(node->left);
                node->data = temp->data;
                node->left = removeRec(node->left, temp->data);
            }
        }
        if (node == NULL)
            return node;

        int balance;
        if (node == NULL)
            balance = 0;
        else {
            balance = getHeightRec(node->left) - getHeightRec(node->right);
        }
        int leftBalance;
        if (node->left == NULL)
            leftBalance = 0;
        else {
            leftBalance = getHeightRec(node->left->left) - getHeightRec(node->left->right);
        }
        int rightBalance;
        if (node->right == NULL)
            rightBalance = 0;
        else {
            rightBalance = getHeightRec(node->right->left) - getHeightRec(node->right->right);
        }


        // Left Left Case  
        if (balance > 1 && leftBalance >= 0)
            return rightRotate(node);

        // Left Right Case  
        if (balance > 1 && leftBalance < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case  
        if (balance < -1 && rightBalance <= 0)
            return leftRotate(node);

        // Right Left Case  
        if (balance < -1 && rightBalance > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    int removeOrder(const BaseData<Order>& value)
    {
        root = removeOrderRec(root, value);
        return showResult();
    }
};