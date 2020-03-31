// Linklist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "LinkList.h"

// RemoveDuplicatesFromUnsortedListUsingLoop
int main()
{
    CLinklist obj;
    CLinklist* root = nullptr;
    obj.Insert(root, 1);
    obj.Insert(root, 2);
    obj.Insert(root, 2);
    obj.Insert(root, 2);
    obj.Insert(root, 4);
    obj.Insert(root, 4);
    obj.Insert(root, 5);
    obj.Insert(root, 6);
    obj.Insert(root, 6);
    obj.Insert(root, 6);

    // obj.RemoveDuplicatesFromSortedList(root);

    CLinklist* root2 = nullptr;
    obj.Insert(root2, 6);
    obj.Insert(root2, 7);
    obj.Insert(root2, 8);
    obj.Insert(root2, 9);

    root2 = obj.ReverseRecursively(root2);
    
    // obj.AddTwoSortedLinkList(root, root2);
}

