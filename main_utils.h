#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include <fstream>
#include <string>
#include "file-handler.h"
#include "file-not-found-exception.h"
#include "invalid-input-exception.h"
#include "validator.h"
#include "admin.h"
#include "doctor.h"
#include "patient.h"
#include "appointment.h"
#include "bill.h"

void clearStdin();
int readInt(const std::string& prompt, int minValue, int maxValue);
std::string readLine(const std::string& prompt);
double readDouble(const std::string& prompt, bool mustBePositive);
std::string readValidatedPassword();
std::string readValidatedContact();

void ensureFileExists(const char* path);
Admin* loginAdmin(Storage<Admin>& admins);
Doctor* loginDoctor(Storage<Doctor>& doctors);
Patient* loginPatient(Storage<Patient>& patients);

void showAppointmentList(const Storage<Appointment>& appointments);
bool isStatusScheduled(const Appointment* apt);
bool isStatusCompleted(const Appointment* apt);
const char* getPatientNameById(const Storage<Patient>& patients, int patientID);
void showAppointmentsForDoctor(const Storage<Appointment>& appointments,
                               const Storage<Patient>& patients,
                               int doctorID);
void showDoctorScheduledAppointments(const Storage<Appointment>& appointments,
                                     const Storage<Patient>& patients,
                                     int doctorID);
void showAppointmentsForPatient(const Storage<Appointment>& appointments,
                                int patientID);
int nextAppointmentId(const Storage<Appointment>& appointments);
int nextBillId(const Storage<Bill>& bills);
bool isSlotTaken(const Storage<Appointment>& appointments, int doctorID,
                 const char* date, const char* time);
Appointment* findAppointmentById(Storage<Appointment>& appointments,
                                 int appointmentID);
bool removeAppointmentById(Storage<Appointment>& appointments, int appointmentID);
Bill* findBillByAppointment(Storage<Bill>& bills, int appointmentID);
bool removeBillById(Storage<Bill>& bills, int billID);
void showAllBills(const Storage<Bill>& bills);
void showBillsForPatient(const Storage<Bill>& bills, int patientID);
bool isBillPaid(const Bill& bill);
void showRevenueReport(const Storage<Bill>& bills);
int getDoctorIdForAppointment(const Storage<Appointment>& appointments,
                              int appointmentID);
void showRevenueByDoctor(const Storage<Doctor>& doctors,
                         const Storage<Bill>& bills,
                         const Storage<Appointment>& appointments);
void showDoctorOwnRevenue(Doctor* doctor,
                          const Storage<Bill>& bills,
                          const Storage<Appointment>& appointments);
void showDoctorDaySlots(const Storage<Appointment>& appointments,
                        int doctorID, const char* date);
bool pickAvailableSlot(const Storage<Appointment>& appointments,
                       int doctorID, const char* date, char* outTime);
void showPatientHistory(const Storage<Appointment>& appointments,
                        const Storage<Bill>& bills,
                        int patientID);
void listDoctorsBrief(const Storage<Doctor>& doctors);
void topUpPatientWallet(Storage<Patient>& patients,
                        FileHandler& fileHandler, int fixedPatientId);
void bookAppointment(Storage<Appointment>& appointments,
                     const Storage<Doctor>& doctors,
                     const Storage<Patient>& patients,
                     FileHandler& fileHandler, int fixedPatientId);
void cancelAppointment(Storage<Appointment>& appointments,
                       FileHandler& fileHandler, int patientID);
void deleteAppointmentAdmin(Storage<Appointment>& appointments,
                            Storage<Bill>& bills,
                            FileHandler& fileHandler);
void manageAppointmentsMenu(Storage<Appointment>& appointments,
                            Storage<Bill>& bills,
                            FileHandler& fileHandler);
void completeAppointment(Storage<Appointment>& appointments,
                         Storage<Bill>& bills,
                         const Storage<Doctor>& doctors,
                         const Storage<Patient>& patients,
                         FileHandler& fileHandler, int doctorID);
void payBill(Patient* patient, Storage<Bill>& bills, FileHandler& fileHandler);
void listAllDoctors(const Storage<Doctor>& doctors);
void listAllPatients(const Storage<Patient>& patients);
void createDoctorAccount(Storage<Doctor>& doctors, FileHandler& fileHandler);
void updateDoctorAccount(Storage<Doctor>& doctors, FileHandler& fileHandler);
void removeDoctorAccount(Storage<Doctor>& doctors, FileHandler& fileHandler);
void createPatientAccount(Storage<Patient>& patients, FileHandler& fileHandler);
void updatePatientAccount(Storage<Patient>& patients, FileHandler& fileHandler);
void removePatientAccount(Storage<Patient>& patients, FileHandler& fileHandler);
void manageDoctorsMenu(Storage<Doctor>& doctors, FileHandler& fileHandler);
void managePatientsMenu(Storage<Patient>& patients,
                        const Storage<Appointment>& appointments,
                        const Storage<Bill>& bills,
                        FileHandler& fileHandler);
void adminMenu(Admin* admin, Storage<Doctor>& doctors, Storage<Patient>& patients,
               Storage<Appointment>& appointments, Storage<Bill>& bills,
               FileHandler& fileHandler);
void doctorMenu(Doctor* doctor, Storage<Appointment>& appointments,
                Storage<Bill>& bills, const Storage<Doctor>& doctors,
                const Storage<Patient>& patients, FileHandler& fileHandler);
void patientMenu(Patient* patient, Storage<Appointment>& appointments,
                 Storage<Bill>& bills, const Storage<Doctor>& doctors,
                 Storage<Patient>& patients, FileHandler& fileHandler);

#endif // MAIN_UTILS_H
