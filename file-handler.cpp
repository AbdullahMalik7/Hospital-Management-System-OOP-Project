#include "file-handler.h"
#include <fstream>
#include "file-not-found-exception.h"
// Loaders Implementation

void FileHandler::loadPatients(Storage<Patient>& patientDB){
    std::ifstream file("patients.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: patient.txt could not be found on Startup");
    }

    int id;
    char name[50];
    char password[50];
    int age;
    char gender[20];
    char contact[20];
    double balance;

    while(file>> id >> name >> password >> age >> gender >> contact >> balance){
        Patient p(id , name , password , age , gender , contact , balance);
        patientDB.add(p);
    }
    file.close();
}



void FileHandler::loadDoctors(Storage<Doctor>& doctorsDB)
{
    std::ifstream file("doctors.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on Startup");
    }

    int id;
    char name[50];
    char password[50];
    char specialization[50];
    char contact[20];
    double fee;

    while(file>> id >> name >> password >> specialization >> contact >> fee ){
        Doctor d (id , name , password , specialization , contact , fee);
        doctorsDB.add(d);
    }
    file.close();
}


void FileHandler::loadAdmins(Storage<Admin>& adminDB){
    std::ifstream file("admins.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: admins.txt could not be found on startup");
    }

    int id;
    char name[50];
    char password[50];

    while(file>> id >> name >>password){
        Admin a(id , name , password);
        adminDB.add(a);
    }

    file.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& appointmentDB){
    std::ifstream file("appointments.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on startup");
    }

    int aptID;
    int pID;
    int dID;
    char date[20];
    char time[10];
    char status[20];

    while(file>> aptID >> pID >> dID >> date >> time >> status){
        Appointment a(aptID , pID , dID , date , time , status);
        appointmentDB.add(a);
    }

    file.close();
}

void FileHandler::loadBills(Storage<Bill>& billDB){
    std::ifstream file("bills.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on startup");
    }

    int billID;
    int appointmentID;
    int patientID;
    double amount;
    char status[20];

    while(file >> billID >> appointmentID >> patientID >> amount >> status){
        Bill b(billID, appointmentID, patientID, amount, status);
        billDB.add(b);
    }
    file.close();
}

// Appending

void FileHandler::appendAdmin(const Admin& a){
    std::ofstream file("admins.txt", std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: admins.txt could not be found on append");
    }
    file << a.getID() << " " << a.getName() << " " << a.getPassword() << "\n";
    file.close();
}

void FileHandler::appendPatient(const Patient& p){
    std::ofstream file("patients.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: patients.txt could not be found on Append");
    }

    file<<p.getID() <<" "
    <<p.getName()<<" "
    <<p.getPassword()<<" "
    <<p.getAge()<<" "
    <<p.getGender()<<" "
    <<p.getContact()<<" "
    <<p.getBalance()<<'\n';

    file.close();
}



void FileHandler::appendDoctor(const Doctor& d){
    std::ofstream file("doctors.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on Append");
    }

    file<<d.getID()<<" "
    <<d.getName()<<" "
    <<d.getPassword()<<" "
    <<d.getSpecialization()<<" "
    <<d.getContact()<<" "
    <<d.getFee()<<"\n";
    file.close();
}

void FileHandler::appendAppointment(const Appointment& a){
    std::ofstream file("appointments.txt", std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on append");
    }

    file << a.getAppointmentID() << " "
         << a.getPatientID() << " "
         << a.getDoctorID() << " "
         << a.getDate() << " "
         << a.getTime() << " "
         << a.getStatus() << "\n";
    file.close();
}

void FileHandler::appendBill(const Bill& b){
    std::ofstream file("bills.txt", std::ios::app);
    if(!file.is_open()){
        std::ofstream create("bills.txt");
        create.close();
        file.open("bills.txt", std::ios::app);
    }
    if(!file.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on append");
    }

    file << b.getBillID() << " "
         << b.getAppointmentID() << " "
         << b.getPatientID() << " "
         << b.getAmount() << " "
         << b.getStatus() << "\n";
    file.close();
}

