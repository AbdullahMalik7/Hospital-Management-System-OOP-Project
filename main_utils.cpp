#include "main_utils.h"
#include <iostream>
#include <limits>
#include <cstring>

using namespace std;

void clearStdin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        int value;
        if (cin >> value) {
            if (value >= minValue && value <= maxValue) {
                clearStdin();
                return value;
            }
            cout << "Please choose a valid option between " << minValue << " and " << maxValue << ".\n";
        } else {
            cout << "Input must be a number. Try again.\n";
            cin.clear();
        }
        clearStdin();
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

double readDouble(const string& prompt, bool mustBePositive) {
    while (true) {
        cout << prompt;
        double value;
        if (cin >> value) {
            clearStdin();
            try {
                if (mustBePositive) {
                    Validator::validatePositiveDouble(value);
                } else if (value < 0) {
                    throw InvalidInputException("Value cannot be negative.");
                }
                return value;
            } catch (const InvalidInputException& error) {
                cout << error.what() << "\n";
            }
        } else {
            cout << "Input must be a number. Try again.\n";
            cin.clear();
            clearStdin();
        }
    }
}

string readValidatedPassword() {
    while (true) {
        string password = readLine("Password: ");
        try {
            Validator::validatePassword(password.c_str());
            return password;
        } catch (const InvalidInputException& error) {
            cout << error.what() << "\n";
        }
    }
}

string readValidatedContact() {
    while (true) {
        string contact = readLine("Contact (03XX-XXXXXXX): ");
        try {
            Validator::validateContact(contact.c_str());
            return contact;
        } catch (const InvalidInputException& error) {
            cout << error.what() << "\n";
        }
    }
}

void ensureFileExists(const char* path) {
    ofstream file(path, ios::app);
}

Admin* loginAdmin(Storage<Admin>& admins) {
    int id = readInt("Admin ID: ", 1, 99999);
    string password = readLine("Password: ");

    Admin* admin = admins.findByID(id);
    if (admin && password == admin->getPassword()) {
        return admin;
    }
    return nullptr;
}

Doctor* loginDoctor(Storage<Doctor>& doctors) {
    int id = readInt("Doctor ID: ", 1, 99999);
    string password = readLine("Password: ");

    Doctor* doctor = doctors.findByID(id);
    if (doctor && password == doctor->getPassword()) {
        return doctor;
    }
    return nullptr;
}

Patient* loginPatient(Storage<Patient>& patients) {
    int id = readInt("Patient ID: ", 1, 99999);
    string password = readLine("Password: ");

    Patient* patient = patients.findByID(id);
    if (patient && password == patient->getPassword()) {
        return patient;
    }
    return nullptr;
}

void showAppointmentList(const Storage<Appointment>& appointments) {
    const Appointment* list = appointments.getAll();
    int size = appointments.getSize();
    if (size == 0) {
        cout << "No appointments available.\n";
        return;
    }
    for (int i = 0; i < size; ++i) {
        cout << list[i] << "\n";
    }
}

bool isStatusScheduled(const Appointment* apt) {
    return apt && strcmp(apt->getStatus(), "Scheduled") == 0;
}

bool isStatusCompleted(const Appointment* apt) {
    return apt && strcmp(apt->getStatus(), "Completed") == 0;
}

const char* getPatientNameById(const Storage<Patient>& patients, int patientID) {
    const Patient* patient = patients.findByID(patientID);
    if (patient) {
        return patient->getName();
    }
    return "Unknown";
}

void showAppointmentsForDoctor(const Storage<Appointment>& appointments,
                               const Storage<Patient>& patients,
                               int doctorID) {
    const Appointment* list = appointments.getAll();
    int count = appointments.getSize();
    bool found = false;
    int scheduled = 0;
    int completed = 0;
    int cancelled = 0;

    cout << "\n--- My Appointments ---\n";
    for (int i = 0; i < count; ++i) {
        if (list[i].getDoctorID() != doctorID) {
            continue;
        }
        found = true;
        cout << "\n[#" << list[i].getAppointmentID() << "] "
             << list[i].getDate() << " " << list[i].getTime()
             << " | Patient: " << getPatientNameById(patients, list[i].getPatientID())
             << " (ID " << list[i].getPatientID() << ")\n";
        cout << "Status: " << list[i].getStatus() << "\n";

        if (isStatusScheduled(&list[i])) {
            scheduled++;
        } else if (isStatusCompleted(&list[i])) {
            completed++;
        } else if (strcmp(list[i].getStatus(), "Cancelled") == 0) {
            cancelled++;
        }
    }

    if (!found) {
        cout << "No appointments found for this doctor.\n";
        return;
    }

    cout << "\nSummary: Scheduled=" << scheduled
         << " | Completed=" << completed
         << " | Cancelled=" << cancelled << "\n";
}

