#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

// Patient class
class Hospital {
private:
    string Name, Address, BGroup, AdmissionDate;

public:
    Hospital() : Name(""), Address(""), BGroup(""), AdmissionDate("") {}
    Hospital(string n, string a, string b, string d) : Name(n), Address(a), BGroup(b), AdmissionDate(d) {}

    void setName(const string &name) { Name = name; }
    void setAddress(const string &address) { Address = address; }
    void setBGroup(const string &group) { BGroup = group; }
    void setAdmissionDate(const string &date) { AdmissionDate = date; }

    string getName() const { return Name; }
    string getAddress() const { return Address; }
    string getBGroup() const { return BGroup; }
    string getAdmissionDate() const { return AdmissionDate; }

    string toCSV() const {
        return Name + "," + Address + "," + BGroup + "," + AdmissionDate;
    }
};

// Cross-platform sleep
void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

// Get current date
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
}

// Backup file
void backupFile() {
    ifstream src("Hospital.txt", ios::binary);
    ofstream dest("Hospital_backup.txt", ios::binary);
    if (src && dest) dest << src.rdbuf();
}

// Load patients
vector<Hospital> loadPatients() {
    vector<Hospital> patients;
    ifstream infile("Hospital.txt");
    if (!infile) return patients;

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, address, bgroup, date;
        getline(ss, name, ',');
        getline(ss, address, ',');
        getline(ss, bgroup, ',');
        getline(ss, date, ',');
        patients.push_back(Hospital(name, address, bgroup, date));
    }
    infile.close();
    return patients;
}

// Save patients
void savePatients(const vector<Hospital>& patients) {
    ofstream outfile("Hospital.txt");
    for (const auto& p : patients) outfile << p.toCSV() << endl;
}

// Admin login
bool login() {
    string username, password;
    cout << "\n\t=== HOSPITAL MANAGEMENT LOGIN ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    if (username == "mahendra" && password == "1234") {
        cout << "Login Successful!\n";
        sleep_ms(1000);
        return true;
    }
    cout << "Invalid credentials! Exiting...\n";
    sleep_ms(1500);
    return false;
}

// Admit patient
void admitPatient(vector<Hospital>& patients) {
    string name, address, group;

    cout << "\n--- Admit New Patient ---\n";
    cin.ignore(); // Clear buffer
    cout << "Name: ";
    getline(cin, name);
    cout << "Address: ";
    getline(cin, address);
    cout << "Blood Group: ";
    getline(cin, group);

    string date = getCurrentDate();
    patients.push_back(Hospital(name, address, group, date));
    savePatients(patients);
    cout << "\nPatient admitted successfully on " << date << "!\n";
    cout << "Press Enter to continue...";
    cin.get();
}

// Discharge patient
void dischargePatient(vector<Hospital>& patients) {
    string name;
    cout << "\n--- Discharge Patient ---\n";
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);

    bool found = false;
    backupFile();

    for (auto it = patients.begin(); it != patients.end(); ++it) {
        if (it->getName() == name) {
            patients.erase(it);
            found = true;
            savePatients(patients);
            cout << "\nPatient discharged successfully!\n";
            cin.get();
            return;
        }
    }
    if (!found) {
        cout << "\nPatient not found!\n";
        cin.get();
    }
}

// Show all patients
void showAllPatients(const vector<Hospital>& patients) {
    cout << "\n--- All Admitted Patients ---\n";
    if (patients.empty()) cout << "No patients currently admitted.\n";
    else {
        for (const auto& p : patients) {
            cout << "Name: " << p.getName()
                 << " | Address: " << p.getAddress()
                 << " | Blood Group: " << p.getBGroup()
                 << " | Admission Date: " << p.getAdmissionDate() << endl;
        }
    }
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

// Search patient
void searchPatient(const vector<Hospital>& patients) {
    int option;
    cout << "\n--- Search Patient ---\n";
    cout << "1. By Name\n2. By Blood Group\nChoice: ";
    cin >> option;
    cin.ignore();

    if (option == 1) {
        string name;
        cout << "Enter Name: ";
        getline(cin, name);
        bool found = false;
        for (const auto& p : patients) {
            if (p.getName() == name) {
                cout << "Found: " << p.getName() << " | " << p.getAddress()
                     << " | " << p.getBGroup() << " | " << p.getAdmissionDate() << endl;
                found = true;
            }
        }
        if (!found) cout << "Patient not found!\n";
    } else if (option == 2) {
        string group;
        cout << "Enter Blood Group: ";
        getline(cin, group);
        bool found = false;
        for (const auto& p : patients) {
            if (p.getBGroup() == group) {
                cout << p.getName() << " | " << p.getAddress()
                     << " | " << p.getBGroup() << " | " << p.getAdmissionDate() << endl;
                found = true;
            }
        }
        if (!found) cout << "No patients with this blood group!\n";
    } else {
        cout << "Invalid choice!\n";
    }
    cout << "Press Enter to continue...";
    cin.get();
}

// Main menu
int main() {
    if (!login()) return 0;

    vector<Hospital> patients = loadPatients();
    int choice;
    bool exitProgram = false;

    cout << "\n\t=== HOSPITAL MANAGEMENT SYSTEM ===\n";

    while (!exitProgram) {
        cout << "\n---------------------------\n";
        cout << "Menu:\n";
        cout << "1. Admit Patient\n2. Discharge Patient\n3. Show All Patients\n";
        cout << "4. Search Patient\n5. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: admitPatient(patients); break;
            case 2: dischargePatient(patients); break;
            case 3: showAllPatients(patients); break;
            case 4: searchPatient(patients); break;
            case 5: exitProgram = true; break;
            default: cout << "Invalid Choice!\n";
        }
    }

    cout << "\nThank you for using the system!\n";
    return 0;
}
