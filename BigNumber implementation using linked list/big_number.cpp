#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;


struct Node {

    Node(int digit) {
        this->digit = digit;
        this->next = this->previous = nullptr;
    }

    int digit;
    Node *previous, *next;
};


struct LinkedList {

    LinkedList() {
        tail = head = nullptr;
    }

    LinkedList(string &numberString) {

        if (numberString.length() == 0) 
            throw invalid_argument("Valid number should contain at least one digit..");

        head = new Node(numberString[0] - '0');
        head->previous = nullptr;
        Node *current = head;
        length = 1;

        for (int i = 1; i < numberString.length(); i++) {

            Node *node = new Node(numberString[i] - '0');
            current->next = node;
            node->previous = current;
            current = node;
            length++;
        }

        tail = current;
    }

    Node *head, *tail;
    int length;
};


class BigNumber {

public:
    BigNumber(string &numberString) {
        listOfDigits = new LinkedList(numberString);
    }

    BigNumber(LinkedList *listOfDigits) {
        this->listOfDigits = listOfDigits;
    }

    BigNumber& operator+(const BigNumber &secondNumber) {

        LinkedList *firstList = this->listOfDigits;
        LinkedList *secondList = secondNumber.listOfDigits;

        if (firstList->length > secondList->length) {

            LinkedList *temp = firstList;
            firstList = secondList;
            secondList = temp;
        }

        LinkedList *resultList = new LinkedList();
        Node *firstCurrent = firstList->tail;
        Node *secondCurrent = secondList->tail;
        int carry = 0;
        int sum = firstCurrent->digit + secondCurrent->digit + carry;
        resultList->tail = new Node(sum % 10);
        carry = sum / 10;
        Node *resultCurrent = resultList->tail;
        firstCurrent = firstCurrent->previous;
        secondCurrent = secondCurrent->previous;

        while (firstCurrent != nullptr) {

            sum = firstCurrent->digit + secondCurrent->digit + carry;
            Node *node = new Node(sum % 10);
            carry = sum / 10;

            node->next = resultCurrent;
            resultCurrent->previous = node;
            firstCurrent = firstCurrent->previous;
            secondCurrent = secondCurrent->previous;
            resultCurrent = node;
        }

        while (secondCurrent != nullptr) {

            sum = secondCurrent->digit + carry;
            Node *node = new Node(sum % 10);
            carry = sum / 10;
            node->next = resultCurrent;
            resultCurrent->previous = node;
            secondCurrent = secondCurrent->previous;
            resultCurrent = node;
        }

        if (carry != 0) {
            Node *node = new Node(carry);
            node->next = resultCurrent;
            resultCurrent->previous = node;
            resultCurrent = node;
        }

        resultList->head = resultCurrent;

        return *(new BigNumber(resultList));
    }

    BigNumber& operator*(const BigNumber &secondNumber) {

        LinkedList *firstList = this->listOfDigits;
        LinkedList *secondList = secondNumber.listOfDigits;

        if (firstList->length == 1 && firstList->head->digit == 0) return *(new BigNumber(ZERO));
        if (secondList->length == 1 && secondList->head->digit == 0) return *(new BigNumber(ZERO));

        if (firstList->length < secondList->length) {
            LinkedList *temp = firstList;
            firstList = secondList;
            secondList = temp;
        }

        LinkedList *resultLeftHalf = new LinkedList();
        resultLeftHalf->tail = resultLeftHalf->head = new Node(0);
        LinkedList *resultRightHalf = new LinkedList();
        resultRightHalf->head = resultRightHalf->tail = new Node(0);
        Node *secondCurrent = secondList->tail;

        while (secondCurrent != nullptr) {

            Node *firstCurrent = firstList->tail;
            LinkedList *temporaryList = new LinkedList();
            int carry = 0;
            int product = secondCurrent->digit * firstCurrent->digit;
            temporaryList->tail = new Node(product % 10);
            Node *temporaryCurrent = temporaryList->tail;
            carry = product / 10;
            firstCurrent = firstCurrent->previous;

            while (firstCurrent != nullptr) {

                product = secondCurrent->digit * firstCurrent->digit + carry;
                Node *node = new Node(product % 10);
                carry = product / 10;
                temporaryCurrent->previous = node;
                node->next = temporaryCurrent;
                temporaryCurrent = node;
                firstCurrent = firstCurrent->previous;
            }

            secondCurrent = secondCurrent->previous;

            if (carry != 0) {

                Node *node = new Node(carry);
                node->next = temporaryCurrent;
                temporaryCurrent->previous = node;
                temporaryCurrent = node;
            }
            temporaryList->head = temporaryCurrent;

            BigNumber b1(resultLeftHalf);
            BigNumber b2(temporaryList);
            resultLeftHalf = (b1 + b2).listOfDigits;

            resultRightHalf->head->previous = resultLeftHalf->tail;
            resultLeftHalf->tail->next = resultRightHalf->head;
            resultRightHalf->head = resultRightHalf->head->previous;
            resultLeftHalf->tail = resultLeftHalf->tail->previous;
        }
        resultRightHalf->tail = resultRightHalf->tail->previous;
        resultRightHalf->tail->next = nullptr;

        return *(new BigNumber(resultLeftHalf));
    }

    friend ostream& operator<<(ostream &os, const BigNumber &bigNumber) {

        Node *current = bigNumber.listOfDigits->head;

        while (current != nullptr) {
            os << current->digit;
            current = current->next;
        }

        return os;
    }

private:
    LinkedList *listOfDigits;
    string ZERO = "0";
};


int main() {

    // Testing summation and multiplication functions 
    
    string firstNumberString, secondNumberString;

    cout << "Enter first number: ";
    cin >> firstNumberString;
    cout << "Enter second number: ";
    cin >> secondNumberString;

    BigNumber number1(firstNumberString);
    BigNumber number2(secondNumberString);

    cout << number1 << " + " << number2 << " = " << number1 + number2 << endl;
    cout << number1 << " * " << number2 << " = " << number1 * number2 << endl;

}
