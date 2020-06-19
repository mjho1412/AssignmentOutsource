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

class BidAndAsk {
public:
    int time;
    float bidPrice;
    float askPrice;
    BidAndAsk(int mTime, float mBidPrice, float mAskPrice) {
        time = mTime;
        bidPrice = mBidPrice;
        askPrice = mAskPrice;
    }
    BidAndAsk() {
    }
};

class Node
{
public:
    BidAndAsk data;
    Node* left;
    Node* right;

    Node(const BidAndAsk& ele)
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


class CurrencyPairInfoTree
{
private:
    Node* root;

public:
    CurrencyPairInfoTree()
    {
        this->root = nullptr;
    }
    ~CurrencyPairInfoTree() {
        while (root != NULL) {
            remove(root->data);
        }
    }


    void insert(const BidAndAsk& ele) {
        //cout<<"Adding" << ele.time<< endl;
        root = insertRec(root, ele);
        //cout << root->data.time;
    }

    Node* insertRec(Node*& node, const BidAndAsk& data)
    {
        /* 1. Perform the normal BST insertion */
        if (node == NULL) {
            return new Node(data);
        }


        if (data.time < node->data.time) {
            node->left = insertRec(node->left, data);
        }

        else if (data.time > node->data.time) {
            node->right = insertRec(node->right, data);
        }
        else // Equal keys will update ask price and bid price 
        {
            node->data = data;
            return node;
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

    Node* getLeftMost(Node*& node)
    {

        Node* current = node;

        /* loop down to find the leftmost leaf */
        while (current->right != NULL)
            current = current->right;

        return current;
    }

    int getBalance(Node* N)
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

    int getHeightRec(Node* node)
    {
        if (node == NULL)
            return 0;
        int lh = this->getHeightRec(node->left);
        int rh = this->getHeightRec(node->right);
        return (lh > rh ? lh : rh) + 1;
    }

    Node* rightRotate(Node*& y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    Node* leftRotate(Node*& x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        return y;
    }

    Node* removeRec(Node*& node, const BidAndAsk& value)
    {
        // TODO
        if (node == NULL)
            return node;

        if (value.time < node->data.time)
            node->left = removeRec(node->left, value);
        else if (value.time > node->data.time)
            node->right = removeRec(node->right, value);

        // if key is same as root's key, then  
        // This is the node to be deleted  
        else
        {
            // node with only one child or no child  
            if ((node->left == NULL) ||
                (node->right == NULL))
            {
                Node* temp = node->left ?
                    node->left :
                    node->right;

                // No child case  
                if (temp == NULL)
                {
                    temp = node;
                    node = NULL;
                }
                else // One child case  
                    *node = *temp; // Copy the contents of  
                                   // the non-empty child  
                free(temp);
            }
            else
            {
                // node with two children: Get the inorder  
                // successor (smallest in the right subtree)  
                Node* temp = getLeftMost(node->left);

                // Copy the inorder successor's  
                // data to this node  
                node->data = temp->data;

                // Delete the inorder successor  
                node->left = removeRec(node->left, temp->data);
            }
        }

        // If the tree had only one node 
        // then return  
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
        if (balance > 1 &&
            leftBalance >= 0)
            return rightRotate(node);

        // Left Right Case  
        if (balance > 1 &&
            leftBalance < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case  
        if (balance < -1 &&
            rightBalance <= 0)
            return leftRotate(node);

        // Right Left Case  
        if (balance < -1 &&
            rightBalance > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void remove(const BidAndAsk& value)
    {
        root = removeRec(root, value);
    }

    void printNSpace(int n)
    {
        for (int i = 0; i < n - 1; i++)
            cout << " ";
    }

    void printTreeStructure()
    {
        int height = this->getHeightRec(root);

        if (this->root == NULL)
        {
            cout << "NULL\n";
            return;
        }
        queue<Node*> q;
        q.push(root);
        Node* temp;
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

                cout << temp->data.time;
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