#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <functional>
#include <cctype>

using namespace std;

static string trim(const string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos)
        return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static int readInt(const string &prompt, int defaultVal = -1)
{
    while (true)
    {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (s.empty())
            return defaultVal;
        try
        {
            size_t pos;
            int v = stoi(s, &pos);
            if (pos != s.size())
                throw invalid_argument("bad");
            return v;
        }
        catch (...)
        {
            cout << "Invalid integer — try again.\n";
        }
    }
}

static double readDouble(const string &prompt, double defaultVal = -1.0)
{
    while (true)
    {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (s.empty())
            return defaultVal;
        try
        {
            size_t pos;
            double v = stod(s, &pos);
            if (pos != s.size())
                throw invalid_argument("bad");
            return v;
        }
        catch (...)
        {
            cout << "Invalid number — try again.\n";
        }
    }
}

static string readLine(const string &prompt, const string &defaultVal = "")
{
    cout << prompt;
    string s;
    getline(cin, s);
    if (trim(s).empty())
        return defaultVal;
    return s;
}

static string readName(const string &prompt, const string &defaultVal = "")
{
    while (true)
    {
        string s = readLine(prompt, defaultVal);
        bool valid = true;
        for (char c : s)
            if (!isalpha(static_cast<unsigned char>(c)) && c != ' ')
            {
                valid = false;
                break;
            }
        if (valid && !s.empty())
            return s;
        cout << "Invalid name. Use letters and spaces only.\n";
    }
}

static int readAge(const string &prompt, int defaultVal = 0)
{
    while (true)
    {
        int v = readInt(prompt, defaultVal);
        if (v == defaultVal)
            return v;
        if (v >= 12 && v <= 55)
            return v;
        cout << "Invalid age. Pregnancy age must be between 12 and 55.\n";
    }
}

static string readContact(const string &prompt, const string &defaultVal = "")
{
    while (true)
    {
        string s = readLine(prompt, defaultVal);

        if (s.empty())
            return defaultVal;

        if (s.size() == 12 &&
            isdigit(static_cast<unsigned char>(s[0])) &&
            isdigit(static_cast<unsigned char>(s[1])) &&
            isdigit(static_cast<unsigned char>(s[2])) &&
            isdigit(static_cast<unsigned char>(s[3])) &&
            s[4] == '-' &&
            isdigit(static_cast<unsigned char>(s[5])) &&
            isdigit(static_cast<unsigned char>(s[6])) &&
            isdigit(static_cast<unsigned char>(s[7])) &&
            isdigit(static_cast<unsigned char>(s[8])) &&
            isdigit(static_cast<unsigned char>(s[9])) &&
            isdigit(static_cast<unsigned char>(s[10])) &&
            isdigit(static_cast<unsigned char>(s[11])))
        {
            return s;
        }

        cout << "Invalid contact. Use format: XXXX-XXXXXXX (e.g., 0300-7893458).\n";
    }
}

static string readDate(const string &prompt, const string &defaultVal = "")
{
    while (true)
    {
        string s = readLine(prompt, defaultVal);
        if (s.empty())
            return defaultVal;

        if (s.size() != 10)
        {
            cout << "Invalid date format. Use YYYY-MM-DD.\n";
            continue;
        }

        if (s[4] != '-' || s[7] != '-')
        {
            cout << "Invalid date format. Use YYYY-MM-DD.\n";
            continue;
        }

        bool validDigits = true;
        for (int i = 0; i < 10; i++)
        {
            if (i == 4 || i == 7)
                continue;
            if (!isdigit(static_cast<unsigned char>(s[i])))
            {
                validDigits = false;
                break;
            }
        }

        if (!validDigits)
        {
            cout << "Invalid date. Use only digits and hyphens (YYYY-MM-DD).\n";
            continue;
        }

        try
        {
            int year = stoi(s.substr(0, 4));
            int month = stoi(s.substr(5, 2));
            int day = stoi(s.substr(8, 2));

            if (month < 1 || month > 12)
            {
                cout << "Invalid month. Must be between 01 and 12.\n";
                continue;
            }

            int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                daysInMonth[1] = 29;

            if (day < 1 || day > daysInMonth[month - 1])
            {
                cout << "Invalid day for month " << month << ". Maximum " << daysInMonth[month - 1] << " days.\n";
                continue;
            }

            if (year < 2000 || year > 2100)
            {
                cout << "Invalid year. Use between 2000 and 2100.\n";
                continue;
            }

            return s;
        }
        catch (...)
        {
            cout << "Invalid date. Try again.\n";
        }
    }
}

