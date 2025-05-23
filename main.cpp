#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> cities;
vector<vector<int>> roadMatrix;
vector<vector<float>> budgetMatrix;

// Helper: find city index
int getCityIndex(const string& name) {
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i] == name) return i;
    }
    return -1;
}

// Save cities to file
void saveCitiesToFile() {
    ofstream cityFile("cities.txt");
    cityFile << "Index  City_Name\n";
    for (int i = 0; i < cities.size(); ++i) {
        cityFile << (i + 1) << "      " << cities[i] << "\n";
    }
    cityFile.close();
}

// Save roads and budgets to file
void saveRoadsToFile() {
    ofstream roadFile("roads.txt");
    roadFile << "ID, Road, Budget(billion)\n";
    int id = 1;
    for (int i = 0; i < cities.size(); ++i) {
        for (int j = i + 1; j < cities.size(); ++j) {
            if (roadMatrix[i][j]) {
                roadFile << id++ << ", " << cities[i] << "-" << cities[j] << ", " << budgetMatrix[i][j] << "\n";
            }
        }
    }
    roadFile.close();
}

void addCities() {
    int n;
    cout << "Enter the number of cities to add: ";
    while (!(cin >> n) || n <= 0) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "You must enter a positive number. Try again: ";
    }
    cin.ignore(); // Clear newline character from input buffer

    for (int i = 0; i < n; ++i) {
        cout << "Enter the name for city " << (cities.size() + 1) << ": ";
        string city;
        getline(cin, city);

        if (getCityIndex(city) != -1) {
            cout << "City \"" << city << "\" already exists. Skipping...\n";
            continue;
        }

        cities.push_back(city);

        for (auto& row : roadMatrix) row.push_back(0);
        roadMatrix.push_back(vector<int>(cities.size(), 0));

        for (auto& row : budgetMatrix) row.push_back(0);
        budgetMatrix.push_back(vector<float>(cities.size(), 0));
    }

    saveCitiesToFile();
}

void addRoad() {
    string city1, city2;
    cin.ignore();
    cout << "Enter the name of the first city: ";
    getline(cin, city1);
    cout << "Enter the name of the second city: ";
    getline(cin, city2);

    int i = getCityIndex(city1);
    int j = getCityIndex(city2);

    if (i == -1 || j == -1 || i == j) {
        cout << "Invalid cities.\n";
        return;
    }

    if (roadMatrix[i][j] == 1) {
        cout << "Road already exists between these cities.\n";
        return;
    }

    roadMatrix[i][j] = roadMatrix[j][i] = 1;
    cout << "Road added between " << city1 << " and " << city2 << ".\n";
}

void addBudget() {
    string city1, city2;
    float budget;
    cin.ignore();
    cout << "Enter the name of the first city: ";
    getline(cin, city1);
    cout << "Enter the name of the second city: ";
    getline(cin, city2);

    int i = getCityIndex(city1);
    int j = getCityIndex(city2);

    if (i == -1 || j == -1 || i == j || roadMatrix[i][j] == 0) {
        cout << "Invalid cities or no road exists.\n";
        return;
    }

    cout << "Enter the budget for the road (billion): ";
    while (!(cin >> budget) || budget < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a non-negative numeric budget: ";
    }

    budgetMatrix[i][j] = budgetMatrix[j][i] = budget;
    saveRoadsToFile();

    cout << "Budget added for the road between " << city1 << " and " << city2 << ".\n";
}

void editCity() {
    int index;
    string newName;
    cout << "Enter the index number of the city to edit: ";
    while (!(cin >> index) || index < 1 || index > cities.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << " Invalid index. Enter a valid city index: ";
    }
    cin.ignore();

    cout << "Enter the new name for the city: ";
    getline(cin, newName);

    if (getCityIndex(newName) != -1) {
        cout << "City name already exists.\n";
        return;
    }

    cities[index - 1] = newName;
    saveCitiesToFile();
    saveRoadsToFile();

    cout << "City updated successfully.\n";
}

void searchCity() {
    int index;
    cout << "Enter the index of the city to search: ";
    while (!(cin >> index) || index < 1 || index > cities.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid index. Enter a valid city index: ";
    }

    cout << " City at index " << index << " is: " << cities[index - 1] << "\n";
}

void displayCities() {
    cout << "Cities:\n";
    for (int i = 0; i < cities.size(); ++i) {
        cout << (i + 1) << ": " << cities[i] << "\n";
    }
}

void displayRoadsMatrix() {
    displayCities();
    cout << "Roads Adjacency Matrix:\n   ";
    for (const auto& city : cities) {
        cout << setw(6) << city.substr(0, 3);
    }
    cout << "\n";

    for (int i = 0; i < roadMatrix.size(); ++i) {
        cout << setw(3) << cities[i].substr(0, 3);
        for (int j = 0; j < roadMatrix[i].size(); ++j) {
            cout << setw(6) << roadMatrix[i][j];
        }
        cout << "\n";
    }
}

void displayAll() {
    displayCities();

    cout << "\nRoads Matrix:\n";
    displayRoadsMatrix();

    cout << "\nBudget Matrix (in billion):\n   ";
    for (const auto& city : cities) {
        cout << setw(6) << city.substr(0, 3);
    }
    cout << "\n";
    for (int i = 0; i < budgetMatrix.size(); ++i) {
        cout << setw(3) << cities[i].substr(0, 3);
        for (int j = 0; j < budgetMatrix[i].size(); ++j) {
            cout << setw(6) << budgetMatrix[i][j];
        }
        cout << "\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\nMenu:\n"
             << "1. Add new cit(y/ies)\n"
             << "2. Add roads between cities\n"
             << "3. Add the budget for roads\n"
             << "4. Edit city\n"
             << "5. Search for a city using its index\n"
             << "6. Display cities\n"
             << "7. Display roads matrix\n"
             << "8. Display recorded data\n"
             << "9. Exit\n"
             << "Enter your choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 9) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Enter a number between 1 and 9: ";
        }

        switch (choice) {
            case 1: addCities(); break;
            case 2: addRoad(); break;
            case 3: addBudget(); break;
            case 4: editCity(); break;
            case 5: searchCity(); break;
            case 6: displayCities(); break;
            case 7: displayRoadsMatrix(); break;
            case 8: displayAll(); break;
            case 9: cout << "Exiting...\n"; break;
        }
    } while (choice != 9);

    return 0;
}
