/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        return (root == nullptr || helper(root->left, root->right));
    }
    
    bool helper(TreeNode* left, TreeNode* right) {
        if (left == nullptr || right == nullptr) {
            return left == right;
        }else if (left->val != right->val) {
            return false;
        } else {
            return helper(left->left, right->right) && helper(left->right, right->left);
        }
        
    }
};
