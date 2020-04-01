#pragma once

class CLinklist
{
private:
    int m_data{ 0 };
    CLinklist* m_nextNode;

private:
    void        InsertAtEnd(CLinklist* root, int data);
    void        InsertMid(CLinklist* root, int data);
    void        InsertAtStart(CLinklist*& root, int data);
    CLinklist* createNewNode(int data);
    int         CountNodes(CLinklist* root);
    void        AddEmptyNodesInFront(CLinklist*& root);
    bool        IsNull(CLinklist* root);

public:
    void        Insert(CLinklist*& root, int data);
    void        Display(CLinklist* root);
    CLinklist* AddTwoSortedLinkList(CLinklist* one, CLinklist* two);
    CLinklist* MergeTwoSortedLinkList(CLinklist* one, CLinklist* two);
    CLinklist* ReverseIteratively(CLinklist* root);
    CLinklist* ReverseRecursively(CLinklist* root);
    void        SortLinkList(CLinklist* root);
    CLinklist* FindMidNode(CLinklist* root);
    bool        IsLinklistContainCycle(CLinklist* root);
    void        DeleteNode(CLinklist*& root, int data);
    void        DeleteAllNodes(CLinklist* root);
    void        RemoveDuplicatesFromUnsortedListUsingLoop(CLinklist* root);
    void        RemoveDuplicatesFromSortedList(CLinklist* root);
    CLinklist* SwapNodesInGroup(CLinklist* start, int k);
    bool        SwapNodes(CLinklist* root, CLinklist** start);
    bool        DetectLoop(CLinklist* root);
    void        RemoveDuplicatesFromUnsortedListUsingHashing(CLinklist* root);
    bool        isPallindrome(CLinklist* root);
    void        DeleteNodeAtPosition(CLinklist** root, int position);
    CLinklist* AddTwoListsRecursively(CLinklist* root1, CLinklist* root2);
};

bool CLinklist::IsNull(CLinklist* root)
{
    return (root == nullptr);
}

CLinklist* CLinklist::createNewNode(int data)
{
    CLinklist* node = new CLinklist();
    node->m_data = data;
    return node;
}

inline int CLinklist::CountNodes(CLinklist* root)
{
    if (!IsNull(root))
    {
        int count = 0;
        while (!IsNull(root))
        {
            count++;
            root = root->m_nextNode;
        }
        return count;
    }
    return 0;
}

inline void CLinklist::AddEmptyNodesInFront(CLinklist*& root)
{
    InsertAtStart(root, 0);
}

void CLinklist::InsertAtEnd(CLinklist* root, int data)
{
    if (root != nullptr)
    {
        while (root && !IsNull(root->m_nextNode))
        {
            root = root->m_nextNode;
        }
        auto tempNode = createNewNode(data);
        if (!IsNull(tempNode))
        {
            root->m_nextNode = tempNode;
        }
    }
}

void CLinklist::InsertMid(CLinklist* root, int data)
{

}

void CLinklist::InsertAtStart(CLinklist*& root, int data)
{
    if (!IsNull(root))
    {
        auto newNode = createNewNode(data);
        if (!IsNull(newNode))
        {
            newNode->m_nextNode = root;
            root = newNode;
        }
    }
}

void CLinklist::Insert(CLinklist*& root, int data)
{
    if (root == nullptr)
    {
        root = createNewNode(data);
    }
    else
    {
        InsertAtEnd(root, data);;
    }
}

void CLinklist::Display(CLinklist* root)
{
    if (!IsNull(root))
    {
        while (!IsNull(root))
        {
            std::cout << root->m_data << " ";;
            root = root->m_nextNode;
        }
    }
}

// addition/sum of two linklist nodes
CLinklist* CLinklist::AddTwoSortedLinkList(CLinklist* one, CLinklist* two)
{
    if (IsNull(one) && IsNull(two))
        return nullptr;
    if (IsNull(one))
        return two;
    else if (IsNull(two))
        return one;

    int oneCount = CountNodes(one);
    int twoCount = CountNodes(two);

    if (oneCount != twoCount)
    {
        if (oneCount < twoCount)
        {
            auto remainingNodes = twoCount - oneCount;
            for (int i = 0; i < remainingNodes; i++)
            {
                AddEmptyNodesInFront(one);
            }
        }
        else
        {
            auto remainingNodes = oneCount - twoCount;
            for (int i = 0; i < remainingNodes; i++)
            {
                AddEmptyNodesInFront(two);
            }
        }
    }

    CLinklist* p = ReverseIteratively(one);
    CLinklist* q = ReverseIteratively(two);
    CLinklist* newLinkList = nullptr;
    CLinklist* node = nullptr;
    int carry = 0, remainder = 0, addition = 0, DivBy10 = 0;
    while (p && q)
    {
        addition = p->m_data + q->m_data + carry;
        DivBy10 = addition / 10;
        if (DivBy10 != 0)
        {
            remainder = addition % 10;
            carry = DivBy10;
            auto lNode = createNewNode(remainder);
            if (IsNull(node))
            {
                node = lNode;
                newLinkList = node;
            }
            else
            {
                InsertAtEnd(node, remainder);
            }
        }
        else
        {
            auto lNode = createNewNode(addition);
            if (IsNull(node))
            {
                node = lNode;
                newLinkList = node;
            }
            else
            {
                InsertAtEnd(node, addition);
            }
        }

        p = p->m_nextNode;
        q = q->m_nextNode;
    }
    if (carry != 0)
    {
        InsertAtEnd(node, carry);
    }

    newLinkList = ReverseIteratively(newLinkList);
    return newLinkList;
}

