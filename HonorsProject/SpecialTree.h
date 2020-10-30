//
// Created by William on 10/19/2020.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#ifndef HONORSPROJECT_SPECIALTREE_H
#define HONORSPROJECT_SPECIALTREE_H

#endif //HONORSPROJECT_SPECIALTREE_H

// each node represents a patient
struct Patient {
    std::string key;
    std::string priority;
    Patient* left;
    Patient* right;
};

class SpecialTree {

private:
    Patient* root;
    int size;
    int rnIndex;
    int fnIndex;
    int lnIndex;
    std::vector<int> randomNumbers;
    std::vector<std::string> firstNameList;
    std::vector<std::string> lastNameList;
public:
    SpecialTree();
    ~SpecialTree();
    bool insertPatient(const std::string& key);
    Patient* recursiveInsert(const std::string& key, Patient* parentNode);
    Patient* correctPriority(Patient* parentNode, bool childOnLeft);
    Patient* leftRotation(Patient* parentNode);
    Patient* rightRotation(Patient* parentNode);
    bool deletePatient(const std::string& key);
    Patient* recursiveDelete(const std::string& key, Patient* node);
    Patient* searchPatient(const std::string& key);
    Patient* recursiveSearch(const std::string& key, Patient* parentNode);
    std::string mostUrgent();
    std::string leastUrgent();
    void recursiveLeastUrgent(Patient* node, std::string& lowestPriority, std::string& patientInfo);
    void showPriorityOrder();
    void copyToArray(Patient* array, int& index, Patient* node);
    void insertionSort(Patient* array);
    int numPatients();

    std::string generatePriority();
    std::string generatePatientInfo();

};

// constructor
SpecialTree::SpecialTree() {
    root = nullptr;
    size = 0;
    rnIndex = 0;
    fnIndex = 0;
    lnIndex = 0;

    std::string line;
    srand(time(0));

    // generate pool of distinct priorities to pull from
    for (int i = 10; i <= 40; i++) {
        randomNumbers.push_back(i);
    }
    random_shuffle(randomNumbers.begin(), randomNumbers.end());

    // read in first name file file
    std::ifstream fnFile("firstNames.txt");
    if (fnFile.is_open()){

        while ( getline (fnFile,line)){
            firstNameList.push_back(line);
        }
        fnFile.close();
    }
    else std::cout << "Unable to open first name file\n";

    // read in first name file file
    std::ifstream lnFile("lastNames.txt");
    if (lnFile.is_open()){

        while ( getline (lnFile,line)){
            lastNameList.push_back(line);
        }
        lnFile.close();
    }
    else std::cout << "Unable to open last name file\n";
}

// destructor
SpecialTree::~SpecialTree() {
    //
}

// insert a patient into the tree
bool SpecialTree::insertPatient(const std::string& key) {

    // couldn't find a way to force failure in the event of key existing
    if (searchPatient(key)) return false;

    // if tree has at least 1 node, use this function to recursively insert
    recursiveInsert(key, root);
    return true;
}

// this function recursively traverses the tree to find an insertion point, then fixes the heap property
//      on the way back up.
Patient* SpecialTree::recursiveInsert(const std::string& key, Patient* node) {

    bool childOnLeft = false;

    // base case: insert the node
    if (node == nullptr) {

        node = new Patient();
        node->key = key;
        node->priority = generatePriority();
        node->left = nullptr;
        node->right = nullptr;

        if (root == nullptr) root = node;
        size++;
        return node;
    }

    // decide if we must traverse left or right by comparing keys
    if (key.compare(node->key) < 0) {
        childOnLeft = true;
        node->left = recursiveInsert(key, node->left);
    }
    else {
        node->right = recursiveInsert(key, node->right);
    }

    // only triggers after node has been inserted, traces node back up the tree
    return correctPriority(node, childOnLeft);
}

// fixes the priority organization of a given node
Patient* SpecialTree::correctPriority(Patient* node, bool childOnLeft) {

    // if left child is ancestor of inserted node
    if (childOnLeft) {

        // if child priority is higher than parent, we need to move it up the tree
        if(node->priority.compare(node->left->priority) >= 0) {
            node = rightRotation(node);
        }
    }

        // else if right child is ancestor of inserted node and priorities are out of order
    else if(node->priority.compare(node->right->priority) >= 0) {
        node = leftRotation(node);
    }

    return node;
}

