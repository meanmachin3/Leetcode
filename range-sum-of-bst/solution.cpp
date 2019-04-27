/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        if (root == NULL) {
            return 0;
        } else if (root->val > R) {
            return rangeSumBST(root->left, L , R);
        } else if (root->val < L) {
            return rangeSumBST(root->right, L , R);
        } else {
            return root->val + rangeSumBST(root->left,L,root->val) + rangeSumBST(root->right, root->val,R);
        }
    }
};
