# Student-Attendance-Management-System
A simple, file-based Student Attendance Management System written in C for the terminal. It supports two roles: Admin and Student. Admins can add/delete students, mark attendance for a date, view any student’s attendance, modify recorded attendance, and generate defaulter reports based on a threshold. Students can log in to view their personal attendance summary and detailed per-date records, and change their passwords.

Data is stored in two binary files (students.dat, attendance.dat) for persistence across runs. Attendance percentage is computed as: Present = 1.0, Late = 0.5, Absent = 0.0 credit. Input is collected via scanf with bounds checks to reduce overflows. The project is portable and builds with any C99-compatible compiler (GCC/Clang) on Linux/macOS/Windows (via MinGW or WSL). No external libraries are required.

Features:

Admin

Add/Delete student

Mark attendance (P/A/L) for a date

View student attendance + detailed records

Modify a recorded attendance entry

Generate reports + defaulter list (threshold)

Student

View own attendance + per-date details

Change password

Build
Prerequisites: GCC or Clang, Make (optional)

GCC:

gcc -std=c99 -O2 -Wall -Wextra -o attendance mini_project.c

Clang:

clang -std=c99 -O2 -Wall -Wextra -o attendance mini_project.c

Run
./attendance

Binary data files will be created automatically in the working directory:

students.dat

attendance.dat

Usage
Main Menu: Admin Login | Student Login | Exit

Default Admin credentials:

Username: admin

Password: admin123

Admin can manage students and attendance; Students can view and change their password.

Notes
Late counts as 0.5 in percentage.

Arrays capped at 100 students, 1000 attendance entries.

For Windows CMD, use attendance.exe.