void showDoctorScheduledAppointments(const Storage<Appointment>& appointments,
                                     const Storage<Patient>& patients,
                                     int doctorID) {
    const Appointment* list = appointments.getAll();
    bool found = false;

    cout << "Scheduled visits you can complete:\n";
    for (int i = 0; i < appointments.getSize(); ++i) {
        if (list[i].getDoctorID() != doctorID || !isStatusScheduled(&list[i])) {
            continue;
        }
        found = true;
        cout << "  Appointment #" << list[i].getAppointmentID() << " - "
             << getPatientNameById(patients, list[i].getPatientID())
             << " on " << list[i].getDate() << " at " << list[i].getTime() << "\n";
    }
    if (!found) {
        cout << "  (none)\n";
    }
}

void showAppointmentsForPatient(const Storage<Appointment>& appointments, int patientID) {
    const Appointment* list = appointments.getAll();
    int count = appointments.getSize();
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (list[i].getPatientID() == patientID) {
            cout << list[i] << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No appointments found for this patient.\n";
    }
}

int nextAppointmentId(const Storage<Appointment>& appointments) {
    int maxId = 0;
    const Appointment* list = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); ++i) {
        if (list[i].getAppointmentID() > maxId) {
            maxId = list[i].getAppointmentID();
        }
    }
    return maxId + 1;
}

int nextBillId(const Storage<Bill>& bills) {
    int maxId = 0;
    const Bill* list = bills.getAll();
    for (int i = 0; i < bills.getSize(); ++i) {
        if (list[i].getBillID() > maxId) {
            maxId = list[i].getBillID();
        }
    }
    return maxId + 1;
}

bool isSlotTaken(const Storage<Appointment>& appointments, int doctorID,
                 const char* date, const char* time) {
    Appointment check(0, 0, doctorID, date, time, "Scheduled");
    const Appointment* list = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); ++i) {
        if (list[i] == check) {
            return true;
        }
    }
    return false;
}

Appointment* findAppointmentById(Storage<Appointment>& appointments, int appointmentID) {
    for (int i = 0; i < appointments.getSize(); ++i) {
        Appointment* apt = appointments.getAt(i);
        if (apt && apt->getAppointmentID() == appointmentID) {
            return apt;
        }
    }
    return nullptr;
}

bool removeAppointmentById(Storage<Appointment>& appointments, int appointmentID) {
    for (int i = 0; i < appointments.getSize(); ++i) {
        Appointment* apt = appointments.getAt(i);
        if (apt && apt->getAppointmentID() == appointmentID) {
            return appointments.removeAt(i);
        }
    }
    return false;
}

Bill* findBillByAppointment(Storage<Bill>& bills, int appointmentID) {
    for (int i = 0; i < bills.getSize(); ++i) {
        Bill* bill = bills.getAt(i);
        if (bill && bill->getAppointmentID() == appointmentID) {
            return bill;
        }
    }
    return nullptr;
}

bool removeBillById(Storage<Bill>& bills, int billID) {
    for (int i = 0; i < bills.getSize(); ++i) {
        Bill* bill = bills.getAt(i);
        if (bill && bill->getBillID() == billID) {
            return bills.removeAt(i);
        }
    }
    return false;
}

void showAllBills(const Storage<Bill>& bills) {
    if (bills.getSize() == 0) {
        cout << "No bills on record.\n";
        return;
    }
    const Bill* list = bills.getAll();
    for (int i = 0; i < bills.getSize(); ++i) {
        cout << list[i] << "\n";
    }
}

void showBillsForPatient(const Storage<Bill>& bills, int patientID) {
    const Bill* list = bills.getAll();
    bool found = false;
    for (int i = 0; i < bills.getSize(); ++i) {
        if (list[i].getPatientID() == patientID) {
            cout << list[i] << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No bills for this patient.\n";
    }
}

bool isBillPaid(const Bill& bill) {
    return bill.getStatus()[0] == 'P' && bill.getStatus()[1] == 'a';
}

void showRevenueReport(const Storage<Bill>& bills) {
    const Bill* list = bills.getAll();
    double totalPaid = 0;
    double totalPending = 0;
    int paidCount = 0;
    int pendingCount = 0;

    for (int i = 0; i < bills.getSize(); ++i) {
        if (isBillPaid(list[i])) {
            totalPaid += list[i].getAmount();
            paidCount++;
        } else {
            totalPending += list[i].getAmount();
            pendingCount++;
        }
    }

    cout << "\n--- Overall Revenue Report ---\n";
    cout << "Total revenue (paid bills): PKR " << totalPaid << "\n";
    cout << "Pending collections: PKR " << totalPending << "\n";
    cout << "Paid bills: " << paidCount << " | Pending bills: " << pendingCount << "\n";
    cout << "Grand total billed: PKR " << (totalPaid + totalPending) << "\n";
}

int getDoctorIdForAppointment(const Storage<Appointment>& appointments, int appointmentID) {
    const Appointment* list = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); ++i) {
        if (list[i].getAppointmentID() == appointmentID) {
            return list[i].getDoctorID();
        }
    }
    return -1;
}

