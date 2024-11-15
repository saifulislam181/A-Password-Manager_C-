#include <iostream>
#include <fstream>
#include <string>
#include <cctype> // For isdigit() and isalpha()
using namespace std;

// Encryption and Decryption Functions
string encrypt(string data) {
    char key = 'K'; // Simple key for XOR
    for (int i = 0; i < data.size(); i++) {
        data[i] = data[i] ^ key; // XOR operation for each character
    }
    return data;
}

string decrypt(string data) {
    return encrypt(data); // XORing again will decrypt it
}

// Helper Functions for Validations
bool isValidEmail(const string& email) {
    return email.find('@') != string::npos; // Check if '@' is in the email
}

bool isValidPassword(const string& password) {
    if (password.length() < 8) return false; // Check length

    bool hasNumber = false, hasLetter = false;

    for (char ch : password) {
        if (isdigit(ch)) hasNumber = true; // Check if there's a number
        if (isalpha(ch)) hasLetter = true; // Check if there's a letter
    }

    return hasNumber && hasLetter; // Valid if it has both a number and a letter
}

// User Class
class User {
public:
    string username;
    string password;

    bool login(string uname, string pwd);
    void registerUser(string uname, string pwd);
    void editPassword(string newPassword);
    void deleteAccount();
};

// Admin Class (inherits User)
class Admin : public User {
public:
    void viewAllPasswords();
    bool loginAsAdmin(string uname, string pwd);
};

// PasswordManager Class
class PasswordManager {
private:
    string type;
    User currentUser;
    Admin admin;

public:
    void selectAccessType();
    void showUserOptions();
    void showAdminOptions();
    void performUserAction(int option);
    void performAdminAction(int option);
};

// Function to Save User Data to a File
void saveUserToFile(User user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.username << " " << encrypt(user.password) << endl; // Encrypt password before saving
        file.close();
    } else {
        cout << "Unable to open file for writing." << endl;
    }
}

// Function to Load User from a File
User loadUserFromFile(string username) {
    ifstream file("users.txt");
    User user;
    string uname, pwd;
    if (file.is_open()) {
        while (file >> uname >> pwd) {
            if (uname == username) {
                user.username = uname;
                user.password = decrypt(pwd); // Decrypt password after reading
                break;
            }
        }
        file.close();
    }
    return user;
}

