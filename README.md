Gynecology Patient Management System
Overview
The Gynecology Patient Management System is a console-based C++ application designed to manage gynecology patient records efficiently. The system allows healthcare staff to store, update, search, and monitor patient information, including pregnancy-related monthly records.
The project demonstrates the implementation of important Data Structures and File Handling concepts such as:
•	Doubly Linked List
•	Binary Search Tree (BST)
•	Vectors
•	File Handling
•	Object-Oriented Programming (OOP)
 
Features
 Patient Management
•	Add new patients
•	Search patients by ID
•	View all patients (Forward Traversal)
•	View all patients (Backward Traversal)
•	Delete patient records
 Patient Information
•	Patient ID
•	Name
•	Age
•	Gender
•	Contact Number
•	Diagnosis
•	Appointment Date
•	Remarks
•	Symptoms

Pregnancy Monitoring
Track monthly pregnancy information for up to 9 months:
•	Weight
•	Blood Pressure
•	Diet Plan
•	Medical Notes
 Data Persistence
•	Save patient records to a text file
•	Load records automatically when the program starts

Data Structures Used

 1. Doubly Linked List
Used for storing patients in insertion order.
Operations:
•	Insert Patient
•	Delete Patient
•	Forward Traversal
•	Backward Traversal
 2. Binary Search Tree (BST)
Used for efficient searching of patients by ID.
Operations:
•	Insert
•	Search
•	Delete
 3. Vector
Used to store:
•	Symptoms list
•	Monthly pregnancy records

Input Validation
The system validates:
•	Name
•	Letters and spaces only
•	Age Allowed range: 12–55 years
•	Contact Number
Format:
XXXX-XXXXXXX
•	Date
Format:
YYYY-MM-DD
File Storage Format
Patient data is stored in:
patients.txt
Example:
BEGIN_PATIENT
101|Sara Khan|28|Female|0300-1234567|Anemia|2025-06-01|Normal
SYMPTOMS: Nausea, Fatigue
MONTH|1|55.5|120/80|Healthy|Balanced Diet
END_PATIENT
Menu Options
1. Add Patient
2. View All Patients (Forward)
3. View All Patients (Backward)
4. Search Patient by ID
5. Update Basic Information
6. Update Monthly Pregnancy Record
7. Delete Patient
8. Exit
Technologies Used
•	C++
•	Object-Oriented Programming
•	File Handling
•	Binary Search Tree (BST)
•	Doubly Linked List
•	Vectors
•	Standard Template Library (STL)
Future Improvements
•	Graphical User Interface (GUI)
•	Database Integration (MySQL)
•	User Authentication System
•	Appointment Scheduling
•	Medical Report Generation
•	Data Encryption and Security Features
License
This project is developed for educational and learning purposes.
