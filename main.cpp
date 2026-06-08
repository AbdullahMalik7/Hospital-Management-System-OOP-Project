#include <iostream>
#include "main_utils.h"

using namespace std;

int main() {
    Storage<Admin> admins;
    Storage<Doctor> doctors;
    Storage<Patient> patients;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    FileHandler fileHandler;

    try {
        fileHandler.loadAdmins(admins);
    } catch (const FileNotFoundException& error) {
        cerr << error.what() << "\n";
        ensureFileExists("admins.txt");
    }

    try {
        fileHandler.loadDoctors(doctors);
    } catch (const FileNotFoundException& error) {
        cerr << error.what() << "\n";
        ensureFileExists("doctors.txt");
    }

    try {
        fileHandler.loadPatients(patients);
    } catch (const FileNotFoundException& error) {
        cerr << error.what() << "\n";
        ensureFileExists("patients.txt");
    }

    try {
        fileHandler.loadAppointments(appointments);
    } catch (const FileNotFoundException& error) {
        cerr << error.what() << "\n";
        ensureFileExists("appointments.txt");
    }

    try {
        fileHandler.loadBills(bills);
    } catch (const FileNotFoundException& error) {
        cerr << error.what() << "\n";
        ensureFileExists("bills.txt");
    }

    if (admins.getSize() == 0) {
        Admin defaultAdmin(1, "admin", "admin123");
        admins.add(defaultAdmin);
        try {
            fileHandler.appendAdmin(defaultAdmin);
        } catch (const FileNotFoundException& error) {
            cerr << error.what() << "\n";
        }
        cout << "Created default admin account (ID=1, password=admin123).\n";
    }

    while (true) {
        cout << "\n=== Hospital Management System ===\n";
        cout << "1. Admin login\n";
        cout << "2. Doctor login\n";
        cout << "3. Patient login\n";
        cout << "4. Exit\n";

        int choice = readInt("Choose an option: ", 1, 4);
        if (choice == 4) {
            cout << "Exiting application.\n";
            break;
        }

        if (choice == 1) {
            Admin* admin = loginAdmin(admins);
            if (admin) {
                adminMenu(admin, doctors, patients, appointments, bills, fileHandler);
            } else {
                cout << "Invalid admin credentials.\n";
            }
        } else if (choice == 2) {
            Doctor* doctor = loginDoctor(doctors);
            if (doctor) {
                doctorMenu(doctor, appointments, bills, doctors, patients, fileHandler);
            } else {
                cout << "Invalid doctor credentials.\n";
            }
        } else if (choice == 3) {
            Patient* patient = loginPatient(patients);
            if (patient) {
                patientMenu(patient, appointments, bills, doctors, patients, fileHandler);
            } else {
                cout << "Invalid patient credentials.\n";
            }
        }
    }

    return 0;
}