void showRevenueByDoctor(const Storage<Doctor>& doctors, const Storage<Bill>& bills,
                         const Storage<Appointment>& appointments) {
    cout << "\n--- Revenue By Doctor ---\n";
    if (doctors.getSize() == 0) {
        cout << "No doctors registered.\n";
        return;
    }

    const Doctor* doctorList = doctors.getAll();
    const Bill* billList = bills.getAll();

    for (int d = 0; d < doctors.getSize(); ++d) {
        int doctorID = doctorList[d].getID();
        double paid = 0;
        double pending = 0;
        int visitCount = 0;

        for (int b = 0; b < bills.getSize(); ++b) {
            if (getDoctorIdForAppointment(appointments, billList[b].getAppointmentID()) != doctorID) {
                continue;
            }
            visitCount++;
            if (isBillPaid(billList[b])) {
                paid += billList[b].getAmount();
            } else {
                pending += billList[b].getAmount();
            }
        }

        cout << "\nDr. " << doctorList[d].getName() << " (ID " << doctorID << ")\n";
        cout << "  Paid revenue: PKR " << paid << "\n";
        cout << "  Pending: PKR " << pending << "\n";
        cout << "  Bills: " << visitCount << "\n";
    }
}

void showDoctorOwnRevenue(Doctor* doctor, const Storage<Bill>& bills,
                          const Storage<Appointment>& appointments) {
    double paid = 0;
    double pending = 0;
    int billCount = 0;
    const Bill* billList = bills.getAll();

    int scheduledVisits = 0;
    int completedVisits = 0;
    const Appointment* aptList = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); ++i) {
        if (aptList[i].getDoctorID() != doctor->getID()) {
            continue;
        }
        if (isStatusScheduled(&aptList[i])) {
            scheduledVisits++;
        } else if (isStatusCompleted(&aptList[i])) {
            completedVisits++;
        }
    }

    for (int i = 0; i < bills.getSize(); ++i) {
        if (getDoctorIdForAppointment(appointments, billList[i].getAppointmentID()) != doctor->getID()) {
            continue;
        }
        billCount++;
        if (isBillPaid(billList[i])) {
            paid += billList[i].getAmount();
        } else {
            pending += billList[i].getAmount();
        }
    }

    cout << "\n--- My Revenue ---\n";
    cout << "Consultation fee: PKR " << doctor->getFee() << " per visit\n";
    cout << "Appointments: " << scheduledVisits << " scheduled, "
         << completedVisits << " completed\n";

    if (billCount == 0) {
        cout << "\nNo bills yet. Use option 2 to complete a scheduled visit.\n";
        cout << "Revenue appears after visits are completed and billed.\n";
        if (scheduledVisits > 0) {
            cout << "Potential from scheduled visits: PKR "
                 << (scheduledVisits * doctor->getFee()) << "\n";
        }
        return;
    }

    cout << "\nPaid revenue: PKR " << paid << "\n";
    cout << "Pending collections: PKR " << pending << "\n";
    cout << "Total billed: PKR " << (paid + pending) << "\n";
    cout << "Bills on record: " << billCount << "\n";
}

const char* STANDARD_SLOTS[] = {"09:00", "10:00", "11:00", "14:00", "15:00", "16:00"};
const int NUM_STANDARD_SLOTS = 6;

void showDoctorDaySlots(const Storage<Appointment>& appointments, int doctorID, const char* date) {
    cout << "\nSlots on " << date << " for doctor ID " << doctorID << ":\n";
    for (int i = 0; i < NUM_STANDARD_SLOTS; ++i) {
        bool taken = isSlotTaken(appointments, doctorID, date, STANDARD_SLOTS[i]);
        cout << "  " << STANDARD_SLOTS[i] << " - "
             << (taken ? "Booked" : "Available") << "\n";
    }
}

bool pickAvailableSlot(const Storage<Appointment>& appointments, int doctorID,
                       const char* date, char* outTime) {
    int availableIndexes[6];
    int availableCount = 0;

    for (int i = 0; i < NUM_STANDARD_SLOTS; ++i) {
        if (!isSlotTaken(appointments, doctorID, date, STANDARD_SLOTS[i])) {
            availableIndexes[availableCount++] = i;
        }
    }

    if (availableCount == 0) {
        cout << "No available slots on this date. Choose another date.\n";
        return false;
    }

    cout << "\nAvailable slots (pick one):\n";
    for (int i = 0; i < availableCount; ++i) {
        int slotIndex = availableIndexes[i];
        cout << "  " << (i + 1) << ". " << STANDARD_SLOTS[slotIndex] << "\n";
    }

    int pick = readInt("Choose slot number: ", 1, availableCount);
    int chosen = availableIndexes[pick - 1];
    int j = 0;
    while (STANDARD_SLOTS[chosen][j] != '\0') {
        outTime[j] = STANDARD_SLOTS[chosen][j];
        j++;
    }
    outTime[j] = '\0';
    return true;
}

void showPatientHistory(const Storage<Appointment>& appointments, const Storage<Bill>& bills,
                        int patientID) {
    cout << "\n--- Appointment History ---\n";
    showAppointmentsForPatient(appointments, patientID);
    cout << "\n--- Bill History ---\n";
    showBillsForPatient(bills, patientID);
}

