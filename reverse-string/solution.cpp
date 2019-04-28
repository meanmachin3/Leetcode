class Solution {
public:
    void reverseString(vector<char>& s) {
        int left = 0, right = s.size() - 1;
        while (left < right) {
            swap(s, left++, right--);
        }
    }
    
    void swap(vector<char>& s, int from , int to) {
        char temp = s[from];
        s[from] = s[to];
        s[to] = temp;
    }
};
