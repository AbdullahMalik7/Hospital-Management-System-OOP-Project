#include "patient.h"
#include <iostream>

Patient::Patient(){
    Age = 0;
    Gender[0] = '\0';
    Contact[0] = '\0';
    Balance = 0;
}

Patient::Patient(int id, const char* name, const char* password, int age, const char* gender, const char* contact, double balance)
: Person(id, name, password), Age(age), Balance(balance)
{
    StringCopy(Gender, gender);
    StringCopy(Contact, contact);
}

Patient::~Patient(){}

// Getters
int Patient::getAge() const {
    return Age;
}

const char* Patient::getGender() const {
    return Gender;
}

const char* Patient::getContact() const {
    return Contact;
}

double Patient::getBalance() const {
    return Balance;
}

// Setters
void Patient::setAge(int age) {
    Age = age;
}

void Patient::setGender(const char* gender) {
    StringCopy(Gender, gender);
}

void Patient::setContact(const char* contact) {
    StringCopy(Contact, contact);
}

void Patient::setBalance(double balance) {
    Balance = balance;
}

// Wallet
void Patient::addFunds(double amount) {
    if(amount > 0) Balance += amount;
}

void Patient::deductFunds(double amount) {       // was "abount" typo in your original
    if(amount > 0) Balance -= amount;
}

// Virtual Functions
void Patient::showMenu() {
    // To be implemented
}

void Patient::printDetails() const {
    std::cout << "Patient Details" << std::endl;
    std::cout << "ID: "      << getID()      << std::endl;
    std::cout << "Name: "    << getName()    << std::endl;
    std::cout << "Age: "     << Age          << std::endl;
    std::cout << "Gender: "  << Gender       << std::endl;
    std::cout << "Contact: " << Contact      << std::endl;
    std::cout << "Balance: " << Balance      << std::endl;
}