inline CLinklist* CLinklist::MergeTwoSortedLinkList(CLinklist* one, CLinklist* two)
{
    if (IsNull(one) && IsNull(two))
        return nullptr;
    if (IsNull(one))
        return two;
    else if (IsNull(two))
        return one;

    CLinklist* p = one;
    CLinklist* q = two;
    CLinklist* start = nullptr;
    CLinklist* S = nullptr;
    if (p->m_data < q->m_data)
    {
        S = p;
        p = p->m_nextNode;
    }
    else
    {
        S = q;
        q = q->m_nextNode;
    }

    start = S;
    while (p && q)
    {
        if (p->m_data < q->m_data)
        {
            S->m_nextNode = p;
            S = p;
            p = p->m_nextNode;
        }
        else
        {
            S->m_nextNode = q;
            S = q;
            q = q->m_nextNode;
        }
    }

    if (p)
    {
        S->m_nextNode = p;
    }
    else if (q)
    {
        S->m_nextNode = q;
    }
    return start;
}

CLinklist* CLinklist::ReverseIteratively(CLinklist* root)
{
    CLinklist* current = root;
    CLinklist* prev = nullptr, * next = nullptr;
    while (!IsNull(current))
    {
        next = current->m_nextNode;
        current->m_nextNode = prev;
        prev = current;
        current = next;
    }
    return prev;
}

CLinklist* CLinklist::ReverseRecursively(CLinklist* root)
{
    if (IsNull(root))
        return root;
    CLinklist* p = root;
    CLinklist* q = p->m_nextNode;
    if (IsNull(q))
        return p;
    q = ReverseRecursively(q);
    p->m_nextNode->m_nextNode = p;
    p->m_nextNode = nullptr;
    return q;
}

void CLinklist::SortLinkList(CLinklist* root)
{
    if (IsNull(root))
        return;

    CLinklist* p = root, * q = nullptr;
    while (!IsNull(p) && !IsNull(p->m_nextNode))
    {
        q = p->m_nextNode;
        CLinklist* minNode = q;
        while (q != nullptr)
        {
            if (p->m_data > q->m_data)
            {
                if (minNode->m_data > q->m_data)
                    minNode = q;
            }
            q = q->m_nextNode;
        }

        //  swap the data of the nodes
        if (minNode && p->m_data > minNode->m_data)
        {
            auto temp = minNode->m_data;
            minNode->m_data = p->m_data;
            p->m_data = temp;
        }
        p = p->m_nextNode;
    }
}

CLinklist* CLinklist::FindMidNode(CLinklist* root)
{
    return nullptr;
}

bool CLinklist::IsLinklistContainCycle(CLinklist* root)
{
    return false;
}

// there could be three cases
// start, mid and end
void CLinklist::DeleteNode(CLinklist*& root, int data)
{
    if (!IsNull(root))
    {
        // start node
        if (root->m_data == data)
        {
            auto nextNode = root->m_nextNode;
            delete root;
            root = nextNode;
            return;
        }

        auto tempNode = root;
        while (tempNode && tempNode->m_nextNode->m_data != data)
        {
            tempNode = tempNode->m_nextNode;
        }
        // end, mid node
        if (!IsNull(tempNode) && !IsNull(tempNode->m_nextNode))
        {
            auto nextNode = tempNode->m_nextNode->m_nextNode;
            delete tempNode->m_nextNode;
            tempNode->m_nextNode = nextNode;
            return;
        }

        std::cout << data << " does not exist in the list for deletion\n";
    }
}

void CLinklist::DeleteAllNodes(CLinklist* root)
{
    if (!IsNull(root))
    {
        while (!IsNull(root))
        {
            auto tempNode = root->m_nextNode;
            delete root;
            root = tempNode;
        }
    }
}

inline void CLinklist::RemoveDuplicatesFromSortedList(CLinklist* root)
{
    if (!IsNull(root) && !IsNull(root->m_nextNode))
    {
        CLinklist* p = root;
        //CLinklist* new_start = p;
        CLinklist* q = nullptr;
        while (p && p->m_nextNode != nullptr)
        {
            if (p->m_data == p->m_nextNode->m_data)
            {
                q = p->m_nextNode->m_nextNode;
                delete p->m_nextNode;
                if (IsNull(q))
                {
                    p->m_nextNode = nullptr;
                    break;
                }
                p->m_nextNode = q;
            }

            if (p->m_data != p->m_nextNode->m_data)
            {
                p = p->m_nextNode;
            }
        }

        //return new_start;
    }
}

inline CLinklist* CLinklist::SwapNodesInGroup(CLinklist* start, int k)
{
    return NULL;
}

inline bool CLinklist::SwapNodes(CLinklist* root, CLinklist** start)
{
    return false;
}

inline bool CLinklist::DetectLoop(CLinklist* root)
{
    return false;
}

inline void CLinklist::RemoveDuplicatesFromUnsortedListUsingLoop(CLinklist* root)
{
    if (IsNull(root))
        return;

    CLinklist* p = root, * q = nullptr;
    while (!IsNull(p))
    {
        q = p->m_nextNode;
        while (!IsNull(q))
        {
            if (p->m_data == q->m_data)
            {
                auto nextNode = q->m_nextNode;
                delete q;
                q = nextNode;
                p->m_nextNode = q;
            }
            else
            {
                if (IsNull(q))
                    break;
                q = q->m_nextNode;
            }
        }
        p = p->m_nextNode;
    }
}

inline bool CLinklist::isPallindrome(CLinklist* root)
{
    return false;
}

inline void CLinklist::DeleteNodeAtPosition(CLinklist** root, int position)
{
}