// rotates nodes on the tree left
Patient* SpecialTree::leftRotation(Patient* node) {
    Patient* rightChild = node->right;
    Patient* intermediateNode = rightChild->left;

    // rotation
    rightChild->left = node;
    node->right = intermediateNode;

    // correct root node labeling if necessary
    if (node == root) root = rightChild;

    return rightChild;
}

// rotates nodes on the tree right
Patient* SpecialTree::rightRotation(Patient* node) {
    Patient* leftChild = node->left;
    Patient* intermediateNode = leftChild->right;

    // rotation
    leftChild->right = node;
    node->left = intermediateNode;

    // correct root node labeling if necessary
    if (node == root) root = leftChild;

    return leftChild;
}

// delete a patient from the tree
bool SpecialTree::deletePatient(const std::string& key) {

    return (recursiveDelete(key, root) != nullptr);
}

// recursively traverses the tree to find and delete a node with specified key
Patient* SpecialTree::recursiveDelete(const std::string& key, Patient* node) {

    // if the node wasn't in the tree
    if (node == nullptr) return nullptr;

    // search for node to be deleted
    if (key.compare(node->key) < 0) {
        // set node->left to return value to reestablish connection
        node->left = recursiveDelete(key, node->left);
    }
    else if (key.compare(node->key) > 0) {
        node->right = recursiveDelete(key, node->right);
    }

    // at this stage we are looking at the node to be deleted

    // if we can delete node (cases 1 and 2)
    else if (node->left == nullptr) {
        Patient* temp = node->right;
        if (node == root) root = node->right; // fixes the root pointer for this class
        delete node;
        node = temp;
    }
    else if (node->right == nullptr) {
        Patient* temp = node->left;
        if (node == root) root = node->left;  // fixes the root pointer for this class
        delete node;
        node = temp;
    }

    // if we can't yet delete node (case 3)
    else if (node->left->priority.compare(node->right->priority) > 0) {
        node = leftRotation(node);
        node->left = recursiveDelete(key, node->left);
    }
    else {
        node = rightRotation(node);
        node->right = recursiveDelete(key, node->right);
    }

    // return the node so that its parent knows where to point
    return node;
}

// initial call for the recursive search function
Patient* SpecialTree::searchPatient(const std::string& key) {
    return recursiveSearch(key, root);
}

// function recursively finds the node with the specified key
Patient* SpecialTree::recursiveSearch(const std::string& key, Patient* parentNode) {

    // if we hit nullptr we know the node is not in the list
    if (parentNode == nullptr) return nullptr;

    // if the patient to delete will be on the left
    else if (key.compare(parentNode->key) < 0) {
        return recursiveSearch(key, parentNode->left);
    }
    // if patient to delete will be on the right
    else if (key.compare(parentNode->key) > 0) {
        return recursiveSearch(key, parentNode->right);
    }
    // only remaining option is that this node is the node we searched for
    return parentNode;
}

// return the info of patient with the highest priority
std::string SpecialTree::mostUrgent() {
    return root->key;
}

// return the info of patient with the lowest priority
std::string SpecialTree::leastUrgent() {
    std::string lowestPriority = root->priority;
    std::string patientInfo = root->key;

    // recursively searches tree to find patient info of node with lowest priority
    recursiveLeastUrgent(root, lowestPriority, patientInfo);

    return patientInfo;
}
// recursively travels the tree and compares all leaf nodes to find lowest priority
void SpecialTree::recursiveLeastUrgent(Patient* node, std::string& lowestPriority, std::string& patientInfo) {

    // recur down left side of tree searching for lowest priority
    if (node->left != nullptr) {
        recursiveLeastUrgent(node->left, lowestPriority, patientInfo);
    }

    // recur down right side of tree searching for lowest priority
    if (node->right != nullptr) {
        recursiveLeastUrgent(node->right, lowestPriority, patientInfo);
    }

    // compare priorities on leaves only, if this node has a lower one this is the new lowest priority
    else if (lowestPriority.compare(node->priority) < 0) {
        lowestPriority = node->priority;    // track lowest priority
        patientInfo = node->key;            // track info of node with lowest priority
    }
}