void listDoctorsBrief(const Storage<Doctor>& doctors) {
    if (doctors.getSize() == 0) {
        cout << "No doctors available.\n";
        return;
    }
    cout << "\nAvailable doctors:\n";
    const Doctor* list = doctors.getAll();
    for (int i = 0; i < doctors.getSize(); ++i) {
        cout << "  ID " << list[i].getID() << " - " << list[i].getName()
             << " (" << list[i].getSpecialization() << ") Fee: PKR "
             << list[i].getFee() << "\n";
    }
}

void topUpPatientWallet(Storage<Patient>& patients, FileHandler& fileHandler, int fixedPatientId) {
    cout << "\n--- Top Up Wallet ---\n";

    Patient* patient = nullptr;
    if (fixedPatientId < 0) {
        int patientID = readInt("Patient ID: ", 1, 99999);
        patient = patients.findByID(patientID);
    } else {
        patient = patients.findByID(fixedPatientId);
    }

    if (!patient) {
        cout << "Patient not found.\n";
        return;
    }

    cout << "Patient: " << patient->getName() << " (ID " << patient->getID() << ")\n";
    cout << "Current balance: PKR " << patient->getBalance() << "\n";

    double amount = readDouble("Amount to add (PKR): ", true);
    patient->addFunds(amount);

    try {
        fileHandler.updatePatient(*patient);
        cout << "Top-up successful. New balance: PKR " << patient->getBalance() << "\n";
    } catch (const FileNotFoundException& error) {
        patient->deductFunds(amount);
        cout << error.what() << "\n";
    }
}

void bookAppointment(Storage<Appointment>& appointments, const Storage<Doctor>& doctors,
                     const Storage<Patient>& patients, FileHandler& fileHandler, int fixedPatientId) {
    cout << "\n--- Book Appointment ---\n";

    int patientID = fixedPatientId;
    if (patientID < 0) {
        patientID = readInt("Patient ID: ", 1, 99999);
    }
    if (!patients.findByID(patientID)) {
        cout << "Patient not found.\n";
        return;
    }

    int doctorID = readInt("Doctor ID: ", 1, 99999);
    const Doctor* doctor = doctors.findByID(doctorID);
    if (!doctor) {
        cout << "Doctor not found.\n";
        return;
    }

    listDoctorsBrief(doctors);

    string date;
    string time;
    char timeBuffer[10];
    bool booked = false;

    while (!booked) {
        while (true) {
            date = readLine("Date (DD-MM-YYYY): ");
            try {
                Validator::validateDate(date.c_str());
                break;
            } catch (const InvalidInputException& error) {
                cout << error.what() << "\n";
            }
        }

        showDoctorDaySlots(appointments, doctorID, date.c_str());
        if (!pickAvailableSlot(appointments, doctorID, date.c_str(), timeBuffer)) {
            string retry = readLine("Try another date? (y/n): ");
            if (retry != "y" && retry != "Y") {
                return;
            }
            continue;
        }
        booked = true;
    }

    time = timeBuffer;

    int aptID = nextAppointmentId(appointments);
    Appointment apt(aptID, patientID, doctorID, date.c_str(), time.c_str(), "Scheduled");
    if (!appointments.add(apt)) {
        cout << "Appointment storage is full.\n";
        return;
    }

    try {
        fileHandler.appendAppointment(apt);
        cout << "Appointment booked. ID: " << aptID
             << " (Fee when completed: PKR " << doctor->getFee() << ")\n";
    } catch (const FileNotFoundException& error) {
        for (int i = 0; i < appointments.getSize(); ++i) {
            Appointment* current = appointments.getAt(i);
            if (current && current->getAppointmentID() == aptID) {
                appointments.removeAt(i);
                break;
            }
        }
        cout << error.what() << "\n";
    }
}

void cancelAppointment(Storage<Appointment>& appointments, FileHandler& fileHandler, int patientID) {
    cout << "\n--- Cancel Appointment ---\n";
    int aptID = readInt("Appointment ID: ", 1, 99999);
    Appointment* apt = findAppointmentById(appointments, aptID);
    if (!apt) {
        cout << "Appointment not found.\n";
        return;
    }
    if (patientID > 0 && apt->getPatientID() != patientID) {
        cout << "This appointment does not belong to you.\n";
        return;
    }
    if (!isStatusScheduled(apt)) {
        cout << "Only Scheduled appointments can be cancelled.\n";
        return;
    }

    apt->setStatus("Cancelled");
    try {
        fileHandler.updateAppointment(*apt);
        cout << "Appointment #" << aptID << " cancelled. Slot is now free.\n";
    } catch (const FileNotFoundException& error) {
        apt->setStatus("Scheduled");
        cout << error.what() << "\n";
    }
}

