#include "LinkedList.h"

template <typename T>
LinkNode<T>::LinkNode(T c) {
	val = c;
	next = nullptr;
}
template <typename T>
void LinkNode<T>::append(T c) {
	if (!next) { next = new LinkNode(c); }
	else { next->append(c); }
}
template <typename T>
void LinkNode<T>::insert(T c) {
	LinkNode* ptr = next;
	next = new LinkNode(c);
	next->next = ptr;
}
void LinkNode<RainDataEntry*>::rainInsert(RainDataEntry* e) {
	// assumes everything earlier is already sorted
}
void LinkNode<char>::appendUnitTest() {
	char key[] = { 'a','b','c','d' };
	size_t keySize = sizeof(key) / sizeof(key[0]);
	LinkNode alphaHead(key[0]);
	LinkNode* node = &alphaHead;
	for (size_t i = 1; i < keySize; i++) {
		alphaHead.append(key[i]);
	}
	for (size_t i = 0; i < keySize; i++) {
		if (key[i] != node->val) {
			std::cout <<
				"Unit test failed for LinkNode::append(char c) !\n"
				"iteration: " << i <<
				"\nresult: " << node->val <<
				"\nexpected: " << key[i];
			throw;
		}
		node = node->next;
	}
	std::cout << "Unit test passed for LinkNode::append(char) !\n";
}
void LinkNode<char>::insertUnitTest() {
	char begin[] = { 'a','b','c' };
	char key[] = { 'a','A','b','B','c','C' };
	size_t 
		beginSize = sizeof(begin) / sizeof(begin[0]), 
		keySize = sizeof(key) / sizeof(key[0]), i;
	LinkNode alphaHead(begin[0]);
	for (i = 1; i < beginSize; i++) {
		alphaHead.append(begin[i]);
	}
	LinkNode* node = &alphaHead;
	while (true) {
		if (!node) { break; }
		char nextVal = toupper(node->val);
		node->insert(nextVal);
		node = node->next->next;
	}
	node = &alphaHead;
	i = 0;
	while (true) {
		if (!node) { break; }
		if (node->val != key[i]) {
			std::cout <<
				"Unit test for LinkNode::insert(char) failed!\n"
				"Iteration: " << i << 
				"\Expected: " << key[i] <<
				"\nResult: " << node->val << '\n';
			throw;
		}
		node = node->next;
		i++;
	}
	if (i != keySize) {
		std::cout <<
			"Unit test for LinkNode::insert(char) failed!\n"
			"Iterations: " << i <<
			"\nExpected: " << keySize << '\n';
		throw;
	}
	std::cout << "Unit test for LinkNode::insert(char) passed!\n";
}
//static void destructorUnitTest() {
	//	// C++ 'delete' keyword sets deleted pointers address to 8123 hex (33059 dec)
	//	const int DELETED = 33059;
	//	const size_t ALPHALEN = 26;
	//	LinkNode* alphaHead = new LinkNode('a');
	//	LinkNode* ptrs[ALPHALEN];
	//	for (size_t i = 1; i < ALPHALEN; i++) {
	//		alphaHead->append((char)('a' + i));
	//	}
	//	ptrs[0] = alphaHead; // sets
	//	/*std::cout 
	//	while (true) {
	//		std::cout << &ptrs[0];
	//	}*/
	//	for (size_t i = 1; i < ALPHALEN; i++) {
	//		ptrs[i] = ptrs[0]->getNext();
	//		std::cout << ptrs[i]->getVal() << '\n';
	//		ptrs[0] = ptrs[0]->getNext(); // increments
	//	}
	//	ptrs[0] = alphaHead; // resets
	//	delete alphaHead;
	//	for (size_t i = 0; i < ALPHALEN; i++) {
	//		if ((int)ptrs[i] != DELETED) {
	//			std::cout << 
	//				"Unit test failed for LinkNode::~LinkNode() !\n"
	//				"iteration: " << i <<
	//				"\nexpected: " << DELETED << "\nresult: " << int(ptrs[i]);
	//			//throw;
	//		}
	//	}
	//	std::cout << "Unit test passed for LinkNode::~LinkNode() !\n";
	//}