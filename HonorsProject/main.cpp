#include <iostream>
#include "SpecialTree.h"

int main() {

    SpecialTree* tree = new SpecialTree();
    bool success;
    std::string info;
    std::string key;
    char input;
    void printMenu();

    // menu loop
    while (true) {

        printMenu();
        std::cin.get(input);
        input = toupper(input);
        std::cin.ignore(20, '\n');

        switch (input) {

            // insert a new patient (randomly generated) into the tree
            case 'I':

                info = tree->generatePatientInfo();
                success = tree->insertPatient(info);

                if (success) {
                    std::cout << "Patient with information: '" + info + "' was added.\n";
                }
                else {
                    std::cout << "Error: A Patient with that information already exists.\n";
                }

                break;

                // search for a patient in the tree
            case 'S':

                std::cout << "Enter key of patient to search for: ";
                std::cin >> key;
                std::cin.ignore(20, '\n');
                success = tree->searchPatient(key);

                if (success) {
                    std::cout << "Patient with information: '" + key + "' was found.\n";
                }
                else {
                    std::cout << "Patient with information: '" + key + "' could not be found in the list.\n";
                }

                break;

                // delete patient from tree
            case 'D':
                std::cout << "Enter key of patient to delete: ";
                std::cin >> key;
                std::cin.ignore(20, '\n');
                success = tree->deletePatient(key);

                if (success) {
                    std::cout << "Patient with information: '" + key + "' was successfully deleted.\n";
                }
                else {
                    std::cout << "Patient with information: '" + key + "' could not be found so was not deleted.\n";
                }

                break;

                // show patient with lowest priority (least urgent)
            case 'L':
                // if the list is empty show error
                if (tree->numPatients() == 0) {
                    std::cout << "There are no patients in the list.\n";
                }
                else {
                    info = tree->leastUrgent();
                    std::cout << "Least urgent patient is: '" + info + "'\n";
                }
                break;

                // show patient with highest priority (most urgent)
            case 'M':
                // if the list is empty show error
                if (tree->numPatients() == 0) {
                    std::cout << "There are no patients in the list.\n";
                }
                else {
                    info = tree->mostUrgent();
                    std::cout << "Most urgent patient is: '" + info + "'\n";
                }

                break;

                // print patient in order of priority
            case 'P':
                // if the list is empty show error
                if (tree->numPatients() == 0) {
                    std::cout << "There are no patients in the list.\n";
                }
                // else print patients
                else tree->showPriorityOrder();

                break;

                // terminate program
            case 'Q':
                exit(0);

            default:
                std::cout << "Unexpected input; try again.";
        }
    }
}

void printMenu() {
    std::cout << "\nMENU";
    std::cout << "\nI - insert a patient with randomly generated data.";
    std::cout << "\nD - delete a patient by key.";
    std::cout << "\nS - search for a patient by key.";
    std::cout << "\nM - locate most urgent patient.";
    std::cout << "\nL - locate least urgent patient.";
    std::cout << "\nP - print patients in order of highest to lowest priority.\n";
    std::cout << "\nEnter Input: ";
}
