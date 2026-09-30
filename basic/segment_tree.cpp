#include <iostream>
#include <vector>
using namespace std;

template <typename X>
struct Node {

	size_t l, r;
	X value;
	Node* left = nullptr;
	Node* right = nullptr;
	X lazy;

public:

	Node(size_t _l, size_t _r) :
		l(_l),
		r(_r)
	{}

	~Node() = default;

};

template <typename X>
class SegmentTree {

	Node<X>* root = nullptr;

	Node<X>* _recursive_construct(const vector<X>& arr, size_t l, size_t r) {

		Node<X>* node = new Node<X>(l, r);

		if (l == r) {
			node->value = arr[l];
			return node;
		}

		size_t q = (l+r)/2;
		Node<X>* left = _recursive_construct(arr, l, q);
		Node<X>* right = _recursive_construct(arr, q+1, r);

		node->left = left;
		node->right = right;
		node->value = left->value + right->value;
		return node;

	}

	void _lazy_propagate(Node<X>* node) {

		node->value += node->lazy * (node->r - node->l + 1);

		if (node->left != nullptr)
			node->left->lazy += node->lazy;

		if (node->right != nullptr)
			node->right->lazy += node->lazy;

		node->lazy = 0;

	}

	void _recursive_print(Node<X>* node, size_t indent = 0) {
		
		if (node == nullptr)
			return;

		string temp (2*indent, ' ');
		cout << temp << node->value << '(' << node->lazy << ')' << '\n';

		_recursive_print(node->left, indent + 1);
		_recursive_print(node->right, indent + 1);

	}

	void _recursive_delete(Node<X>* node) {

		if (node == nullptr)
			return;
		
		_recursive_delete(node->left);
		_recursive_delete(node->right);

		delete node;

	}

	X _recursive_range_sum(Node<X>* node, size_t l, size_t r, X acc_lazy = 0) {

		if (node->r < l || r < node->l)
			return 0;

		acc_lazy += node->lazy;

		if (l <= node->l && node->r <= r)
			return node->value + (node->r - node->l + 1)*acc_lazy;

		X left_sum = _recursive_range_sum(node->left, l, r, acc_lazy);
		X right_sum = _recursive_range_sum(node->right, l, r, acc_lazy);

		return left_sum + right_sum;

	}

	void _recursive_add(Node<X>* node, size_t l, size_t r, X value) {

		_lazy_propagate(node);

		if (r < node->l || node->r < l)
			return;

		if (l <= node->l && node->r <= r) {
			node->lazy = value;
			_lazy_propagate(node);
			return;
		}

		_recursive_add(node->left, l, r, value);
		_recursive_add(node->right, l, r, value);

		node->value = node->left->value + node->right->value;

	}

public:

	~SegmentTree<X>() {
		_recursive_delete(root);
	}

	void construct(const vector<X>& arr) {

		if (arr.empty())
			return;

		root = _recursive_construct(arr, 0, arr.size()-1);

	}

	void print() {
		_recursive_print(root);
	}

	X range_sum(size_t l, size_t r) {
		return _recursive_range_sum(root, l, r);
	}

	void add(size_t l, size_t r, X value) {
		_recursive_add(root, l, r, value);
	}
	
};

int main() {
	 
	SegmentTree<int> s;

	s.construct({3, 1, 4, 1, 5, 9, 2});

	s.add(2, 5, 10);

	s.add(0, 2, 5);

	cout << s.range_sum(0, 6) << '\n';

	s.print();

}
