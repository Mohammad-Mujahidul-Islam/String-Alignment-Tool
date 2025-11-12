# String-Alignment-Tool
Developed as a project for my CSE246: Algorithm course, this DNA Sequence Alignment Tool in C++ applies the Minimum Penalty Algorithm using Dynamic Programming to align DNA sequences efficiently. It includes password protection, history logging, and a simple user interface
# 🔡 String Alignment Tool (C++)
# About
A simple yet powerful **Dynamic Programming**-based string alignment tool implemented in **C++**.  
It allows users to align two strings by minimizing the total penalty based on **gap** and **mismatch** costs. 

---

## 🚀 Features
- Aligns two strings using minimum penalty method
- Supports customizable gap and mismatch penalties
- Saves alignment history with timestamps
- Password-protected admin access
- View and clear history anytime

---

## 📁 File Structure
| File | Description |
|------|--------------|
| `Source.cpp` | Main C++ source code |
| `password.txt` | Stores admin password |
| `history.txt` | Stores alignment logs |

---

## 🧮 Example Output
````
Enter Admin Password: ****
Access Granted! 

--- Menu ---
1. Perform Alignment
2. Show History
3. Clear History
4. Exit
Enter choice: 1

Enter string X: AGTAC
Enter string Y: GTC
Enter Gap Penalty: 2
Enter Mismatch Penalty: 3

Minimum Penalty: 4
Aligned X: AGTAC
Aligned Y: -G-TC 
````
 `history.txt` 
[2025-11-12 15:43:07] Alignment saved successfully!

---
