#include <iostream>
#include <unordered_map>
#include <vector>
#define ll long long
using namespace std;

const int MOD = 1e9;

template <typename X>
struct Node {

	X value;
	Node* left = nullptr;
	Node* right = nullptr;

};

template <typename X>
class PersistentSegmentTree {

public:

	vector<Node<X>*> versions;
	size_t sz;

private:

	Node<X>* _recursive_construct(const vector<X>& arr, size_t l, size_t r) {

		Node<X>* node = new Node<X>();

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
		cout << temp << node->value << '\n';

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

	Node<X>* _recursive_update(
		Node<X>* node, size_t l, size_t r, size_t i, X value, bool p
	) {

		Node<X>* new_node = new Node<X>();

		if (l == r) {
			if (p == true) {
				cout << node->value << '\n';
			}
			new_node->value = value;
			return new_node;
		}

		size_t q = (l+r)/2;

		if (i <= q) {
			new_node->left = _recursive_update(
				node->left, l, q, i, value, p
			);
			new_node->right = node->right;
		} else {
			new_node->left = node->left;
			new_node->right = _recursive_update(
				node->right, q+1, r, i, value, p
			);
		}

		new_node->value = new_node->left->value + new_node->right->value;

		return new_node;
			
	}

	Node<X>* _recursive_add(
		Node<X>* node, size_t l, size_t r, size_t i, X value, bool p
	) {

		Node<X>* new_node = new Node<X>();

		if (l == r) {
			if (p == true) {
				cout << node->value << '\n';
			}
			new_node->value = node->value + value;
			return new_node;
		}

		size_t q = (l+r)/2;

		if (i <= q) {
			new_node->left = _recursive_add(
				node->left, l, q, i, value, p
			);
			new_node->right = node->right;
		} else {
			new_node->left = node->left;
			new_node->right = _recursive_add(
				node->right, q+1, r, i, value, p
			);
		}

		new_node->value = new_node->left->value + new_node->right->value;

		return new_node;
			
	}

	size_t _recursive_kth_smallest(
		size_t k, Node<X>* latter, Node<X>* former, size_t l, size_t r
	) {
		if (l == r)
			return l;

		X left_half_freq = latter->left->value - former->left->value;
		size_t q = (l+r)/2;

		if (left_half_freq >= k) {
			return _recursive_kth_smallest(
				k, latter->left, former->left, l, q
			);
		} else {
			return _recursive_kth_smallest(
				k - left_half_freq, latter->right, former->right, q+1, r
			);
		}

	}

	size_t _recursive_bigger_than_k(
		size_t k, Node<X>* latter, Node<X>* former, size_t l, size_t r
	) {
		
		if (l == r)
			return (l <= k) ? 0 : latter->value - former->value;

		size_t q = (l+r)/2;

		if (k <= q) {
			size_t right_half = latter->right->value - former->right->value;
			return right_half + _recursive_bigger_than_k(
				k, latter->left, former->left, l, q
			);
		} else {
			return _recursive_bigger_than_k(
				k, latter->right, former->right, q+1, r
			);
		}

	}

	X _recursive_range_sum(
		Node<X>* node, size_t l, size_t r, size_t L, size_t R
	) {

		if (r < L || R < l)
			return 0;

		if (L <= l && r <= R)
			return node->value;

		size_t q = (l+r)/2;

		X x1 = _recursive_range_sum(node->left, l, q, L, R);
		X x2 = _recursive_range_sum(node->right, q+1, r, L, R);
		return x1 + x2;

	}

public:

	void construct(const vector<X>& arr) {

		if (arr.empty())
			return;

		Node<X>* root = _recursive_construct(arr, 0, arr.size()-1);
		sz = arr.size();
		versions.push_back(root);

	}

	void print(size_t ver) {
		_recursive_print(versions[ver]);
	}

	void update(size_t ver, size_t i, X value, bool p = false) {
		Node<X>* new_root = _recursive_update(
			versions[ver], 0, sz-1, i, value, p
		);
		versions.push_back(new_root);
	}

	void add(size_t ver, size_t i, X value, bool p = false) {
		Node<X>* new_root = _recursive_add(
			versions[ver], 0, sz-1, i, value, p
		);
		versions.push_back(new_root);
	}


	size_t kth_smallest(size_t l, size_t r, size_t k) {
		return _recursive_kth_smallest(
			k, versions[r], versions[l-1], 0, sz-1
		);
	}

	size_t bigger_than_k(size_t l, size_t r, size_t k) {
		return _recursive_bigger_than_k(
			k, versions[r], versions[l-1], 0, sz-1
		);
	}

	X range_sum(size_t ver, size_t l, size_t r) {
		return _recursive_range_sum(versions[ver], 0, sz-1, l-1, r-1);
	}
	
};


int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
	int n; cin >> n;
	vector<int> arr(n, 0);
	for (int i = 0; i < n; ++i)
		cin >> arr[i];

	PersistentSegmentTree<int> tree;
	tree.construct(vector<int>(n, 1));

	unordered_map<int, int> map;
	for (int i = 0; i < n; ++i) {
		if (map.find(arr[i]) != map.end()) {
			tree.update(i, map[arr[i]], 0);
		}
		else {
			tree.update(i, i, 1);
		}
		map[arr[i]] = i;
	}

	int q; cin >> q;
	while (q--) {
		int l, r; cin >> l >> r;
		cout << tree.range_sum(r, l, r) << '\n';
	}


}