struct MonthRecord
{
    int monthNo = 0;
    double weightKg = 0.0;
    string bloodPressure = "N/A";
    string notes = "N/A";
    string dietPlan = "N/A";
};

struct Patient
{
    int id = 0;
    string name;
    int age = 0;
    string gender;
    string contact;
    string diagnosis;
    string appointmentDate;
    string remarks;
    vector<string> symptoms;
    vector<MonthRecord> pregnancy;

    Patient *prev = nullptr;
    Patient *next = nullptr;

    Patient() = default;
    Patient(int _id, const string &_name) : id(_id), name(_name) {}
};

class PatientList
{
public:
    Patient *head = nullptr;
    Patient *tail = nullptr;

    void append(Patient *p)
    {
        if (!p)
            return;
        p->next = nullptr;
        if (!head)
        {
            p->prev = nullptr;
            head = tail = p;
        }
        else
        {
            tail->next = p;
            p->prev = tail;
            tail = p;
        }
    }

    void remove(Patient *p)
    {
        if (!p)
            return;
        if (p->prev)
            p->prev->next = p->next;
        else
            head = p->next;
        if (p->next)
            p->next->prev = p->prev;
        else
            tail = p->prev;
        p->prev = p->next = nullptr;
    }
};

struct BSTNode
{
    int id;
    Patient *patient;
    BSTNode *left = nullptr;
    BSTNode *right = nullptr;
    BSTNode(Patient *p) : id(p->id), patient(p) {}
};

class PatientBST
{
public:
    BSTNode *root = nullptr;

    void insert(Patient *p) { root = insertRec(root, p); }
    BSTNode *insertRec(BSTNode *node, Patient *p)
    {
        if (!node)
            return new BSTNode(p);
        if (p->id < node->id)
            node->left = insertRec(node->left, p);
        else if (p->id > node->id)
            node->right = insertRec(node->right, p);
        return node;
    }

    Patient *search(int id) { return searchRec(root, id); }
    Patient *searchRec(BSTNode *node, int id)
    {
        if (!node)
            return nullptr;
        if (id == node->id)
            return node->patient;
        if (id < node->id)
            return searchRec(node->left, id);
        return searchRec(node->right, id);
    }

    void remove(int id) { root = removeRec(root, id); }

private:
    BSTNode *removeRec(BSTNode *node, int id)
    {
        if (!node)
            return nullptr;
        if (id < node->id)
        {
            node->left = removeRec(node->left, id);
            return node;
        }
        if (id > node->id)
        {
            node->right = removeRec(node->right, id);
            return node;
        }
        if (!node->left)
        {
            BSTNode *r = node->right;
            delete node;
            return r;
        }
        if (!node->right)
        {
            BSTNode *l = node->left;
            delete node;
            return l;
        }
        BSTNode *succParent = node;
        BSTNode *succ = node->right;
        while (succ->left)
        {
            succParent = succ;
            succ = succ->left;
        }
        node->id = succ->id;
        node->patient = succ->patient;
        if (succParent->left == succ)
            succParent->left = removeRec(succParent->left, succ->id);
        else
            succParent->right = removeRec(succParent->right, succ->id);
        return node;
    }
};

class GyneManager
{
public:
    PatientList list;
    PatientBST bst;

    bool addPatient(int id, const string &name)
    {
        if (bst.search(id))
            return false;
        Patient *p = new Patient(id, name);
        p->pregnancy.resize(9);
        for (int i = 0; i < 9; i++)
            p->pregnancy[i].monthNo = i + 1;
        list.append(p);
        bst.insert(p);
        return true;
    }