// prints the patients in order of their priority by transferring tree into array and sorting
void SpecialTree::showPriorityOrder() {

    // create an array which will temporarily store patient and be sorted
    Patient* array = new Patient[size];
    int index = 0;

    copyToArray(array, index, root);
    insertionSort(array);

    // print the array in priority sorted order
    for (int i = 0; i < size; i++) {
        std::cout << '\n' << array[i].key << "\t\tPriority: " << array[i].priority;
    }

    // free memory
    delete [] array;
}

// copy all of the elements from the tree into an array by traversing
void SpecialTree::copyToArray(Patient* array, int& index, Patient* node) {

    // copy to array
    array[index] = *node;
    index++;

    // traverse left side of tree
    if (node->left != nullptr) {
        copyToArray(array, index, node->left);
    }

    // traverse right side of tree
    if (node->right != nullptr) {
        copyToArray(array, index, node->right);
    }
}

// standard insertion sort
void SpecialTree::insertionSort(Patient* array) {

    Patient key;
    int j;

    // traverse array applying insertion sort
    for (int i = 0; i < size; i++) {
        key = array[i];
        j = i;

        // move elements down the array when/while necessary (sorting)
        while (j > 0 && key.priority.compare(array[j-1].priority) < 0) {
            array[j] = array[j-1];
            j--;
        }
        array[j] = key;
    }
}

// returns the number of patients in the list
int SpecialTree::numPatients() {
    return size;
}

// generates a priority at random (format of P#L#)
std::string SpecialTree::generatePriority() {

    // there are only so many non-repeating numbers, if we exceed that amount we reshuffle and allow repetition
    if (rnIndex > 30) {
        std::cout << "exhausted list of non-repeating priorities, priorities may now repeat.";
        random_shuffle(randomNumbers.begin(), randomNumbers.end());
        rnIndex = 0;
    }

    // we work through an array of randomly shuffled, unique priorities.
    int temp = randomNumbers[rnIndex];
    std::string priority = "P";

    // priority 4 (dead) has no level so must be handled differently
    if (temp == 40) priority = "P4";

    // standard priorities
    else {
        priority += std::to_string(temp / 10);
        priority += "L";
        priority += std::to_string(temp % 10);
    }

    rnIndex++;
    return priority;
}

std::string SpecialTree::generatePatientInfo() {
    std::string firstName = "";
    std::string lastName = "";

    // special error handling in case we hit the end of the first name list
    while (firstName == "") {
        // try to read a name
        try{
            firstName = firstNameList[fnIndex];
        }
        // if out of range of array reshuffle and start from beginning
        catch (const std::out_of_range& oor) {
            fnIndex = 0;
            random_shuffle(firstNameList.begin(), firstNameList.end());
        }
    }
    // add name to key and increment first name list
    fnIndex++;
    std::string key = firstName + ",";

    // special error handling in case we hit the end of last name list
    while (lastName == "") {
        // try to read last name
        try {
            lastName = lastNameList[lnIndex];
        }
        // if out of range, reshuffle and start from beginning
        catch (const std::out_of_range& oor) {
            lnIndex = 0;
            random_shuffle(lastNameList.begin(), lastNameList.end());
        }
    }

    // add last name to key and increment last name list
    lnIndex++;
    key += lastName + ",";

    bool male = rand() % 2;
    if (male) key += "M,";
    else key += "F,";

    int month = rand() % 11 + 1;        // random month
    if (month < 10) key += "0" + std::to_string(month) + "/";
    else key += std::to_string(month) + "/";

    // for simplicity, every month will have exactly 30 days
    int day = rand() % 29 + 1;          // random day
    if (day < 10) key += "0" + std::to_string(day) + "/";
    else key += std::to_string(day) + "/";

    int year = rand() % 101 + 1920;    // random year between 1920 and 2020
    key += std::to_string(year);

    return key;
}
