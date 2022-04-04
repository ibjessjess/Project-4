#include "RainList.h"
bool RainList::hasValue(IDLookup t) const
{
    RainListNode* nodeptr = head;
    while (nodeptr)
    {
        if (t.str == nodeptr->value.str) { return true; }
        nodeptr = nodeptr->next;
    }
    return false;
}
IDLookup RainList::get(std::string s)
{
    RainListNode* nodeptr = head;
    while (nodeptr)
    {
        if (s == nodeptr->value.str) { return nodeptr->value; }
        nodeptr = nodeptr->next;
    }
    return NULLSTRNUM;
}
void RainList::append(IDLookup strnum)
{
    RainListNode* newNode;
    RainListNode* nodePtr;
    newNode = new RainListNode;
    newNode->value = strnum;
    newNode->next = nullptr;

    if (!head) { head = newNode; } else
    {
        nodePtr = head;
        while (nodePtr->next) { nodePtr = nodePtr->next; }
        nodePtr->next = newNode;
    }
}
void RainList::displayList() const
{
    RainListNode* nodePtr;
    nodePtr = head;
    while (nodePtr)
    {
        std::cout
            << nodePtr->value.str << " at "
            << nodePtr->value.num << '\n';
        nodePtr = nodePtr->next;
    }
}
void RainList::insert(IDLookup val)
{
    RainListNode* newNode;
    newNode = new RainListNode;
    newNode->value = val;
    if (!head)
    {
        head = newNode;
        newNode->next = nullptr;
    } 
    else
    {
        newNode->next = head;
        head = newNode;
    }
}

void RainList::remove(IDLookup val)
{
    RainListNode* nodePtr = nullptr;
    RainListNode* previousNode = nullptr;
    if (!head) { return; }
    else if (head->value.str == val.str)
    {
        nodePtr = head->next;
        delete head;
        head = nodePtr;
    } 
    else
    {
        nodePtr = head;
        while (nodePtr != nullptr && nodePtr->value.str != val.str)
        {
            previousNode = nodePtr;
            nodePtr = nodePtr->next;
        }
        if (nodePtr)
        {
            previousNode->next = nodePtr->next;
            delete nodePtr;
        }
    }
}
RainList::~RainList()
{
    RainListNode* traversingNode;
    RainListNode* nextNode;
    traversingNode = head;
    while (traversingNode != nullptr)
    {
        nextNode = traversingNode->next;
        delete traversingNode;
        traversingNode = nextNode;
    }
}