    void listForward()
    {
        Patient *cur = list.head;
        if (!cur)
        {
            cout << "\nNo patients in the system.\n";
            return;
        }
        cout << "\n================= All Patients (Forward) =================\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(5) << "Age" << setw(12) << "Contact" << "\n";
        cout << "----------------------------------------------------------\n";
        while (cur)
        {
            cout << left << setw(5) << cur->id
                 << setw(20) << cur->name
                 << setw(5) << cur->age
                 << setw(12) << cur->contact << "\n";
            cur = cur->next;
        }
        cout << "==========================================================\n";
    }

    void listBackward()
    {
        Patient *cur = list.tail;
        if (!cur)
        {
            cout << "\nNo patients in the system.\n";
            return;
        }
        cout << "\n================= All Patients (Backward) =================\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(5) << "Age" << setw(10) << "Gender" << setw(12) << "Contact" << "\n";
        cout << "-----------------------------------------------------------\n";
        while (cur)
        {
            cout << left << setw(5) << cur->id
                 << setw(20) << cur->name
                 << setw(5) << cur->age
                 << setw(12) << cur->contact << "\n";
            cur = cur->prev;
        }
        cout << "===========================================================\n";
    }

    void printFull(Patient *p)
    {
        if (!p)
        {
            cout << "\nPatient not found.\n";
            return;
        }
        cout << "\n====================================================\n";
        cout << "                Patient Full Record                \n";
        cout << "====================================================\n";
        cout << left << setw(20) << "ID:" << p->id << "\n";
        cout << left << setw(20) << "Name:" << p->name << "\n";
        cout << left << setw(20) << "Age:" << p->age << "\n";
        cout << left << setw(20) << "Contact:" << p->contact << "\n";
        cout << left << setw(20) << "Diagnosis:" << p->diagnosis << "\n";
        cout << left << setw(20) << "Appointment:" << p->appointmentDate << "\n";
        cout << left << setw(20) << "Remarks:" << p->remarks << "\n";

        cout << left << setw(20) << "Symptoms:";
        if (p->symptoms.empty())
            cout << "None";
        else
            for (auto &s : p->symptoms)
                cout << s << "; ";
        cout << "\n----------------------------------------------------\n";
        cout << "           Monthly Pregnancy Data                  \n";
        cout << "----------------------------------------------------\n";
        cout << left << setw(7) << "Month" << setw(10) << "Weight" << setw(12) << "BP" << setw(15) << "Diet Plan" << setw(20) << "Notes" << "\n";
        cout << "----------------------------------------------------\n";
        for (auto &m : p->pregnancy)
        {
            cout << left << setw(7) << m.monthNo
                 << setw(10) << fixed << setprecision(2) << m.weightKg
                 << setw(12) << m.bloodPressure
                 << setw(15) << m.dietPlan
                 << setw(20) << m.notes << "\n";
        }
        cout << "====================================================\n";
    }

    string dropdownSelect(const string &title, const vector<string> &options)
    {
        cout << title << "\n";
        for (size_t i = 0; i < options.size(); i++)
            cout << i + 1 << ". " << options[i] << "\n";
        cout << "0. Keep current / skip\n";
        int choice = readInt("Your choice: ", 0);
        if (choice >= 1 && choice <= (int)options.size())
            return options[choice - 1];
        return "";
    }

