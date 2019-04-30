class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int size = nums.size();
        
        for(int i = 0; i < size;) {
            if (nums[i] == val) {
                nums[i] = nums[size - 1];
                size--;
            } else {
                i++;
            }
        }
        return size;
    }
};
