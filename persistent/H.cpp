#include <algorithm>
#include <iostream>
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

	size_t kth_smallest(size_t l, size_t r, size_t k, size_t n) {
		return _recursive_kth_smallest(k, versions[r], versions[l-1], 0, n-1);
	}

	size_t bigger_than_k(size_t l, size_t r, size_t k, size_t n) {
		return _recursive_bigger_than_k(k, versions[r], versions[l-1], 0, n-1);
	}
	
};


int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
	size_t n; cin >> n;
	vector<size_t> arr(n, 0);
	for (int i = 0; i < n; ++i)
		cin >> arr[i];
	int q, Q; cin >> q;
	Q = q;

	vector<size_t> copy = arr;
	sort(copy.begin(), copy.end());
	copy.erase(unique(copy.begin(), copy.end()), copy.end());
	for (int i = 0; i < n; ++i)
		arr[i] = lower_bound(copy.begin(), copy.end(), arr[i]) - copy.begin();

	PersistentSegmentTree<size_t> tree;
	tree.construct(vector<size_t>(copy.size(), 0));

	for (int i = 0; i < n; ++i)
		tree.add(i, arr[i], 1);

	size_t last_ans = 0;

	while (q--) {
		size_t i, j, k;
		cin >> i >> j >> k;
		i = min(i ^ last_ans, n);
		j = min(j ^ last_ans, n);
		k = k ^ last_ans;
		k = upper_bound(copy.begin(), copy.end(), k) - copy.begin();
		
		if (i == 0 && j == 0) {
			last_ans = 0;
		}
		else if (k == 0) {
			last_ans = j - i + 1;
		} else {
			last_ans = tree.bigger_than_k(i, j, k-1, copy.size());
		}
		cout << last_ans << '\n';
	}

}