    void updateBasic(Patient *p)
    {
        if (!p)
            return;
        p->age = readAge("Age (" + to_string(p->age) + "): ", p->age);
        p->gender = "Female";
        p->contact = readContact("Contact (" + p->contact + "): ", p->contact);

        vector<string> diagnoses = {"Anemia", "Gestational Hypertension", "Pre-eclampsia", "Gestational Diabetes", "Other"};
        string diag = dropdownSelect("Select Diagnosis:", diagnoses);
        if (!diag.empty())
        {
            if (diag == "Other")
                p->diagnosis = readLine("Enter custom diagnosis: ", p->diagnosis);
            else
                p->diagnosis = diag;
        }

        p->appointmentDate = readDate("Appointment (" + p->appointmentDate + "): ", p->appointmentDate);

        vector<string> remarksOptions = {"Normal", "High Risk", "Follow-up Needed", "Critical"};
        string rem = dropdownSelect("Select Remarks:", remarksOptions);
        if (!rem.empty())
            p->remarks = rem;

        vector<string> symptomsOptions = {"Nausea", "Vomiting", "Fatigue", "Back Pain", "Swelling", "Headache"};
        p->symptoms.clear();
        cout << "Select symptoms (type numbers separated by comma, e.g. 1,3):\n";
        for (size_t i = 0; i < symptomsOptions.size(); i++)
            cout << i + 1 << ". " << symptomsOptions[i] << "\n";
        string line = readLine("Your choices: ");
        if (!line.empty())
        {
            stringstream ss(line);
            string token;
            while (getline(ss, token, ','))
            {
                token = trim(token);
                if (token.empty())
                    continue;
                try
                {
                    size_t pos;
                    int num = stoi(token, &pos);
                    if (pos != token.size())
                        continue;
                    if (num >= 1 && num <= (int)symptomsOptions.size())
                        p->symptoms.push_back(symptomsOptions[num - 1]);
                }
                catch (...)
                {
                }
            }
        }
    }

    void updateMonthly(Patient *p, int month)
    {
        if (!p)
        {
            cout << "Patient not found.\n";
            return;
        }
        if (month < 1 || month > (int)p->pregnancy.size())
        {
            cout << "Invalid month.\n";
            return;
        }
        MonthRecord &m = p->pregnancy[month - 1];
        m.weightKg = readDouble("Weight (" + to_string(m.weightKg) + "): ", m.weightKg);
        m.bloodPressure = readLine("Blood Pressure (" + m.bloodPressure + "): ", m.bloodPressure);
        m.notes = readLine("Notes (" + m.notes + "): ", m.notes);
        m.dietPlan = readLine("Diet Plan (" + m.dietPlan + "): ", m.dietPlan);
    }

    void removePatient(int id)
    {
        Patient *p = bst.search(id);
        if (!p)
        {
            cout << "Not found.\n";
            return;
        }

        bst.remove(id);
        list.remove(p);

        delete p;
        cout << "Deleted patient.\n";
    }

    void clear()
    {

        Patient *cur = list.head;
        while (cur)
        {
            Patient *n = cur->next;
            delete cur;
            cur = n;
        }
        list.head = list.tail = nullptr;

        std::function<void(BSTNode *)> del;
        del = [&](BSTNode *node)
        {
            if (!node)
                return;
            del(node->left);
            del(node->right);
            delete node;
        };
        del(bst.root);
        bst.root = nullptr;
    }

    bool saveToFile(const string &path)
    {
        ofstream out(path);
        if (!out)
            return false;
        Patient *cur = list.head;
        while (cur)
        {
            out << "BEGIN_PATIENT\n";
            out << cur->id << '|' << cur->name << '|' << cur->age << '|' << cur->gender << '|' << cur->contact
                << '|' << cur->diagnosis << '|' << cur->appointmentDate << '|' << cur->remarks << '\n';
            out << "SYMPTOMS:";
            for (size_t i = 0; i < cur->symptoms.size(); ++i)
            {
                if (i)
                    out << ',';
                out << cur->symptoms[i];
            }
            out << '\n';
            for (auto &m : cur->pregnancy)
            {
                out << "MONTH|" << m.monthNo << '|' << m.weightKg << '|' << m.bloodPressure << '|' << m.notes << "|" << m.dietPlan << "\n";
            }
            out << "END_PATIENT\n";
            cur = cur->next;
        }
        return true;
    }

