#include <vector>

struct tree {
int x;
struct tree *l;
struct tree *r;
};

int solution(tree *T){
	vector<int> past;
	vector<int> *d = help(T, &past);
	return d->size();
}

vector<int> *help(tree *T, vector<int> *past){
if (!T) return past;
else {
	if (T->l == NULL && T->r == NULL) {
		if (past->find(T->x)) return past;
		else {
			past->push_back(T->x);
			return past;
		}
	}
	else {
		int flag = 1;
		for (int i = 0; i < past->size(); i++) {
			if (T->x == past[i]) {
				flag = 0; 
				break;
			}
		}
		
		if (flag == 1) past->push_back(T->x);
		
		vector<int> v1(*past);
		vector<int> v2(*past);
		vector<int> *right = help(T->r, &v1);
		vector<int> *left = help(T->l, &v2);

		int right_count = 0;
		int left_count = 0;
		
		for (int i = 0; i < left->size(); i++) {
			if (past->find(left[i])) {
				continue;
			}
			else {
				left_count++;
			}			
		}
		
		for (int i = 0; i < right->size(); i++) {
			if (past->find(right[i])) {
				continue;
			}
			else {
				right_count++;
			}			
		}
			
		if (left_count > right_count) {
			for (int i = 0; i < left->size(); i++) {
				if (past->find(left[i])) {
					continue;
				}
				else {
					past->push_back(left[i]);
				}
		    }
		}
		else {
			for (int i = 0; i < right->size(); i++) {
				if (past->find(right[i])) {
					continue;
				}
				else {
					past->push_back(right[i]);
				}
		    } 
		}
		return past;
	}
}