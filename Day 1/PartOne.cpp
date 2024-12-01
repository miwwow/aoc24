#include <iostream>
#include <fstream>
#include <string>

struct bstNode {
    int value;
    bstNode* left;
    bstNode* right;

    bstNode(int val) {
        value = val;
        left = right = nullptr;
    }
};

void bstInsert(bstNode* &head, int value) {
    bstNode* inserted = new bstNode(value);

    if (head == nullptr) {
        head = inserted;
        return;
    }

    if (value <= head->value) {
        bstInsert(head->left, value);
    } else {
        bstInsert(head->right, value);
    }
}

class InorderIterator {
    private:
        bstNode* current;
        bstNode* rightMost;
    public:
        InorderIterator(bstNode *root) {
            current = root;
            rightMost = nullptr;
        }

        bool hasNext() { return current != nullptr; }

        bstNode* next() {
            if (!hasNext())
                return nullptr;

            if (current->left == nullptr) {
                // no element on left, so return current and backtrack via link
                bstNode* temp = current;
                current = current->right;
                return temp;
            }

            // find rightmost node of left tree, so we can link it to the current one
            bstNode* rightMost = current->left;
            while (rightMost->right != nullptr && rightMost->right != current)
                rightMost = rightMost->right;

            if (rightMost->right == nullptr) {
                // link rightmost node to current node so we can backtrack later
                rightMost->right = current;
                current = current->left;
            } else {
                // fully traversed left tree, so return current node and backtrack via link
                rightMost->right = nullptr;
                bstNode* temp = current;
                current = current->right;
                return temp;
            }

            return next();
        }
};

void PartOne(bstNode* leftHead, bstNode* rightHead) {
    InorderIterator leftIter(leftHead);
    InorderIterator rightIter(rightHead);
    int total = 0;

    while (leftIter.hasNext() && rightIter.hasNext()) {
        total += abs(leftIter.next()->value - rightIter.next()->value);
    }

    std::cout << "Part one solution: " << total << std::endl;
}

bstNode* searchBinaryTree(bstNode* head, int needle) {
    if (head == nullptr) {
        return nullptr;
    }

    if (head->value == needle) {
        return head;
    } else if (needle < head->value) {
        return searchBinaryTree(head->left, needle);
    } else {
        return searchBinaryTree(head->right, needle);
    }
}

void PartTwo(bstNode* leftHead, bstNode* rightHead) {
    InorderIterator leftIter(leftHead);
    int similarity = 0;

    while (leftIter.hasNext()) {
        bstNode* currentNode = leftIter.next();
        bstNode* found = searchBinaryTree(rightHead, currentNode->value);
        if (found == nullptr)
            continue;

        int count = 0;
        while (found != nullptr && found->value == currentNode->value) {
            count++;
            found = searchBinaryTree(found->left, currentNode->value);
        }
        similarity += (currentNode->value * count);
    }

    std::cout << "Part two solution: " << similarity << std::endl;
}

int main() {
    bstNode* leftHead = nullptr;
    bstNode* rightHead = nullptr;

    std::ifstream input("input.txt");
    std::string currentLn;

    while (std::getline(input, currentLn)) {
        // split string by space, and add both values to their respective BST
        bstInsert(leftHead, std::stoi(currentLn.substr(0, currentLn.find(" "))));
        bstInsert(rightHead, std::stoi(currentLn.substr(currentLn.find(" ") + 1, currentLn.length())));
    }
    input.close();

    PartOne(leftHead, rightHead);
    PartTwo(leftHead, rightHead);

    return 0;
}