void FileHandler::updatePatient(const Patient& updatePatient){
    std::ifstream fileIn("patients.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: patients.txt could not be found on updatePatient");
    }

    int id;
    char name[50];
    char password[50];
    int age;
    char gender[20];
    char contact[20];
    double balance;

    while(fileIn>> id >> name >> password >> age >> gender >> contact >> balance){
        if(id == updatePatient.getID()){
            fileOut<<updatePatient.getID() <<" "
            <<updatePatient.getName()<<" "
            <<updatePatient.getPassword()<<" "
            <<updatePatient.getAge()<<" "
            <<updatePatient.getGender()<<" "
            <<updatePatient.getContact()<<" "
            <<updatePatient.getBalance()<<'\n';
        }

        else{
            fileOut<<id<<" "<<name<<" "<<password<<" "<<age<<" "<<gender<<" "<<contact<<" "<<balance<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("patients.txt");
    rename("temp.txt", "patients.txt");

}



void FileHandler::updateDoctor(const Doctor& updateDoctor){
    std::ifstream fileIn("doctors.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on updateDoctor");
    }



    int id;
    char name[50];
    char password[50];
    char specialization[50];
    char contact[20];
    double fee;

    while(fileIn>> id >> name >> password >> specialization >> contact >> fee){
        if(id == updateDoctor.getID()){
            fileOut<<updateDoctor.getID() <<" "
            <<updateDoctor.getName()<<" "
            <<updateDoctor.getPassword()<<" "
            <<updateDoctor.getSpecialization()<<" "
            <<updateDoctor.getContact()<<" "
            <<updateDoctor.getFee()<<"\n";
        }

        else{
            fileOut<<id<<" "<<name<<" "<<password<<" "<<specialization<<" "<<contact<<" "<<fee<<" "<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("doctors.txt");
    rename("temp.txt", "doctors.txt");
}

void FileHandler::updateAppointment(const Appointment& updated){
    std::ifstream fileIn("appointments.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on update");
    }

    int aptID;
    int pID;
    int dID;
    char date[20];
    char time[10];
    char status[20];

    while(fileIn >> aptID >> pID >> dID >> date >> time >> status){
        if(aptID == updated.getAppointmentID()){
            fileOut << updated.getAppointmentID() << " "
                    << updated.getPatientID() << " "
                    << updated.getDoctorID() << " "
                    << updated.getDate() << " "
                    << updated.getTime() << " "
                    << updated.getStatus() << "\n";
        } else {
            fileOut << aptID << " " << pID << " " << dID << " "
                    << date << " " << time << " " << status << "\n";
        }
    }
    fileIn.close();
    fileOut.close();
    remove("appointments.txt");
    rename("temp.txt", "appointments.txt");
}

void FileHandler::updateBill(const Bill& updated){
    std::ifstream fileIn("bills.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on update");
    }

    int billID;
    int appointmentID;
    int patientID;
    double amount;
    char status[20];

    while(fileIn >> billID >> appointmentID >> patientID >> amount >> status){
        if(billID == updated.getBillID()){
            fileOut << updated.getBillID() << " "
                    << updated.getAppointmentID() << " "
                    << updated.getPatientID() << " "
                    << updated.getAmount() << " "
                    << updated.getStatus() << "\n";
        } else {
            fileOut << billID << " " << appointmentID << " " << patientID << " "
                    << amount << " " << status << "\n";
        }
    }
    fileIn.close();
    fileOut.close();
    remove("bills.txt");
    rename("temp.txt", "bills.txt");
}

void FileHandler::deletePatient(int patientID){
    std::ifstream fileIn("patients.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: patients.txt could not be found on deletePatient");
    }

    int id;
    char name[50];
    char password[50];
    int age;
    char gender[20];
    char contact[20];
    double balance;

    while(fileIn>>id>>name>>password>>age>>gender>>contact>>balance){
        if(id == patientID){
            continue;
        }

        else {
            fileOut<<id<<" "<<name<<" "<<password<<" "<<age<<" "<<gender<<" "<<contact<<" "<<balance<<"\n";
        }
    }

    fileIn.close();
    fileOut.close();


    remove("patients.txt");
    rename("temp.txt" , "patients.txt" );



}


void FileHandler::deleteDoctor(int doctorID){
    std::ifstream fileIn("doctors.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on deleteDoctor");
    }

    int id;
    char name[50];
    char password[50];
    char specialization[50];
    char contact[20];
    double fee;

    while(fileIn>>id>>name>>password>>specialization>>contact>>fee){
        if(id == doctorID){
            continue;
        }

        else {
            fileOut<<id<<" "<<name<<" "<<password<<" "<<specialization<<" "<<contact<<" "<<fee<<"\n";
        }
    }

    fileIn.close();
    fileOut.close();


    remove("doctors.txt");
    rename("temp.txt" , "doctors.txt" );
}

void FileHandler::deleteAppointment(int appointmentID){
    std::ifstream fileIn("appointments.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on deleteAppointment");
    }

    int aptID;
    int pID;
    int dID;
    char date[20];
    char time[10];
    char status[20];

    while(fileIn >> aptID >> pID >> dID >> date >> time >> status){
        if(aptID == appointmentID){
            continue;
        }
        fileOut << aptID << " " << pID << " " << dID << " "
                << date << " " << time << " " << status << "\n";
    }

    fileIn.close();
    fileOut.close();
    remove("appointments.txt");
    rename("temp.txt", "appointments.txt");
}

void FileHandler::deleteBill(int billID){
    std::ifstream fileIn("bills.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on deleteBill");
    }

    int id;
    int appointmentID;
    int patientID;
    double amount;
    char billStatus[20];

    while(fileIn >> id >> appointmentID >> patientID >> amount >> billStatus){
        if(id == billID){
            continue;
        }
        fileOut << id << " " << appointmentID << " " << patientID << " "
                << amount << " " << billStatus << "\n";
    }

    fileIn.close();
    fileOut.close();
    remove("bills.txt");
    rename("temp.txt", "bills.txt");
}