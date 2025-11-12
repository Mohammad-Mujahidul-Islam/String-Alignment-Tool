#include <bits/stdc++.h>
using namespace std;

const string PASSWORD_FILE = "password.txt";
const string HISTORY_FILE  = "history.txt";

// ------------------- Utility Functions -------------------

string readPasswordFromFile() {
    ifstream file(PASSWORD_FILE);
    string password;
    if (file.is_open()) {
        getline(file, password);
        file.close();
    }
    return password;
}

string getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &currentTime); // Windows secure version
#else
    localtime_r(&currentTime, &localTime); // Linux/Unix
#endif
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return string(buffer);
}

void saveHistory(const string &X, const string &Y, int penalty, const string &alignedX, const string &alignedY) {
    ofstream file(HISTORY_FILE, ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentDateTime() << "]\n";  // Add timestamp
        file << "String X: " << X << "\n";
        file << "String Y: " << Y << "\n";
        file << "Minimum Penalty: " << penalty << "\n";
        file << "Aligned X: " << alignedX << "\n";
        file << "Aligned Y: " << alignedY << "\n";
        file << "-----------------------------------\n";
        file.close();
    }
}

void showHistory() {
    
    if (!checkPassword())
        return;
    ifstream file(HISTORY_FILE);
    if (file.is_open()) {
        string line;
        cout << "\n--- Alignment History ---\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        cout << "--------------------------\n";
        file.close();
    } else {
        cout << "No history found.\n";
    }
}

void clearHistory() {
    ofstream file(HISTORY_FILE, ios::trunc);
    if (file.is_open()) {
        cout << "History cleared successfully.\n";
        file.close();
    }
}

// ------------------- Menu Option Functions -------------------

void performAlignment() {
    string X, Y;
    int Pgap, Pxy;
    cout << "Enter string X: ";
    cin >> X;
    cout << "Enter string Y: ";
    cin >> Y;
    cout << "Enter Gap Penalty: ";
    cin >> Pgap;
    cout << "Enter Mismatch Penalty: ";
    cin >> Pxy;

    int m = X.size();
    int n = Y.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++)
        dp[i][0] = i * Pgap;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j * Pgap;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (X[i - 1] == Y[j - 1]) ? 0 : Pxy;
            dp[i][j] = min({
                dp[i - 1][j - 1] + cost,
                dp[i - 1][j] + Pgap,
                dp[i][j - 1] + Pgap
            });
        }
    }

    int i = m, j = n;
    string alignedX = "", alignedY = "";
    while (i > 0 && j > 0) {
        int cost = (X[i - 1] == Y[j - 1]) ? 0 : Pxy;
        if (dp[i][j] == dp[i - 1][j - 1] + cost) {
            alignedX = X[i - 1] + alignedX;
            alignedY = Y[j - 1] + alignedY;
            i--; j--;
        } else if (dp[i][j] == dp[i - 1][j] + Pgap) {
            alignedX = X[i - 1] + alignedX;
            alignedY = "-" + alignedY;
            i--;
        } else {
            alignedX = "-" + alignedX;
            alignedY = Y[j - 1] + alignedY;
            j--;
        }
    }
    while (i > 0) {
        alignedX = X[i - 1] + alignedX;
        alignedY = "-" + alignedY;
        i--;
    }
    while (j > 0) {
        alignedX = "-" + alignedX;
        alignedY = Y[j - 1] + alignedY;
        j--;
    }

    cout << "\nMinimum Penalty: " << dp[m][n] << endl;
    cout << "Aligned X: " << alignedX << endl;
    cout << "Aligned Y: " << alignedY << endl;

    saveHistory(X, Y, dp[m][n], alignedX, alignedY);
}

bool checkPassword() {
    string savedPassword = readPasswordFromFile();
    if (savedPassword.empty()) {
        cout << "Password file missing or empty!\n";
        return false;
    }

    string inputPassword;
    cout << "Enter Admin Password: ";
    cin >> inputPassword;

    if (inputPassword == savedPassword) {
        cout << "Access Granted!\n";
        return true;
    } else {
        cout << "Access Denied. Exiting program.\n";
        return false;
    }
}

// ------------------- Main -------------------

int main() {

    while (true) {
        cout << "\n--- Menu ---\n";
        cout << "1. Perform Alignment\n";
        cout << "2. Show History\n";
        cout << "3. Clear History\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: performAlignment(); break;
            case 2: showHistory(); break;
            case 3: clearHistory(); break;
            case 4: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

