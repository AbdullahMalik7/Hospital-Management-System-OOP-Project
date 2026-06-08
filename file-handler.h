#pragma once

#include "storage.h"
#include "patient.h"
#include "doctor.h"
#include "admin.h"
#include "appointment.h"
#include "bill.h"

//#include "FileNotFountException.h"


class FileHandler {
    public:
    //Loades Objects in Memory
    void loadPatients(Storage<Patient>& patientDB);
    void loadDoctors(Storage<Doctor>& doctorsDB);
    void loadAdmins(Storage<Admin>& adminDB);
    void loadAppointments(Storage<Appointment>& appointmentDB);
    void loadBills(Storage<Bill>& billDB);

    //Appenders
    void appendAdmin(const Admin& a);
    void appendPatient(const Patient& p);
    void appendDoctor(const Doctor& d);
    void appendAppointment(const Appointment& a);
    void appendBill(const Bill& b);

    //Updaters
    void updatePatient(const Patient& updatePatient);
    void updateDoctor(const Doctor& updateDoctor);
    void updateAppointment(const Appointment& updated);
    void updateBill(const Bill& updated);

    //Deleters
    void deletePatient(int patientID);
    void deleteDoctor(int doctorID);
    void deleteAppointment(int appointmentID);
    void deleteBill(int billID);
};

