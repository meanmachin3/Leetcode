class Solution {
public:
    int numJewelsInStones(string J, string S) {
        int result = 0;
        for(char& stone : S) {
            if (J.find(stone) != std::string::npos) {
                result++;
            }
        }
        return result;
    }
};
