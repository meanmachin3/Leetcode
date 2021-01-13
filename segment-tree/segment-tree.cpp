// See https://codeforces.com/blog/entry/44478#comment-290116
// This (mostly untested) implementation is for educational purposes only
// and suffers from several drawbacks. Most notably, segment ends have to
// be stored in every operation (and every node), even though they can be
// calculated 'on the fly'.

#include <vector>

using namespace std;

// Dummy operation when lazy propagation is not required.
template<typename T>
struct NoOp {
    int L, R;
    T applyAggregate(const T& old) const { return old; }
    void compose(const NoOp<T>&) { }
};

template<typename T, typename F>
struct Node {
    T concat{};
    F op{};

    void init(int L, int R) {
        op.L = L;
        op.R = R;
    }

    T getConcat() const {
        return op.applyAggrgate(concat);
    }

    void push(Node<T, F>& leftChild, Node<T, F>& rightChild) {
        leftChild.op.compose(op);
        rightChild.op.compose(op);
        concat = getConcat();
        F oldOp = std::move(op);
        op = F{};
        init(oldOp.L, oldOp.R);
    }
};

template<typename T, typename F=NoOp<T>>
class SegmentTree {
public:
    explicit SegmentTree(int size)
        : n(size)
        , nodes(4*size)
    {
        initImpl(0, n-1, 0);
    }

    T get(int i) {
        return concat(i, i);
    }

    void set(int i, const T& val) {
        setImpl(i, val, 0);
    }

    T concat(int left, int right) {
        return concatImpl(left, right, 0);
    }

    void apply(const F& op) {
        applyImpl(op, 0);
    }

private:
    int n;
    vector<Node<T, F>> nodes;

    static int getLeft(int node) {
        return 2*node + 1;
    }

    static int getRight(int node) {
        return 2*node + 2;
    }

    void push(int node) {
        nodes[node].push(nodes[getLeft(node)], nodes[getRight(node)]);
    }

    void recalc(int node) {
        nodes[node].concat = nodes[getLeft(node)].getConcat() + nodes[getRight(node)].getConcat();
    }

    void initImpl(int L, int R, int node) {
        nodes[node].init(L, R);
        if (L < R) {
            int M = L + (R - L) / 2;
            initImpl(L, M, getLeft(node));
            initImpl(M+1, R, getRight(node));
        }
    }

    void setImpl(int i, const T& val, int node) {
        int L = nodes[node].op.L, R = nodes[node].op.R;
        if (i < L || i > R)
            return;
        if (L == R) {
            nodes[node].concat = val;
            return;
        }

        push(node);
        setImpl(i, val, getLeft(node));
        setImpl(i, val, getRight(node));
        recalc(node);
    }

    T concatImpl(int left, int right, int node) {
        int L = nodes[node].op.L, R = nodes[node].op.R;
        if (right < L || left > R)
            return T{};
        if (left <= L && R <= right)
            return nodes[node].getConcat();
        push(node);
        return concatImpl(left, right, getLeft(node)) +
               concatImpl(left, right, getRight(node));
    }

    void applyImpl(const F& op, int node) {
        int L = nodes[node].op.L, R = nodes[node].op.R;
        if (op.R < L || op.L > R)
            return;
        if (op.L <= L && R <= op.R) {
            nodes[node].op.compose(op);
            return;
        }

        push(node);
        applyImpl(op, getLeft(node));
        applyImpl(op, getRight(node));
        recalc(node);
    }
};


struct F1 {      // Represents aggregate operation
    int L, R;    // applied at sub-array a[L..R]
    int add = 0; // add this to all elements

    int applyAggregate(int oldAggregate) const {
        return oldAggregate + add * (R - L + 1);  // add to each of R-L+1 elements
    }

    void compose(const F1& parent) {  // compose in-place
        add += parent.add;
    }
};

struct F2 {     // Represents aggregate operation
    int L, R;   // applied at sub-array a[L..R]
    int v = -1; // change all elements to v; by convention, v == -1 means no change

    int applyAggregate(int oldAggregate) const {
        if (v == -1) return oldAggregate; // no change
        return v * (R - L + 1);           // each of R-L+1 elements is set to v
    }

    void compose(const F2& parent) {  // compose in-place
        // if parent is changed, we change to the same value too; otherwise stay the same
        if (parent.v != -1) {
            v = parent.v;
        }
    }
};

struct F3 {           // Represents aggregate operation
    int L, R;         // applied at sub-array a[L..R]
    int v = -1;       // First, change all elements to v; by convention, v == -1 means no change
    int c = 1;        // Second, multiply all elements by c
    int k = 0, a = 0; // Third, add a to the first element, a+k to the second, a+2k to the third etc

    int applyAggregate(int oldAggregate) const {
        int result = oldAggregate;

        // replace
        if (v != -1)
            result = v * (R - L + 1);

        // multiply
        result *= c;

        // add progression
        result += (R-L+1) * (2*a + (R-L)*k) / 2; // sum of progression

        return result;
    }

    void compose(const F3& parent) {  // compose in-place
        // replace
        if (parent.v != -1) {
            v = parent.v;
            c = 1;
            k = a = 0;
        }

        // multiply
        c *= parent.c;
        k *= parent.c;
        a *= parent.c;

        // add progression
        int newA = parent.a + parent.k * (L - parent.L); // the progression restricted to [L,R] starts with this number
        k += parent.k;
        a += newA;
    }
};

#include <iostream>

const int n = 11;

template<typename Tree>
void print(Tree& t) {
    cout << t.concat(0, 4) << " " << t.concat(1, 2) << " " << t.concat(5, n-1) << endl;
#if 1
    cout << "[";
    for (int i = 0; i < n; ++i)
        cout << " " << t.get(i);
    cout << " ]" << endl;
#endif
    cout << endl;
}

int main() {
    {
        SegmentTree<int> tree(n);
        for (int i = 0; i < n; ++i)
            tree.set(i, i);
        print(tree);
    }
    {
        SegmentTree<int, F1> tree(n);
        for (int i = 0; i < n; ++i)
            tree.set(i, i);
        print(tree);

        tree.apply({0, n-1, 1});
        print(tree);

        tree.apply({1, 3, 1});
        print(tree);
    }

    {
        SegmentTree<int, F3> tree(n);
        tree.apply({0, n-1, -1, 1, 1, 1});
        print(tree);

        tree.apply({2, 4,   42, 1, 0, 0});
        print(tree);

        tree.apply({5, 8,   -1, 2, 0, 0});
        print(tree);
    }
}e
