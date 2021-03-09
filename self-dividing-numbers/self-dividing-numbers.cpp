class Solution {
public:
    vector<int> selfDividingNumbers(int l, int r) {
        vector<int> res;
        
        for(int i = l; i <= r; i++)
            if (selfDivisible(i)) 
                res.push_back(i);
        
        return res;
    }
    
    bool selfDivisible(int i){
        int num = i;
        int lastDigit;
        while (num > 0) {
            lastDigit = num % 10;
            if (lastDigit == 0 || i % lastDigit != 0) return false;
            num /= 10;
        }
        return true;
    }
};