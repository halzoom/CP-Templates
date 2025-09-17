#include<bits/stdc++.h>
using namespace std;
#define int long long

// 17
int kadane(vector<int> &arr, int &start, int &end) {
    int maxSum = INT_MIN, currSum = 0, tempStart = 0;
    start = end = 0;
    for (int i = 0; i < arr.size(); ++i) {
        currSum += arr[i];
        if (currSum < arr[i]) {
            currSum = arr[i];
            tempStart = i;
        }
        if (currSum > maxSum) {
            maxSum = currSum;
            start = tempStart;
            end = i;
        }
    }
    return maxSum;
}

int maxRectSum(vector <vector<int>> &mat) {
    int rows = mat.size(), cols = mat[0].size(), maxSum = INT_MIN;
    int finalTop = 0, finalBottom = 0, finalLeft = 0, finalRight = 0;
    vector<int> temp(rows);
    // 14
    for (int left = 0; left < cols; ++left) {
        fill(temp.begin(), temp.end(), 0);  // reset temp for each new left
        for (int right = left; right < cols; ++right) {
            for (int row = 0; row < rows; ++row) temp[row] += mat[row][right];
            int startRow, endRow;
            int sum = kadane(temp, startRow, endRow);
            if (sum > maxSum) {
                maxSum = sum;
                finalTop = startRow;
                finalBottom = endRow;
                finalLeft = left;
                finalRight = right;
            }
        }
    }
    cout << "(Top, Left)     : (" << finalTop << ", " << finalLeft << ")\n";
    cout << "(Bottom, Right) : (" << finalBottom << ", " << finalRight << ")\n";
    cout << "Maximum Sum     : " << maxSum << endl;
    return maxSum;
}
// vector<vector<int>> mat = {{1, 2, -1, -4, -20}, {-8, -3, 4, 2, 1}, {3, 8, 10, 1, 3}};
// maxRectSum(mat);
