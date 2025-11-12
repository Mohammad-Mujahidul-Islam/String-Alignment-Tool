#include <bits/stdc++.h>
using namespace std;

const string PASSWORD_FILE = "password.txt";
const string HISTORY_FILE  = "history.txt";

// Check Admin Password
bool checkPassword()
{
    ifstream file(PASSWORD_FILE); // Open password file
    string password;

    if (!file)
    {
        cout << "Password file missing or empty!\n";
        return false;
    }
    else
    {
        getline(file, password); // Read password from file
        file.close();
    }

    string input;
    cout << "Enter Admin Password: ";
    cin >> input;

    if (input == password)
    {
        cout << "Access Granted!\n";
        return true;
    }
    else
    {
        cout << "Access Denied. Returning to Main page.\n";
        return false;
    }
}

// Get Current Time [year-month-date hour(24h):min:sec]
string getTime()
{
    time_t now = time(0); // get current time from cpu
    tm localTime;         // For human readable components
    localtime_s(&localTime, &now);  // Windows safe version
    char temp[80];
    strftime(temp, sizeof(temp), "%Y-%m-%d %H:%M:%S", &localTime);
    return string(temp);
}

// Save Alignment History
void saveHistory(const string &X, const string &Y, int penalty, int Pgap, int Pxy, const string &alignedX, const string &alignedY)
{
    ofstream file(HISTORY_FILE, ios::app); // Append to file
    if (file.is_open())
    {
        file << "[" << getTime() << "]\n";
        file << "String X: " << X << "\n";
        file << "String Y: " << Y << "\n";
        file << "Gap Penalty: " << Pgap << "\n";
        file << "Mismatch Penalty: " << Pxy << "\n";
        file << "Minimum Penalty: " << penalty << "\n";
        file << "Aligned X: " << alignedX << "\n";
        file << "Aligned Y: " << alignedY << "\n";
        file << "-----------------------------------\n";
        file.close();
    }
}

//Show Alignment History
void showHistory()
{

    ifstream file(HISTORY_FILE);
    if (file.is_open())
    {
        string line;
        cout << "\n--- Alignment History ---\n";
        while (getline(file, line))
            cout << line << endl;
        cout << "\n--------------------------\n";
        file.close();
    }
    else
    {
        cout << "No history found.\n";
    }
}

//Clear Alignment History
void clearHistory()
{
    if (!checkPassword())
        return;

    ofstream file(HISTORY_FILE, ios::trunc); // Delete all content
    if (file.is_open())
    {
        cout << "History cleared successfully.\n";
        file.close();
    }
}

//  **Perform Global Sequence Alignment**
void performAlignment()
{
    // 1. Input sequences and penalties
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

    // 2. Create DP table (2D dynamic array)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 3. Initialize first row and column with gap penalties
    for (int i = 0; i <= m; i++)
        dp[i][0] = i * Pgap;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j * Pgap;

    // 4. Fill the DP table using Needleman-Wunsch algorithm
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            int cost;
            if (X[i - 1] == Y[j - 1])
                cost = 0;
            else
                cost = Pxy;

            // Choose minimum among match/mismatch or inserting a gap

            dp[i][j] = min({dp[i - 1][j - 1] + cost,  // diagonal (match/mismatch)
                            dp[i - 1][j] + Pgap,      // up (gap in Y)
                            dp[i][j - 1] + Pgap});    // left (gap in X)
        }
    }

    // 5. Traceback to construct aligned sequences
    int i = m, j = n;
    string alignedX = "", alignedY = "";
    while (i > 0 && j > 0)
    {
        int cost = (X[i - 1] == Y[j - 1]) ? 0 : Pxy;

        if (dp[i][j] == dp[i - 1][j - 1] + cost)
        {
            alignedX = X[i - 1] + alignedX;
            alignedY = Y[j - 1] + alignedY;
            i--;
            j--;
        }
        else if (dp[i][j] == dp[i - 1][j] + Pgap)
        {
            alignedX = X[i - 1] + alignedX;
            alignedY = "_" + alignedY;
            i--;
        }
        else
        {
            alignedX = "_" + alignedX;
            alignedY = Y[j - 1] + alignedY;
            j--;
        }
    }

    // Add remaining characters with gaps if one sequence is finished
    while (i > 0)
    {
        alignedX = X[i - 1] + alignedX;
        alignedY = "_" + alignedY;
        i--;
    }
    while (j > 0)
    {
        alignedX = "_" + alignedX;
        alignedY = Y[j - 1] + alignedY;
        j--;
    }

    // 6. Output the results
    cout << "\nMinimum Penalty: " << dp[m][n] << endl;
    cout << "Aligned X: " << alignedX << endl;
    cout << "Aligned Y: " << alignedY << endl;

    // 7. Save alignment history to file
    saveHistory(X, Y, dp[m][n], Pgap, Pxy, alignedX, alignedY);
}

// Main Menu
int main()
{
    while (true)
    {
        cout << "\n--- Main Page ---\n";
        cout << "1. Perform Alignment\n";
        cout << "2. Show History\n";
        cout << "3. Clear History\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            performAlignment();
            break;
        case 2:
            showHistory();
            break;
        case 3:
            clearHistory();
            break;
        case 4:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

