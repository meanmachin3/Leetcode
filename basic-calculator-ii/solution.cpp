class Solution {
public:
    int calculate(string s) {
        vector<int> nums;    
        char op = '+';
        int cur = 0;
        int pos = 0;
        while (pos < s.size()) {
          if (s[pos] == ' ') {
            ++pos;
            continue;
          }
          while (isdigit(s[pos]) && pos < s.size())
            cur = cur * 10 + (s[pos++] - '0');      
          if (op == '+' || op == '-') {
            nums.push_back(cur * (op == '+' ? 1 : -1));
          } else if (op == '*') {
            nums.back() *= cur;
          } else if (op == '/') {
            nums.back() /= cur;
          }
          cur = 0;      
          op = s[pos++];
        }
        return accumulate(begin(nums), end(nums), 0);
    }
};
