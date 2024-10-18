#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to check if a degree sequence is graphical using Havel-Hakimi Algorithm
bool havelHakimi(vector<int>& degrees) {
    while (true) {
        // Remove zeros from the sequence
        degrees.erase(remove(degrees.begin(), degrees.end(), 0), degrees.end());
        
        // If all degrees are zero, the sequence is graphical
        if (degrees.empty()) return true;

        // Sort the sequence in non-increasing order
        sort(degrees.begin(), degrees.end(), greater<int>());

        // Take the largest element (degree) and remove it
        int d = degrees[0];
        degrees.erase(degrees.begin());

        // If the largest degree is greater than the remaining number of nodes, it's not graphical
        if (d > degrees.size()) return false;

        // Subtract 1 from the next d degrees
        for (int i = 0; i < d; i++) {
            degrees[i]--;
            // If degree becomes negative, it's not graphical
            if (degrees[i] < 0) return false;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of vertices: ";
    cin >> n;

    vector<int> degrees(n);
    cout << "Enter the degree sequence: ";
    for (int i = 0; i < n; i++) {
        cin >> degrees[i];
    }

    if (havelHakimi(degrees)) {
        cout << "The degree sequence is graphical.\n";
        // Drawing the graph would require graphical libraries like SFML or OpenGL
    } else {
        cout << "The degree sequence is not graphical.\n";
    }

    return 0;
}