void deleteAppointmentAdmin(Storage<Appointment>& appointments, Storage<Bill>& bills,
                            FileHandler& fileHandler) {
    cout << "\n--- Delete Appointment ---\n";
    showAppointmentList(appointments);
    int aptID = readInt("Appointment ID to delete: ", 1, 99999);
    Appointment* apt = findAppointmentById(appointments, aptID);
    if (!apt) {
        cout << "Appointment not found.\n";
        return;
    }

    Bill* bill = findBillByAppointment(bills, aptID);
    if (bill && isBillPaid(*bill)) {
        cout << "Cannot delete: a paid bill exists for this visit.\n";
        return;
    }

    cout << "Appointment #" << aptID << " (" << apt->getStatus() << ")\n";
    if (bill) {
        cout << "Linked bill #" << bill->getBillID() << " (" << bill->getStatus() << ") will also be removed.\n";
    }

    string confirm = readLine("Type YES to confirm deletion: ");
    if (confirm != "YES") {
        cout << "Deletion cancelled.\n";
        return;
    }

    try {
        if (bill) {
            fileHandler.deleteBill(bill->getBillID());
            removeBillById(bills, bill->getBillID());
        }
        fileHandler.deleteAppointment(aptID);
        if (removeAppointmentById(appointments, aptID)) {
            cout << "Appointment deleted successfully.\n";
        } else {
            cout << "Removed from file; memory update failed.\n";
        }
    } catch (const FileNotFoundException& error) {
        cout << error.what() << "\n";
    }
}

void manageAppointmentsMenu(Storage<Appointment>& appointments, Storage<Bill>& bills,
                            FileHandler& fileHandler) {
    while (true) {
        cout << "\n--- Appointment Management ---\n";
        cout << "1. View all appointments\n";
        cout << "2. Cancel appointment (Scheduled only)\n";
        cout << "3. Delete appointment\n";
        cout << "4. Back to admin dashboard\n";

        int choice = readInt("Choose an option: ", 1, 4);
        if (choice == 4) {
            break;
        }
        if (choice == 1) {
            showAppointmentList(appointments);
        } else if (choice == 2) {
            cancelAppointment(appointments, fileHandler, -1);
        } else if (choice == 3) {
            deleteAppointmentAdmin(appointments, bills, fileHandler);
        }
    }
}

void completeAppointment(Storage<Appointment>& appointments, Storage<Bill>& bills,
                         const Storage<Doctor>& doctors, const Storage<Patient>& patients,
                         FileHandler& fileHandler, int doctorID) {
    cout << "\n--- Complete Visit & Generate Bill ---\n";
    if (doctorID > 0) {
        showDoctorScheduledAppointments(appointments, patients, doctorID);
    }
    int aptID = readInt("Appointment ID (see list above): ", 1, 99999);
    Appointment* apt = findAppointmentById(appointments, aptID);
    if (!apt) {
        cout << "Appointment not found.\n";
        return;
    }
    if (doctorID > 0 && apt->getDoctorID() != doctorID) {
        cout << "This appointment is not assigned to you.\n";
        return;
    }
    if (!isStatusScheduled(apt)) {
        cout << "Only Scheduled appointments can be completed.\n";
        return;
    }
    if (findBillByAppointment(bills, aptID)) {
        cout << "A bill already exists for this appointment.\n";
        return;
    }

    const Doctor* doctor = doctors.findByID(apt->getDoctorID());
    if (!doctor) {
        cout << "Doctor not found for this appointment.\n";
        return;
    }

    apt->setStatus("Completed");
    int billID = nextBillId(bills);
    Bill bill(billID, aptID, apt->getPatientID(), doctor->getFee(), "Pending");

    if (!bills.add(bill)) {
        apt->setStatus("Scheduled");
        cout << "Bill storage is full.\n";
        return;
    }

    try {
        fileHandler.updateAppointment(*apt);
        fileHandler.appendBill(bill);
        cout << "Visit marked completed. Bill #" << billID
             << " created for PKR " << doctor->getFee() << " (Pending).\n";
    } catch (const FileNotFoundException& error) {
        apt->setStatus("Scheduled");
        removeBillById(bills, billID);
        cout << error.what() << "\n";
    }
}

void payBill(Patient* patient, Storage<Bill>& bills, FileHandler& fileHandler) {
    cout << "\n--- Pay Bill ---\n";
    showBillsForPatient(bills, patient->getID());

    int billID = readInt("Bill ID to pay: ", 1, 99999);
    Bill* bill = nullptr;
    for (int i = 0; i < bills.getSize(); ++i) {
        Bill* current = bills.getAt(i);
        if (current && current->getBillID() == billID) {
            bill = current;
            break;
        }
    }
    if (!bill) {
        cout << "Bill not found.\n";
        return;
    }
    if (bill->getPatientID() != patient->getID()) {
        cout << "This bill does not belong to you.\n";
        return;
    }
    if (isBillPaid(*bill)) {
        cout << "This bill is already paid.\n";
        return;
    }

    double amount = bill->getAmount();
    if (patient->getBalance() < amount) {
        cout << "Insufficient balance. Need PKR " << amount
             << ", you have PKR " << patient->getBalance() << ".\n";
        return;
    }

    patient->deductFunds(amount);
    bill->setStatus("Paid");

    try {
        fileHandler.updatePatient(*patient);
        fileHandler.updateBill(*bill);
        cout << "Payment successful. Remaining balance: PKR " << patient->getBalance() << "\n";
    } catch (const FileNotFoundException& error) {
        patient->addFunds(amount);
        bill->setStatus("Pending");
        cout << error.what() << "\n";
    }
}