    bool loadFromFile(const string &path)
    {
        ifstream in(path);
        if (!in)
            return false;
        clear();
        string line;
        while (getline(in, line))
        {
            if (line == "BEGIN_PATIENT")
            {
                if (!getline(in, line))
                    break;
                vector<string> parts;
                {
                    string tmp;
                    stringstream ss(line);
                    while (getline(ss, tmp, '|'))
                        parts.push_back(tmp);
                }
                Patient *p = new Patient();
                if (parts.size() >= 8)
                {
                    try
                    {
                        p->id = stoi(parts[0]);
                    }
                    catch (...)
                    {
                        p->id = 0;
                    }
                    p->name = parts[1];
                    try
                    {
                        p->age = stoi(parts[2]);
                    }
                    catch (...)
                    {
                        p->age = 0;
                    }
                    p->gender = parts[3];
                    p->contact = parts[4];
                    p->diagnosis = parts[5];
                    p->appointmentDate = parts[6];
                    p->remarks = parts[7];
                }

                if (getline(in, line) && line.rfind("SYMPTOMS:", 0) == 0)
                {
                    string s = line.substr(9);
                    string tmp;
                    stringstream ss(s);
                    while (getline(ss, tmp, ','))
                    {
                        tmp = trim(tmp);
                        if (!tmp.empty())
                            p->symptoms.push_back(tmp);
                    }
                }
                p->pregnancy.clear();
                while (getline(in, line) && line != "END_PATIENT")
                {
                    if (line.rfind("MONTH|", 0) == 0)
                    {
                        vector<string> mp;
                        string tmp;
                        stringstream ss(line);
                        while (getline(ss, tmp, '|'))
                            mp.push_back(tmp);
                        if (mp.size() >= 6)
                        {
                            MonthRecord m;
                            try
                            {
                                m.monthNo = stoi(mp[1]);
                            }
                            catch (...)
                            {
                                m.monthNo = 0;
                            }
                            try
                            {
                                m.weightKg = stod(mp[2]);
                            }
                            catch (...)
                            {
                                m.weightKg = 0.0;
                            }
                            m.bloodPressure = mp[3];
                            m.notes = mp[4];
                            m.dietPlan = mp[5];
                            p->pregnancy.push_back(m);
                        }
                    }
                }
                if (p->pregnancy.empty())
                {
                    p->pregnancy.resize(9);
                    for (int i = 0; i < 9; ++i)
                        p->pregnancy[i].monthNo = i + 1;
                }
                list.append(p);
                bst.insert(p);
            }
        }
        return true;
    }
};

int main()
{
    GyneManager mgr;
    mgr.loadFromFile("patients.txt");
    while (true)
    {
        cout << "\n===== Gyne Management Menu =====\n";
        cout << "[1] Add Patient\n[2] View All (Forward)\n[3] View All (Backward)\n";
        cout << "[4] Search by ID\n[5] Update Basic Info\n[6] Update Monthly Record\n[7] Delete Patient\n[0] Exit\n";
        int opt = readInt("Choose: ", -1);

        if (opt == 1)
        {
            int id = readInt("Enter ID: ");
            string name = readName("Name: ");
            if (!mgr.addPatient(id, name))
            {
                cout << "ID already exists.\n";
                continue;
            }
            Patient *p = mgr.bst.search(id);
            mgr.updateBasic(p);
            mgr.saveToFile("patients.txt");
        }
        else if (opt == 2)
            mgr.listForward();
        else if (opt == 3)
            mgr.listBackward();
        else if (opt == 4)
        {
            int id = readInt("Enter ID: ");
            mgr.printFull(mgr.bst.search(id));
        }
        else if (opt == 5)
        {
            int id = readInt("Enter ID: ");
            mgr.updateBasic(mgr.bst.search(id));
            mgr.saveToFile("patients.txt");
        }
        else if (opt == 6)
        {
            int id = readInt("Enter ID: ");
            int m = readInt("Month (1-9): ");
            mgr.updateMonthly(mgr.bst.search(id), m);
            mgr.saveToFile("patients.txt");
        }
        else if (opt == 7)
        {
            int id = readInt("Enter ID to delete: ");
            mgr.removePatient(id);
            mgr.saveToFile("patients.txt");
        }
        else if (opt == 0)
        {
            mgr.saveToFile("patients.txt");
            break;
        }
        else
            cout << "Invalid option.\n";
    }

    return 0;
}
