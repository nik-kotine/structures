#include <iostream>
#include <vector>
using namespace std;

template <typename X>
struct Node {

	size_t l, r;
	X value;
	Node* left = nullptr;
	Node* right = nullptr;
	X lazy = 0;
	bool can_update = false;

public:

	Node(size_t _l, size_t _r) :
		l(_l),
		r(_r)
	{}

	~Node() = default;

};

template <typename X>
class SegmentTree {

	vector<Node<X>*> versions;

	size_t n_vers = 0;

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
			return node->value + (node->r - node->l + 1)*(acc_lazy);

		X left_sum = _recursive_range_sum(node->left, l, r, acc_lazy);
		X right_sum = _recursive_range_sum(node->right, l, r, acc_lazy);

		return left_sum + right_sum;

	}

	Node<X>* _lazy_propagate(Node<X>* node, Node<X>* new_node, X add) {
		
		new_node->value =
			node->value + (node->r - node->l + 1)*(node->lazy+add);

		if (node->left == nullptr) {
			new_node->lazy = 0;
			return new_node;
		}

		size_t q = (node->l + node->r)/2;
		Node<X>* new_left = new Node<X>(node->l, q);
		Node<X>* new_right = new Node<X>(q+1, node->r);

		new_left->value = node->left->value;
		new_left->lazy = node->lazy + add;
		new_left->left = node->left->left;
		new_left->right = node->left->right;
		new_left->can_update = true;

		new_right->value = node->right->value;
		new_right->lazy = node->lazy + add;
		new_right->left = node->right->left;
		new_right->right = node->right->right;
		new_right->can_update = true;

		new_node->left = new_left;
		new_node->right = new_right;

		new_node->lazy = 0;
		return new_node;

	}

	Node<X>* _recursive_range_add(Node<X>* node, size_t l, size_t r, X add) {
		
		cout << '(' << node->value << "): called\n";
		if (node->r < l || r < node->l) {
			node->can_update = false;
			cout << '(' << node->value << "): completely outside; done\n";
			return node;
		}

		Node<X>* new_node;
		if (node->can_update == true) {
			new_node = node;
			cout << '(' << node->value << "): can_update was true\n";
		} else {
			new_node = new Node<X>(node->l, node->r);
			cout << '(' << node->value << "): can_update was false, created new_node\n";
		}

		cout << '(' << node->value << "): " << l << ':' << node->l << ':' << node->r << ':' << r << '\n';

		if (l <= node->l && node->r <= r) {
			cout << '(' << node->value << "): completely inside; update and propagate\n";
			new_node = _lazy_propagate(node, new_node, add);
			new_node->can_update = false;
			if (new_node->left != nullptr) {
				new_node->left->can_update = false;
				new_node->right->can_update = false;
			}
			cout << '(' << node->value << "): done\n";
			return new_node;
		}

		if (node->lazy != 0) {
			cout << '(' << node->value << "): must propagate, has children\n";
			new_node = _lazy_propagate(node, new_node, add);
			new_node->left = _recursive_range_add(new_node->left, l, r, add);
			new_node->right = _recursive_range_add(new_node->right, l, r, add);
			new_node->can_update = false;
			cout << '(' << node->value << "): done\n";
			return new_node;
		}

		cout << '(' << node->value << "): partial, calling children\n";
		new_node->left = _recursive_range_add(node->left, l, r, add);
		new_node->right = _recursive_range_add(node->right, l, r, add);
		new_node->value = new_node->left->value + new_node->right->value;
		new_node->left->can_update = false;
		new_node->right->can_update = false;
		cout << '(' << node->value << "): done\n";
		return new_node;

	}

public:

	void construct(const vector<X>& arr) {

		if (arr.empty())
			return;

		Node<X>* root = _recursive_construct(arr, 0, arr.size()-1);
		versions.push_back(root);

	}

	void print(size_t ver) {
		_recursive_print(versions[ver]);
	}

	X range_sum(size_t ver, size_t l, size_t r) {
		return _recursive_range_sum(versions[ver], l, r);
	}

	void range_add(size_t ver, size_t l, size_t r, X add) {
		Node<X>* new_root = _recursive_range_add(versions[ver], l, r, add);
		versions.push_back(new_root);
		++n_vers;
	}
	
};

int main() {
	 
	SegmentTree<int> s;

	s.construct({3, 1, 4, 1, 5, 9, 2, 6});

	s.print(0);

	s.range_add(0, 0, 3, 10);

	s.print(0);

	s.print(1);

	s.range_add(1, 1, 4, 5);

	s.print(0);

	s.print(1);

	s.print(2);

	s.range_add(1, 0, 7, 1);

	s.print(3);

	s.range_add(2, 0, 7, 1);

	s.print(4);

}