// Function to Update User in a File
void updateUserInFile(User user) {
    ifstream file("users.txt");
    ofstream tempFile("temp.txt");
    string uname, pwd;

    if (file.is_open() && tempFile.is_open()) {
        while (file >> uname >> pwd) {
            if (uname == user.username) {
                tempFile << user.username << " " << encrypt(user.password) << endl;
            } else {
                tempFile << uname << " " << pwd << endl;
            }
        }
        file.close();
        tempFile.close();
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
}

// Function to Delete User from a File
void deleteUserFromFile(string username) {
    ifstream file("users.txt");
    ofstream tempFile("temp.txt");
    string uname, pwd;

    if (file.is_open() && tempFile.is_open()) {
        while (file >> uname >> pwd) {
            if (uname != username) {
                tempFile << uname << " " << pwd << endl;
            }
        }
        file.close();
        tempFile.close();
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
}

// User Class Method Implementations

bool User::login(string uname, string pwd) {
    if (!isValidEmail(uname)) {
        cout << "Error: Username must be a valid email address." << endl;
        return false;
    }
    if (!isValidPassword(pwd)) {
        cout << "Error: Password must be at least 8 characters long and contain at least one number and one letter." << endl;
        return false;
    }

    User user = loadUserFromFile(uname);
    if (user.username == uname && user.password == pwd) {
        cout << "Login successful." << endl;
        return true;
    } else {
        cout << "Invalid username or password." << endl;
        return false;
    }
}

void User::registerUser(string uname, string pwd) {
    if (!isValidEmail(uname)) {
        cout << "Error: Username must be a valid email address." << endl;
        return;
    }
    if (!isValidPassword(pwd)) {
        cout << "Error: Password must be at least 8 characters long and contain at least one number and one letter." << endl;
        return;
    }

    this->username = uname;
    this->password = pwd;
    saveUserToFile(*this);
    cout << "Registration successful." << endl;
}

void User::editPassword(string newPassword) {
    if (!isValidPassword(newPassword)) {
        cout << "Error: Password must be at least 8 characters long and contain at least one number and one letter." << endl;
        return;
    }
    this->password = newPassword;
    updateUserInFile(*this);
    cout << "Password updated successfully." << endl;
}

void User::deleteAccount() {
    deleteUserFromFile(this->username);
    cout << "Account deleted successfully." << endl;
}

// Admin Class Method Implementations
bool Admin::loginAsAdmin(string uname, string pwd) {
    if (uname == "admin" && pwd == "admin123") { // Simple hardcoded admin login
        cout << "Admin login successful." << endl;
        return true;
    }
    cout << "Invalid admin credentials." << endl;
    return false;
}

void Admin::viewAllPasswords() {
    ifstream file("users.txt");
    string uname, pwd;

    if (file.is_open()) {
        cout << "All User Accounts:\n";
        while (file >> uname >> pwd) {
            cout << "Username: " << uname << ", Password: " << decrypt(pwd) << endl; // Decrypt password for display
        }
        file.close();
    } else {
        cout << "Unable to open file." << endl;
    }
}

// PasswordManager Class Method Implementations
void PasswordManager::selectAccessType() {
    cout << "Select Access Type:\n1. Desktop Application\n2. Website\n3. Game\n";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1: type = "Desktop Application"; break;
        case 2: type = "Website"; break;
        case 3: type = "Game"; break;
        default: cout << "Invalid choice."; return;
    }

    cout << "1. User Options\n2. Admin Options\n";
    cin >> choice;

    if (choice == 1) {
        showUserOptions();
    } else if (choice == 2) {
        showAdminOptions();
    } else {
        cout << "Invalid choice." << endl;
    }
}

void PasswordManager::showUserOptions() {
    cout << "User Options:\n1. Login\n2. Register\n3. Edit Password\n4. Delete Account\n";
    int option;
    cin >> option;
    performUserAction(option);
}

void PasswordManager::showAdminOptions() {
    cout << "Admin Options:\n1. Login\n2. Edit Password\n3. Delete Account\n4. View All Passwords\n";
    int option;
    cin >> option;
    performAdminAction(option);
}

void PasswordManager::performUserAction(int option) {
    string uname, pwd, newPwd;
    switch (option) {
        case 1:
            cout << "Enter username and password: ";
            cin >> uname >> pwd;
            currentUser.login(uname, pwd);
            break;
        case 2:
            cout << "Enter new username and password: ";
            cin >> uname >> pwd;
            currentUser.registerUser(uname, pwd);
            break;
        case 3:
            cout << "Enter new password: ";
            cin >> newPwd;
            currentUser.editPassword(newPwd);
            break;
        case 4:
            currentUser.deleteAccount();
            break;
        default:
            cout << "Invalid option." << endl;
    }
}

void PasswordManager::performAdminAction(int option) {
    string uname, pwd, newPwd;
    switch (option) {
        case 1:
            cout << "Enter admin username and password: ";
            cin >> uname >> pwd;
            admin.loginAsAdmin(uname, pwd);
            break;
        case 2:
            cout << "Enter new admin password: ";
            cin >> newPwd;
            admin.editPassword(newPwd);
            break;
        case 3:
            admin.deleteAccount();
            break;
        case 4:
            admin.viewAllPasswords();
            break;
        default:
            cout << "Invalid option." << endl;
    }
}

int main() {
    PasswordManager manager;
    manager.selectAccessType();
    return 0;
}
