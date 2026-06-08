# Hospital Management System

## Project Overview

A console-based hospital management system in C++. It supports role-based login, admin management of doctors and patients, appointment booking with slot availability, billing, payments, wallet top-ups, per-doctor revenue, appointment cancel/delete, and history views. Data is stored in text files in the project folder.

## Features

- **Main menu** — Admin, doctor, or patient login
- **Admin** — CRUD doctors/patients, book appointments, manage appointments (cancel/delete), complete visits, bills, revenue (overall + per doctor), patient top-up and history
- **Doctor** — View appointments, complete scheduled visits, view own revenue
- **Patient** — Balance display, top-up wallet, book by available slots, pay bills, cancel scheduled appointments, full history
- **Slot booking** — Times `09:00`, `10:00`, `11:00`, `14:00`, `15:00`, `16:00`; shows Available/Booked per date
- **Billing** — Bill created on visit complete (doctor fee); pay from patient balance
- **Persistence** — `FileHandler` load/append/update/delete for all data files

## Executable

The project builds to a **single** output file:

| File | Purpose |
|------|---------|
| `hospital.exe` | Run this application (do not use other `.exe` names) |

Rebuild with the `g++` command in the section below (output must be `hospital.exe`).

## Sample Login Data

### Admins (`admins.txt`)

| ID | Username   | Password      |
|----|------------|---------------|
| 1  | admin      | admin123      |
| 2  | superadmin | admin456      |
| 3  | manager    | hospital789   |

### Doctors (`doctors.txt`)

| ID  | Name     | Password  | Specialization | Fee (PKR) |
|-----|----------|-----------|----------------|-----------|
| 401 | DrDawood | doctor123 | Orthopedic     | 1800      |
| 402 | DrSara   | doctor456 | Cardiology     | 2000      |
| 403 | DrOmar   | doctor789 | Pediatrics     | 1500      |

### Patients (`patients.txt`)

| ID | Name | Password   | Balance (PKR) |
|----|------|------------|---------------|
| 1  | Ali  | patient123 | 5000          |
| 2  | Sara | patient456 | 3000          |
| 3  | Omar | patient789 | 1500          |

### Sample appointments (`appointments.txt`)

Pre-loaded bookings for patients 1–3 with doctor 401 (see file for dates/times).

## Build and Run

**Compile (output must be `hospital.exe`):**

```bash
g++ main.cpp admin.cpp appointment.cpp bill.cpp doctor.cpp file-handler.cpp file-not-find-exception.cpp hospital-exception.cpp insufficient-funds-exception.cpp invalid-input-exception.cpp patient.cpp person.cpp slot-unavailable-exception.cpp validator.cpp -o hospital.exe
```

**Windows PowerShell:**

```powershell
.\hospital.exe
```

**Linux / Git Bash:** `./hospital.exe`

## Usage

### Main menu

| Option | Action        |
|--------|---------------|
| 1      | Admin login   |
| 2      | Doctor login  |
| 3      | Patient login |
| 4      | Exit          |

### Admin dashboard

| Option | Action |
|--------|--------|
| 1 | Manage doctors |
| 2 | Manage patients |
| 3 | Book appointment |
| 4 | Complete visit & create bill |
| 5 | Manage appointments (view / cancel / delete) |
| 6 | View all bills |
| 7 | Overall revenue report |
| 8 | Revenue by doctor |
| 9 | View all doctors |
| 10 | View all patients |
| 11 | Logout |

**Manage doctors** — list, create, update, remove.

**Manage patients** — list, create, update, remove, top up balance, view history.

**Manage appointments** — view all; cancel (`Scheduled` → `Cancelled`); delete (removes record; removes unpaid bill; blocks if bill is **Paid**).

### Doctor dashboard

| Option | Action |
|--------|--------|
| 1 | My appointments (patient names, status summary) |
| 2 | Complete visit — lists **scheduled** appointment IDs first, then bill is created |
| 3 | My revenue (paid/pending bills; shows guidance if no bills yet) |
| 4 | Logout |

**Doctor revenue note:** Revenue is **PKR 0** until you complete a scheduled visit (option 2). Use appointment IDs **1, 2, 3** from option 1 — not random numbers.

### Patient dashboard

| Option | Action |
|--------|--------|
| 1 | My appointments |
| 2 | My bills |
| 3 | Pay a bill |
| 4 | Book appointment (available slots) |
| 5 | Top up wallet |
| 6 | My history |
| 7 | Cancel my appointment (`Scheduled` only) |
| 8 | Logout |

### Booking flow

1. Choose doctor (list shown with ID, specialization, fee).
2. Enter date `DD-MM-YYYY` (year ≥ 2026).
3. View slots — **Available** or **Booked**.
4. Pick an available slot number (or try another date).
5. Appointment saved as `Scheduled`.

### Billing and revenue

1. Complete a **Scheduled** visit → bill `Pending` (doctor fee).
2. Patient pays → balance deducted, bill `Paid`.
3. Admin: overall revenue or **revenue by doctor**.
4. Doctor: **My revenue** for own visits only.

### Cancel vs delete (appointments)

- **Cancel** — Status `Cancelled`; record kept; slot freed.
- **Delete** — Removed from `appointments.txt`; unpaid linked bill removed; cannot delete if bill is **Paid**.

## Data Files

Created/loaded at startup in the project root:

| File | Format |
|------|--------|
| `admins.txt` | `id name password` |
| `doctors.txt` | `id name password specialization contact fee` |
| `patients.txt` | `id name password age gender contact balance` |
| `appointments.txt` | `aptID patientID doctorID date time status` |
| `bills.txt` | `billID appointmentID patientID amount status` |

Use **single-word** values for names and specialization (files are space-delimited).

## Validation

| Field | Rule |
|-------|------|
| Password | ≥ 6 characters, no spaces |
| Contact | `03XX-XXXXXXX` (12 characters) |
| Date | `DD-MM-YYYY`, year ≥ 2026 |
| Booking times | `09:00`, `10:00`, `11:00`, `14:00`, `15:00`, `16:00` |

## Source Files (compile list)

`main.cpp`, `admin.cpp`, `appointment.cpp`, `bill.cpp`, `doctor.cpp`, `file-handler.cpp`, `file-not-find-exception.cpp`, `hospital-exception.cpp`, `insufficient-funds-exception.cpp`, `invalid-input-exception.cpp`, `patient.cpp`, `person.cpp`, `slot-unavailable-exception.cpp`, `validator.cpp`

## Status

- Builds cleanly to `hospital.exe` with `-Wall -Wextra`
- Full admin/doctor/patient flows with file persistence
- Sample data in `admins.txt`, `doctors.txt`, `patients.txt`, `appointments.txt`