void listAllDoctors(const Storage<Doctor>& doctors) {
    if (doctors.getSize() == 0) {
        cout << "No doctors registered.\n";
        return;
    }
    const Doctor* doctorList = doctors.getAll();
    for (int i = 0; i < doctors.getSize(); ++i) {
        cout << doctorList[i] << "\n";
    }
}

void listAllPatients(const Storage<Patient>& patients) {
    if (patients.getSize() == 0) {
        cout << "No patients registered.\n";
        return;
    }
    const Patient* patientList = patients.getAll();
    for (int i = 0; i < patients.getSize(); ++i) {
        patientList[i].printDetails();
        cout << "\n";
    }
}

void createDoctorAccount(Storage<Doctor>& doctors, FileHandler& fileHandler) {
    cout << "\n--- Create Doctor Account ---\n";
    int id = readInt("Doctor ID: ", 1, 99999);
    try {
        Validator::validateID(id);
    } catch (const InvalidInputException& error) {
        cout << error.what() << "\n";
        return;
    }
    if (doctors.findByID(id)) {
        cout << "A doctor with this ID already exists.\n";
        return;
    }

    string name = readLine("Name: ");
    if (name.empty()) {
        cout << "Name cannot be empty.\n";
        return;
    }

    string password = readValidatedPassword();
    string specialization = readLine("Specialization: ");
    if (specialization.empty()) {
        cout << "Specialization cannot be empty.\n";
        return;
    }

    string contact = readValidatedContact();
    double fee = readDouble("Consultation fee: ", true);

    Doctor doctor(id, name.c_str(), password.c_str(), specialization.c_str(), contact.c_str(), fee);
    if (!doctors.add(doctor)) {
        cout << "Doctor storage is full.\n";
        return;
    }

    try {
        fileHandler.appendDoctor(doctor);
        cout << "Doctor account created successfully.\n";
    } catch (const FileNotFoundException& error) {
        doctors.removeByID(id);
        cout << error.what() << "\n";
    }
}

void updateDoctorAccount(Storage<Doctor>& doctors, FileHandler& fileHandler) {
    cout << "\n--- Update Doctor Account ---\n";
    int id = readInt("Doctor ID to update: ", 1, 99999);
    Doctor* doctor = doctors.findByID(id);
    if (!doctor) {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "Current details:\n" << *doctor << "\nLeave a field blank to keep the current value.\n";

    string name = readLine("New name: ");
    if (!name.empty()) {
        doctor->setName(name.c_str());
    }

    string password = readLine("New password: ");
    if (!password.empty()) {
        try {
            Validator::validatePassword(password.c_str());
            doctor->setPassword(password.c_str());
        } catch (const InvalidInputException& error) {
            cout << error.what() << " Password not changed.\n";
        }
    }

    string specialization = readLine("New specialization: ");
    if (!specialization.empty()) {
        doctor->setSpecialization(specialization.c_str());
    }

    string contact = readLine("New contact (03XX-XXXXXXX): ");
    if (!contact.empty()) {
        try {
            Validator::validateContact(contact.c_str());
            doctor->setContact(contact.c_str());
        } catch (const InvalidInputException& error) {
            cout << error.what() << " Contact not changed.\n";
        }
    }

    string feeInput = readLine("New consultation fee: ");
    if (!feeInput.empty()) {
        try {
            double fee = stod(feeInput);
            Validator::validatePositiveDouble(fee);
            doctor->setFee(fee);
        } catch (const exception&) {
            cout << "Invalid fee. Fee not changed.\n";
        }
    }

    try {
        fileHandler.updateDoctor(*doctor);
        cout << "Doctor account updated successfully.\n";
    } catch (const FileNotFoundException& error) {
        cout << error.what() << "\n";
    }
}

void removeDoctorAccount(Storage<Doctor>& doctors, FileHandler& fileHandler) {
    cout << "\n--- Remove Doctor Account ---\n";
    int id = readInt("Doctor ID to remove: ", 1, 99999);
    if (!doctors.findByID(id)) {
        cout << "Doctor not found.\n";
        return;
    }

    string confirm = readLine("Type YES to confirm removal: ");
    if (confirm != "YES") {
        cout << "Removal cancelled.\n";
        return;
    }

    try {
        fileHandler.deleteDoctor(id);
        if (doctors.removeByID(id)) {
            cout << "Doctor account removed successfully.\n";
        } else {
            cout << "Doctor removed from file but not found in memory.\n";
        }
    } catch (const FileNotFoundException& error) {
        cout << error.what() << "\n";
    }
}

void createPatientAccount(Storage<Patient>& patients, FileHandler& fileHandler) {
    cout << "\n--- Create Patient Account ---\n";
    int id = readInt("Patient ID: ", 1, 99999);
    try {
        Validator::validateID(id);
    } catch (const InvalidInputException& error) {
        cout << error.what() << "\n";
        return;
    }
    if (patients.findByID(id)) {
        cout << "A patient with this ID already exists.\n";
        return;
    }

    string name = readLine("Name: ");
    if (name.empty()) {
        cout << "Name cannot be empty.\n";
        return;
    }

    string password = readValidatedPassword();
    int age = readInt("Age: ", 1, 150);
    string gender = readLine("Gender: ");
    if (gender.empty()) {
        cout << "Gender cannot be empty.\n";
        return;
    }

    string contact = readValidatedContact();
    double balance = readDouble("Initial balance: ", false);

    Patient patient(id, name.c_str(), password.c_str(), age, gender.c_str(), contact.c_str(), balance);
    if (!patients.add(patient)) {
        cout << "Patient storage is full.\n";
        return;
    }

    try {
        fileHandler.appendPatient(patient);
        cout << "Patient account created successfully.\n";
    } catch (const FileNotFoundException& error) {
        patients.removeByID(id);
        cout << error.what() << "\n";
    }
}

void updatePatientAccount(Storage<Patient>& patients, FileHandler& fileHandler) {
    cout << "\n--- Update Patient Account ---\n";
    int id = readInt("Patient ID to update: ", 1, 99999);
    Patient* patient = patients.findByID(id);
    if (!patient) {
        cout << "Patient not found.\n";
        return;
    }

    patient->printDetails();
    cout << "Leave a field blank to keep the current value.\n";

    string name = readLine("New name: ");
    if (!name.empty()) {
        patient->setName(name.c_str());
    }

    string password = readLine("New password: ");
    if (!password.empty()) {
        try {
            Validator::validatePassword(password.c_str());
            patient->setPassword(password.c_str());
        } catch (const InvalidInputException& error) {
            cout << error.what() << " Password not changed.\n";
        }
    }

    string ageInput = readLine("New age: ");
    if (!ageInput.empty()) {
        try {
            int age = stoi(ageInput);
            if (age >= 1 && age <= 150) {
                patient->setAge(age);
            } else {
                cout << "Age must be between 1 and 150. Age not changed.\n";
            }
        } catch (const exception&) {
            cout << "Invalid age. Age not changed.\n";
        }
    }

    string gender = readLine("New gender: ");
    if (!gender.empty()) {
        patient->setGender(gender.c_str());
    }

    string contact = readLine("New contact (03XX-XXXXXXX): ");
    if (!contact.empty()) {
        try {
            Validator::validateContact(contact.c_str());
            patient->setContact(contact.c_str());
        } catch (const InvalidInputException& error) {
            cout << error.what() << " Contact not changed.\n";
        }
    }

    string balanceInput = readLine("New balance: ");
    if (!balanceInput.empty()) {
        try {
            double balance = stod(balanceInput);
            if (balance >= 0) {
                patient->setBalance(balance);
            } else {
                cout << "Balance cannot be negative. Balance not changed.\n";
            }
        } catch (const exception&) {
            cout << "Invalid balance. Balance not changed.\n";
        }
    }

    try {
        fileHandler.updatePatient(*patient);
        cout << "Patient account updated successfully.\n";
    } catch (const FileNotFoundException& error) {
        cout << error.what() << "\n";
    }
}

void removePatientAccount(Storage<Patient>& patients, FileHandler& fileHandler) {
    cout << "\n--- Remove Patient Account ---\n";
    int id = readInt("Patient ID to remove: ", 1, 99999);
    if (!patients.findByID(id)) {
        cout << "Patient not found.\n";
        return;
    }

    string confirm = readLine("Type YES to confirm removal: ");
    if (confirm != "YES") {
        cout << "Removal cancelled.\n";
        return;
    }

    try {
        fileHandler.deletePatient(id);
        if (patients.removeByID(id)) {
            cout << "Patient account removed successfully.\n";
        } else {
            cout << "Patient removed from file but not found in memory.\n";
        }
    } catch (const FileNotFoundException& error) {
        cout << error.what() << "\n";
    }
}

void manageDoctorsMenu(Storage<Doctor>& doctors, FileHandler& fileHandler) {
    while (true) {
        cout << "\n--- Doctor Management ---\n";
        cout << "1. List all doctors\n";
        cout << "2. Create doctor account\n";
        cout << "3. Update doctor account\n";
        cout << "4. Remove doctor account\n";
        cout << "5. Back to admin dashboard\n";

        int choice = readInt("Choose an option: ", 1, 5);
        if (choice == 5) {
            break;
        }
        if (choice == 1) {
            listAllDoctors(doctors);
        } else if (choice == 2) {
            createDoctorAccount(doctors, fileHandler);
        } else if (choice == 3) {
            updateDoctorAccount(doctors, fileHandler);
        } else if (choice == 4) {
            removeDoctorAccount(doctors, fileHandler);
        }
    }
}

void managePatientsMenu(Storage<Patient>& patients, const Storage<Appointment>& appointments,
                        const Storage<Bill>& bills, FileHandler& fileHandler) {
    while (true) {
        cout << "\n--- Patient Management ---\n";
        cout << "1. List all patients\n";
        cout << "2. Create patient account\n";
        cout << "3. Update patient account\n";
        cout << "4. Remove patient account\n";
        cout << "5. Top up patient balance\n";
        cout << "6. View patient history\n";
        cout << "7. Back to admin dashboard\n";

        int choice = readInt("Choose an option: ", 1, 7);
        if (choice == 7) {
            break;
        }
        if (choice == 1) {
            listAllPatients(patients);
        } else if (choice == 2) {
            createPatientAccount(patients, fileHandler);
        } else if (choice == 3) {
            updatePatientAccount(patients, fileHandler);
        } else if (choice == 4) {
            removePatientAccount(patients, fileHandler);
        } else if (choice == 5) {
            topUpPatientWallet(patients, fileHandler, -1);
        } else if (choice == 6) {
            int patientID = readInt("Patient ID: ", 1, 99999);
            if (patients.findByID(patientID)) {
                showPatientHistory(appointments, bills, patientID);
            } else {
                cout << "Patient not found.\n";
            }
        }
    }
}

void adminMenu(Admin* admin, Storage<Doctor>& doctors, Storage<Patient>& patients,
               Storage<Appointment>& appointments, Storage<Bill>& bills,
               FileHandler& fileHandler) {
    while (true) {
        cout << "\n=== Admin Dashboard ===\n";
        admin->printDetails();
        cout << "\nTotal Doctors: " << doctors.getSize() << "\n";
        cout << "Total Patients: " << patients.getSize() << "\n";
        cout << "Total Appointments: " << appointments.getSize() << "\n";
        cout << "Total Bills: " << bills.getSize() << "\n";
        cout << "\n1. Manage doctors\n";
        cout << "2. Manage patients\n";
        cout << "3. Book appointment\n";
        cout << "4. Complete visit & create bill\n";
        cout << "5. Manage appointments (view / cancel / delete)\n";
        cout << "6. View all bills\n";
        cout << "7. Overall revenue report\n";
        cout << "8. Revenue by doctor\n";
        cout << "9. View all doctors\n";
        cout << "10. View all patients\n";
        cout << "11. Logout\n";

        int choice = readInt("Choose an option: ", 1, 11);
        if (choice == 11) {
            cout << "Logged out.\n";
            break;
        }
        if (choice == 1) {
            manageDoctorsMenu(doctors, fileHandler);
        } else if (choice == 2) {
            managePatientsMenu(patients, appointments, bills, fileHandler);
        } else if (choice == 3) {
            bookAppointment(appointments, doctors, patients, fileHandler, -1);
        } else if (choice == 4) {
            completeAppointment(appointments, bills, doctors, patients, fileHandler, -1);
        } else if (choice == 5) {
            manageAppointmentsMenu(appointments, bills, fileHandler);
        } else if (choice == 6) {
            showAllBills(bills);
        } else if (choice == 7) {
            showRevenueReport(bills);
        } else if (choice == 8) {
            showRevenueByDoctor(doctors, bills, appointments);
        } else if (choice == 9) {
            listAllDoctors(doctors);
        } else if (choice == 10) {
            listAllPatients(patients);
        }
    }
}

void doctorMenu(Doctor* doctor, Storage<Appointment>& appointments, Storage<Bill>& bills,
                const Storage<Doctor>& doctors, const Storage<Patient>& patients,
                FileHandler& fileHandler) {
    while (true) {
        cout << "\n=== Doctor Dashboard ===\n";
        doctor->printDetails();
        cout << "\n1. My appointments\n";
        cout << "2. Complete visit & create bill\n";
        cout << "3. My revenue\n";
        cout << "4. Logout\n";

        int choice = readInt("Choose an option: ", 1, 4);
        if (choice == 4) {
            cout << "Logged out.\n";
            break;
        }
        if (choice == 1) {
            showAppointmentsForDoctor(appointments, patients, doctor->getID());
        } else if (choice == 2) {
            completeAppointment(appointments, bills, doctors, patients, fileHandler, doctor->getID());
        } else if (choice == 3) {
            showDoctorOwnRevenue(doctor, bills, appointments);
        }
    }
}

void patientMenu(Patient* patient, Storage<Appointment>& appointments, Storage<Bill>& bills,
                 const Storage<Doctor>& doctors, Storage<Patient>& patients, FileHandler& fileHandler) {
    while (true) {
        cout << "\n=== Patient Dashboard ===\n";
        patient->printDetails();
        cout << "\n1. My appointments\n";
        cout << "2. My bills\n";
        cout << "3. Pay a bill\n";
        cout << "4. Book appointment (available slots)\n";
        cout << "5. Top up wallet\n";
        cout << "6. My history\n";
        cout << "7. Cancel my appointment\n";
        cout << "8. Logout\n";

        int choice = readInt("Choose an option: ", 1, 8);
        if (choice == 8) {
            cout << "Logged out.\n";
            break;
        }
        if (choice == 1) {
            showAppointmentsForPatient(appointments, patient->getID());
        } else if (choice == 2) {
            showBillsForPatient(bills, patient->getID());
        } else if (choice == 3) {
            payBill(patient, bills, fileHandler);
        } else if (choice == 4) {
            bookAppointment(appointments, doctors, patients, fileHandler, patient->getID());
        } else if (choice == 5) {
            topUpPatientWallet(patients, fileHandler, patient->getID());
        } else if (choice == 6) {
            showPatientHistory(appointments, bills, patient->getID());
        } else if (choice == 7) {
            cancelAppointment(appointments, fileHandler, patient->getID());
        }
    }
}
