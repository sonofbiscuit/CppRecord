﻿#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <stack>
#include <queue>
#include <array>
#include <deque>
#include <time.h>
#include "StlStringChange.h"
#include <numeric>
#include <functional>


/* #pragma GCC optimize(2) //O2优化
*/
using namespace std;
using namespace strtool;

using PII = pair<int, int>;


//[captures] (params) mutable-> return-type{...} //lambda 表达式的完整形式


/*
string s1 = "hiya";    // 拷贝初始化
string s2("hello");    // 直接初始化
string s3(10, 'c');    // 直接初始化
*/
//多用直接初始化，少用拷贝初始化

//循环时，前自增大于后自增，前自增返回的是自增后的自己，后自增返回的是自增前自己的副本(临时变量)


template <typename Dtype>
unsigned int __builtin_popcount(Dtype u)
{
	u = (u & 0x55555555) + ((u >> 1) & 0x55555555);
	u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
	u = (u & 0x0F0F0F0F) + ((u >> 4) & 0x0F0F0F0F);
	u = (u & 0x00FF00FF) + ((u >> 8) & 0x00FF00FF);
	u = (u & 0x0000FFFF) + ((u >> 16) & 0x0000FFFF);
	return u;
}


//=======================================================并查集=====================================
class UnionFindOp {
public:
	vector<int> roots;
	vector<int> size;//根节点下的全部的结点数
	//int size;

	UnionFindOp(int n) {
		roots.resize(n);
		size.resize(n, 1);
		for (int i = 0; i < n; i++) {
			roots[i] = i;
		}
	};

	int find(int i) { // 使用路径压缩
		return i == roots[i] ? i : (roots[i] = find(roots[i]));
	}

	void union_element(int i, int j) {//按秩合并
		int x = find(i);
		int y = find(j);
		if (x != y) {
			if (size[x] > size[y]) {
				roots[y] = x;
				size[x] += size[y];
			}
			else {
				roots[x] = y;
				size[y] += size[x];
			}
		}
	}
};


//=========================================字典树/前缀树=============================================
class TrieOp {
private:
	bool isEnd;
	TrieOp* next[26];
public:
	/** Initialize your data structure here. */
	TrieOp() {
		isEnd = false;
		memset(next, 0, sizeof(next));
	}

	/** Inserts a word into the trie. */
	void insert(string word) {
		TrieOp* node = this;
		for (auto a : word) {
			if (node->next[a - 'a'] == NULL) {
				node->next[a - 'a'] = new TrieOp();
			}
			node = node->next[a - 'a'];
		}
		node->isEnd = true;
	}

	/** Returns if the word is in the trie. */
	bool search(string word) {
		TrieOp* node = this;
		for (auto a : word) {
			if (node->next[a - 'a'] == NULL) {
				return false;
			}
			node = node->next[a - 'a'];
		}
		return node->isEnd;
	}

	/** Returns if there is any word in the trie that starts with the given prefix. */
	bool startsWith(string prefix) {
		TrieOp* node = this;
		for (auto a : prefix) {
			if (node->next[a - 'a'] == NULL) {
				return false;
			}
			node = node->next[a - 'a'];
		}
		return true;
	}
};


string reorganizeString(string S) {
	int word_maxcount = 0;
	char word_index = ' ';
	int length = int(S.size());
	vector<int> m2(26, 0);
	for (int i = 0; i < length; i++) {
		m2[S[i]-'a']++;
		word_maxcount = max(word_maxcount, m2[S[i] - 'a']);
		word_index = word_maxcount == m2[S[i] - 'a']?S[i]:word_index;
	}
	if (word_maxcount > (S.size() + 1) / 2)
		return "";
	
	vector<char> ans(length,0);
	int index = 0;
	for (int i = word_maxcount; i > 0; i--) { //排序最多的字母
		ans[index] = word_index;
		m2[word_index - 'a']--;  //更新m2中存储的信息
		index += 2;
	}
	for (int j = 0; j < 26; j++) {
		while (m2[j]-- > 0) {
			if (index >= length)
				index = 1;
			ans[index] = char('a' + j);
			index += 2;
		}
	}
	for (int i = 0; i < ans.size(); i++) {
		cout << ans[i] << endl;
	}

	string ansbuf;
	return ansbuf.assign(ans.begin(), ans.end());
}


vector<int> searchRange(vector<int>& nums, int target) {
	vector<int> ans;
	int n = nums.size();
	int low = 0;
	int high = n - 1;
	int mid = (low+high) / 2;
	int count = 0;
	while (low <= high) {
		mid = (low + high) / 2;
		if (nums[mid] < target) {
			low = mid+1;
		}
		else if (nums[mid] > target) {
			high = mid - 1;
		}
		else { //找到target,得判断数目,此时nums[mid]=target
			count++;
			int opleft = 1;
			int opright = 1;
			while (mid - opleft >= 0 && nums[mid - opleft] == target) {
				count++;
				opleft++;
			}
			ans.push_back(mid - opleft + 1);
			while (mid + opright < n && nums[mid + opright] == target) {
				count++;
				opright++;
			}
			ans.push_back(mid + opright - 1);
			for (auto c : ans) {
				cout << c << endl;
			}
			return ans;
		}
	}
	ans.push_back(-1);
	ans.push_back(-1);
	return ans;
}


int countPrimes(int n) {
	int count = 0;
	vector<int> ans(n, 0);
	for (int i = 2; i < n; i++) {
		ans[i] = 1;
	}
	for (int j = 2; j < n; j++) {
		if (ans[j] == 1) { //是素数
			count++;
			for (int m = j * 2; m < n; m+=j) {
				ans[m] = 0;
			}
		}
	}
	cout << count << endl;
	return count;
}


int leastInterval(vector<char>& tasks, int n) {
	if (tasks.empty()) {
		return 0;
	}
	vector<int> task_tag(26);
	for (auto temp : tasks) {
		task_tag[temp - 'A']++;//优点调度记录
	}

	sort(task_tag.begin(), task_tag.end());//优先度排序
	
	int total_size = (n + 1) * (task_tag[25] - 1);//忽略最后一行
	for (int i = 0; i < 26; i++) {
		total_size -= min(task_tag[i], task_tag[25] - 1);
	}
	//if()
	//cout << tasks.size() + total_size << endl;
	return total_size > 0 ? tasks.size() + total_size : tasks.size();
}

vector<vector<int>> generate(int numRows) {
	vector<vector<int>> ans(numRows);
	for (int i = 0; i < numRows; i++) {
		ans[i].resize(i + 1);
		ans[i][0] = ans[i][i] = 1;
		for (int j = 1; j < i; j++) {
			ans[i][j] = ans[i - 1][j - 1] + ans[i - 1][j];
		}
	}
	for (int i = 0; i < ans.size(); i++) {
		for (int j = 0; j < ans[i].size(); j++) {
			cout << ans[i][j];
		}
		cout << endl;
	}
	return ans;
}


int matrixScore(vector<vector<int>>& A) {
	if (A.size() == 0) {
		return 0;
	}
	
	for (int i = 0; i < A.size();i++) {
		if (A[i][0] != 1) {
			for (int j = 0; j < A[i].size(); j++) {
				A[i][j] = !A[i][j];
			}
		}
	}

	for (int i = 0; i < A[0].size(); i++) {
		int count_0 = 0;
		int count_1 = 0;
		for (int j = 0; j < A.size(); j++) {
			if (A[j][i] == 0) {
				count_0 += 1;
			}
			else {
				count_1 += 1;
			}
		}
		if (count_0 > count_1) { //列变换
			for (int k = 0; k < A.size();k++) {
				A[k][i] = !A[k][i];
			}
		}
	}
	int ans = 0;
	for (int i = A.size() - 1; i >= 0; i--) {
		for (int j = A[i].size() - 1; j >= 0; j--) {
			ans += A[i][j] * pow(2, (A[i].size() - j - 1));
			//cout << A[i][j] * pow(2, (A[i].size() - j - 1)) << ' ';
		}
		//cout << endl;
	}
	//cout << ans << endl;
	return ans;
}


bool backtrack(vector<int>& vec, string s, int index, int length, long long sum, int prev) {
	if (index == length) { //一条路结束
		return vec.size() >= 3;
	}
	long long temp = 0;
	for (int i = index; i < length; i++) {
		if (i > index&&s[index] == '0') {
			break;
		}
		temp = temp * 10 + s[i] - '0'; //123456579
		if (temp > INT_MAX) {
			break;
		}
		if (vec.size() >= 2) {
			if (temp > sum) {
				break;
			}
			else if (temp < sum) {
				continue;
			}
		}
		vec.push_back(temp);
		if (backtrack(vec, s, i + 1, length, temp + prev, temp)) { // 后移一
			return true;
		}
		vec.pop_back();
	}
	return false;
}

vector<int> splitIntoFibonacci(string S) {
	vector<int> ans;
	backtrack(ans, S, 0, S.length(), 0, 0);
	for (auto a : ans) {
		cout << a << endl;
	}
	return ans;
}


int wiggleMaxLength(vector<int>& nums) {
	if (nums.size() < 2) {
		return nums.size();
	}
	int prenum = nums[1] - nums[0];
	int count = prenum != 0 ? 2 : 1;
	for (int i = 2; i < nums.size(); i++) {
		if ((nums[i] - nums[i - 1] > 0 && prenum <= 0) || (nums[i] - nums[i - 1] < 0 && prenum>=0)) {
			prenum = nums[i] - nums[i - 1];
			count++;
		}
	}
	cout << count << endl;
	return count;
}




bool quicksort(vector<int>& vec, int low, int high) {
	if (low >= high) {
		return false;
	}
	swap(vec[low], vec[low + rand() % (high - low + 1)]);  // 随机化版本的快排
	int pivot = vec[low], i = low, j = high;
	while (i < j) {
		while (i < j && vec[j] >= pivot) {
			if (vec[j] != pivot)
				j--;
			else
				return true;
		}
		vec[i] = vec[j];
		while (i < j && vec[i] <= pivot) {
			if (vec[i] != pivot)
				i++;
			else
				return true;
		}
		vec[j] = vec[i];
	}
	vec[i] = pivot;
	return quicksort(vec, low, i - 1) || quicksort(vec, i + 1, high);
}

bool containsDuplicate(vector<int>& nums) {
	return quicksort(nums, 0, nums.size()-1);
}

vector<vector<string>> groupAnagrams(vector<string>& strs) {
	map<string, vector<string>> cmp;
	vector<vector<string>> ans;
	for (const auto a : strs) {
		auto temp = a;
		sort(temp.begin(), temp.end());
		cmp[temp].emplace_back(a);
	}

	for (const auto a : cmp) {
		ans.emplace_back(a.second);
	}
	return ans;
}


int monotoneIncreasingDigits(int N) {
	string num = to_string(N);
	int idx = 0, pre = -1;
	for (int i = 0; i < num.size() - 1; i++) {
		if (pre < num[i]) {
			idx = i;
			pre = num[i];
		}
		if (num[i] > num[i + 1]) {
			num[idx]--;
			for (int j = idx + 1; j < num.size(); j++) {
				num[j] = '9';
			}
			break;
		}
	}
	for (int i = 0; i < num.size(); i++) {
		cout << num[i] << endl;
	}
	int ans = atoi(num.c_str());
	cout << ans;
	return ans;
}

//双向映射
bool wordPattern(string pattern, string s) {
	vector<string> str_s;
	strtool::split(s, str_s, " ");
	int n = (int)pattern.size();
	map<char, set<string>> pat1;
	map<string, set<char>> pat2;
	
	for (int i = 0; i < n; i++) {
		pat1[pattern[i]].insert(str_s[i]);
		pat2[str_s[i]].insert(pattern[i]);
	}
	for (int j = 0; j < n; j++) {
		if (pat1[pattern[j]].size() > 1 || pat2[str_s[j]].size() > 1) {
			return false;
		}
	}
	return true;
}


char findTheDifference(string s, string t) {
	int temp = 0;
	for (auto a : s)
		temp ^= a; //
	for (auto b : t)
		temp ^= b;
	cout << temp;
	return 0;
}

void rotate(vector<vector<int>>& matrix) {
	int n = matrix.size();
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			int temp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n / 2; j++) {
			int temp = matrix[i][j];
			matrix[i][j] = matrix[i][n - 1 - j];
			matrix[i][n - 1 - j] = temp;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}


int maximumUniqueSubarray(vector<int>& nums) {
	if (nums.size() == 0)
		return 0;
	unordered_set<int> lookup;
	int left = 0;
	int sum = 0;
	int maxsum = 0;
	for (int i = 0; i < nums.size(); i++) {
		while (lookup.find(nums[i]) != lookup.end()) {//找到重复
			sum -= nums[left];
			lookup.erase(nums[left]);//删左
			left++;
		}
		sum += nums[i];
		maxsum = max(maxsum, sum);
		lookup.insert(nums[i]);
		
	}
	cout<< maxsum;
	return maxsum;
}


string reformatNumber(string number) {
	string temp = "";
	string ans_str = "";
	for (auto a : number) {
		if (a == ' ' || a == '-') {
			continue;
		}
		temp.push_back(a);
	}
	if (temp.size() < 4)
		return temp;
	else if (temp.size() == 4) {
		temp.insert(temp.begin() + 2, '-');
		return temp;
	}
	else {
		for (int i = 3; i < temp.size(); i+=3) {
			temp.insert(temp.begin() + i, '-');
			i += 1;
			if (temp.size() - i == 4) {
				temp.insert(temp.begin() + i + 2, '-');
				break;
			}
		}
		
	}
	for (auto a : temp) {
		cout << a;
	}
	return ans_str;
}


int candy(vector<int>& ratings) {
	int n = ratings.size();
	vector<int> left(n);
	for (int i = 0; i < n; i++) {
		if (i > 0 && ratings[i] > ratings[i - 1]) {
			left[i] = left[i - 1] + 1;
		}
		else {
			left[i] = 1;
		}
	}
	int right = 0, ret = 0;
	for (int i = n - 1; i >= 0; i--) {
		if (i < n - 1 && ratings[i] > ratings[i + 1]) {
			right++;
		}
		else {
			right = 1;
		}
		ret += max(left[i], right);
	}
	return ret;
}



int maximalRectangle(vector<vector<char>>& matrix) {
	int row = matrix.size();
	int col = matrix[0].size();
	vector<vector<int>> tag(row, vector<int>(col, 0));
	return 0;
}





bool canPlaceFlowers(vector<int>& flowerbed, int n) {
	//no push_front on vector but we can insert before begin()；
	flowerbed.insert(flowerbed.begin(), 0);
	flowerbed.emplace_back(0);
	int count = n;
	for (int i = 1; i < flowerbed.size() - 1; i++) {
		if (flowerbed[i - 1] == 0 && flowerbed[i + 1] == 0 && flowerbed[i] == 0) {
			flowerbed[i] = 1;
			count--;
		}
	}
	cout << count << endl;
	return count == 0 ? true : false;
}




vector<int> maxSlidingWindow(vector<int>& nums, int k) {
	if (k > nums.size() || nums.size() == 0) {
		return nums;
	}
	vector<int> ans;
	priority_queue<pair<int, int>> que; // pair内存储<value,index>
	for (int i = 0; i < k; i++) {
		que.emplace(nums[i], i);
	}
	ans.emplace_back(que.top().first);
	
	for (int j = k; j < nums.size(); j++) {
		que.emplace(nums[j], j);
		while (que.top().second < j - k+1) {
			que.pop(); // remove the top element
			
		}
		ans.push_back(que.top().first);
	}

	for (auto a : ans) {
		cout << a << endl;
	}
	return ans;
}




int maximumUnits(vector<vector<int>>& boxTypes, int truckSize) {
	int count = 0;
	int weight = 0;
	for (int i = 0; i < boxTypes.size(); i++) {
		for (int j = i; j < boxTypes.size(); j++) {
			if (boxTypes[i][1] < boxTypes[j][1]) {
				swap(boxTypes[i], boxTypes[j]);
			}
		}
	}

	for (int k = 0; k < boxTypes.size(); k++) {
		if (count + boxTypes[k][0] <= truckSize) {
			count += boxTypes[k][0];
			weight += boxTypes[k][0] * boxTypes[k][1];
		}
		else {
			weight += (truckSize - count)*boxTypes[k][1];
			//cout << weight;
			return weight;
		}
	}
	//cout << weight;
	return weight;
}


//找出vector中有多少个两个数的和是2的幂
int countPairs(vector<int>& deliciousness) {
	int mod = 1e9 + 7;
	int ans = 0;
	unordered_map<int, int> num;
	for (auto a : deliciousness) {
		for (int i = 0; i < 22; i++) {
			if (num.count((1 << i) - a)) {//找到了
				ans = ans + num[(1 << i) - a];
			}
		}
		num[a]++;
	}
	return ans;
}



//将数组分为三个子数组，前两个
int waysToSplit(vector<int>& nums) {
	const int mod = 1e9 + 7;
	int n = nums.size();
	long long count = 0;
	vector<int> presum(n, 0);
	presum[0] = nums[0];
	for (int i = 1; i < n; i++) {//前缀和
		presum[i] = presum[i-1] + nums[i];
	}

	for (int i = 0; i < n; i++) {
		if (presum[i] * 3 > presum[n - 1]) {
			break;
		}
		int left = i+1, right = n - 2;
		while (left <= right) {//二分找左边界
			int mid = (left + right) / 2;
			if (presum[mid] - presum[i] < presum[i]) {
				left = mid + 1;
			}
			else {
				right = mid - 1;
			}
		}

		int left_2 = i+1, right_2 = n - 2;
		while (left_2 <= right_2) { //找右边界
			int mid = (left_2 + right_2) / 2;
			if (presum[n-1] - presum[mid] < presum[mid] - presum[i]) {
				right_2 = mid - 1;
			}
			else {
				left_2 = mid + 1;
			}
		}
		cout << left << " " << right << " " << left_2 << " " << right_2 << endl;
		int temp = right_2 - left + 1;
		count += temp;
	}

	cout << count << endl;
	return count%mod;
}



vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
	unordered_map<string, int> rec_word;
	int count = 0;
	int n = equations.size();
	for (int i = 0; i < n; i++) {
		if (rec_word.find(equations[i][0]) == rec_word.end()) {//没找到的话，就记录并且赋予编号
			rec_word[equations[i][0]] = count++;
		}
		if (rec_word.find(equations[i][1]) == rec_word.end()) {
			rec_word[equations[i][1]] = count++;
			//将字符串用整数替代掉，（感觉直接写string也可以）
		}
	}

	vector<vector<pair<int, double>>> matrix(count);
	//save the weight creat weight-matrix
	for (int i = 0; i < n; i++) {
		int a = rec_word[equations[i][0]],
			b = rec_word[equations[i][1]];
		matrix[a].emplace_back(make_pair(b, values[i]));
		matrix[b].emplace_back(make_pair(a, 1.00 / values[i]));
	}

	vector<double> ans;
	for (int j = 0; j < queries.size(); j++) {
		double default_answer = -1.000;
		if (rec_word.find(queries[j][0]) != rec_word.end() && rec_word.find(queries[j][1]) != rec_word.end()){
			int a = rec_word[queries[j][0]], b = rec_word[queries[j][1]];
			if (a == b) {
				default_answer = 1.00;
			}
			else {
				vector<double> ratio(count, -1.00);
				queue<int> que;
				que.push(a);
				ratio[a] = 1.00;
				while (!que.empty() && ratio[b] < 0) { //b<0说明b与a无直接关联
					int temp = que.front();
					que.pop();

					for (auto node : matrix[temp]) {//找b
						if (ratio[node.first] < 0) {
							ratio[node.first] = ratio[temp] * node.second;
							que.push(node.first);
						}
					}
				}
				default_answer = ratio[b];
			}
		}
		ans.emplace_back(default_answer);
	}
	for (auto m : ans) {
		cout << m << endl;
	}
	return ans;
}

void dfs(vector<vector<int>> connected, vector<int>& is_visited, int i) {
	for (int j = 0; j < connected[i].size(); j++) {
		if (connected[i][j] == 1 && is_visited[j] != 1) {
			is_visited[j] = 1;
			dfs(connected, is_visited, j);
		}
	}
}

int findCircleNum(vector<vector<int>>& isConnected) {//leetcode547，找相邻省份
	int n = isConnected.size();
	int count = 0;
	vector<int> is_visited(n, 0);
	for (int i = 0; i < n; i++) {
		if (is_visited[i] != 1) {
			is_visited[i] = 1;
			dfs(isConnected, is_visited, i);
			count++;
		}
		
	}
	cout << count << endl;
	return count;
}



int maximumGain(string s, int x, int y) {
	int ans = 0;
	if (x >= y) {
		while (s.find_first_of("ab") != std::string::npos) {
			ans += x;
			std::size_t found = s.find_first_of("ab");
			s.erase(found, found + 2);
			
		}
		while (s.find_first_of("ba") != std::string::npos) {
			ans += y;
			std::size_t found1 = s.find_first_of("ba");
			s.erase(found1, found1 + 2);
			
		}
	}
	else {
		while (s.find_first_of("ba") != std::string::npos) {
			ans += y;
			std::size_t found1 = s.find_first_of("ba");
			s.erase(found1, found1 + 2);
			
		}
		while (s.find_first_of("ab") != std::string::npos) {
			ans += x;
			std::size_t found = s.find_first_of("ab");
			s.erase(found, found + 2);
			
		}		
	}
	for (auto a : s) {
		cout << a << endl;
	}
	cout << ans << endl;
	return ans;
}

//===========================================并查集================================================================

int minimumHammingDistance(vector<int>& source, vector<int>& target, vector<vector<int>>& allowedSwaps) {
	int n = source.size();
	UnionFindOp un(n);
	for (auto& a : allowedSwaps) {
		un.union_element(a[0], a[1]); //下标进行并查集操作初始化
	}
	unordered_map<int, vector<int>> groups;
	for (int i = 0; i < n; i++) { //下标最大为n-1
		groups[un.find(i)].emplace_back(i); //插入根节点所有的子节点
	}
	
	int ans = 0;
	for (int i = 0; i < groups.size();i++) {
		unordered_map<int, int> source_map;
		unordered_map<int, int> target_map;
		for (auto k : groups[i]) { //groups为根节点下的子节点,也包含了根节点本身
			source_map[source[k]]++; //k为下标，source[k]为值，把同属于一个并查集的结点合并
			target_map[target[k]]++;//target中下标属于一个并查集的合并到一块
		}
		for (auto[num, freq] : target_map)//找两者之间多少个相同的
			ans += min(freq, source_map[num]); 
		//freq为target中某个并查集元素的数量，source_map[num]为source_map中属于同一个并查集的元素数量
	}
	cout << n - ans << endl;
	return n - ans;
}

string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
	int n = s.size();
	UnionFindOp un(n);
	for (auto& b : pairs) {
		un.union_element(b[0], b[1]);
	}
	unordered_map<int, vector<int>> sub_roots;
	for (int i = 0; i < n; i++) {
		sub_roots[un.find(i)].emplace_back(s[i]); //根节点index和其对应的所有子节点
	}

	for (auto& [root, sub_root] : sub_roots) {
		sort(sub_root.begin(), sub_root.end(), greater<int>()); // greater不加就错误  
	}
	for (int i = 0; i < s.size(); i++) {
		int x = un.find(i);
		s[i] = sub_roots[x].back();//从后往前，前面排序是从大到小
		sub_roots[x].pop_back();
	}
	return s;
}


//leetcode 1203
//vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
	
//}

//leetcode 684
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
	int n = edges.size();
	UnionFindOp un(n); //初始化
	vector<int> ans;
	
	//结点是从1开始，但是初始化是从0-n-1，因此后续结点记得-1;
	for (auto nodes : edges) {
		if (un.find(un.roots[nodes[0]-1]) != un.find(nodes[1]-1)) {
			un.union_element(nodes[0] - 1, nodes[1] - 1); //逐个合并，逐个判断；
		}
		else {
			ans.clear();
			ans.emplace_back(nodes[0]);
			ans.emplace_back(nodes[1]);
		}
	}

	for (auto a : ans) {
		cout << a << endl;
	}
	return ans;
}








class findUnion {
public:
	vector<int> roots;
	vector<int> size; //秩

	findUnion(int n) {
		roots.resize(n);
		size.resize(n, 1);
		for (int i = 0; i < n; i++) {
			roots[i] = i;
		}
	}

	int find(int x) {
		return x == roots[x] ? x : (roots[x] = find(roots[x]));
	}

	void unions(int x, int y) {
		int a = find(x);
		int b = find(y);
		if (a != b) {
			if (size[a] < size[b]) {
				roots[a] = b;
				size[b] += size[a];
			}
			else { // 若两个节点都为新的，则第二个归并到第一个
				roots[b] = a;
				size[a] += size[b];
			}
		}
	}
};

//class Solution {
//public:   //721账户合并
vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
	//string难以用index直接转换和并查集使用，因此进行转换。
	unordered_map<string, int> emailToIndex;
	unordered_map<string, string> emailToName;
	int email_index = 0;
	for (int i = 0; i < accounts.size(); i++) {
		string name = accounts[i][0];
		int sub_n = accounts[i].size();
		for (int j = 1; j < sub_n; j++) {
			if (!emailToIndex.count(accounts[i][j])) { //没找到才标号
				emailToIndex[accounts[i][j]] = email_index++; // 各邮件编号
				emailToName[accounts[i][j]] = name;  // 邮件对应的人
			}
		}
	}

		//编号完成后，进行属于同一个人的合并
	findUnion un(email_index);
	for (auto account : accounts) {
		string name = account[0];
		string first_email = account[1];
		for (int j = 2; j < account.size(); j++) {
			un.unions(emailToIndex[first_email], emailToIndex[account[j]]);//同一人的邮箱进行归并
		}
	}

	//找出每一个根节点下包含的所有邮箱有哪些
	map<int, vector<string>> rootAndemails;
	for (auto& [email, email_index] : emailToIndex) {
		int index = un.find(email_index);//找根节点
		rootAndemails[index].emplace_back(email);
	}


	//依次存储起来
	vector<vector<string>> ans;
	for (auto [root_index, sub_emails] : rootAndemails) {
		vector<string> temp;
		sort(sub_emails.begin(), sub_emails.end());
		string email_name = emailToName[sub_emails[0]];
		temp.emplace_back(email_name); //先插入姓名，接下来插入邮箱
		for (auto a : sub_emails) {
			temp.emplace_back(a);
		}
		ans.emplace_back(temp);
	}
	return ans;
}
//};

		


//vector<vector<int>> findCriticalAndPseudoCriticalEdges(int n, vector<vector<int>>& edges) {
	
//}



class UnionFindOp1 {
public:
	vector<int> roots;
	vector<int> size;//根节点下的全部的结点数
	int count;
	int release;

	UnionFindOp1(int n) {
		roots.resize(n);
		size.resize(n, 1);
		for (int i = 0; i < n; i++) {
			roots[i] = i;
		}
		count = n;
		release = 0;
	};

	int find(int i) { // 使用路径压缩
		return i == roots[i] ? i : (roots[i] = find(roots[i]));
	}

	void union_element(int i, int j) {//按秩合并
		int x = find(i);
		int y = find(j);
		if (x != y) {
			if (size[x] > size[y]) {
				roots[y] = x;
				size[x] += size[y];
			}
			else {
				roots[x] = y;
				size[y] += size[x];
			}
			count--;
		}
		else {
			release++;
		}
	}
};


int makeConnected(int n, vector<vector<int>>& connections) {
	if (connections.size() < n - 1) {
		return -1;
	}
	UnionFindOp1 un(n);
	for (auto temp : connections) {
		un.union_element(temp[0], temp[1]);
	}
	cout << un.release << endl;
	return un.release;
}



//===============================双指针=========================================

int characterReplacement(string s, int k) {
	vector<int> nums(26, 0);
	int n = s.size();
	int max_count = 0;
	int left = 0;
	int right = 0;
	while (right < n) {
		nums[s[right]-'A']++;
		max_count = max(max_count, nums[s[right] - 'A']); //找出出现次数最多的那个字母
		if (right - left + 1 - max_count > k) { //已经到达了k次
			nums[s[left]-'A']--;//左边移除，次数减一,直到满足
			left++;
		}
		cout << left <<"   "<< right << endl;
		right++;
	}
	cout << left << "   " << right << endl;
	cout << right - left << endl;
	return right - left;
}

int equalSubstring(string s, string t, int maxCost) {
	int n = s.size();
	vector<int> temp(n, 0);
	for (int i = 0; i < n; i++) {
		temp[i] = abs(s[i] - t[i]);
	}
	int left = 0;
	int right = 0;
	int sumup = 0;
	int countsize = 0;
	while (right < n) {
		if ((sumup + temp[right]) <= maxCost) { //未超出最大开销
			sumup = sumup + temp[right]; //存储开销
			right++;
		}
		else {
			if (left > right) {
				sumup = 0;
				left++;
				right++;
			}
			else {
				sumup = sumup - temp[left];
				left++; //否则删除左侧元素
			}
			
		}
		countsize = max(countsize, right - left);
	}
	cout << countsize << endl;
	return countsize ;
}



//lc480
vector<double> medianSlidingWindow(vector<int>& nums, int k) {
	vector<double> ans;
	multiset<double> slidingwindow;
	for (int i = 0; i < nums.size(); i++) {
		if (slidingwindow.size() > k) { //大于k个了，就把最开始的删除掉
			slidingwindow.erase(slidingwindow.find(nums[i - k]));
		}
		slidingwindow.insert(nums[i]);//逐渐插入
		if (i >= k - 1) { //滑动窗口已经满了,可以开始求均值
			auto mid = slidingwindow.begin();
			std::advance(mid, k / 2);//移动到中间位置
			ans.emplace_back((*mid + *prev(mid, 1 - k / 2)) / 2);//k为偶数的时候，mid加上了mid左移一位的数
		}
	}
	return ans;
}

//lc480改写
//vector<double> medianSlidingWindow1(vector<int>& nums, int k) {
	
//}


vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {
	if (nums.empty())
		return nums;
	vector<vector<int>> ans(r, vector<int>(c));
	int row = nums.size();
	int col = nums[0].size();
	if ((row * col) == (r * c)) {
		for (int i = 0; i < r * c; i++) {
			ans[i / c][i % c] = nums[i / col][i % col];
		}
	}
	return ans;
}


int minKBitFlips(vector<int>& A, int K) {
	int n = A.size();
	vector<int> diff(n + 1); //差分数组
	int ans = 0;
	int differCount = 0; //differCount记录的是此处的反转次数
	for (int i = 0; i < n; i++) {
		differCount += diff[i];
		if ((A[i] + differCount) % 2 == 0) { //为偶数时
			//若A[i]处是1,1反转了奇数次为0，若此处是0，0反转了偶数次为0
			//那么需要在翻转一次，这样才能全1
			if (i + K > n) { //后面还需反转但是已经越界了
				return -1;
			}
			ans++;
			diff[i]++;
			diff[i + K]--; //差分数组变化时候只改变两端的值
		}
	}
	return ans;
}

// 二维矩阵前缀和
//f(i,j)=f(i−1,j)+f(i,j−1)−f(i−1,j−1)+matrix[i][j]
class NumMatrix {
private:
	vector<vector<int>> two_dimension_presum;
public:
	NumMatrix(vector<vector<int>>& matrix) {
		if (matrix.empty()) {
			return;
		}
		int m = matrix.size();
		int n = matrix[0].size();
		two_dimension_presum.resize(m + 1, vector<int>(n + 1));
		//two_dimension_presum[i][j]是以(i-1,j-1)为右下角的矩阵的和
		for (int i = 1; i <= m; i++) {
			for (int j = 1; j <= n; j++) {
				two_dimension_presum[i][j] = two_dimension_presum[i - 1][j] + two_dimension_presum[i][j - 1] - two_dimension_presum[i - 1][j - 1] + matrix[i - 1][j - 1];
				//计算二维矩阵前缀和
			}
		}
	}

	int sumRegion(int row1, int col1, int row2, int col2) {
		return two_dimension_presum[row2 + 1][col2 + 1] - two_dimension_presum[row1][col2 + 1] - two_dimension_presum[row2 + 1][col1] + two_dimension_presum[row1][col1];
	}
};






//回溯+记忆化搜索
class BackAndMem {
private:
	vector<string> temp; //
	vector<vector<string>> ans;
	vector<vector<int>> mem; // mem[i][j]记录从i到j位置是否为回文字符串

	int n;
public:
	void dfs(string s, int index) {//判断s从index开始，之后的情况
		if (index == n) {
			ans.emplace_back(temp);
			return;
		}
		else {
			for (int i = index; i < n; i++) {
				if (isPalindrome(s, index, i) == 1) { //是回文，那么继续往后
					temp.emplace_back(s.substr(index, i - index + 1));
					dfs(s, i + 1);
					temp.pop_back();//说明dfs此条路失败了
				}
			}
		}
	}

	int isPalindrome(string s, int i, int j) { //判断字符串s的i到j是否为回文串
		if (mem[i][j]) {
			return mem[i][j];
		}
		if (i >= j) {
			return mem[i][j] = 1;
		}
		return mem[i][j] = (s[i] == s[j] ? isPalindrome(s, i+1, j-1) : -1);
	}

	vector<vector<string>> partition(string s) {
		n = s.size();
		mem.assign(n, vector<int>(n));
		dfs(s, 0);
		return ans;
		
	}

	/*
	此处的mem就是当作标记位，对于更新mem，可以使用dp,不需要isPalindrome重复判断
	vector<vector<string>> partition(string s) {
		n = s.size();
		mem.assign(n, vector<int>(n,true));

		for(int i=n-1;i>=0;i--){
			for(int j=i+1;j<n;j++){
				mem[i][j] = (s[i]==s[j])&&(mem[i+1][j-1]);
			}
		}
		dfs(s, 0);
		return ans;

	}
	*/
	
};

//分割为回文子串的最小次数
int minCut(string s) {
	int n = s.size();
	vector<vector<int>> tag(n, vector<int>(n, true)); //标记是否为回文字符串
	vector<int> f(n, INT_MAX); //记录分割的最小次数

	for (int i = n - 1; i >= 0; i--) {
		for (int j = i + 1; j < n; j++) {
			tag[i][j] = (s[i] == s[j]) && tag[i + 1][j - 1]; //dp方法更新tag,找出回文串的位置
		}
	}
	//找出位置以后，可以更新f，记录次数，但是要考虑整个字符串为回文串，此时分割次数为0
	for (int i = 0; i < n; i++) {
		if (tag[0][i]) { //为回文，往后
				f[i] = 0;
		}
		else { //i处不是回文了，此时f[i]处仍然是INT_MAX
			for (int j = 0; j < i; j++) { //从j到i的子串更新
				if (tag[j + 1][i]) { 
					//0到i已经不是回文了，在考虑一下1到i之间是否还有回文
					//若没有回文了，那么当j=i-1时候，tag[j+1][i]=1
					//f[i] = min(f[i],f[i-1]+1);
					//f[i] = min(MAX_INT, f[i-1]+1);
					f[i] = min(f[i], f[j] + 1);
				}
			}
		}
	}
	cout << f[n - 1] << endl;
	return f[n - 1];
}

int beautySum(string s) {
	int n = s.size();
	int ans = 0;

	for (int left = 0; left < n; left++) {
		vector<int> temp(26, 0);
		for (int right = left; right < n; right++) {
			int _max = 1;
			int _min = n;
			temp[s[right] - 'a']++;
			for (auto a : temp) {
				if (a > 0) {
					_max = max(_max, a);
					_min = min(_min, a);
				}
			}
			ans += _max - _min;
		}
	}
	cout << ans << endl;
	return ans;
}


//中缀表达式计算，此处只有+, -, (, ), " "(空格)
int calculate(string s) {
	int sign = 1; //初始化符号位
	int ans = 0;
	int n = s.size();
	string temp;
	stack<int> ops; //存储符号位
	ops.push(1);
	int i = 0; //index
	while (i < n) {
		if (s[i] == '(') {
			ops.push(sign);
		}
		else if (s[i] == ')') {
			ops.pop(); //只有当括号去除完毕后，才删掉（之前的符号，类似于去掉表达式的括号
		}
		else if (s[i] == '+') {
			if (!temp.empty()) {
				ans += sign * (stoi(temp)); //这个符号用完了，准备用下一个了
			}
			sign = ops.top();
			temp.clear();
		}
		else if (s[i] == '-') {
			if (!temp.empty()) {
				ans += sign * (stoi(temp));
			}
			sign = -ops.top(); //符号的改变类似于去掉括号
			temp.clear();
		}
		else {
			if (s[i] != ' ') { //表达式中是存在空格的
				temp.push_back(s[i]); //说明此时是数字，加入到temp中
			}
		}
		i++; //后移
	}
	//考虑最后一个数字
	if (!temp.empty()) {
		ans += sign * (stoi(temp));
	}
	return ans;
}


//中缀表达式计算，只有 +,-,*,/ 四种符号 和 ' '空格 
int calculate2(string s) { //注意先计算的是*, /, 可以把减法取相反数变为加法
	stack<int> nums; //存储数字
	int ans = 0;//最后的结果
	int temp = 0; //临时存储
	char sign = '+'; //保存前一个符号位
	int n = s.size();
	int i = 0;
	while (i < n) {
		if (isdigit(s[i])) { //s[i]是数字的时候
			temp = temp * 10 + (s[i] - '0'); //数字不一定只有一位
		}
		if ((!isdigit(s[i]) && s[i] != ' ') || i == n - 1) { //s[i]为符号且不为空,temp已经保存了这个符号之前的数字
			//考虑最后一位数字，此时i=n-1
			if (sign == '+') {
				nums.push(temp);
			}
			else if (sign == '-') {
				nums.push(-temp);
			}
			else if (sign == '*') {  //先算乘除法，在保存到nums中，最后一起加起来
				nums.top() = nums.top() * temp;
			}
			else {
				nums.top() = nums.top() / temp;
			}
			sign = s[i]; //更新符号
			temp = 0; //重置temp，为保存s[i]之后的数字做准备
		}
		i++;
	}
	while (!nums.empty()) {
		ans += nums.top();
		nums.pop();
	}
	cout << ans << endl;
	return ans;
}


string removeDuplicates(string S) {
	string str;
	for (auto a : S) {
		if (str.empty() || a != str.back()) {
			str.push_back(a);
		}
		else{
			str.pop_back();
		}
	}
	return str;
}

//入度出度来解决树的问题 331
bool isValidSerialization(string preorder) { 
	stack<int> rec;
	int i = 0;
	int n = preorder.size();
	rec.push(1);
	while (i < n) {
		if (isdigit(preorder[i])) { //为数字时
			while (isdigit(preorder[i])) {
				i++; //数字不一定是一位
			}
			if (rec.empty()) {
				return false;
			}
			rec.top() -= 1;
			if (rec.top() == 0) {
				rec.pop();
			}
			rec.push(2);
			i++;
		}
		else {//为符号时
			if (rec.empty()) {
				return false;
			}
			if (i < n && preorder[i] == ',') {
				i++;
			}
			else { //#
				rec.top() -= 1;
				if (rec.top() == 0) {
					rec.pop();
				}
				i++;
			}
		}
	}
	cout << rec.empty() << endl;
	return rec.empty();
}


//最长严格递增子序列
int lengthOfLIS(vector<int>& nums) {
	int n = nums.size();
	if (n == 0) {
		return 0;
	}
	vector<int> dp(n);
	//dp代表的是当前位置最长的序列
	//状态方程  dp[i] = max{dp[i], dp[j]+1};  j为小于i的点，且nums[j]<nums[i]
	for (int i = 0; i < n; i++) {
		dp[i] = 1;
		for (int j = 0; j < i; j++) {
			if (nums[j] < nums[i]) {
				dp[i] = max(dp[i], dp[j] + 1);
			}
		}
	}
	cout << *max_element(dp.begin(),dp.end()) << endl;
	return *max_element(dp.begin(), dp.end());
}


//Russian Doll Envelopes
//俄罗斯套娃
// 首先我们将所有的信封按照 w 值第一关键字升序、h 值第二关键字降序进行排序；
//随后我们就可以忽略 w 维度，求出 h 维度的最长严格递增子序列，其长度即为答案。
int maxEnvelopes(vector<vector<int>>& envelopes) {
	if (envelopes.empty()) {
		return 0;
	}
	int n = envelopes.size();
	vector<int> dp; //dp表示当前处最大的信封嵌套次数
	

	sort(envelopes.begin(), envelopes.end(), [](auto const& e1, auto const& e2) { //第一个元素先由小到大比较进行总体排序
		return e1[0] < e2[0] || (e1[0]==e2[0]&&e1[1]>e2[1]); //按照 w 值第一关键字升序、h 值第二关键字降序进行排序
	});

	//更新dp
	dp.resize(n, 1);
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < i; j++) {
			if (envelopes[j][1] < envelopes[i][1]) {
				dp[i] = max(dp[i], dp[j] + 1);
			}
		}
	}
	cout << *max_element(dp.begin(), dp.end()) << endl;
	return *max_element(dp.begin(), dp.end());
}



//螺旋形输出矩阵
vector<int> spiralOrder(vector<vector<int>>& matrix) {
	if (matrix.empty()||matrix[0].empty()) {
		return {};
	}
	int m = matrix.size();
	int n = matrix[0].size();
	vector<vector<int>> visited(m, vector<int>(n, 0));
	vector<vector<int>> way = { {0,1},{1,0},{0,-1},{-1,0} }; //右，下，左，上

	int row = 0, column = 0;
	int total = m * n;
	int countIndex = 0;
	vector<int> ans(total);
	
	for (int i = 0; i < total; i++) {
		ans[i] = matrix[row][column];
		visited[row][column] = 1;
		int nextrow = row + way[countIndex][0];
		int nextcol = column + way[countIndex][1]; //更新位置
		if (nextrow < 0 || nextrow >= m || nextcol < 0 || nextcol >= n || visited[nextrow][nextcol]) { //越界了
			countIndex = (countIndex + 1) % 4;
		}
		row = row + way[countIndex][0];
		column = column + way[countIndex][1]; //未越界时，nextrow = row ，。。。。
		//越界后，更新
	}
	return ans;
}



//下一个最大元素I
//找出 nums1 中每个元素在 nums2 中的下一个比其大的值
vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
	if (nums1.empty() || nums2.empty()) {
		return {};
	}
	int n = nums1.size();
	stack<int> st;
	map<int, int> mp;
	vector<int> ans;

	for (int i = 0; i < nums2.size(); i++) {
		while (!st.empty() && st.top() < nums2[i]) { //因为是找第一个比它大的，所以找到后就可以出栈了
			mp[st.top()] = nums2[i]; //
			st.pop();
		}
		st.push(nums2[i]);
	}

	for (int j = 0; j < n; j++) {
		if (mp.count(nums1[j])) { //里面有
			ans.emplace_back(mp[nums1[j]]);
		}
		else {
			ans.emplace_back(-1);
		}
	}
	return ans;
}

//下一个最大元素II
//给定一个循环数组（最后一个元素的下一个元素是数组的第一个元素），输出每个元素的下一个更大元素
//和I相比，此处的数组可以循环,通过取余完成循环
vector<int> nextGreaterElements(vector<int>& nums) {
	if (nums.empty()) {
		return {};
	}
	int n = nums.size();
	vector<int> ans(n,-1);
	stack<int> st;

	for (int i = 0; i < 2*n-1; i++) { //i<2n-1时候,i%n能完整覆盖 0~(n-1)。 n-1 < i < 2n-1时，开始循环搜索
		while (!st.empty() && (nums[st.top()] < nums[i % n])) { 
			ans[st.top()] = nums[i % n];
			st.pop();
		}
		st.push(i % n); //存下标
	}
	return ans;
}

//下一个最大元素III
/*给你一个正整数 n ，请你找出符合条件的最小整数(最小的大于n的整数)，其由重新排列 n 中存在的每位数字组成，并且其值大于 n 。如果不存在这样的正整数，则返回 -1 。*/
int nextGreaterElement(int n) {
	//先将n转为可操作的vector或者string
	string s = to_string(n);
	//从右往左找到第一个不满足从左到右为降序的数字
	//如： 645321，则为4
	int len = s.size();
	int first_min = -1;
	for (int i = len - 1; i >= 1; i--) {
		if (s[i - 1] < s[i]) {
			first_min = i - 1;
			break;
		}
	}
	if (first_min == -1)
		return -1;

	//在first_min到len-1之间，找到满足大于s[first_min]的最小数字
	int first_big = -1;
	int cmp_temp=INT_MAX;
	for (int j = first_min; j < len; j++) {
		if (s[j] > s[first_min]) {//找到大的了
			if (cmp_temp >= s[j]) { // s[first_min]<cmp_temp,同时还存在比cmp_temp更小的数，也满足s[first_min]<cmp_temp
				cmp_temp = s[j]; //大于s[first_min]的最小数字
				first_big = j;
			}
		}
	}
	if (first_big == -1) {
		return -1;
	}
	swap(s[first_min], s[first_big]);
	//若有158476531
	/*
		首先找到了4，为s[first_min]
		再找到5为s[first_big]
		交换后变为158576431
		为了满足1585后的  76431有最小值，此时76431为降序
		将其转换为升序即可得到最小值
	*/
	int i = first_min + 1; //交换
	int j = len - 1;
	while (i < j) {
		swap(s[i], s[j]);
		i++;
		j--;
	}	
	//12222333 = > 12223332
	//12223332
	//12223233
	int ans = atoi(s.c_str());
	cout << ans << endl;
	return ans;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*========================================字符串  DP    DFS   DFS+MEM====================================================*/

//不同的子序列
//s为父串
//给定一个字符串 s 和一个字符串 t ，计算在 s 的子序列中 t 出现的个数。
class dpAndMemory {
private:
	vector<vector<int>> dfs_vec;
	vector<vector<int>> dp_vec;
public:
	int dfs(string s, int s_index, string t, int t_index) {
		if (s_index == s.size()) {
			return 0;
		}
		if (t_index = t.size()) {
			return 1;
		}
		if (s[s_index] == t[t_index]) {
			return dfs(s, s_index + 1, t, t_index) + dfs(s, s_index + 1, t, t_index + 1);
			//位置相同的时候，可以选择父串和子串全都往后移动，或者父串往后移动，子串不动
		}
		else {
			return dfs(s, s_index + 1, t, t_index);
			//不相同的时候，父串往后移动，子串不动
		}
	}

	//dfs记忆化搜索
	int dfs_mem(string s, int s_index, string t, int t_index) {
		//边界条件
		if (t_index == t.size()) { //子串匹配成功
			return 1;
		}
		if (s_index == s.size()) { //父串走完了，字串没走完，匹配失败
			return 0;
		}
		
		if (dfs_vec[s_index][t_index] != -1) //访问过了
			return dfs_vec[s_index][t_index];


		if (s[s_index] == t[t_index]) { //判断一下当前位置的的字符是否相同
			dfs_vec[s_index][t_index] = dfs_mem(s, s_index + 1, t, t_index + 1) + dfs_mem(s, s_index + 1, t, t_index);
		}
		else {
			dfs_vec[s_index][t_index] = dfs_mem(s, s_index + 1, t, t_index);
		}
		
		return dfs_vec[s_index][t_index];
	}

	//dp
	int dp(string s, string t) { //t_len x s_len大小
		int s_len = s.size();
		int t_len = t.size();
		if (s_len < t_len) {
			return 0;
		}
		dp_vec.resize(t_len + 1, vector<int>(s_len + 1));

		for (int i = 0; i <= s_len; ++i) {
			dp_vec[t_len][i] = 1;
		}

		for (int i = t_len - 1; i >= 0; i--) {
			for (int j = s_len - 1; j >= 0; j--) {
				if (t[i] == s[j]) {
					dp_vec[i][j] = dp_vec[i + 1][j + 1] + dp_vec[i][j + 1]; 
				}
				else {
					dp_vec[i][j] = dp_vec[i][j + 1];//主串动，子串不动
				}
			}
		}
		return dp_vec[0][0];
	}

	int numDistinct(string s, string t) {
		int s_len = s.size();
		int t_len = t.size();

		int dfs_ans = dfs(s, 0, t, 0);


		dfs_vec.resize(s_len, vector<int>(t_len, -1)); 
		//dfs_mem[i][j]是s[0:i] t[0:j]中匹配的子串的个数
		int def_mem_ans = dfs_mem(s, 0, t, 0);
		cout << def_mem_ans << endl;

		int dp_ans = dp(s, t);

		return 0;
	}
};

/**/


//两个字符串的最长公共子序列的长度
int longestCommonSubsequence(string text1, string text2) {
	int l1 = text1.size();
	int l2 = text2.size();
	if (!(l1 && l2)) {
		return 0;
	}
	vector<vector<int>> dp(l1 + 1, vector<int>(l2 + 1, 0));
	for (int i = 1; i <= l1; i++) {
		for (int j = 1; j <= l2; j++) {
			if (text1[i-1] == text2[j-1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else {
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}
	cout << dp[l1][l2] << endl;
	return dp[l1][l2];
}


bool isNum(string a) {
	return (a != "+" && a != "-" && a != "*" && a != "/");
}

int evalRPN(vector<string>& tokens) {
	if (tokens.empty()) {
		return 0;
	}
	int n = tokens.size();
	stack<int> st;
	int ans = 0;
	for (string a : tokens) {
		if (isNum(a)) { //数字
			st.push(atoi(a.c_str()));
		}
		else {
			int e2 = st.top();
			st.pop();
			int e1 = st.top();
			st.pop();
			if (a == "+") {
				st.push(e1 + e2);
			}
			else if (a == "-") {
				st.push(e1 - e2);
			}
			else if (a == "*") {
				st.push(e1 * e2);
			}
			else {
				st.push(e1 / e2);
			}
		}
	}
	cout << st.top();
	return st.top();
}

//5710 订单采购
int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
	priority_queue<pair<int, int>> buy_list;  //别人的购买价格，从最高开始看
	priority_queue<pair<int, int>> sale_list; //别人的销售价格，从最低开始看，因为优先队列默认顶部为最大，因此加符号颠倒栈内元素
	int ans = 0;
	for (auto a : orders) {
		if (a[2]) { //sale销售订单,查看其他人当前 ==最高的== 采购订单，若高于我的sale价格，则卖出
			sale_list.push(make_pair(-a[0], a[1]));
		}
		else {//buy采购订单，差看其他人当前的  ==最低的==  销售价格， 若低于 我的采购价格，则买入
			buy_list.push(make_pair(a[0], a[1]));
		}
		//就是需要sale<buy 才成立
		ans += a[1]; //统计所有的数量

		while (!buy_list.empty()) {
			if (sale_list.empty() || -sale_list.top().first>buy_list.top().first) { //价格不满足
				break;
			}
			//有合适的
			auto buy_temp = buy_list.top();
			buy_list.pop();
			auto sale_temp = sale_list.top();
			sale_list.pop();
			
			//更新数量
			int sale_amount = min(buy_temp.second, sale_temp.second);
			buy_temp.second -= sale_amount;
			sale_temp.second -= sale_amount;
			ans = ans - 2 * sale_amount; //减去卖出的和采购的订单
			if (buy_temp.second) { //销售量不满足采购量
				buy_list.push(buy_temp);
			}
			else{ //采购完成，但是销售还有
				sale_list.push(sale_temp);
			}

		}
	}
	cout << ans;
	return ans;
}


//leetcode 1802
//类似于三角形的台阶
int maxValue(int n, int index, int maxSum) {
	if (maxSum < n) {
		return 1;
	}
	//取index位置
	int l = index, r = index;
	int ans = 1; //index处的值
	int surplus = maxSum - n; //剩余值
	while (l > 0 || r < n - 1) {//还未到边界
		int len = r - l + 1;
		if (surplus < len) { //剩余值不够len内分了
			break;
		}
		ans++;
		surplus -= len; //len内均匀加1
		l = max(0, l - 1);
		r = min(n - 1, r + 1); //l和r不会满足越界条件，即使l=0,r=n-1，只有surplus不够分，才会break
	}
	//surplus不够窗口内分了
	ans += surplus / n; //均分一下
	return ans;

	/*
	其实while之后也可以写成这种形式
	while(l>=0||r<=n-1){//还未到边界
		int len = r-l+1;
		if(surplus<len){ //剩余值不够len内分了
			break;
		}
		ans++;
		surplus -=len; //len内均匀加1
		l = max(0,l-1);
		r = min(n-1,r+1); //l和r不会满足越界条件，即使l=0,r=n-1，只有surplus不够分，才会break
	}
	//surplus不够窗口内分了
	return ans;


	这样写的不好就是当len的长度是窗口时候，我的surplus还够窗口内分很多次，那样的话就会循环非常多次的while
	影响了时间复杂度
	因此，当len等于窗口长度时，就跳出，剩余的值直接均分，这样避免了特殊情况下大量的循环
	比如用例  2 1 21
	会额外循环9次
	*/
}


//132模式
// i < j < k   nums[i]<nums[k]<nums[j]  
bool find132pattern(vector<int>& nums) {
	if (nums.empty()) {
		return false;
	}
	priority_queue<int,vector<int>,greater<int>> sto; //从小到大排序
	int temp = INT_MIN; //temp来标记当前的最小值
	for (int i = nums.size() - 1; i >= 0; i--) {
		if (nums[i] < temp) { //找位置1
			return true;
		}
		while (!sto.empty() && sto.top() < nums[i]) {//右边最小的数小于当前数字，此时已经满足了该数大于它右侧的一个数
			temp = sto.top();  //temp记录的是位置2的数
			cout << temp << endl;
			sto.pop(); //把栈内小于当前数字的全部出栈，此时栈顶是位置3的数字
		}
		sto.push(nums[i]);
	}
	return false;
}


class Solution22 {
private:
	vector<vector<int>> step = { {-1,0},{1,0},{0,-1},{0,1} };
	vector<vector<int>> visited;
public:
	bool dfs(vector<vector<char>>& board, vector<vector<int>>& step, int index, string word, int i, int j) {
	//step不加&会导致程序运行时间大幅度增长，因为&step引用step就是传入step本身
	//而直接写vector<vector<int>> step是传值，相当于对step进行了一次拷贝
		if (board[i][j] != word[index]) {
			return false;
		}
		if (index == word.size() - 1) {
			return true;
		}
		visited[i][j] = true;
		for (auto& a : step) {
			int newrow = i + a[0], newcol = j + a[1];
			if (newrow >= 0 && newrow < board.size() && newcol >= 0 && newcol < board[0].size()) {
				if (!visited[newrow][newcol]) {
					bool looptemp = dfs(board, step, index + 1, word, newrow, newcol);
					if (looptemp) {
						return true;
						break;
					}
				}
			}
		}
		visited[i][j] = false;
		return false;
	}

	bool exist(vector<vector<char>>& board, string word) {
		int index = 0; //记录word当前在第几位
		int m = board.size(), n = board[0].size();
		visited.resize(m, vector<int>(n, false));
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				bool ans = dfs(board, step, index, word, i, j);
				if (ans) {
					return ans;
				}
			}
		}
		return false;
	}
};


//数组中两个数异或的最大值
class trie_ex { //字典树
private:
	trie_ex* next[2] = {nullptr}; //不设置nullptr的话，后续对于补位的那些数字，指针会报错
public:
	trie_ex() {}
	void insert(int x, int n) {
		//从高位到低位依次，因为对于二进制而言，高位为1最大
		trie_ex* node = this;
		for (int i = n-1; i >= 0; i--) { 
			int temp = (x >> i) & 1; //二进制下标从0开始，因此只需要移动i位，从1开始则移动i-1位
			if (node->next[temp] == nullptr) {
				node->next[temp] = new trie_ex();
			}
			node = node->next[temp];
		}
	}

	int tr_find(int x,int n) {
		trie_ex* node = this;
		int ans_find = 0; //根据x的值，所查找到的能保证最大异或值的数
		for (int i = n - 1; i >= 0; i--) { //全部以最长的位数为准，少的补齐
			int temp = (x >> i) & 1; //取第i位
			if (node->next[!temp]) { //存在,那么走反方向,反方向在的话
				node = node->next[!temp];
				ans_find = ans_find * 2 + !temp;
			}
			else { //反方向为空，走同向
				node = node->next[temp];
				ans_find = ans_find * 2 + temp;
			}
		}
		ans_find = ans_find ^ x;
		return ans_find;
	}
};
int findMaximumXOR(vector<int>& nums) {
	trie_ex tr;
	int ans = 0;
	int max_ele = *max_element(nums.begin(), nums.end());
	int n = 0;
	while (max_ele) {
		n++;
		max_ele = max_ele >> 1;
	}
	for (auto c : nums) {
		tr.insert(c,n);
	}
	for (auto a : nums) {
		ans = max(ans, tr.tr_find(a,n));
	}
	cout << ans << endl;
	return ans;
}

//子集I
class Perm {
private:
	vector<vector<int>> ansI;
	vector<int> temp;
public:
	void dfs(vector<int>& nums, int level, vector<vector<int>>& ans, vector<int> temp) {
		if (level == nums.size()) {
			ans.emplace_back(temp);
			return;
		}
		//level+1 表示第level个位置上的元素是否被选中
		dfs(nums, level + 1, ans, temp); // 此时不将当前元素放入temp
		temp.emplace_back(nums[level]);
		dfs(nums, level + 1, ans, temp);
	}

	vector<vector<int>> subsets(vector<int>& nums) {
		dfs(nums, 0, ansI, temp);
		for (auto a : ansI) {
			for (auto b : a) {
				cout << b << " ";
			}
			cout << endl;
		}
		return ansI;
	}
};


int numRabbits(vector<int>& answers) {
	if (answers.empty()) {
		return 0;
	}
	sort(answers.begin(), answers.end());
	int total = answers[0] + 1; //记录总数
	int temp_count = answers[0]; //记录当前颜色的数目
	int temp_color = answers[0]; //记录当前类别
	for (int i = 1; i < answers.size(); ++i) {
		if (answers[i] == temp_color) {
			if (temp_count == 0) { //当前颜色计算完了,即使数量相同，那也是另一种颜色，比如2,2,2,2,最后一个2为其他颜色
				temp_count = answers[i] + 1;
				total += temp_count;
			}
			temp_count--;
		}
		else {
			temp_color = answers[i];
			temp_count = answers[i];
			total += temp_count + 1;
		}

	}
	return total;
}

//   81搜索旋转排序数组
bool search(vector<int>& nums, int target) {
	int l = 0, r = nums.size() - 1;
	while (l <= r) {
		while (l < r && nums[l] == nums[l + 1]) //l<r保证了l后至少还有一位，等号不成立
			l++;
		while (l < r && nums[r - 1] == nums[r])
			r--;
		int mid = (l + r) / 2;
		if (nums[mid] == target) {
			return true;
		}
		//不重复
		if (nums[l] <= nums[mid]) { //左侧有序了
			if (target >= nums[l] && target <= nums[mid]) { //target在左侧
				r = mid - 1;
			}
			else {
				//左侧有序但是target在右侧
				l = mid + 1;
			}
		}else if (nums[mid] <= nums[r]) { //右侧有序了
			if (target >= nums[mid] && target <= nums[r]) {
				l = mid + 1;
			}
			else { //右侧有序但是在左侧
				r = mid - 1;
			}
		}
	}
	return false;
}


int findMin(vector<int>& nums) {
	//如何判断在最小值的左侧、右侧
	int n = nums.size();
	int l = 0, r = n - 1;
	while (l <= r) {
		int mid = (l + r) / 2;
		if (nums[mid] < nums[r]) { //右侧有序,不能写等号，有重复元素
			r = mid;
		}
		else if (nums[mid] > nums[r]) { //右侧无序minimum在mid右
			l = mid + 1;
		}
		else {
			r--; //重复时右侧挨个减掉
		}
	}
	return nums[l];

}


//474 1和0
/*
给你一个二进制字符串数组 strs 和两个整数 m 和 n 。
请你找出并返回 strs 的最大子集的大小，该子集中 最多 有 m 个 0 和 n 个 1 。
如果 x 的所有元素也是 y 的元素，集合 x 是集合 y 的 子集 。
*/
pair<int, int> zeroandone(string str) {
	int count0 = 0, count1 = 0;
	for (auto a : str) {
		a == '0' ? count0++ : count1++;
	}
	return make_pair(count0, count1);
}

int findMaxForm(vector<string>& strs, int m, int n) {
	int len = strs.size();
	vector<pair<int, int>> count_word;
	for (auto a : strs) {
		count_word.push_back(zeroandone(a));
	}
	vector<vector<vector<int>>> dp(len+1,vector<vector<int>>(m+1,vector<int>(n+1)));
	
	for (int k = 1; k <= len; ++k) {
		for (int i = 0; i <= m; ++i) { 
			for (int j = 0; j <= n; ++j) {  //此时使用i个0,j个1
				if (i >= count_word[k - 1].first && j >= count_word[k - 1].second) {
					dp[k][i][j] = max(dp[k - 1][i][j], dp[k - 1][i - count_word[k - 1].first][j - count_word[k - 1].second] + 1);
				}
				else {
					dp[k][i][j] = dp[k - 1][i][j];
				}
			}
		}
	}


	return dp[len][m][n];
}

//约瑟夫环
int findTheWinner(int n, int k) {
	vector<int> nums(n);
	for (int i = 0; i < n; i++) {
		nums[i] = i + 1;
	}

	int begin = 0;
	while (n > 1) {
		int step = (begin + k - 1) % n; //移动到的位置
		int del_ele = nums[step];
		for (int j = step; j < nums.size() - 1; j++) {
			nums[j] = nums[j + 1];
		}
		
		nums[nums.size() - 1] = del_ele;
		nums.pop_back();
		n--;
		begin = (step+1)%n;
		begin--;

	}
	cout << nums[0];
	return nums[0];
}



int min_ele(int a, int b, int c) {
	int m = a <= b ? a : b;
	int n = a <= c ? a : c;

	return (m <= n ? m : n);
}

//最少侧跳次数
int minSideJumps(vector<int>& obstacles) {
	int n = obstacles.size();
	vector<vector<int>> dp(n, vector<int>(4));
	int begin = 2;
	int ans = 0;
	for (int i = 0; i < n; i++) {
		if (i == 0) {
			dp[0][1] = 1, dp[0][3] = 1, dp[0][2] = 0;
		}
		//dp[i][1] = 1, dp[i][3] = 1;
		if (obstacles[i] != 0) {
			dp[i][obstacles[i]] = n;
		}

	}

	//dp[n][3]
	//dp[i][1] = min(dp[i-1][1],dp[i][2],dp[i][3])
	for (int j = 1; j < n; ++j) {
		for (int i = 1; i <= 3; ++i) {
			if (obstacles[j] == i) {
				dp[j][i] = n;
			}
			else {
				if (i == 1) {
					dp[j][i] = min_ele(dp[j - 1][1], dp[j - 1][2] + 1 + (dp[j][2] >= n), dp[j - 1][3] + 1 + (dp[j][3] >= n));
				}
				else if (i == 2) {
					dp[j][i] = min_ele(dp[j - 1][2], dp[j - 1][1] + 1 + (dp[j][1] >= n), dp[j - 1][3] + 1 + (dp[j][3] >= n));
				}
				else {
					dp[j][i] = min_ele(dp[j - 1][3], dp[j - 1][1] + 1 + (dp[j][1] >= n), dp[j - 1][2] + 1 + (dp[j][2] >= n));
				}
			}
		}
	}
	return min_ele(dp[n - 1][1], dp[n - 1][2], dp[n - 1][3]);
}



//丑数II   264
int nthUglyNumber(int n) {
	if (n == 1) {
		return 1;
	}
	int count = 1;
	//如何确保顺序?
	//priority_queue每次取出最小值
	priority_queue<int,vector<int>,greater<int>> pr_que;
	pr_que.emplace(1);
	while (n > 0) {
		int min_ele = pr_que.top();
		pr_que.pop();
		pr_que.emplace((min_ele * 2));
		pr_que.emplace((min_ele * 3));
		pr_que.emplace((min_ele * 5));
		n--;
	}
	return pr_que.top();
}

//丑数 方法2
//dp  三指针
int nthUglyNumber_case2(int n) {
	if (n == 1) {
		return 1;
	}
	vector<int> dp(n);
	dp[1] = 1;
	int p2 = 1, p3 = 1, p5 = 1;
	for (int i = 1; i < n; ++i) {
		int nums1 = p2 * 2, nums2 = p3 * 3, nums3 = p5 * 5;
		dp[i] = min(min(nums1, nums2), nums3); //找出乘了以后的最小值
		if (dp[i] == nums1) {
			p2++;
		}
		if (dp[i] == nums2) {
			p3++;
		}
		if (dp[i] == nums3) {
			p5++; 
		}//选取了，则后移

	}
	cout << dp[n - 1] << endl;
	return dp[n - 1];
}



//打家劫舍II   213
int rob(vector<int>& nums) {
	//房间i偷或者不偷
	// dp[i]表示到达房间i时，所获得的利润
	//dp[i] = max(dp[i-1], dp[i-2]+dp[i]);
	int n = nums.size();
	if (n == 1) {
		return nums[0];
	}
	else if (n == 2) {
		return max(nums[0], nums[1]);
	}

	vector<int> dp(n - 1, 0); //0-n-2
	vector<int> dp1(n, 0); //1-n-1  //
	dp[0] = nums[0];
	dp[1] = max(nums[0], nums[1]); //注意这里的初始化
	dp1[1] = nums[1];
	dp1[2] = max(nums[2], nums[1]);
	for (int i = 2; i < n - 1; i++) {
		dp[i] = max(dp[i - 1], (dp[i - 2] + nums[i]));
	}
	for (int i = 3; i < n; i++) {
		dp1[i] = max(dp1[i - 1], (dp1[i - 2] + nums[i]));
	}
	return max(dp[n - 2], dp1[n - 1]);
}



//474 一和零  1和0
class ZeroAndOne {
public:
	vector<int> countZeroOne(string s) {
		vector<int> count(2,0);
		for (auto a : s) {
			count[a - '0']++;
		}
		return count;
	}

	int findMaxForm(vector<string>& strs, int m, int n) { //最多m个0，n个1
		int lens = strs.size();
		vector<vector<vector<int>>> dp(lens + 1, vector<vector<int>>(m + 1,vector<int>(n+1)));

		//dp[k][i][j]表示在0-k之间，使用i个0 j个1的最大子串数目
		for (int k = 1; k <= lens;++k) {
			auto count = countZeroOne(strs[k-1]);
			for (int i = 0; i <= m; i++) {
				for (int j = 0; j <= n; j++) {
					//不能直接将i==0||j==0时，dp[i][j]=0,因为可能子串为“000”，“111”等
					if (i >= count[0] && j >= count[1]) {
						dp[k][i][j] = max(dp[k - 1][i][j], dp[k - 1][i - count[0]][j - count[1]] + 1);
					}
					else {//装不下
						dp[k][i][j] = dp[k - 1][i][j];
					}
					
				}
			}
		}
		return dp[lens][m][n];
	}
};


//单线程CPU   短作业调度1834
vector<int> getOrder(vector<vector<int>>& tasks) {
	int n = tasks.size();
	vector<int> index_rec(n);
	iota(index_rec.begin(), index_rec.end(), 0);
	sort(index_rec.begin(), index_rec.end(), [&](int a, int b) { //[&]    lambda以引用方式捕获
		return tasks[a][0] < tasks[b][0]; //用大小为n的vector记录下标
	});

	priority_queue<pair<int, int>,vector<pair<int, int>>,greater<pair<int, int>>> que;
	int time_rec = 0;
	int p = 0; //记录当前进入了多少个任务
	vector<int> ans;

	for (int i = 0; i < n; ++i) {
		if (que.empty()) {
			time_rec = max(time_rec, tasks[index_rec[p]][0]);
		}
		while (p < n && tasks[index_rec[p]][0] <= time_rec) {
			que.emplace(make_pair(tasks[index_rec[p]][1], index_rec[p]));
			//priority_queue先排序第一个参数，在排序第二个参数
			//因此，存储时，要将执行时间放在第一个参数，不能将index放在第一个
			++p;
		}

		auto [ length, index] = que.top();
		ans.emplace_back(index);
		time_rec += length;
		que.pop();

	}
	return ans;

}


//363 矩形区域不超过 K 的最大数值和
int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
	//先拿二维矩阵前缀和暴力
	if (matrix.empty()) {
		return 0;
	}
	int m = matrix.size();
	int n = matrix[0].size();
	vector<vector<int>> matrix_presum(m + 1, vector<int>(n + 1));

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			matrix_presum[i][j] = matrix_presum[i - 1][j] + matrix_presum[i][j - 1] - matrix_presum[i - 1][j - 1] + matrix[i - 1][j - 1];
		}
	}

	long ans = INT_MIN;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) { //左上角
			for (int a = i + 1; a <= m; a++) {
				for (int b = j + 1; b <= n; b++) { //右下角
					long sumup = matrix_presum[a][b] + matrix_presum[i][j] - matrix_presum[a][j] - matrix_presum[i][b];
					if (sumup < k) {
						ans = max(ans, sumup);
					}
				}
			}
		}
	}
	return ans;
}


//368,最大整除子集
vector<int> largestDivisibleSubset(vector<int>& nums) {
	int n = nums.size();
	sort(nums.begin(), nums.end());
	vector<int> dp(n + 1, 1);
	//dp[i]表示[0-i]最长子序列个数
	int max_length = 1;
	int max_index = 0;
	int max_value = 0;

	for (int i = 1; i < n; ++i) {
		for (int j = 0; j < i;++j) {
			if (nums[i] % nums[j] == 0) {
				dp[i] = max(dp[i], dp[j] + 1);
			}
		}
		if (dp[i] > max_length) {
			//保存下标、长度、最大值很重要
			max_index = i;
			max_value = nums[i];
			max_length = dp[i];
		}
	}

	vector<int> ans;
	for (int j = max_index; j >= 0; --j) {   //倒序查找
		if (max_value % nums[j] == 0&&dp[j]==max_length) {
			ans.emplace_back(nums[j]);
			max_value = nums[j];
			max_length--;
		}
	}
	return ans;

}




//1011. 在 D 天内送达包裹的能力
// 找出满足D天内送达的最小船负载能力
//包裹的顺序不能改变
int shipWithinDays(vector<int>& weights, int D) {
	//船的负载范围在  (max(单个货物重量)) ~ (所有货物重量之和)  之间
	int lower_bound = *max_element(weights.begin(), weights.end());
	int upper_bound = accumulate(weights.begin(), weights.end(), 0);
	//二分查找在此区间验证重量是否满足
	
	int ans = 1;
	while (lower_bound <= upper_bound) {
		int sum = 0;
		int count = 1;
		int mid = (lower_bound + upper_bound) / 2;
		for (auto weight : weights) {
			sum += weight;
			if (sum > mid) {
				count++;
				sum = weight;
			}
		}
		if (count <= D) { //船大了,时间比预计时间短,船也有可能能继续缩小，同时count满足D
			upper_bound = mid - 1;
		}
		else { //船小了，count>D,规定时间装不完
			lower_bound = mid + 1;
		}
	}
	return lower_bound;
}


//判断是否存在a^2+b^2 = c
bool judgeSquareSum(int c) {
	for (long a = 0; a * a <= c; ++a) {
		double b = sqrt(c - a * a);
		if (b == int(b)) {
			return true;
		}
	}
	return false;
}




int deleteAndEarn(vector<int>& nums) {
	int n = nums.size();
	int max_ele = 0;
	for (int i = 0; i < n; ++i) {
		max_ele = max(max_ele, nums[i]);
	}

	vector<int> all(max_ele + 1);

	for (auto a : nums) {
		all[a] += a;
	}
	vector<int> dp(max_ele + 1);
	dp[0] = 0;
	dp[1] = all[1];

	for (int i = 2; i <= max_ele; ++i) {
		dp[i] = max(dp[i - 1], dp[i - 2] + all[i]);
	}
	cout << dp[max_ele] << endl;
	return dp[max_ele];
}


//137 只出现一次的数字II
//给你一个整数数组 nums ，除某个元素仅出现 一次 外，其余每个元素都恰出现 三次 。请你找出并返回那个只出现了一次的元素
int singleNumber(vector<int>& nums) {
	//找到一个方法，使得出现三次的数字位操作后变为0
	//将二进制中的count方法延展到32位int,二进制中出现一次的方法
	int one = 0;
	int two = 0;
	for (auto num : nums) {
		one = (num ^ one) & (~two);
		two = (num ^ two) & (~one);
	}
	return one;
}

//case 2
int singleNumber2(vector<int>& nums) {
	//遍历，统计每一位二进制的数量
	int ans = 0;
	for (int i = 0; i < 32; ++i) {
		int index_count = 0;
		for (auto num : nums) {
			index_count += (num >> i) & 1; //取最低位
		}
		if (index_count % 3 != 0) {
			ans |= (1 << i); //构造ans
		}
	}
	return ans;
}


//554 砖墙
//使得直线穿过的砖块数量最少
int leastBricks(vector<vector<int>>& wall) {
	unordered_map<int, int> un;
	int m = wall.size();
	for (int i = 0; i < m; ++i) {
		int sum = 0;
		int n = wall[i].size();   //每行的砖块数量不一样
		for (int j = 0; j < n - 1; ++j) {
			sum += wall[i][j];
			un[sum]++;
		}
	}
	int count = 0;
	for (auto [_, cnt] : un) {
		count = max(count, cnt);
	}
	cout << m - count << endl;
	return m - count;
}



//1723 完成所有工作所需要的最短时间
class Jobs {
private:
	int ans = 0x3f3f3f3f;
	int n = 0;
	int k_ = 0;
	vector<int> sums;
public:
	void dfs(int index,vector<int> sums, vector<int>& jobs,int max_ele) {
		if (max_ele >= ans) { //剪枝
			return;
		}
		if (index == n) { //工作完了
			ans = max_ele;
			return;
		}
		for (int i = 0; i < k_; ++i) { //遍历人
			sums[i] += jobs[index]; //分配了当前工作
			dfs(index + 1, sums, jobs, max(sums[i], max_ele));
			sums[i] -= jobs[index];
		}
	}

	//优化防止超时
	//改变了分配次序，优先分配给空闲的工人
	void dfs2(vector<int>& jobs, vector<int>& nums, int job_index, int worker_count, int max_ele) {
		//job_index表示当前的工作编号
		//worker_count表示当前用了多少个工人
		if (max_ele >= ans) {
			return;
		}
		if (job_index == n) {
			ans = max_ele;
			return;
		}

		//优先分配空闲工人
		if (worker_count < k_) {
			sums[worker_count] = jobs[job_index];
			dfs2(jobs, nums, job_index + 1, worker_count + 1, max(max_ele, sums[worker_count]));
			sums[worker_count] = 0;
		}

		//空闲工人分配完毕了
		//没有空闲工人了，从所有工人中正常dfs
		for (int i = 0; i < worker_count; ++i) {
			sums[i] += jobs[job_index];
			dfs2(jobs, nums, job_index + 1, worker_count, max(max_ele, sums[i]));
			sums[i] -= jobs[job_index];
		}

	}

	int minimumTimeRequired(vector<int>& jobs, int k) {
		if (k == jobs.size()) {
			return *max_element(jobs.begin(), jobs.end());
		}
		n = jobs.size();
		k_ = k;
		int job_index = 0;
		sums.resize(k);
		//dfs(0, sums, jobs,0);
		dfs2(jobs, sums, 0, 0, 0);
		cout << ans << endl;
		return ans;
	}
};

//403 青蛙过河
class Frog {
private:
	unordered_set<int> stone_rec;
	vector<unordered_map<int, int>> mem;
	int n;
public:
	//普通的dfs，会超时
	void dfs(vector<int>& stones,int index, int last_step, bool& tag) { //要记录上次跳了多少步,index为当前位置
		if (index == n) {
			tag = true;
			return;
		}
		//上次为last_step,这次为last_step-1 ~ last_step+1
		//要保证大于0
		for (int this_step = last_step - 1; this_step <= last_step + 1 && this_step<=n; ++this_step) {
			if (this_step > 0) {
				//有石头则继续
				int trans = index + this_step;
				if (stone_rec.count(trans)) {
					dfs(stones, trans, this_step,tag);
				}
			}
		}
		
	}

	//记忆化搜索
	//mem[stone][step] 记录在当前石头，上次走了step步时，能否到达下一块石头,step不要当数组，不然会越界,当值存入unordered_map
	//mem中存入stone的下标，存值会越界
	//vector<unordered_map<int,int>> mem
	//记忆化搜索每次更新的就是mem，通过mem判断是否能到达
	bool dfs2(vector<int>& stones, vector<unordered_map<int, int>>& mem, int index, int last_step) {
		if (index == n-1) { //走到了最后一块石头
			cout << "done" << endl;
			return true;
		}
		//index不会是没有石头的位置，下面if除去了这种可能
		if (mem[index].count(last_step)) { //当前位置走得通
			//有
			return mem[index][last_step];
		}

		for (int this_step = last_step - 1; this_step <= last_step + 1; ++this_step) {
			if (this_step > 0) { //走几步
				//按石头来找，不按照每次的步数来找
				int find_stone = lower_bound(stones.begin(), stones.end(), stones[index] + this_step) - stones.begin();
				if (find_stone < n && stone_rec.count(stones[index] + this_step) && dfs2(stones, mem, find_stone, this_step)) {
					return mem[index][last_step] = true;
				}
			}
		}
		return mem[index][last_step] = false;
	}


	bool canCross(vector<int>& stones) {
		//n = *max_element(stones.begin(),stones.end());
		stone_rec = unordered_set<int>(stones.begin(), stones.end()); //记录了石头的位置
		//bool tag = false;
		//dfs(stones, 0, 0, tag);
		//return tag;

		//若n记录的是所有石头数目，当用例为
		//[0,2147483647]  会越界
		//n用来只记录石头位置，不采用所有数组
		
		n = stones.size();
		mem.resize(n);
		return dfs2(stones, mem, 0, 0);
	}
};


//7 整数反转
// 123=》321   -123=》-321
// 不使用64位整数，即long
int reverse(int x) {
	//-2^31 = -2147483648 =>-147483648
	//2^31-1 = 2147483647 =>147483647
	return 0;
}


//1482 制作m束花需要的最少天数
//每束花需要使用相邻的k朵花，bloomDay[i]表示第i多花的开放时间，返回m束需要的最少天数，无法制作返回-1
class Flowers {
public:
	bool can_make(vector<int>& bloomDay, int m, int k, int mid) { //在mid情况下，看有多少满足的
		int done_flowers = 0; //已经做的花束
		int flower_count = 0; //记录当前花束已经用的花朵
		for (int i = 0; i < bloomDay.size(); ++i) {
			if (bloomDay[i] <= mid) { //可以开放
				flower_count++;
				if (flower_count == k) {
					done_flowers++; //完成数加1
					flower_count = 0; //为下次做准备
				}
			}
			else { //开不了，那么把之前已经计算的花朵清0
				flower_count = 0;
			}
		}
		cout<< (done_flowers >= m);
		return done_flowers >= m;
	}

	int minDays(vector<int>& bloomDay, int m, int k) {
		if (bloomDay.size() < m * k) {
		return -1;
		}
		//题目可理解为在长度为n的数组中，找出m个长度为k且不相交的区间，找出区间中所有元素的最大值中的最小值
		//看到最大值的最小值，应想到二分
		int max_day = *max_element(bloomDay.begin(), bloomDay.end());
		int min_day = *min_element(bloomDay.begin(), bloomDay.end());
	
		while (min_day <= max_day) {
			int mid = (min_day + max_day) / 2;
			if (can_make(bloomDay, m, k, mid)) { //当前值可以完成,那么再把它缩小一些
				max_day = mid;
			}
			else {//不能完成，那增大一点
				min_day = mid+1;
			}
		}
		return min_day;
	}

};





//5751. 下标对 中的最大距离
int maxDistance(vector<int>& nums1, vector<int>& nums2) {
	int m = nums1.size();
	int n = nums2.size();
	if (nums1[m - 1] > nums2[0]) { //最小大于最大，nums1[i]<nums2[j]不可能成立
		return 0;
	}
	int ans = 0;
	int i = 0;
	for (int j = 0; j < n; ++j) {
		while (nums1[i] > nums2[j] && i < m) {
			i++; //找到满足nums1[i]>nums2[j]的第一个数
		}
		if (i < m) {
			//不能倒序求解，倒序求解的话，i初始在m-1，j初始在n-1.若此时也有m>n
			/*while(i>=0&&nums1[i]<nums2[j]){
				i--;
			}*/
			//若nums1[m-1] == nums2[0],nums[m-2]<nums[0]...
			//此时while没执行，返回的是i-j，并非0
			ans = max(ans, j - i);
		}
	}
	
	return ans;
}




//5750. 人口最多的年份
int maximumPopulation(vector<vector<int>>& logs) {
	vector<int> count(101);
	for (auto a : logs) {
		for (int i = a[0]; i < a[1];++i) {
			count[i-1950]++; //把活着的那些年全部标记
		}
	}

	int ans = 0;
	for (int i = 0; i < 101;++i) {
		if (count[i] > count[ans]) {
			ans = i;
		}
	}
	return ans+1950;
}

//1856. 子数组最小乘积的最大值
/*
数组 [3,2,5] （最小值是 2）的最小乘积为 2 * (3+2+5) = 2 * 10 = 20 。
给你一个正整数数组 nums ，请你返回 nums 任意 非空子数组 的最小乘积 的 最大值
*/
int maxSumMinProduct(vector<int>& nums) {
	//当前子数组的最小值没有变时，子数组长度越长，最小乘积越大
	//倘若我存储当前位置左右两侧比它小的数的位置时候
	//[2,3,3,1,2]可以得到 =>>>rec = [-1,3],[0,3],[0,3],[-1,-1],[3,-1]
	//得到前缀和为=>>>[2,5,8,9,11]
	//最小乘积:  0-2 2*8=16  3 1*11=11 

	nums.emplace_back(0); //头尾加0便于后续处理
	nums.insert(nums.begin(),0);
	int n = nums.size();
	//存储前缀和
	vector<int> pre_sum(n + 1);
	for (int i = 1; i <= n; ++i) {
		pre_sum[i] = pre_sum[i - 1] + nums[i - 1];
	}
	vector<pair<int, int>> smaller_ele(n); //存左右侧比当前位置小的下标
	//拿栈+循环找下标
	stack<int> st; //存储下标
	

	for (int i = 0; i < n; ++i) {
		while (!st.empty() && nums[i] < nums[st.top()]) {
			smaller_ele[st.top()].second = i;
			st.pop(); //存储过最小值的下标全部弹出
			
		}
		st.push(i);
	}
	
	stack<int> st2; //找左侧第一个比它小的
	for (int j = n-1; j >=0 ; --j) {
		while (!st2.empty() && nums[st2.top()] > nums[j]) {
			smaller_ele[st2.top()].first = j;
			st2.pop();
		}
		st2.push(j);
	}
	
	int ans = 0;
	for (int i = 1; i < n; ++i) {
		int l = smaller_ele[i].first;
		int r = smaller_ele[i].second; //左右的最小值都不计算(l,r)中的值
		ans = max(ans, (pre_sum[r] - pre_sum[l + 1]) * nums[i]);
	}
	cout << ans << endl;
	return ans;
}



//1734.解码异或后的排列
vector<int> decode(vector<int>& encoded) {
	//前n个正整数的排列，n为奇数
	//观察可知，encoded的所有数的异或值等于perms[0]^perms[end]
	//可以得到 perms[0]^encoded[所有下标位奇数] = encoded[所有下标偶数]^perms[end] = perms[all]^
	/*
	perms是前n个正整数的排列，因此，perms[all]^的值 为1~n的异或值
	*/
	int n = encoded.size();
	int perms_all = 0;
	for (int i = 1; i <= n + 1; ++i) {
		perms_all ^= i;
	}
	int p_0 = 0; //perms[0]
	int encoded_allodd_xor = 0;
	for (int i = 0; i < n; ++i) {
		if (i % 2 != 0) { //odd
			encoded_allodd_xor ^= encoded[i];
		}
		//xor_all^=encoded[i];
	}
	p_0 = perms_all ^ encoded_allodd_xor;
	vector<int> perms(n + 1);
	perms[0] = p_0;
	for (int i = 0; i < n; i++) {
		perms[i + 1] = perms[i] ^ encoded[i];
	}
	return perms;
}



//87 扰乱字符串 hard
class scramble {
private:
	vector<vector<vector<int>>> mem;
	int n;
	string s1, s2;
public:
	//先写爆搜
	bool parameter_same(string s1, string s2) { //判断二者所含的各种字母的数量是否相同
		vector<int> str(26);
		for (auto a : s1) {
			str[a - 'a']++;
		}

		for (auto b : s2) {
			str[b - 'a']--;
		}
		if (any_of(str.begin(), str.end(), [](const auto& a) {
			return a != 0;  //存在不等于0，说明不完全相同
		})) {
			return false;
		}
		return true;
	}

	//递归的边界条件，字符串相等了， 字符串元素不相等
	bool isScramble(string s1, string s2) {
		if (s1 == s2) {
			return true;
		}
		if (!parameter_same(s1, s2)) {
			return false;
		}
		int n = s1.size();

		for (int i = 1; i < s1.size(); ++i) {  //[0~i) [i~n),开始划分
			string a = s1.substr(0, i), b = s1.substr(i);
			string c = s2.substr(0, i), d = s2.substr(i);
			//先计算不交叉的
			if (isScramble(a, c) && isScramble(b, d)) {
				return true;
			}

			//若交叉了,s1的前i个字符实际上是s2的后i个字符
			string c1 = s2.substr(s2.size() - i);//取s2的后s2.size()-i个字符
			string d1 = s2.substr(0, s2.size() - i); //取s2的前s2.size()-i个字符
			if (isScramble(a, c1) && isScramble(b, d1)) {
				return true;
			}
		}
		return false;
	}

	//使用记忆化搜索
	//mem[i][j][k]表示s1在位置i，s2在位置j，长度为k时是否匹配
	//mem 0表示没访问过，-1表示不成立，1表示成立
	bool dfs(int s1_index, int s2_index, int len) { //更新mem和使用mem判断
		if (mem[s1_index][s2_index][len] != 0) {
			return mem[s1_index][s2_index][len] == 1; //当前位置走过了，直接返回mem所记录的值
		}
		
		string a = s1.substr(s1_index, len);
		string b = s2.substr(s2_index, len); //取长度为len的子串
		
		if (a == b) {
			mem[s1_index][s2_index][len] = 1;
			return true;
		}
		if (!parameter_same(a, b)) { //false
			mem[s1_index][s2_index][len] = -1;
			return false;
		}

		for (int i = 1; i < len; i++) {
			if (dfs(s1_index, s2_index, i) && dfs( s1_index + i, s2_index + i, len - i)) { //不交换
				mem[s1_index][s2_index][len] = 1; //不交换走的通
				return true;
			}
			//交换
			//len+s2_index 表示从s2_index开始取len长的子串
			//len + s2_index - i表示取len长的子串的后i位，即交换了
			if (dfs(s1_index, s2_index + len - i, i) && dfs(s1_index + i, s2_index, len - i)) {//前部和后部
				mem[s1_index][s2_index][len] = 1;
				return true; //交换后走的通
			}
		}
		mem[s1_index][s2_index][len] = -1;
		return false;
	}

	bool isScramble2(string _s1, string _s2) {
		s1 = _s1, s2 = _s2;
		if (s1 == s2) {
			return true;
		}
		if (s1.size() != s2.size()) {
			return false;
		}
		n = s1.size();
		mem.resize(n, vector<vector<int>>(n, vector<int>(n+1, 0)));
		return dfs(0, 0, n);
	}

};




//组合总数
class Combination_Sum {
private:
	vector<pair<int, int>> frequency;
	vector<vector<int>> comb2_ans;
	vector<int> comb2_temp;

	vector<int> temp3;
	vector<vector<int>> ans3;

	vector<int> mem4;
	int temp4;
	int n4;
	int n4_2;

public:
	void dfs1(vector<int>& candidates, int target, vector<vector<int>>& ans, vector<int>& temp_str, int index) {
		if (index == candidates.size()) {
			return;
		}
		if (target == 0) {
			ans.emplace_back(temp_str);
			return;
		}
		//不选择当前位置
		dfs1(candidates, target, ans, temp_str, index + 1);
		if (target - candidates[index] >= 0) { //当前位置可以选择
			temp_str.emplace_back(candidates[index]); //加入
			dfs1(candidates, target - candidates[index], ans, temp_str, index); //因为每一位可以重复选取，所以此处的index可以直接写index
			temp_str.pop_back(); //这路走完了
		}
	}

	/*
	* 组合总和I  39
	* 给定一个无重复元素的数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。
	  candidates 中的数字可以无限制重复被选取
	*/
	//递归+回溯
	vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
		vector<vector<int>> ans;
		vector<int> temp_str;
		dfs1(candidates, target, ans, temp_str, 0);
		return ans;
	}


	/*
	* 组合总和II  40
	* 给定一个数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。
	  candidates 中的每个数字在每个组合中只能使用一次。
	* candidates包含了重复元素，[1,1,2]与[2,1,1]、[1,2,1]这些为同一种组合
	*/


	//因为有重复元素，使用递归+回溯，会造成结果重复，因此可以提前排序，然后相同的数字统一处理，for(..;i<frequency;..)来控制选择重复元素的个数
	//将包含重复元素的candidates转换为包含不重复数字和其出现频率的frequency

	//case 2 : 或者类似于组合总数I一样处理，最后set去重
	void dfs2(vector<pair<int, int>>& frequency, int target, int index) {
		if (target == 0) {
			comb2_ans.emplace_back(comb2_temp);
			return;
		}
		if (index == frequency.size()) {
			return;
		}

		//不选当前位置
		dfs2(frequency, target, index + 1);

		//选当前位置，考虑一下重复元素一起递归
		int max_time = min(target / frequency[index].first, frequency[index].second); //当前数字最多的选取次数
		for (int j = 1; j <= max_time; ++j) {
			comb2_temp.emplace_back(frequency[index].first);
			dfs2(frequency, target - frequency[index].first * j, index + 1);
			//选取1~most个重复元素，逐次便利
		}
		for (int k = 1; k <= max_time; ++k) {
			//所有的重复元素都执行完了,最后一次一共塞入了max_time个元素，删除掉他们
			comb2_temp.pop_back();
		}
	}

	vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
		sort(candidates.begin(), candidates.end());
		int n = candidates.size();
		for (int i = 0; i < n; ++i) {
			if (frequency.empty() || frequency.back().first != candidates[i]) {
				frequency.emplace_back(candidates[i], 1);
			}
			else {
				frequency.back().second++; //重复
			}
		}
		dfs2(frequency, target, 0);
		return comb2_ans;
	}

	/*
	* 组合总和III  216
	* 找出所有相加之和为 n 的 k 个数的组合。组合中只允许含有 1 - 9 的正整数，并且每种组合中不存在重复的数字。
		说明：
			 所有数字都是正整数。
			 解集不能包含重复的组合。

	*/

	void dfs3(int k, int n, int num) {
		if (n == 0 && k == 0) {
			ans3.emplace_back(temp3);
			return;
		}
		if (num > 9 || k == 0) {
			return;
		}
		temp3.emplace_back(num);
		dfs3(k - 1, n - num, num + 1);
		temp3.pop_back();
		dfs3(k, n, num + 1);
	}
	vector<vector<int>> combinationSum3(int k, int n) {
		dfs3(k, n, 1);
		return ans3;
	}


	/*
	* 组合总和IV  377
	* 给你一个由 不同 整数组成的数组 nums ，和一个目标整数 target 。请你从 nums 中找出并返回总和为 target 的元素组合的个数。
	* 注意：
		   顺序不同的序列被视作不同的组合
	*/

	void dfs4(vector<int> nums, int target, int& ans) {
		if (target == 0) {
			ans++;
			return;
		}
		if (target < 0) {
			return;
		}
		for (auto num : nums) {
			dfs4(nums, target - num, ans);
		}
	}

	int combinationSum4(vector<int>& nums, int target) {
		n4 = nums.size();
		temp4 = 0;
		dfs4(nums, target,temp4);
		return temp4;
	}   //暴力会超时


	//记忆化搜索
	//mem[value]表示target为value时有几个方案数
	int mem_search(vector<int>& nums, int target) {
		if (mem4[target] != -1) {
			return mem4[target]; //走过了，直接返回数目
		}
		//没走过
		int res = 0;
		for (auto num : nums) {
			if (target - num >= 0) {
				res += mem_search(nums, target - num);
			}
		}
		mem4[target] = res;
	}

	int combinationSum4_2(vector<int>& nums, int target) {
		n4_2 = nums.size();
		mem4.resize(target + 1, -1);
		mem_search(nums, target);
		mem4[0] = 1;
		return mem4[target];
	}


	//dp
	int combinationSum4_3(vector<int>& nums, int target) {
		int ans = 0;
		vector<vector<unsigned long long>> dp(target + 1, vector<unsigned long long> (target+1));
		dp[0][0] = 1;
		for (int i = 1; i <= target; ++i) { //选择多少个,因为最小数为1，因此最大长度为taregt
			for (int j = 1; j <= target; ++j) { //当前值
				for (auto num : nums) { //从头开始取
					if (j - num >= 0) {
						dp[i][j] += dp[i - 1][j - num];
					}
				}
			}
			ans += dp[i][target];
		}
		return ans;
	}
};



//12. 整数转罗马数字
//给你一个整数，将其转为罗马数字。
string intToRoman(int num) {
	vector<int> nums = { 1,4,5,9,10,40,50,90,100,400,500,900,1000 };
	vector<string> rep = { "I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M" };

	string ans("");
	for (int i = 12; i >= 0; --i) {
		while (num >= nums[i]) {
			ans += rep[i];
			num -= nums[i];
		}
	}
	return ans;
}



//8. 字符串转换整数 (atoi)
int myAtoi(string s) {
	auto a = s.find_first_not_of(' ');
	int pre_zero = 1;
	int pos = 1;
	long long ans = 0;
	if (s[a] == '-') {
		pos = -1;
		a++;
	}
	else if (s[a] == '+') {
		pos = 1;
		a++;
	}
		
	for (int i = a; i < s.size(); ++i) {
		if (s[i] == '0' && pre_zero) {

			continue;
		}
		if (s[i] != 0) {
			if (s[i] < '0' && s[i]>9) {
				break;
			}
			else {
				ans = ans * 10 + s[i] - '0';
				pre_zero = 0;
			}
		}
	}
	cout << pos*ans << endl;
	return ans;
}

//1827. 最少操作使数组递增
int minOperations(vector<int>& nums) {
	int opt = 0;
	for (int i = 1; i < nums.size(); ++i) {
		if (nums[i] <= nums[i - 1]) {
			opt += nums[i - 1] + 1 - nums[i];
			nums[i] = nums[i - 1] + 1;
		}
	}
	cout << opt << endl;
	return opt;
}


//455. 分发饼干
int findContentChildren(vector<int>& g, vector<int>& s) {
	sort(g.begin(), g.end());
	sort(s.begin(), s.end());
	int sum = 0;
	int g_index = 0, s_index = 0;
	while (g_index < g.size() && s_index < s.size()) {
		if (s[s_index] >= g[g_index]) {
			g_index++;
			//sum++;
		}
		s_index++;
	}
	cout << g_index << endl;
	return g_index;
}

//52双周赛 1
string sortSentence(string s) {
	string temp("");
	vector<string> str(10);
	for (int i = 0, pre_index = 0; i < s.size(); ++i) {
		if ('0' <= s[i] && s[i] <= '9') {
			temp = s.substr(pre_index, i - pre_index);
			str[s[i] - '0'] = temp;
			pre_index = i + 2;
			temp = "";
		}
	}

	string ans("");
	for (auto a : str) {
		if (a != "") {
			ans += a;
			ans += " ";
		}
	}
	ans.erase(ans.size() - 1);
	return ans;
}




vector<vector<string>> rotateTheBox(vector<vector<string>>& box) {
	int m = box.size(); //row
	int n = box[0].size(); //col
	for (int i = 0; i < m; ++i) {
		int empty_count = 0;
		for (int j = n - 1; j >= 0; --j) {
			if (box[i][j] == ".") {
				empty_count++;
			}
			else if (box[i][j] == "*") {
				empty_count = 0;
			}
			else {
				string temp = box[i][j + empty_count];
				box[i][j + empty_count] = box[i][j];
				box[i][j] = temp;
			}
		}
	}
	return box;
}


//5759. 找出所有子集的异或总和再求和
void dfssubset(vector<int> nums, int& ans,int temp,int index) {
	if (index == nums.size()) {
		ans += temp;
		return;
	}
	dfssubset(nums, ans, temp, index+1);
	dfssubset(nums, ans, temp^= nums[index],index+1);

}

//使用二进制表示法，遍历所有情况
int subsetXORSum(vector<int>& nums) {
	//首先考虑所有的子集数量，用n位长的二进制数来表示nums的每一位取或不取
	//如1010表示取第一位和第三位
	//爆搜

	int ans = 0;
	int n = nums.size();
	int total_sub = 1 << n;
	for (int i = 0; i < total_sub; i++) { //i穷举所有情况
		int temp = 0;
		for (int j = 0; j < n; ++j) { //j取位
			if ((1 & (i >> j)) != 0) { //取出二进制i的末位，若这位是1，则挑选出nums[j]
				temp ^= nums[j];
			}
		}
		ans += temp;
	}
	return ans;

	////dfs爆搜
	//int ans = 0;
	//dfssubset(nums, ans, 0, 0);
	//cout << ans << endl;
	//return ans;
}



//5760. 构成交替字符串需要的最小交换次数
int minSwaps(string s) {
	/*int count_1 = 0;
	int count_0 = 0;
	int ans = 0;
	for (auto a : s) {
		if (a == '0') {
			count_0++;
		}
		else {
			count_1++;
		}
	}
	if (abs(count_1-count_0) > 1) {
		return -1;
	}

	if (count_1 == count_0) {
		int rep1 = 0;
		for (int i = 0; i < s.size(); i+=2) {
			if (s[i] != '1')
				rep1++;
		}
		int rep0 = 0;
		for (int i = 1; i < s.size(); i += 2) {
			if (s[i] != '0')
				rep0++;
		}
		ans = min(rep1, rep0);
	}
	else if (count_1 > count_0) {
		for (int i = 0; i < s.size(); i += 2) {
			if (s[i] != '1')
				ans++;
		}
	}
	else {
		for (int i = 0; i < s.size(); i += 2) {
			if (s[i] != '0')
				ans++;
		}
	}
	cout << ans << endl;
	return ans;*/


	//第一步：统计1和0的个数
	int count_1 = count(s.begin(), s.end(), '1'); // algorithm count
	int count_0 = count(s.begin(), s.end(), '0');
	//count_if(a.begin(),a.end(),pred)
	/*
	bool IsOdd (int i) { return ((i%2)==1); }
	int mycount = count_if (myvector.begin(), myvector.end(), IsOdd);
	*/

	//s的形式只能是1010...   0101...
	int n = s.size();
	int ans = INT_MAX;
	int c1 = 0;

	int c2 = 0;
	if (count_1 == (n + 1) / 2 && count_0 == (n / 2)) { //满足条件说明1的个数和0相等或者比0多一个,此时形式为1010...
		for (int i = 0; i < n; i += 2) {
			if (s[i] != '1') {
				c1++;
			}
		}
		ans = min(ans, c1);
	}


	//0101...
	
	if (count_0 == (n + 1) / 2 && count_1 == n / 2) { //当0和1的个数相同或者0比1多一个时，满足条件
		for (int i = 0; i < n; i += 2) {
			if (s[i] != '0') {
				c2++;
			}
		}
		ans = min(ans, c2);
	}
	if (ans == INT_MAX) {
		return -1;
	}
	else {
		return ans;
	}
}



//5761. 找出和为指定值的下标对
//FindSumPairs(int[] nums1, int[] nums2) 使用整数数组 nums1 和 nums2 初始化 FindSumPairs 对象。
//void add(int index, int val) 将 val 加到 nums2[index] 上，即，执行 nums2[index] += val 。
//int count(int tot) 返回满足 nums1[i] + nums2[j] == tot 的下标对(i, j) 数目。
class FindSumPairs {
private:
	vector<int> _nums1;
	vector<int> _nums2;
	unordered_map<int, int> mp1, mp2;
public:
	FindSumPairs(vector<int>& nums1, vector<int>& nums2) {
		_nums1 = nums1;
		_nums2 = nums2;
		for (auto a : _nums1) {
			mp1[a]++;
		}
		for (auto b : _nums2) {
			mp2[b]++; //统计每种数字的个数
		}
	}

	void add(int index, int val) {
		mp2[_nums2[index]]--;
		_nums2[index] += val;
		mp2[_nums2[index] + val]++;
	}

	int count(int tot) {
		int ans = 0;
		for (auto a : _nums1) {
			int gap = tot - a;
			if (mp2.count(gap)) {
				ans += mp2[gap];
			}
		}
		return ans;
	}
};

//5762. 恰有 K 根木棍可以看到的排列数目
//有 n 根长度互不相同的木棍，长度为从 1 到 n 的整数。
//请你将这些木棍排成一排，并满足从左侧 可以看到 恰好 k 根木棍。从左侧 可以看到 木棍的前提是这个木棍的 左侧 不存在比它 更长的 木棍。

int rearrangeSticks(int n, int k) {
	int MOD = 1e9 + 7;
	vector<vector<long long>> dp(n + 1,vector<long long>(k+1));
	//dp[i][j]表示i根木棍里，可以看到j根木棍
	//最后一根再放最短的
	dp[1][1] = 1;
	for (int i = 2; i <= n; ++i) {
		for (int j = 1; j <= k; ++j) {
			dp[i][j] = (dp[i - 1][j] * (i-1) + dp[i - 1][j - 1] * 1)%MOD;
			//前i-1根木头已经看到了j根，我最后一根是最短的，那么放在任何一个木头之后都看不到我dp[i-1][j]*(i-1)
			//前i-1根木头已经看到了j-1根，差最后一根，我最后一根最短，那么只能放在第一根，为dp[i-1][j-1]*1
		}
	}
	cout << dp[n][k];
	return dp[n][k];
}


//1442. 形成两个异或相等数组的三元组数目
int countTriplets(vector<int>& arr) {
	int n = arr.size();
	vector<int> pre_xor(n + 1);
	for (int i = 1; i <= n; ++i) {
		pre_xor[i] = pre_xor[i - 1] ^ arr[i - 1];
	}
	int ans = 0;
	int temp1 = 0, temp2 = 0;
	for (int i = 1; i < n; i++) {
		for (int j = i + 1; j <= n; j++) {
			for (int k = i + 1; k <= j; ++k) {
				if (pre_xor[i - 1] == pre_xor[j]) {
					ans++;
				}
			}
		}
	}
	return ans;
}

//1738. 找出第 K 大的异或坐标值
int kthLargestValue(vector<vector<int>>& matrix, int k) {
	//前缀和
	int m = matrix.size();
	int n = matrix[0].size();
	vector<vector<int>> pre_xor(m + 1, vector<int>(n + 1));

	vector<int> ans;
	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			pre_xor[i][j] = matrix[i - 1][j - 1] ^ pre_xor[i - 1][j] ^ pre_xor[i][j - 1] ^ pre_xor[i - 1][j - 1];
			ans.push_back(pre_xor[i][j]);
		}
	}

	sort(ans.begin(), ans.end(), greater<int>());
	return ans[k - 1];
}


//快速选择算法
// 第K大的数
class quickSelect {
public:
	int findKthLargest(vector<int>& nums, int k) {
		int n = nums.size();
		return quickselect(nums, 0, n-1, n - k);
	}

	//quickselect判断下标
	//random 产生随即下标
	//partion分离，进行每次子排序
	int quickselect(vector<int>& nums, int low, int high, int k) {
		int r_index = random_index(low, high);
		swap(nums[r_index], nums[high]);
		int proper_index = partion(nums, low, high);
		if (proper_index == k) {
			cout << nums[proper_index];
			return nums[proper_index];
		}
		else if (proper_index < k) {
			quickselect(nums, proper_index + 1, high, k);
		}
		else {
			quickselect(nums, low, proper_index - 1, k);
		}

	}

	int random_index(int left, int right) {
		return rand() % (right - left + 1) + left; //generate index between left and right
	}
	
	int partion(vector<int>& nums, int left, int right) {
		int i = left - 1;
		for (int j = left; j < right; ++j) {
			if (nums[j] <= nums[right]) {
				swap(nums[++i], nums[j]); //很巧妙
			}
		}
		swap(nums[right], nums[i+1]);
		return i+1;
	}

};

//692. 前K个高频单词
vector<string> topKFrequent(vector<string>& words, int k) {
	unordered_map<string, int> mp;
	for (auto a : words) {
		mp[a]++;
	}
	

	auto cmp = [](const auto& a, const auto& b) {
		return a.second == b.second ? a.first<b.first : a.second>b.second; //小根堆，出现次数相同则字典序优先
	};
	priority_queue<pair<string, int>, vector<pair<string, int>>, decltype(cmp)> que(cmp);
	
	//将单词存入优先队列
	for (auto& a : mp) {
		que.push(a);
		if (que.size() > k) {
			que.pop();
		}
	}

	vector<string> ans;
	for (int i = 0; i < k;++i) {
		ans.emplace_back(que.top().first);
		que.pop();
	}
	return ans; 

}


//1035. 不相交的线
//1143. 最长公共子序列的变种题
int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
	int m = nums1.size();
	int n = nums2.size();
	vector<vector<int>> dp(m+1, vector<int>(n+1));
	//dp[i][j]表示 nums1[0~i)，nums2[0~j)的最大连线长度
	dp[0][0] = 0;
	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (nums1[i - 1] == nums2[j - 1]) { //相等了，可以选
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else { //不相等，不选
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}
	cout << dp[m][n] << endl;
	return dp[m][n];
}


//15. 三数之和
//先确定a，再寻找b+c = -a， b从前往后，c从后往前寻找
vector<vector<int>> threeSum(vector<int>& nums) {
	sort(nums.begin(), nums.end()); //先排序，便于后续去重
	vector<vector<int>> ans;
	for (int i = 0; i < nums.size() - 1; ++i) {
		if (i > 0 && nums[i] == nums[i - 1]) { //防止重复元素
			continue;
		}
		int temp = nums[i];
		int right = nums.size() - 1; //right只用走一次
		for (int j = i + 1; j < nums.size(); ++j) {
			if (nums[j] == nums[j - 1]) {
				continue;
			}
			while (j < right && nums[j] + nums[right] + temp>0) {
				right--;
			}
			if (j == right) {
				break;
			}
			if (nums[j] + nums[right] + temp == 0) {
				ans.push_back({ nums[j],nums[right],temp });
			}
		}
	}
	return ans;
}



bool checkZeroOnes(string s) {
	int ele_0 = 0;
	int ele_1 = 0;
	int temp0 = 0, temp1 = 0;
	int i = 0, j = 0;
	while (j < s.size()) {
		if (s[j] == s[i]) {
			if (s[j] == '1') {
				
				j++;
				temp1++;
			}
			else {

				j++;
				temp0++;
			}
		}
		else {
			ele_0 = max(ele_0, temp0);
			ele_1 = max(ele_1, temp1);
			temp0 = 0;
			temp1 = 0;
			i = j;
			j = j + 1;
			if (s[i] == '1') {
				temp1 = 1;
			}
			else {
				temp0 = 1;
			}
		}
	}
	ele_0 = max(ele_0, temp0);
	ele_1 = max(ele_1, temp1);
	return ele_1 > ele_0 ? true : false;
}


//二分
bool can_arive(vector<int>& dist, double hour, int mid) {
	double hour_count = 0;
	for (int i = 0; i < dist.size() - 1; ++i) {
		int temp = 0;
		if (dist[i] % mid == 0) {
			temp = dist[i] / mid;
		}
		else {
			temp = dist[i] / mid + 1;
		}
		hour_count += (double)temp;
	}
	hour_count += dist[dist.size() - 1] / (double)mid;
	return hour_count <= hour;
}


//1870. 准时到达的列车最小时速
int minSpeedOnTime(vector<int>& dist, double hour) {
	int n = dist.size();
	if ((int)hour < n - 1) {
		return -1;
	}

	int min_v = 1;

	int max_v = *max_element(dist.begin(), dist.end())*100;

	while (min_v < max_v) {
		int mid = (min_v + max_v) / 2;
		if (can_arive(dist, hour, mid)) {
			max_v = mid;
		}
		else {
			min_v = mid + 1;
		}
	}
	return min_v;
}


//1871. 跳跃游戏 VII
bool canReach(string s, int minJump, int maxJump) {
	int n = s.size();
	vector<int> pre(n+1); //前缀和
	pre[1] = 1;
	vector<int> dp(n,0);
	dp[0] = 1;
	for (int i = 1; i < n; ++i) {
		if (s[i] == '0') {
			int left = max(0, i - maxJump);
			int right = i - minJump;
			if (right >= 0 && left <= right && pre[right + 1] - pre[left] > 0) {//前缀和差大于0，说明区间内有可以到达的石头
				dp[i] = 1; //能到达
			}
		}
		pre[i + 1] = pre[i] + dp[i]; //dp[i]表示可以到达此处
	}
	return dp[n - 1];
}



//810. 黑板异或游戏
//数学解法
bool xorGame(vector<int>& nums) {
	return false;

}

//1707. 与数组中元素的最大异或值
//vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
//
//}


//664. 奇怪的打印机
int strangePrinter(string s) { //和最长回文子串的思想很相似
	int n = s.size();
	vector<vector<int>> dp(n, vector<int>(n));
	//dp[i][j]  表示从s[i]~s[j]的区间[i,j]最小打印次数
	
	for (int i = n - 1; i >= 0; --i) {
		dp[i][i] = 1;
		for (int j = i+1; j < n; ++j) {
			if (s[j] == s[i]) {
				dp[i][j] = dp[i][j - 1];  
			}
			else {
				//两端字母不相同时，需要分别打印左右两边的打印。
				//记两部分分别为区间 [i,k][i,k] 和区间 [k+1,j][k+1,j]（其中 i <= k < j）
				//dp[i][j] = min(dp[i][k]+dp[k+1][j])   k=[i,j) 
				int min_print = INT_MAX;
				for (int k = i; k < j; ++k) {
					min_print = min(min_print, dp[i][k] + dp[k + 1][j]);
				}
				dp[i][j] = min_print;
			}
		}
	}
	cout << dp[0][n - 1];
	return dp[0][n - 1];
}





string reverseParentheses(string s) {
	stack<char> st;
	string temp = "";
	for (auto a : s) {
		if (a == ')') {
			while (st.top() != '(') {
				temp += st.top();
				st.pop();
			}
			st.pop();
			for (int i = 0; i < temp.size();++i) {
				st.push(temp[i]);
			}
			temp = "";
		}
		else {
			st.push(a);
		}
	}
	string ans = "";
	while (!st.empty()) {
		ans.push_back(st.top());
		st.pop();
	}
	return ans;
}

//238. 除自身以外数组的乘积
vector<int> productExceptSelf(vector<int>& nums) {
	//不使用除法，输出数组中除了当前位置数的其他所有数的乘积
	int  n = nums.size();
	/*vector<int> left(n+1,1);
	vector<int> right(n+1,1);
	for (int i = 0; i < n; ++i) {
		left[i + 1] = left[i] * nums[i];
		right[n - i - 1] = right[n - i] * nums[n - i - 1];
	}
	vector<int> ans(n);
	for (int i = 0; i < n; ++i) {
		ans[i] = left[i] * right[i + 1];
	}
	return ans;*/
	vector<int> ans(n,1);
	int left = 1, right = 1;
	for (int i = 0; i < n; ++i) {
		ans[i] *= left;
		left *= nums[i];

		ans[n - i - 1] *= right;
		right *= nums[n - i - 1];
	}
	return ans;
}



//477. 汉明距离总和
//计算一个数组中，任意两个数之间汉明距离的总和。
int totalHammingDistance(vector<int>& nums) {
	if (nums.empty()) {
		return 0;
	}
	int n = nums.size();
	int ans = 0;
	for (int i = 0; i < 32; ++i) {
		int cnt[1][2];
		memset(cnt, 0, sizeof(cnt));
		for (auto num : nums) {
			int temp = num >> i;
			cnt[0][temp & 1]++;
		}
		ans += cnt[0][0] * cnt[0][1];
	}
	return ans;
}



//5774. 使用服务器处理任务
//vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
//	int n = servers.size(), m = tasks.size();
//	//记录服务器下标
//	vector<int> ser_index(n);  //将其按照权重大小排序后，记录每个位置对应的原先的下标
//	iota(ser_index.begin(), ser_index.end(), 0);
//	sort(ser_index.begin(), ser_index.end(), [&](const auto& a, const auto& b) {
//		return servers[b] > servers[a];
//	});
//	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> que;   //按照结束时间排序，结束时间相同则按照服务器权重排序
//	vector<int> ans(m);
//	vector<int> ser_rec(n, 0);
//	queue<int> delay;
//
//	for (int i = 0; i < m; ++i) {
//		while (!que.empty() && que.top().first <= i) {
//			ser_rec[ser_index[que.top().second]] = 0;
//			que.pop();
//		}
//		if (que.size() == n) {
//			delay.push(i);
//			continue;
//		}
//		
//		for (int j = 0; j < n;++j) {
//			if (ser_rec[ser_index[j]] == 0) {
//				if (delay.empty()) {
//					que.emplace(make_pair(i + tasks[i], j)); //按照权重从小到大取
//					ans[i] = ser_index[j];
//					ser_rec[ser_index[j]] = 1;
//					break;
//				}
//				else {
//					que.emplace(make_pair(delay.front() + tasks[i], j));
//					ans[delay.front()] = ser_index[j];
//					ser_rec[ser_index[j]] = 1;
//					delay.pop();
//					break;
//				}
//			}
//		}
//		
//	}
//	int time_rec = 0;
//	vector<pair<int,int>> delay_ser;
//	while (!delay.empty()) {
//		time_rec = max(time_rec, que.top().first); //目前队列中第一个任务的结束时间
//		delay_ser.push_back(que.top());
//		que.pop();
//		while (que.top().first == time_rec) {  //若多个服务器同时完成
//			delay_ser.push_back(que.top());
//			que.pop();
//		}
//		sort(delay_ser.begin(), delay_ser.end(), [](const auto& a, const auto& b) {
//			return a.second > b.second; //按照服务器优先级倒序排序
//		});
//		
//		int task_cnt = min(delay.size(), delay_ser.size());
//		while(task_cnt) {
//			ans[m-delay.size()]= ser_index[delay_ser[delay_ser.size()-1].second];
//			delay_ser.pop_back();
//			delay.pop();
//			task_cnt--;
//		}
//	}
//	
//	return ans;
//}

class TasksProcessing {
private:
	using PII = pair<int, int>;
public:
	vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
		int n = servers.size(), m = tasks.size();
		//server 
		priority_queue<PII, vector<PII>, greater<PII>> busy_que; //结束时间，所使用服务器下标
		priority_queue<PII, vector<PII>, greater<PII>> idle_que; //未使用的server
		for (int i = 0; i < n; ++i) {
			//初始化按照服务器权值排序，权值相同，按照下标排序
			idle_que.emplace(make_pair(servers[i], i)); 
		}
		
		int time_stamp = 0; // 当前时间
		//当busy队列中的任务满足t <= time_stamp ,则说明执行完成，可转移到idle

		//[captures] (params) mutable-> type{...} //lambda 表达式的完整形式
		auto release = [&]() {
			while (!busy_que.empty() && busy_que.top().first <= time_stamp) {
				auto&& [_, index] = busy_que.top(); //释放的服务器下标
				idle_que.emplace(servers[index], index); //加入到空闲服务器
				busy_que.pop();
			} // && universal reference 万能引用
		};

		vector<int> ans;
		for (int i = 0; i < m; ++i) { //遍历任务
			time_stamp = max(time_stamp, i);
			release();
			if (idle_que.empty()) { //空闲服务器为空
				time_stamp = busy_que.top().first; //为空则看一下正在运行的最小结束时间
				release();//当timestamp为busyque里的最小结束时间时，释放一下任务
			}

			auto&& [_, index] = idle_que.top();
			busy_que.emplace(make_pair(tasks[i] + time_stamp, index)); //该任务的结束时间和使用的服务器编号
			ans.push_back(index); //加入所使用的服务器编号，为空也可以
			idle_que.pop(); //将已经使用的服务器弹出
		}
		return ans;
	}
};


//1744. 你能在你最喜欢的那天吃到你最喜欢的糖果吗？
vector<bool> canEat(vector<int>& candiesCount, vector<vector<int>>& queries) {
	//前缀和
	int n = candiesCount.size();
	vector<int> pre_sum(n+1,0);
	for (int i = 1; i <= n; ++i) {
		pre_sum[i] = pre_sum[i - 1] + candiesCount[i - 1];
	}
	vector<bool> ans(queries.size());
	//能否吃到，判断糖果范围在不在 pre_sum[queries[][0]]~pre_sum[queries[][0]+1]。吃的糖果的范围在  day*1~day*maxcandiesnum
	for (int i = 0; i < queries.size(); ++i) {
		int candies_low = queries[i][1] * 1;
		int candies_upper = queries[i][1] * queries[i][2];
		if (candies_upper<pre_sum[queries[i][0]] || candies_low>pre_sum[queries[i][0]+1]) {
			ans[i] = false;
		}
		else {
			ans[i] = true;
		}
	}
	return ans;   ////结果错了，下标错误
}


//523. 连续的子数组和
//前缀和+同余定理
bool checkSubarraySum(vector<int>& nums, int k) {
	int pre_sum = 0;
	int pre_mod = 0;
	unordered_map<int, int> mp;
	mp[0] = -1;
	for (int i = 0; i < nums.size();++i) {
		pre_sum += nums[i];
		pre_mod = pre_sum % k;
		if (mp.count(pre_mod)) {
			if (i - mp[pre_mod] >= 2) {
				return true;
			}
		}
		else {
			mp[pre_mod] = i;
		}
	}
	return false;
}



//525. 连续数组
//给定一个二进制数组 nums , 找到含有相同数量的 0 和 1 的最长连续子数组，并返回该子数组的长度。
//使用前缀和
int findMaxLength(vector<int>& nums) {
	unordered_map<int, int> mp;
	int cnt = 0;
	int n = nums.size();
	vector<int> renums(n);
	for (int i = 0; i < n; ++i) {
		if (nums[i] == 0) {  //0变为-1
			renums[i] = -1;
		}
		else {
			renums[i] = nums[i];
		}
	}

	int maxlength = 1;
	mp[0] = -1;
	for (int i = 0; i < n; ++i) {
		cnt = cnt + renums[i];
		/*
			if(nums[i]==1){  //简写前缀和
            cnt++;
        }else{
            cnt--;
        }
		*/
		if (mp.count(cnt)) {
			int index = mp[cnt];
			maxlength = max(maxlength, i - index);
		}
		else {
			mp[cnt] = i;
		}
	}
	cout << maxlength << endl;
	return maxlength;
}




int reductionOperations(vector<int>& nums) {
	unordered_map<int, int> mp;
	for (auto a : nums) {
		mp[a]++;
	}
	set<int> st = set<int>(nums.begin(), nums.end());
	int cnt = 0, index = 0;
	for (auto a : st) {
		if (index == 0) {
			index++;
			continue;
		}
		cnt += mp[a] * index;
		index++;
	}
	return cnt;
}




//494. 目标和
/*
向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数
返回可以通过上述方法构造的、运算结果等于 target 的不同 表达式 的数目。
*/

class targetsum {
private:
	int ans;
public:
	void dfs(vector<int>& nums, int target, int index, int temp_sum) {
		if(index==nums.size()){
			if (temp_sum == target) {
				ans++;
				return;
			}
			else {
				return;
			}
		}
		//当前位置为+
		dfs(nums, target, index + 1, temp_sum + nums[index]);
		//当前位置为-
		dfs(nums, target, index + 1, temp_sum - nums[index]);
		return;
	}

	int findTargetSumWays(vector<int>& nums, int target) {
		ans = 0;
		dfs(nums, target, 0, 0);
		return ans;
	}


	//使用dp
	/*
		当总和为target时，添加正号的和为non_neg，添加负号的和为neg, 则有non_neg = sum - neg。
		target = non_neg - neg=> target = sum-2 * neg => neg = (sum-target)/2
		将问题转化为，在数组nums中选取一定数目的数字，使得他们的和为neg
		dp[i][j]表示再前i个数字里，选取若干个数字，他们的和为j的方法
	*/
	int findTargetSumWays_dp(vector<int>& nums, int target) {
		int n = nums.size();
		int sum = accumulate(nums.begin(), nums.end(), 0);
		int gap = sum - target;
		int neg = (sum - target) / 2;
		if (gap < 0 || gap % 2 != 0) {
			return 0;
		}

		vector<vector<int>> dp(n + 1, vector<int>(neg + 1));
		dp[0][0] = 1;
		for (int i = 1; i <= n; ++i) {  //选取数
			for (int j = 0; j <= neg; ++j) { //值
				dp[i][j] = dp[i - 1][j];
				if (j >= nums[i - 1]) { //可选
					dp[i][j] += dp[i - 1][j - nums[i - 1]];
				}
				
				
			}
		}
		return dp[n][neg];
	}
};


//1049. 最后一块石头的重量 II
/*
每一回合，从中选出任意两块石头，然后将它们一起粉碎。假设石头的重量分别为 x 和 y，且 x <= y。那么粉碎的可能结果如下：
如果 x == y，那么两块石头都会被完全粉碎；
如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x。
最后，最多只会剩下一块 石头。返回此石头 最小的可能重量 。如果没有石头剩下，就返回 0。
*/

/*
按照  494.目标和  的方法， 给每块石头加上正号和负号，最后选出绝对值离0最近的结果 

方法一：回溯法 （不写）
方法二：dfs+mem
方法三：dp
*/
int lastStoneWeightII(vector<int>& stones) {
	//执行相减的石头，记为non
	//non的和最大为sum/2
	int n = stones.size();
	int sum = accumulate(stones.begin(), stones.end(), 0);
	int acc = sum / 2;
	vector<vector<int>> dp(n + 1, vector<int>(acc+1));  //dp[i][j]表示i个石头下，能装物品的最大值
	dp[0][0] = 0;
	for (int i = 1; i <= n; ++i) {
		for (int j = 0; j <= acc; ++j) {
			if (j >= stones[i - 1]) {
				dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - stones[i - 1]] + stones[i - 1]);
			}
			else {
				dp[i][j] = dp[i - 1][j];
			}
		}
	}
	for (int j = acc;; --j) {
		if (dp[n][j]) {
			return abs(sum - 2 * dp[n][j]);
		}
	}
}


//1888. 使二进制字符串字符交替的最少反转次数
//滑动窗口
int minFlips(string s) {
	string s_sum = s + s;
	int n = s.size();
	string s0 = "01";
	string s1 = "10";
	int cnt0 = 0, cnt1 = 0;
	int ans = 0x3f3f3f3f;

	for (int i = 0; i < 2 * n; ++i) {
		if (s_sum[i] != s0[i%2]) {
			cnt0++;
		}
		if (s_sum[i] != s1[i%2]) { //当前位不同，则修改次数+1
			cnt1++;
		}
		if (i >= n) {
			if (s0[(i - n) % 2] != s_sum[i - n]) {  //删除第一位，若第一位和字符串第一位不同，则说明不用再修改那一位，便把当时已经加上的修改次数减1
				cnt0--;
			}
			if (s1[(i - n) % 2] != s_sum[i - n]) {
				cnt1--;
			}
		}
		if (i >= n - 1) {
			ans = min({ ans,cnt0,cnt1 });
		}
	}
	return ans;
}

//879. 盈利计划
/*
n 名员工
第 i 种工作会产生 profit[i] 的利润，它要求 group[i] 名成员共同参与。
如果成员参与了其中一项工作，就不能参与另一项工作。
工作的任何至少产生 minProfit 利润的子集称为 盈利计划 。并且工作的成员总数最多为 n 。
有多少种计划可以选择

0-1背包，特殊情况，即要求价值最小
*/
int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
	int MOD = (int)1e9 + 7;
	int m = group.size();
	vector<vector<vector<int>>> dp(m + 1, vector<vector<int>>(n + 1, vector<int>(minProfit + 1)));
	//dp[i][j][k]表示  i项工作，j个人，利润至少为k的方案数
	dp[0][0][0] = 1;
	for (int i = 1; i <= m; ++i) {
		for (int j = 0; j <= n; ++j) {
			for (int k = 0; k <= minProfit; ++k) {
				if (j >= group[i - 1]) { //人够
					dp[i][j][k] = dp[i - 1][j][k] + dp[i-1][j - group[i - 1]][max(0, k - profit[i - 1])];  //当前利润为k
					/*
					第一种是当前工作利润 >=k，此时该背包的容量直接为 0（也就是说当前的工作利润已经满足了这个条件了，以后的工作利润至少为 0 就可以了）；

					第二种是当前工作利润 < k，此时该背包的容量减少了 profit[i]
					（也就是说在遍历下一个工作时，满足的工作利润至少为 k - profit[i]，二者取较大的。

					*/
				}
				else { //人不够，无法开启
					dp[i][j][k] = dp[i - 1][j][k];
				}
			}
		}

	}
	int sum = 0;
	for (int j = 0; j <= n; j++) {
		sum = (sum + dp[m][j][minProfit]) % MOD;
	}
	return sum;
}

// 518. 零钱兑换 II
// 给定不同面额的硬币和一个总金额。写出函数来计算可以凑成总金额的硬币组合数。假设每一种面额的硬币有无限个。 
// 完全背包问题   组合问题
int change(int amount, vector<int>& coins) {
	int n = coins.size();
	vector<int> dp(amount + 1);
	dp[0] = 1;
	for (auto coin : coins) { //选硬币 =>找物品
		for (int i = coin; i <= amount; ++i) { //算价值=>算背包容量
			dp[i] += dp[i - coin]; //i从coin开始选，避免重复
		}
	}
	return dp[amount];
}


int bagProblemcomp(vector<int> weight, vector<int> value, int num, int capcity) {
	vector<vector<int>> dp(num + 1, vector<int>(capcity + 1)); //一共num件物品，背包容量为capcity
	//dp[i][j]表示将前i件物品装进限重为j的背包可以获得的最大价值
	for (int i = 1; i <= num; ++i) { //num
		for (int j = 0; j <= capcity; ++j) { //weight
			if (j >= weight[i - 1]) { //此时是可以装下物品i的
				dp[i][j] = max(dp[i - 1][j], dp[i][j - weight[i - 1]] + value[i - 1]); //选物品i与不选物品i.物品i可以重复选取，因此选取时写dp[i][...]
			}
			else {
				dp[i][j] = dp[i - 1][j];
			}
		}
	}
	return dp[num][capcity];
}

//279. 完全平方数
//给定正整数 n，找到若干个完全平方数（比如 1, 4, 9, 16, ...）使得它们的和等于 n。你需要让组成和的完全平方数的个数最少。
int numSquares(int n) {
	//1<=n<=10000
	//1<=sqrt(n)<=100
	//类似于 硬币面值为1~sqrt(n)
	//硬币数量最大为n
	vector<int> dp(n + 1,0x3f3f3f3f);
	dp[0] = 0;
	for (int i = 1; i*i <= n; ++i) {
		for (int j = (pow(i,2)); j <= n; j++) {
			dp[j] = min(dp[j], dp[j - (pow(i, 2))] + 1);
		}
	}
	return dp[n];
}

//bfs求解完全平方数
//若有结点值为12， 找出小于等于12的平方数，有1，4，9。 那么12的子节点分别为12-1  12-4  12-9
//按照此规律，可构建出多叉树 
 /*
		          12
	    11         8            3
   10    7    2    7    4       2
 9 6 1  6 3   1   6  3  3  0    1    //12-4-4-4 = 0

 所使用的最少的平方数，即为找到0时候的层数
 */     




//1449. 数位成本和为目标值的最大数字
/*
动态规划
目标值（容量）恰好为target
数字1~9的cost为cost[i-1]，每位可以重复选取
*/
string largestNumber(vector<int>& cost, int target) {
	//先使用dp计算出总cost为target的最长长度
	vector<int> dp(target + 1, -0x3f3f3f3f);
	dp[0] = 0;
	for (auto a : cost) {
		for (int i = a; i <= target; ++i) {
			dp[i] = max(dp[i],dp[i-a]+1);
		}
	}

	if (dp[target] < 0) {
		return "0";
	}//未找到恰巧等于target的组合
	
	int max_length = dp[target]; //最大长度 ，接着进行状态倒退
	//当dp[i]和dp[i-cost[j]+1]相等时。说明选取了第j个数
	string ans = "";
	for (int i = 8, sum_cost = target; i >= 0; --i) {  //找出在满足最长长度的情况下，最大的数，当然从大到小遍历
		for (int c = cost[i]; c <= sum_cost && dp[sum_cost] == dp[sum_cost - c] + 1; sum_cost -= c) {
			ans += ('1' + i);  //下标i对应的数字为i+1
		}
	}

	return ans;
}

//54双周赛
//5767. 检查是否区域内所有整数都被覆盖
//已知区间 ranges[i] = [starti, endi] ，如果整数 x 满足 starti <= x <= endi ，那么我们称整数x 被覆盖了
bool isCovered(vector<vector<int>>& ranges, int left, int right) {
	/*int tag = false;
	for (int i = left; i <= right; ++i) {
		tag = false;
		for (auto a : ranges) {
			if (i >= a[0] && i <= a[1]) {
				tag = true;
				break;
			}
		}
		if (tag == false) {
			return tag;
		}
	}
	return true;*/
	//暴力法，时间复杂度太高

	//使用差分数组
	/*对于差分数组，当[i,j]区间内全部+1，那么差分数组的i位置+1，j+1位置-1*/
	vector<int> dff(52, 0);
	for (auto range : ranges) {
		dff[range[0]]++;
		dff[range[1] + 1]--;
	}
	int pre_sum = 0;
	for (int i = 0; i <= 50; ++i) {
		pre_sum += dff[i];
		if (i >= left && i <= right && pre_sum <= 0) {
			return false;
		}
	}
	return true;
}


//5768. 找到需要补充粉笔的学生编号
class Solution_chalk {
private:
	using LL = long long;
public:
	int chalkReplacer(vector<int>& chalk, int k) {
		LL sum = accumulate(chalk.begin(), chalk.end(), 0LL);  //0为 long long，  只写0为int，累加会越界
		int p = k % sum; //
		int index = -1;
		for (int i = 0; i < chalk.size(); ++i) {
			if (p - chalk[i] >= 0) {
				p -= chalk[i];
			}
			else {
				index = i;
				break;
			}
		}
		return index;
	}
};


//1857. 有向图中最大颜色值
/*
路径的 颜色值 是路径中 出现次数最多 颜色的节点数目。
请你返回给定图中有效路径里面的 最大颜色值
colors[i] 是小写英文字母
edges[j] = [aj, bj] 表示从节点 aj 到节点 bj 有一条 有向边 。
*/
// 拓扑排序判断图中是否有环， bfs统计最大颜色数
int largestPathValue(string colors, vector<vector<int>>& edges) { //n == colors.length   m == edges.length  =》n个结点，m条边
	int node_size = colors.size();
	//邻接表
	vector<vector<int>> Graph(node_size);
	//入度
	vector<int> indeg(node_size,0);

	for (auto edge : edges) {
		Graph[edge[0]].push_back(edge[1]);
		indeg[edge[1]]++;
	}
	//初始化拓扑排序结点
	stack<int> st;
	for (int i = 0; i < node_size; ++i) {
		if (!indeg[i]) {
			st.push(i); // 入度为0
		}
	}


	int node_found = 0;  //node_found !=node_size  有环
	vector<vector<int>> color_rec(node_size, vector<int>(26));
	while (!st.empty()) {
		node_found++;
		int p = st.top();
		st.pop();
		color_rec[p][colors[p] - 'a']++;
		//开始bfs p的所有后继节点q并统计颜色数目
		for (auto q : Graph[p]) {
			--indeg[q]; //入度减1
			for (int i = 0; i < 26; ++i) {
				color_rec[q][i] = max(color_rec[p][i], color_rec[q][i]); //更新后继结点的路径颜色最大值
			}
			if (!indeg[q]) { //in==0 ?
				st.push(q);
			}
		}
	}
	if (node_found != node_size) {
		return -1;
	}
	int ans = 0;
	for (int i = 0; i < node_size; ++i) {
		ans = max(ans, *max_element(color_rec[i].begin(), color_rec[i].end()));
	}
	return ans;
}

//==================================================石子游戏=======================================
class Stone {
public:
//877. 石子游戏
bool stoneGame(vector<int>& piles) {
	//pair<先手拿能获得的最大分数，后手拿能获得的最大分数>
	int n = piles.size();
	vector<vector<PII>> dp(n, vector<PII>(n));
	//初始化dp
	for (int i = 0; i < n; ++i) {
		dp[i][i] = { piles[i], 0 };
	}

	for (int i = n - 2; i >= 0; --i) {
		for (int j = i + 1; j < n; ++j) {
			//先手选择左边或者右边，可以获得的分数
			int left = piles[i] + dp[i + 1][j].second;  //先拿了最左侧的i，那么只能获得[i+1][j]后手的分数
			int right = piles[j] + dp[i][j - 1].second;
			
			//先手肯定会选取最大的
			if (left > right) {
				dp[i][j].first = left;
				dp[i][j].second = dp[i + 1][j].first;
			}
			else {
				dp[i][j].first = right;
				dp[i][j].second = dp[i][j-1].first;
			}
		}
	}
	return max(dp[0][n - 1].first,dp[0][n-1].second);
}

bool stoneGame1_1(vector<int>& piles) {
	//dp[i][j]表示在[i,j]范围，拿石头，先手拿与后手拿的差值
	int n = piles.size();
	vector<vector<int>> dp(n, vector<int>(n));
	//初始化dp
	for (int i = 0; i < n; ++i) {
		dp[i][i] = piles[i];
	}
	//更新dp
	for (int i = n - 2; i >= 0; ++i) {
		for (int j = i + 1; j < n; ++j) {
			dp[i][j] = max(piles[i] - dp[i + 1][j], piles[j] - dp[i][j - 1]);
			//当前的一方，拿走左侧石头i 或者 拿走右侧石头j ， 另一方在剩下石头里选取最优解
		}
	}
	return dp[0][n - 1] > 0;
}

};


//1895. 最大的幻方
//预先处理每行、每列的前缀和（将对角线的处理放到遍历中
int largestMagicSquare(vector<vector<int>>& grid) {
	//嗯算
	int m = grid.size(); //行
	int n = grid[0].size(); //列
	vector<vector<int>> pre_sum_row(m, vector<int>(n));  //行前缀和
	vector<vector<int>> pre_sum_col(m, vector<int>(n)); //列前缀和

	for (int i = 0; i < m; ++i) {
		pre_sum_row[i][0] = grid[i][0];
		for (int j = 1; j < n; ++j) {
			pre_sum_row[i][j] = pre_sum_row[i][j - 1] + grid[i][j];
		}
	}
	for (int j = 0; j < n; ++j) { //列
		pre_sum_col[0][j] = grid[0][j];
		for (int i = 1; i < m; ++i) { //行
			pre_sum_col[i][j] = pre_sum_col[i - 1][j] + grid[i][j];
		}
	}

	int edge_size = min(m, n);//最大的边长
	for (int i = 2; i <= edge_size; i++) {
		for (int j = 0; j + i <= m; ++j) {
			for (int k = 0; k + i <= n; ++k) { //枚举左上角(j,k)
				//计算第一行的和
				int reference_value = pre_sum_row[j][k + i - 1] - (k ? pre_sum_row[j][k-1] : 0);
				bool flag = false;
				//验证后续行，与第一行做比对
				for (int jj = j + 1; jj < j + i; ++jj) {
					int temp_sum = pre_sum_row[jj][k + i - 1] - (k ? pre_sum_row[jj][k-1] : 0);
					if (temp_sum != reference_value) {
						flag = true;
						break;
					}
				}
				if (flag) {
					continue;
				}
				//验证每一列是否和第一行的和相等
				for (int kk = k; kk < k + i; kk++) {
					int temp_sum = pre_sum_col[j + i - 1][kk] - (j ? pre_sum_col[j-1][kk] : 0);
					if (temp_sum != reference_value) {
						flag = true;
						break;
					}
				}
				if (flag) {
					continue;
				}

				//验证对角线
				int diag_temp_sum = 0, diag_temp_sum2 = 0;
				for (int c = 0; c < i; ++c) {
					diag_temp_sum += grid[j + c][k + c];
					diag_temp_sum2 += grid[j + c][k + i - 1 - c];
				}
				if (diag_temp_sum == reference_value && diag_temp_sum2 == reference_value) {
					return i;
				}
			}
		}
	}
	return 1;
}

//65. 有效数字
bool isNumber(string s) {
	int n = s.size();
	bool dot_flag = false; //.只能出现一次，因此要做标记
	for (int i = 0; i < n; ++i) {
		if (s[i] == '+' || s[i] == '-') {
			if (!((i < n - 1) && (s[i+1] - '0' >= 0 && s[i+1] - '0' <= 9) || s[i + 1] == '.')) { //+ - 号后为 数字 或者 .
				return false;
			}
		}
		else if (s[i] - '0' >= 0 && s[i] - '0' <= 9) { //为数字
			if (i < n - 1 && (s[i + 1] == '+' || s[i + 1] == '-')) {//数字后面不能跟+ 或 -
				return false;
			}
		}
		else if (s[i] == '.') { //遇到了.
	   //.的前或后必须有一个数字 ， 同时.也不应该出现两次
			if (!((i > 0 && s[i-1] - '0' >= 0 && s[i-1] - '0' <= 9) || (i < n - 1 && s[i + 1] - '0' >= 0 && s[i +1] - '0' <= 9)) || dot_flag) {
				return false;
			}
			dot_flag = true;
		}
		else if (s[i] == 'e' || s[i] == 'E') { //为e或者E 。 E/e的后面不能有小数
			if (i == 0 || i == n - 1) {
				return false;
			}
			else {
				for (int j = i + 1; j < n; ++j) {
					if ((s[j] == '+' || s[j] == '-') && !(j == i + 1 && j != n - 1)) {
						return false;
					}
					if ((s[j] == '.') || (s[j] - 'a' >= 0 && s[j] - 'a' < 26) || (s[j] - 'A' >= 0 && s[j] - 'A' < 26)) {
						return false;
					}
				} //遍历结束
				break;
			}

		}
		else {
			return false;
		}
	}
	return true;
}


//65. 有效数字
//有限状态机
class Legal_Number {
public:
	enum State { //枚举出所有状态
		STATE_INITIAL,
		STATE_INT_SIGN,
		STATE_INTEGER,
		STATE_POINT,
		STATE_POINT_WITHOUT_INT,
		STATE_POINT_FRACTION,  //小数部分
		STATE_EXP,
		STATE_EXP_SIGN,
		STATE_EXP_INT,
		STATE_END
	};

	enum CharType {
		CHAR_NUMBER,
		CHAR_SIGN,
		CHAR_POINT,
		CHAR_EXP,
		CHAR_ILLEGAL
	};

	CharType toCharType(char ch) { //string[i]->const char
		if (ch >= '0' && ch <= '9') {
			return CHAR_NUMBER;
		}
		else if (ch == '+' || ch == '-') {
			return CHAR_SIGN;
		}
		else if (ch == 'e' || ch == 'E') {
			return CHAR_EXP;
		}
		else if (ch == '.') {
			return CHAR_POINT;
		}
		else {
			return CHAR_ILLEGAL;
		}
	}

	bool isNumber(string s) {
		unordered_map<State, unordered_map<CharType, State>> transfer{
			//状态->数据类型->下一个状态
			//根据状态机的图来推导
			{
				STATE_INITIAL,{
					{CHAR_SIGN,STATE_INT_SIGN},
					{CHAR_POINT,STATE_POINT_WITHOUT_INT},
					{CHAR_NUMBER,STATE_INTEGER}
				}
			},
			{
				STATE_INT_SIGN,{
					{CHAR_NUMBER,STATE_INTEGER},
					{CHAR_POINT,STATE_POINT_WITHOUT_INT}
				}
			},
			{
				STATE_POINT_WITHOUT_INT,{
					{CHAR_NUMBER,STATE_POINT_FRACTION}
				}
			},
			{
				STATE_INTEGER,{
					{CHAR_NUMBER,STATE_INTEGER},
					{CHAR_EXP,STATE_EXP},
					{CHAR_POINT,STATE_POINT}
				}
			},
			{
				STATE_EXP,{
					{CHAR_NUMBER,STATE_EXP_INT},
					{CHAR_SIGN,STATE_EXP_SIGN}
				}
			},
			{
				STATE_POINT,{
					{CHAR_NUMBER,STATE_POINT_FRACTION},
					{CHAR_EXP,STATE_EXP}
				}
			},
			{
				STATE_EXP_INT,{
					{CHAR_NUMBER,STATE_EXP_INT}
				}
			},
			{
				STATE_EXP_SIGN,{
					{CHAR_NUMBER,STATE_EXP_INT}
				}
			},
			{
				STATE_POINT_FRACTION,{
					{CHAR_NUMBER,STATE_POINT_FRACTION},
					{CHAR_EXP,STATE_EXP}
				}
			}
		};

		int n = s.size();
		State st = STATE_INITIAL;
		for (int i = 0; i < n; ++i) {
			if (transfer[st].find(toCharType(s[i])) != transfer[st].end()) {
				st = transfer[st][toCharType(s[i])];
			}
			else {
				return false;
			}
		}
		return st == STATE_INTEGER || st == STATE_POINT || 
			   st == STATE_POINT_FRACTION || st == STATE_EXP_INT || st == STATE_END || st == STATE_INT_SIGN;
	}
};




//483. 最小好进制
using LL = long long;
int check(LL max_length, LL mid, LL N) {  //最大长度，当前进制，目标值
	LL sum = 0;
	for (int i = 0; i < max_length; ++i) {
		if (sum > (N - 1) / mid) {  //防止溢出
			return 1;
		}
		sum = sum * mid + 1;
	}
	if (sum == N) {
		return 0;
	}
	return sum > N ? 1 : -1;
}

string smallestGoodBase(string n) {
	//设数字n转化为a进制后全1
	//可以得到，n=a^0+a^1+...+a^(length(n)-1)
	//因为k>=2， 所以可以得到，全1的最大长度为log2(n)+2
	//穷举最大长度，二分合适每次长度的最小进制
	LL N = stoll(n);
	int max_length = (int)(log(N) / log(2) + 1); //转换为其他进制的最大长度

	for (int i = max_length; i >= 3; --i) {
		LL l = 2, r = N-1;	//找进制
		while (l <= r) {
			LL mid = l + (r - l) / 2;
			if (check(i, mid, N)>=0) {
				//减小进制
				r = mid - 1;
			}
			else {
				l = mid + 1;
			}
		}
		//验证是否等于
		if (check(i, l, N) == 0) {
			return to_string(l);
		}
	}
	return to_string(N - 1);
}


//1239. 串联字符串的最大长度
class StrLength {
private:
	int max_l;
public:
	int backtrack(vector<string>& arr, vector<int>& cnt, int index) {
		if (index == arr.size()) {
			return 0;
		}
		vector<int> temp = cnt;  //保存当前值，留给下次不选当前位置用
		for (auto a : arr[index]) {
			if (cnt[a - 'a']>0) {
				return backtrack(arr, temp, index + 1);
			}
			else {
				cnt[a - 'a']++; //出现			
			}
		}


		return max(backtrack(arr, temp, index + 1), (backtrack(arr, cnt, index + 1) + (int)arr[index].size()));
		
	}

	int maxLength(vector<string>& arr) {
		vector<int> cnt(26,0);
		int ans = backtrack(arr,cnt,0);
		return ans;
	}


	//不使用哈希表
	//使用二进制表示法来判断是否有重复数字
	int maxLength_1(vector<string>& arr) {
		vector<int> unique_subarr;
		//筛选出没有重复字母的子串
		for (auto& a : arr) {
			int m = 0;
			for (auto b : a) {
				//看m中是否包含重复字符串
				if ((m >> (b - 'a')) & 1) {
					m = 0; 
					break;
				}
				else {
					m |= (1 << (b - 'a'));
				}
			}
			if (m > 0) {
				unique_subarr.push_back(m);
			}
		}

		int ans = 0;
		//对本身没有重复字符串的子串进行匹配
		//[captures] (params) mutable-> type{...} //lambda 表达式的完整形式
		std::function<void(int, int)> backtrack = [&](int index, int rec) {  // index表示当前位置, rec记录字符使用情况便于判重
			if (index == (int)unique_subarr.size()) { //边界条件
				ans = max(ans, (int)__builtin_popcount(rec));
				return;
			}
			//选
			if ((unique_subarr[index] & rec) == 0) { //=0表示无重复元素
				backtrack(index + 1, rec | unique_subarr[index]);
			}
			//不选
			backtrack(index + 1, rec);
		};
		
		backtrack(0, 0);
		return ans;
	}

};


class BinaryWatch {
private:
	vector<string> ans;
public:
	string transfer(int num) {
		int hour = (num & 0b1111000000)>>6;
		int minute = (num & 0b111111);
		if (hour >= 12) {
			return "";
		}
		if (minute >= 60) {
			return "";
		}

		string time;
		time += to_string(hour);
		if (minute < 10) {
			time += ":0";
			time += to_string(minute);
		}
		else {
			time += ":";
			time += to_string(minute);
		}
		return time;
	}

	void backtrack(int choose, int index, int length, int allDigit) {
		if (length == 0) {
			string temp = transfer(choose);
			if (temp != "") {
				ans.push_back(temp);
			}
			return;
		}
		
		if (allDigit - index<length) {
			return;
		}

		//不选与选
		backtrack(choose, index + 1, length, allDigit);
		choose |= (1 << index);
		backtrack(choose, index + 1, length-1, allDigit);
		return;
	}

	vector<string> readBinaryWatch(int turnedOn) { //hour 0-11  minute 0-59
		//turnedOn表示亮灯的个数
		//vector<int> vec = { 1,2,4,8,1,2,4,8,16,32 };
		if (turnedOn > 8) {
			return ans;
		}
		backtrack(0, 0, turnedOn, 10);
		return ans;
	}
};


// 剑指 Offer 38. 字符串的排列
class Perms {
private:
	vector<string> ans;
	vector<int> visited;
public:
	void backtrack(string& s, int index, int n, string& temp) {
		if (index == n) {
			ans.emplace_back(temp);
			return;
		}

		for (int i = 0; i < n; ++i) {
			if (visited[i] || (i > 0 && visited[i - 1] && s[i] == s[i - 1])) {  //当前位置计算过，或者当前位置和前一个数位置一样
				continue;
			}
			visited[i] = 1;
			temp.push_back(s[i]);
			backtrack(s, index + 1, n, temp);
			temp.pop_back();
			visited[i] = 0;
		}
	}

	vector<string> permutation(string s) {
		int n = s.size();
		visited.resize(n, 0);
		string temp = "";
		sort(s.begin(), s.end());  //将字符串排序后，相同的字符都相邻，为了去除重复的情况，每种重复字符，只计算最左侧出现的一个
		backtrack(s, 0, n, temp);
		return ans;
	}
};


//31. 下一个排列
void nextPermutation(vector<int>& nums) {

	//=====此块测试STL函数 std::next_permutation
	vector<int> temp = { 1,2,3 };
	do {
		for (auto a : temp) {
			cout << a << "\t";
		}
		cout << endl;
	} while (std::next_permutation(temp.begin(),temp.end()));

	int temp2[] = { 1,2,3 };
	do {
		cout << temp2[0] << "\t" << temp2[1] << "\t" << temp2[2] << endl;
	} while (next_permutation(temp2, temp2 + 3));
	//===========================================

	int n = nums.size();
	int first_less = nums.size() - 1;
	int first_large = nums.size() - 1;
	while (first_less > 0 && nums[first_less] <= nums[first_less - 1]) {
		first_less--;
	}
	if (first_less >= 1) {  //当序列为5 4 3 2 1   first_less为0
		while (first_large >= first_less && nums[first_large] <= nums[first_less - 1]) {
			first_large--;
		}
		swap(nums[first_large], nums[first_less-1]);
		//swap很重要，当顺序为1 2 3 4 5 ， swap直接调整为1 2 3 5 4
		//解决初始问题
	}
	reverse(nums.begin() + first_less, nums.end());
}

//1906. 查询差绝对值的最小值
//   1 <= nums[i] <= 100
vector<int> minDifference(vector<int>& nums, vector<vector<int>>& queries) {
	//考虑一个数组中的差的最小值，当一个数组有序，那么相邻的两个数字有最小的差值
	//使用pre_sum[i][j]表示在nums[0-i]中j出现的数目
	int n = nums.size();
	int max_num = 100;
	vector<vector<int>> pre_sum(n + 1, vector<int>(101,0));

	for (int i = 0; i < n; i++) {
		copy_n(pre_sum[i].begin(), 101, pre_sum[i + 1].begin());
		++pre_sum[i + 1][nums[i]];
	}

	int m = queries.size();
	vector<int>ans;
	for (int i = 0; i < m; ++i) {
		int l = queries[i][0], r = queries[i][1];
		int temp = INT_MAX;
		int last_ele = 0;
		for(int j=1;j<=100;++j){
			if (pre_sum[r + 1][j] != pre_sum[l][j]) {
				if (last_ele) {
					temp = min(temp, j - last_ele);
				}
				last_ele = j;
			}
		}
		if (temp == INT_MAX) {
			temp = -1;
		}
		ans.emplace_back(temp);
	}
	return ans;
}


//1905. 统计子岛屿
//dfs / bfs
class ssss {
private:
	vector<vector<int>> move = { {0, -1}, {-1, 0}, {1, 0}, {0, 1} };
	vector<vector<int>> visited;
public:
	void que_clear(queue<pair<int, int>>& que) {
		queue<pair<int, int>> empty;
		swap(que, empty);
	}

	int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
		int m = grid1.size(), n = grid1[0].size();
		visited.resize(m, vector<int>(n));

		int ans = 0;
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				if (visited[i][j] || grid2[i][j] == 0) {
					continue;
				}
				visited[i][j] = 1;
				queue<pair<int, int>> que;
				que.push(make_pair(i, j));
				int count = 1;

				while (!que.empty()) {
					auto temp = que.front();
					que.pop();
					if (grid1[temp.first][temp.second] == 0) {
						count = 0;
					}
					for (int k = 0; k < 4; ++k) {
						int new_x = temp.first + move[k][0];
						int new_y = temp.second + move[k][1];
						if (new_x < 0 || new_x >= m || new_y < 0 || new_y >= n || 
							visited[new_x][new_y] == 1 || grid2[new_x][new_y]==0) {
							continue;
						}
						visited[new_x][new_y] = 1;
						que.push(make_pair(new_x, new_y));
					}
				}
				ans += (count == 0 ? 0 : 1);
			}
		}
		return ans;
	}
};

//X-Y坐标系里，求最多有多少个点在同一条直线上。
int gcd(int x, int y) {
	return y == 0 ? x : (gcd(y, x % y));
}

int maxPoints(vector<vector<int>>& points) {
	int n = points.size();
	if (n <= 2) {
		return n;
	}
	int ans = 0;
	for (int i = 0; i < n; ++i) {
		if (ans > n / 2) {
			return ans;
		}
		unordered_map<string, int> mp;
		//斜率写为delta_y/delta_x，数据类型为float，会出现精度问题
		//将其改写为  delta_x/gcd(delta_x,delta_y)  delta_y/gcd(delta_x,delta_y)  
		for (int j = i + 1; j < n; ++j) {
			int del_y = points[j][1] - points[i][1];
			int del_x = points[j][1] - points[i][0];
			int temp = gcd(del_x, del_y);
			del_x = del_x / temp;
			del_y = del_y / temp;
			string str = to_string(del_x) + "_" + to_string(del_y);
			mp[str]++;
			ans = max(ans, mp[str]);
		}
	}
	return ans;
}

//752. 打开转盘锁
using PSI = pair<string, int>;
int openLock(vector<string>& deadends, string target) {
	if (target == "0000") {
		return 0;
	}
	unordered_set<string> deadcodes(deadends.begin(), deadends.end());
	if (deadcodes.count("0000")) {
		return -1;
	}

	//前移
	auto num_pri = [=](char x)->char {
		return x == '9' ? '0' : x + 1;
	};
	//后移
	auto num_pos = [=](char x)->char {
		return x == '0' ? '9' : x - 1;
	};

	//遍历所有的情况并存储
	auto gets = [&](string& s)->vector<string> {
		vector<string> temp;
		for (int i = 0; i < 4; ++i) { //每一位
			auto a = s[i];
			s[i] = num_pri(a);
			temp.push_back(s);
			s[i] = num_pos(a);
			temp.push_back(s);
			s[i] = a;
		}
		return temp;
	};

	queue<PSI> que;
	unordered_set<string> visited = {"0000"};
	que.emplace("0000", 0);
	while (!que.empty()) {
		auto [status, step] = que.front();
		que.pop();
		vector<string> vec = gets(status);
		for (auto a : vec) {
			if (!visited.count(a) && !deadcodes.count(a)) {
				if (a == target) {
					cout << step + 1 << endl;
					return step + 1;
				}
				visited.insert(a);
				que.emplace(a, step + 1);
			}
		}
	}
	return -1;
}


//773. 滑动谜题
// 少一块的拼图
/*
	bfs进行计算， 保存每一步可能走向的位置。   拼图大小  2 x 3
*/
/*
	要求结果为    1  2  3    ===》 编号   0  1  2
				4  5  0                3  4  5
			可以看到，每个位置能移动到的下一个位置为   
			0 -> 1  3
			1 -> 0  2  4
			2 -> 1  5
			3 -> 0  4
			4 -> 1  3  5 
			5 -> 2  4
	 因此，当找到初始值0的位置，那么便可得到下一次的位置
	*/
vector<vector<int>> next_step{ {1,3},{0,2,4},{1,5},{0,4},{1,3,5},{2,4} };
int slidingPuzzle(vector<vector<int>>& board) {
	//next step
	string target = "123450";
	auto trans = [&](string& s)->vector<string> {
		vector<string> vec{};
		int zero_index = s.find('0');
		for (auto a : next_step[zero_index]) {
			string ss = s;
			swap(s[a], s[zero_index]);
			vec.emplace_back(s);
			s = ss;
		}
		return vec;
	};

	string initial = "";
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 3; ++j) {
			initial += (board[i][j] + '0');
		}
	}
	if (initial == target) {
		return 0;
	}

	queue<pair<string,int>> que;
	que.emplace(initial,0);
	unordered_set<string> us{ initial };
	while (!que.empty()) {
		auto [begin, step] = que.front();
		que.pop();
		vector<string> temp_vec = trans(begin);
		for (auto aa : temp_vec) {
			if (!us.count(aa)) {
				if (aa == target) {
					return step + 1;
				}
				que.emplace(aa, step + 1);
				us.insert(move(aa));  //直接拷贝，节省时间
				//move(a) 之后，a会变为空
				//一定要先存到que再使用move存储
			}
		}
	}

	return -1;
}



//5799. 最美子字符串的数目      ////////字符串类前缀和，没掌握
/*
1.只考虑每个字母频率的奇偶性，就只考虑模2后的结果即可

只有0和1两种情况

2.只有10个字母，又只有0和1两种情况

用状态压缩

3.如果是区间dp的思想，就可能做不出
用前缀dp的思路，就ok

4.前缀和加速查找当前位置，前缀区间，某个状态的值

5.题目最多只有一个字母的频率为奇数====只有一个字母的奇偶性不同====state只有一位不同
（1）0个字母的频率为奇数
（2）10个字母中，某个字母的频率为奇数
共11种情况

6.初始化
presum_state[0] = 1
一个字母都不选的时候，state = 0000000000，种类数为1
*/
long long wonderfulSubstrings(string word) {    //《===这个方法没理解     类似dp？
	int n = word.size();
	//区间问题，考虑前缀和
	vector<int> pre(1<<10); //保存所有的情况
	pre[0] = 1;
	int state = 0;
	int ans = 0;
	for (auto w : word) {
		int t = 1 << (w - 'a'); 
		state = state ^ t; //当前前缀和
		ans += pre[state];//所有字符均出现偶数次
		//有一个字母出现奇数次
		for (int i = 1; i < (1 << 10); i = i<<1) {
			ans += pre[state ^ i];  //枚举第i个字母出现的奇偶性
		}
		pre[state]++;
	}
	return ans;
}


//909. 蛇梯棋
/*
bfs
queue存储的是每一次所能到达的下一步，期间遇到梯子或者蛇，下一步直接变为蛇或梯子的目标点
普通走的话，每次只能向前走1~6步

虽然棋盘的编号是反的，但是假装左下角为（0，0）， 从下到上顺序递增就行
*/
using TIII = tuple<int, int, int>;
int snakesAndLadders(vector<vector<int>>& board) {
	int n = board.size();
	//穷举下一步
	auto nextStep = [&](int& a)->vector<TIII> {
		vector<TIII> vec;
		for (int i = 1; i <= 6; ++i) {
			if (a + i > n * n) {
				continue;
			}
			int x = (a - 1 + i) / n, y = (a - 1 + i) % n;  // 新的行 列 下标
			if (x % 2 == 1) { //从下往上，奇数行逆向 最下面一行编号为0
				y = n - 1 - y;
			}
			vec.emplace_back(n - 1 - x, y, a + i); //下一步的横纵坐标, 和编号
			//行号变为 n-1-x
		}
		reverse(vec.begin(), vec.end());
		return vec;
	};
	vector<int> visited(n * n);
	visited[1] = 1;
	queue<PII> que;
	unordered_set<int> us{ 1 };
	que.emplace(1,0);
	while (!que.empty()) {
		auto [begin,step] = que.front(); //初始为1
		que.pop();
		auto temp = nextStep(begin);
		for (auto a : temp) {
			int next = get<2>(a);
			if (board[std::get<0>(a)][get<1>(a)] > 0) { //存在蛇或者梯子，那么直接跳跃
				next = board[std::get<0>(a)][get<1>(a)];
			}
			if (next == n * n) {
				return step + 1;
			}
			if (!visited[next]) {
				visited[next] = 1;
				que.emplace(next, step + 1);
			}
		}
	}
	return -1;
}

//LCP 07. 传递信息 easy
int numWays(int n, vector<vector<int>>& relation, int k) {
	//要求经过k轮传递到最后一个人
	//dp[i][j]表示经过i轮传到j的方案数
	//dp[0][0]=1
	vector<vector<int>> dp(k+1, vector<int>(n));
	dp[0][0] = 1;
	for (int i = 1; i <= k; ++i) {
		for (auto a : relation) {
			dp[i][a[1]] += dp[i - 1][a[0]];
		}
	}
	return dp[k][n-1];
}


//1418. 点菜展示表
vector<vector<string>> displayTable(vector<vector<string>>& orders) {
	//保存菜品
	//保存桌号以及每桌所点的菜品和数量
	unordered_map<int, unordered_map<string, int>> table;
	unordered_set<string> food;

	//提取orders的内容
	for (auto& a : orders) {
		table[atoi(a[1].c_str())][a[2]]++;
		food.emplace(a[2]);
	}

	vector<string> food_name{ "Table" };
	for (auto& a : food) {
		food_name.emplace_back(a);
	}

	sort(food_name.begin() + 1, food_name.end());

	vector<vector<string>> ans;

	ans.emplace_back(food_name);

	for (int i = 1; i <= 500; ++i) {
		if (!table[i].empty()) {
			vector<string> temp;
			temp.emplace_back(to_string(i));
			for (int j = 1; j < food_name.size(); ++j) {
				//if(table[i][a]!=0){
				temp.emplace_back(to_string(table[i][food_name[j]]));
				//}//else{
					//temp.emplace_back("0");
				//}
			}
			ans.emplace_back(temp);
		}
	}
	return ans;
}


//726. 原子的数量
/*
给定一个化学式formula（作为字符串），返回每种原子的数量。

原子总是以一个大写字母开始，接着跟随0个或任意个小写字母，表示原子的名字。
如果数量大于 1，原子后会跟着数字表示原子的数量。如果数量等于 1 则不会跟数字。H2O 和 H2O2 是可行的，但 H1O2 这个表达是不可行的。

两个化学式连在一起是新的化学式
一个括号中的化学式和数字（可选择性添加）也是化学式
*/
string countOfAtoms(string formula) {
	int n = formula.size();
	int index = 0;
	auto findAtom = [&]()->string {
		string temp = "";
		temp += formula[index++]; //保存第一个大写字母
		while (index < n && islower(formula[index])) {
			temp += formula[index];   //小写的也加上
			index++;
		}
		//原子统计完了
		return temp;
	};

	//原子之后为数字，数字>=1    ，为1时，不显示
	auto findNum = [&]()->int {
		if (index == n || !isdigit(formula[index])) {
			//结束了或者原子的后一位不为数字
			//那么说明原子只有一个
			return 1;
		}
		//原子后有数字
		int ans = 0;
		while (index < n && isdigit(formula[index])) {
			ans = ans * 10 + (formula[index] - '0');
			index++;
		}
		return ans;
	};

	//因为要处理括号，所以选择stack
	stack<unordered_map<string, int>> st;
	st.push({});
	while (index < n) {
		if (formula[index] == '(') {
			st.push({});  //存储当前的原子
			index++;
		}
		else if (formula[index] == ')') {
			//当前原子已经走完了
			//看看括号外有没有数字
			index++;
			int right_num = findNum();
			auto temp = st.top();
			//去掉括号，加到上一层
			st.pop();
			for (auto& [atom,count] : temp) {
				st.top()[atom] += count * right_num;
			}
		}
		else {
			string atom = findAtom();
			int num = findNum();
			//加到当前原子的哈希表中
			st.top()[atom] += num;
		}
	}

	//st中为所有的原子数
	vector<pair<string, int>> vec;
	auto temp = st.top();
	for (auto& [atom, num] : temp) {
		vec.emplace_back(atom, num);
	}

	sort(vec.begin(), vec.end());

	string ans = "";
	for (auto a : vec) {
		ans += a.first;
		if (a.second > 1) {
			ans += (to_string(a.second));
		}
	}
	return ans;

}

//981. 基于时间的键值存储
class TimeMap {
public:
	unordered_map<string, vector<pair<int, string>>> mp;
	/** Initialize your data structure here. */
	TimeMap() {

	}

	void set(string key, string value, int timestamp) {
		mp[key].emplace_back(timestamp, value);
	}
	//所有的键/值字符串都是小写的。
	string get(string key, int timestamp) {
		auto& a = mp[key];
		auto index = upper_bound(a.begin(), a.end(), make_pair(timestamp, string({127})));
		//string({127})为最大的字符串
		//string之间的比较，  c与abc    c为99  a为97  .  当c与abc之间比较时，按最短的长度来比较，即只比较 a 97  与  c 99  =>> abc<c
		//string({123-127})均可以，因为c为122
		if (index > a.begin()) {
			return (index - 1)->second;
		}
		return "";
	}
};




class AAA {
public:
	int ans = 0;
	unordered_set<string> st;
	int isPalindrome(string str) {
		int n = str.size();
		int l = 0, r = n - 1;
		while (l <= r) {
			if (str[l] != str[r]) {
				return 0;
			}
			l++;
			r--;
		}
		return 1;
	}

	void bfs(int count, string& s, string& temp, int judge) {
		if (judge == s.size()) {
			if (count ==3 && isPalindrome(temp)) {
				st.insert(temp);
			}
			return;
		}

		if (judge < s.size()&&count==3) {
			if (isPalindrome(temp)) {
				st.insert(temp);
			}
			return;
		}

		//选
		
		bfs(count + 1, s, temp += s[judge], judge + 1);
		temp.pop_back();
		
		//不选
		bfs(count, s, temp, judge + 1);
	}

	int countPalindromicSubsequence(string s) {
		// 长度为3
		string temp = "";
		bfs(0, s, temp, 0);
		ans = st.size();
		return ans;
	}
};


//930. 和相同的二元子数组
//前缀和+map
int numSubarraysWithSum(vector<int>& nums, int goal) {
	// mp里保存的是sum-goal
	int ans = 0;
	int sum = 0;
	unordered_map<int, int> mp;
	for (auto& a : nums) {
		mp[sum]++;  //sum == goal的时候，为0，所以0要记一次
		sum += a;
		ans += mp[sum - goal];  //<--逆向思维
	}
	return ans;
}


//218. 天际线问题
//1、扫描线
//2、线段树
vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
	auto cmp = [&](const pair<int, int>& a, pair<int, int>& b)->bool {return a.second < b.second; };
	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> que(cmp);
	//priority_queue的第三个参数为class类型，decltype自动推断类型

	vector<int> index;
	for (auto& a : buildings) {
		index.emplace_back(a[0]);
		index.emplace_back(a[1]);
	}
	sort(index.begin(), index.end());  //所有左右端点排序 

	vector<vector<int>>ans;
	int n = buildings.size();
	int idx = 0;
	//按照所有的坐标，划分成的矩形，对于每个矩形，找到其中的最大值（因为可能同一位置有多栋建筑）
	for (auto& a : index) {  //遍历矩形
		while (idx < n && buildings[idx][0] <= a) {   //while防止了同一左端点上有多栋建筑的情况
			que.emplace(buildings[idx][1], buildings[idx][2]);  //存入右端点和高度
			idx++;
		}
		//若所有建筑左端点都不重叠的话，那么while可以写为if
		while (!que.empty() && que.top().first <= a) {
			que.pop();  //<==说明已经计算过
		}
		int highest = que.empty() ? 0 : que.top().second;
		if (ans.size() == 0 || highest != ans.back()[1]) {
			ans.push_back({ a,highest });
		}
	}
	return ans;
}

//1818. 绝对差值和
//对于取MOD后的操作，很巧妙
static constexpr int MOD = 1000000007;
int minAbsoluteSumDiff(vector<int>& nums1, vector<int>& nums2) {
	//排完序再找对于每个nums[i]最接近的nums[j]
	//使得| |nums1[i]-nums2[i]| - |nums1[j]-nums2[j]| |有最大值
	vector<int> vec(nums1);
	sort(vec.begin(), vec.end());

	int max_gap = 0;
	int n = nums1.size();
	int sum = 0;
	for (int i = 0; i < n; ++i) {
		int i_gap = abs(nums1[i] - nums2[i]);
		sum = (sum + i_gap) % MOD;
		int j = lower_bound(vec.begin(), vec.end(), nums2[i]) - vec.begin();
		//找到nums1中距离nums2[i]最近的一个数
		if (j < n) {
			max_gap = max(max_gap, i_gap - abs(vec[j] - nums2[i]));
		}
		if (j > 0) {  //试j的前一个
			max_gap = max(max_gap, i_gap - abs(vec[j - 1] - nums2[i]));
		}
	}
	//sum因为取了mod，因此会出现没取模前sum>max_gap， 取模后sum<max_gap，实际上少了一个MOD
	//因此需要加上mod
	return (abs(sum - max_gap + MOD)) % MOD;
}


//1846. 减小和重新排列数组后的最大元素
int maximumElementAfterDecrementingAndRearranging(vector<int>& arr) {
	int n = arr.size();
	vector<int> count(n + 1);  //存储1~n的数出现的次数
	int have = 0;
	for (auto a : arr) {
		count[min(a, n)]++; //<==min(a,n)的考虑了当a大于n的情况,很巧妙
	}
	int miss = 0;
	int tag = 1;
	for (int i = 1; i <= n; ++i) {
		if (count[i] == 0) {
			miss++;
		}
		else {//要注意只能变为比当前值小的值
			miss -= min(miss, count[i] - 1);
			//在当前位置之前有miss个空缺，最多只能消除miss个，因为只能变为比当前值小的值
		}
	}
	return n - miss;
	//一共有n个数，若miss=1即缺失了1个，那么当前的最大值肯定为4，又因为只能填充比自己小的数
	//所以最大值为4
}


//815. 公交路线
//因为数据量太大，若存储每个路线对应的站点，那么遍历的数量级太大
//因此存储每个站点对应的路线，牺牲一点空间换取大量的时间
int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
	if (target == source) {
		return 0;
	}
	int n = routes.size();
	unordered_map<int, vector<int>> rec;
	//存储每个站点包含哪些路线
	for (int i = 0; i < n; ++i) {
		for (auto& a : routes[i]) {
			rec[a].push_back(i);
		}
	}
	//存储完毕后，准备进行bfs
	vector<int> visited(n, 0);//记录线路的访问情况
	queue<int> que;
	for (auto& a : rec[source]) {
		visited[a] = 1;
		que.push(a);
	}
	int step = 1;
	while (!que.empty()) {
		int m = que.size();
		for (int i = 0; i < m; ++i) {
			int tp = que.front();
			que.pop();
			for (auto& a : routes[tp]) {   //路线里有哪些点
				if (a == target) {
					return step;
				}
				for (auto& b : rec[a]) { //存储每个点对应的新路线					
					if (!visited[b]) {
						visited[b] = 1;
						que.push(b);
					}
				}
			}
		}
		step++;
	}
	return -1;
}


//线段树的思想递归求解
//53. 最大子序和     (数组中的一个或连续多个整数组成一个子数组。求所有子数组的和的最大值。)
//同 剑指 Offer 42. 连续子数组的最大和 
class SegmentTree_exp1 {
public:
	struct Treeproperty {
		int lsum, rsum, isum, msum;
		//lsum is the largest sum of sub segments with l is the left endpoint
		//rsum have the same mean
		//isum is the largest sum of sub segments with l is the left endpoint and r is the right endpoint
		//msum is interval sum
	};

	Treeproperty pullUp(Treeproperty l, Treeproperty r) { //区间长度变为1后，向上合并
		int isum = l.isum + r.isum;
		int lsum = max(l.lsum, l.isum + r.lsum);
		int rsum = max(r.rsum, l.rsum + r.isum);
		int msum = max(max(l.msum, r.msum), l.rsum + r.lsum);
		//两个区间合并已经完成
		return Treeproperty{ lsum,rsum,isum,msum };
	}

	Treeproperty getValue(vector<int>& nums, int left, int right) {  //迭代到区间长度为1
		if (left == right) {
			return Treeproperty{nums[left], nums[left], nums[left], nums[left] };
		}
		int mid = left + (right - left) / 2;
		Treeproperty lSub = getValue(nums, left, mid);
		Treeproperty rSub = getValue(nums, mid + 1, right);
		//划分完成
		return pullUp(lSub, rSub);
	}



	int maxSubArray(vector<int>& nums) {
		int n = nums.size();
		Treeproperty ans = getValue(nums, 0, n - 1);
		return ans.msum;
	}
};



/**/
int main() {
	//clock_t start, end;
	//start = clock();

	//vector<int> weight = { 3,4,5,3,6 };
	//vector<int> value = { 4,5,6,3,5 };

	//bagProblemcomp(weight, value, 5, 10);


	//numSquares(12);


	//vector<int> piles = { 5,3,4,5 };
	//Stone sto;
	//sto.stoneGame(piles);


	vector<vector<int>> routes = { {0, 1, 6, 16, 22, 23},{4, 10, 12, 20, 24, 28, 33},{14, 15, 24, 32},{1, 10, 11, 19, 27, 33},{15, 20, 21, 23, 29},{29} };
	numBusesToDestination(routes, 4, 21);




	return 0;
}




	//AAA a;
	//a.countPalindromicSubsequence("nhzosdwmwomlevcctvopoiiayudhvauitqutiboveumsqvbulhbfbynzogtejuwi");

	//string formula = "K4(ON(SO3)2)2";
	//countOfAtoms(formula);


	//vector<vector<string>> orders = { {"David", "3", "Ceviche"},{"Corina", "10", "Beef Burrito"},{"David", "3", "Fried Chicken"},{"Carla", "5", "Water"},{"Carla", "5", "Ceviche"},{"Rous", "3", "Ceviche"} };
	//displayTable(orders);

	//int n = 5;
	//vector<vector<int>> relation = { {0, 2},{2, 1},{3, 4},{2, 3},{1, 4},{2, 0},{0, 4} };
	//int k = 3;
	//numWays(n, relation, k);


	/*vector<vector<int>> vec = {
		{-1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1 },
		{-1, -1, -1, -1, -1, -1},
		{ -1, 35, -1, -1, 13, -1 },
		{-1, -1, -1, -1, -1, -1},
		{ -1, 15, -1, -1, -1, -1 }};
	snakesAndLadders(vec);*/

	//slidingPuzzle(vec);

	//string a = "aabb";
	//wonderfulSubstrings(a);

	//vector<string> lock = { "8888" };
	//string s = "0009";
	//openLock(lock, s);

	//ssss s;
	//vector<vector<int>> g1 = { {1, 0, 1, 0, 1},{1, 1, 1, 1, 1},{0, 0, 0, 0, 0},{1, 1, 1, 1, 1},{1, 0, 1, 0, 1} };
	//vector<vector<int>> g2 = { {0, 0, 0, 0, 0} , {1, 1, 1, 1, 1},{0, 1, 0, 1, 0},{0, 1, 0, 1, 0},{1, 0, 0, 0, 1} };
	///s.countSubIslands(g1, g2);


	//vector<int> nums = { 1,3,4,8 };
	//vector<vector<int>> queries = { {0,1},{1,2},{2,3},{0,3} };
	//minDifference(nums, queries);

	//vector<int>nums = { 1,5,1 };
	//nextPermutation(nums);

	//Perms pe;
	//string s = "abc";
	//pe.permutation(s);

	//BinaryWatch bw;
	//bw.readBinaryWatch(7);

	////vector<string> arr = { "abcdefghijklm", "bcdefghijklmn", "cdefghijklmno", "defghijklmnop", "efghijklmnopq", "fghijklmnopqr", "ghijklmnopqrs", "hijklmnopqrst", "ijklmnopqrstu", "jklmnopqrstuv", "klmnopqrstuvw", "lmnopqrstuvwx", "mnopqrstuvwxy", "nopqrstuvwxyz", "opqrstuvwxyza", "pqrstuvwxyzab" };
	//StrLength sl;
	//sl.maxLength_1(arr);


	//string s = "1000000000000000000";
	//smallestGoodBase(s);

	//string s = "3.";
	//Legal_Number ln;
	//ln.isNumber(s);


	//vector<vector<int>> matrix = { {8,1, 6},{3,5,7},{4,9,2},{7,10,9} };
	//largestMagicSquare(matrix);


	//vector<vector<int>> node = { {0, 1},{0, 2},{2, 3},{3, 4} };
	//largestPathValue("abaca", node);

	//vector<int> cost = { 2,4,6,2,4,6,4,4,4 };
	//int target = 5;
	//largestNumber(cost, target);


	//int amount = 5;
	//vector<int> coins = { 1,2,5 };
	//change(amount, coins);

	//int n = 10, minProfit = 5;
	//vector<int> group = { 2, 3, 5 }, profit = { 6, 7, 8 };
	//profitableSchemes(n, minProfit, group, profit);

	//minFlips("11100");

	//vector<string> strs = { "10", "0001", "111001", "1", "0" };
	//findMaxForm(strs, 5, 3);

	//vector<int> nums = { 5,1,3 };
	//reductionOperations(nums);

	//vector<int> stones = { 1,1,4,2,2 };
	//lastStoneWeightII(stones);

	//targetsum tg;
	//vector<int> nums{ 1,1,1,1,1 };
	//tg.findTargetSumWays_dp(nums, 3);

	//vector<int> nums = { 23,2,4,6,6 };
	//int k = 7;
	//checkSubarraySum(nums, k);



	//vector<int> nums = { 0,1,0,1,0,1 };
	//findMaxLength(nums);

	//vector<int> servers = { 74,57,61,82,67,97,67,21,61,79,21,50,14,88,48,52,76,64 }, tasks = { 21,100,48,64,20,8,28,10,3,63,7 };
	//TasksProcessing tp;
	//tp.assignTasks(servers,tasks);


	//vector<int> nums = { 4,14,2 };
	//totalHammingDistance(nums);


	//vector<int> nums = { 1,2,3 };
	//Combination_Sum cb;
	//cb.combinationSum4_2(nums, 4);


	//vector<int> nums = { 1,2,3,4 };
	//productExceptSelf(nums);

	//string s = "010010"; 
	//int minJump = 2, maxJump = 3;
	//canReach(s, minJump, maxJump);

	//string s = "(ed(et(oc))el)";
	//reverseParentheses(s);


	//string s = "aaabbb";
	//strangePrinter(s);

	//string s = "111000";
	//checkZeroOnes(s);

	//vector<int> a = { 1,1,100000 };
	//double hours = 2.01;
	//minSpeedOnTime(a, hours);

	//vector<int> nums1 = { 1,4,2 };
	//vector<int> nums2 = { 1,2,4 };
	//maxUncrossedLines(nums1, nums2);

/*
	auto cmp = [](const auto& a, const auto& b) {
		return a > b;
	};
	priority_queue< int, vector<int>, decltype(cmp)> que(cmp);  
	//优先队列的排序函数是反着来的，定义的return a>b,但是却是小根堆
	
	vector<int> nums = { 1,6,8,9,3,54,6,8,9,3,2 };
	for (auto a : nums) {
		que.push(a);
	}*/


	
	//vector<string> words = { "i", "love", "leetcode", "i", "love", "coding" };
	//int k = 2;
	//topKFrequent(words,k);


	//vector<int> nums = { 3,2,1,5,6,4 };
	//quickSelect qs;
	//qs.findKthLargest(nums,4);

	//vector<int> nums = { 2,3,1,6,7 };
	//countTriplets(nums);


	//vector<int> nums = { 3, 10, 5, 25, 2, 8 };
	//findMaximumXOR(nums);

	//rearrangeSticks(5, 4);


	//string s = "010";
	//minSwaps(s);

	//vector<int> nums = { 5,1,6 };
	//subsetXORSum(nums);


	//vector<vector<string>>box = { {"#", "#", "*", ".", "*", "."},
		//{"#", "#", "#", "*", ".", "." },
		//{"#", "#", "#", ".", "#", "."} };

	//rotateTheBox(box);

	//vector<int> g = { 8,5,2,9,1 };
	//vector<int> s = { 0,0,0,0,0 };
	//findContentChildren(g, s);


	//vector<int> nums = { 1,1,3,6,8 };
	//minOperations(nums);

	//string s = "-43";
	//myAtoi(s);


	//string s1 = "great", s2 = "rgeat";
	//scramble sc;
	//sc.isScramble2(s1, s2);

	//AAA A;
	//A.maxSumMinProduct(n1);

	//vector<int> nums = { 2,3,3,1,2 };
	//maxSumMinProduct(nums);

	//vector<int> encoded = { 6,5,4,6 };
	//decode(encoded);


	//vector<int> bloomDays = { 1,10,3,10,2 };
	//Flowers fl;
	//fl.minDays(bloomDays,3,1);

	//Frog fr;
	//vector<int> stones = { 0,1,3,5,6,8,12,17 };
	//fr.canCross(stones);

	//vector<int> n1 = { 55,30,5,4,2 };
	//vector<int> n2 = { 100,20,10,10,5 };
	//maxDistance(n1, n2);

	//vector<vector<int>> logs = { {1982, 1998}, {2013, 2042},{2010, 2035},{2022, 2050},{2047, 2048} };
	//maximumPopulation(logs);


	//vector<int> jobs = { 1,2,4,7,8 };
	//Jobs jbs;
	//jbs.minimumTimeRequired(jobs, 2);


	//vector<vector<int>> walls = { {1, 2, 2, 1},{3, 1, 2},{1, 3, 2},{2, 4},{3, 1, 2},{1, 3, 1, 1} };
	//leastBricks(walls);


	//vector<int> nums = { 3,4,2 };
	//deleteAndEarn(nums);


	//judgeSquareSum(8);

	//vector<int> nums = { 9,18,54,90,108,180,360,540,720 };
	//largestDivisibleSubset(nums);


	//vector<vector<int>> matrix = { {1, 0, 1},{0, -2, 3} };
	//int k = 2;
	//maxSumSubmatrix(matrix, k);


	//vector<vector<int>> tasks = { {1,2},{2,4},{3,2},{4,1} };
	//getOrder(tasks);


	//nthUglyNumber_case2(10);

	//vector<int> nums = { 2,3,2,6,2,5,9,0,1,3 };
	//rob(nums);



	//vector<int> nums = { 0,2,2,1,0,3,0,3,0,1,3,1,1,0,1,3,1,1,1,0,2,0,0,3,3,0,3,2,2,0,0,3,3,3,0,0,2,0,0,3,3,0,3,3,0,0,3,1,0,1,0,2,3,1,1,0,3,3,0,3,1,3,0,2,2,0,1,3,0,1,0,3,0,1,3,1,2,2,0,0,3,0,1,3,2,3,2,1,0,3,2,2,0,3,3,0,3,0,0,1,0 };
	//minSideJumps(nums);


	//findTheWinner(5, 2);


	//vector<int> nums = {5,1,3};
	//search(nums, 3);

	//vector<int> m1 = { 0,0,1,1,1 };
	//numRabbits(m1);

	//Perm pm;
	//vector<int> nums = { 1,2,3 };
	//pm.subsets(nums);


	//vector<int> nums = { 3, 10, 5, 25, 2, 8 };
	//findMaximumXOR(nums);


	//Solution22 sol22;
	/*vector<vector<char>> board =
		{
			{'S', 'F', 'C', 'S'},
			{'A', 'D', 'E', 'E'},
			{'S', 'F', 'C', 'S'},
			{'A', 'D', 'E', 'E'},
			{'A', 'B', 'C', 'E'},
			{'S', 'F', 'C', 'S'},
			{'A', 'D', 'E', 'E'}
		};*/
//sol22.exist(board,"ABCBEFG");

	//vector<int> nums = { 1,0,1,-4,-3 };
	//find132pattern(nums);



	//maxValue(2,1,21);

	//vector<vector<int>> orders = { {7, 1000000000, 1},{15, 3, 0},{5, 999999995, 0},{5, 1, 1} };
	//getNumberOfBacklogOrders(orders);

	//vector<string> s = { "10","6","9","3","+","-11","*","/","*","17","+","5","+" };
	//evalRPN(s);


	//int n = 12222333;
	//nextGreaterElement(n);

	//string text1 = "abcdef", text2 = "def";
	//longestCommonSubsequence(text1, text2);

	//string s = "rabbbit";
	//string t = "rabbit";
	//dpAndMemory DM;
	//DM.numDistinct(s, t);

	//vector<int> a = { 1,2,1,7,8,9 };
	//nextGreaterElements(a);

	//vector<vector<int>> env = { {4,5},{4,6},{6,7},{2,3},{1,1} };
	//maxEnvelopes(env);

	//vector<int> nums = { 7,7,7,7,7,7,7,7,7,7 };
	//lengthOfLIS(nums);

	//string s = "9,#,92,#,#";
	//isValidSerialization(s);

	//string S = "abbacd";
	//removeDuplicates(S);

	//string s = "3+2*2";
	//calculate2(s);

	//string s = " 2-1 + 2 ";
	//calculate(s);

	//string s = "aabcb";
	//beautySum(s);

	//string s = "aab";
	//minCut(s);

	//string s = "cdbcbbaaabab";
	//int x = 4, y = 5;
	//maximumGain(s, x, y);
	
	//int n = 8, m = 2;
	//vector<int> group = { -1, -1, 1, 0, 0, 1, 0, -1 };
	//vector<vector<int>> beforeItems = { {},{6},{5},{6},{3, 6},{},{},{} };
	//sortItems(n, m, group, beforeItems);

	//vector<vector<int>> edges = { {1,2}, {2,3}, {2,3},{3,4},{1,4},{1,5} };
	//findRedundantConnection(edges);


	//int n = 5;
	//vector<vector<int>> edges = { {0, 1, 1},{1, 2, 1},{2, 3, 2},{0, 3, 2},{0, 4, 3},{3, 4, 3},{1, 4, 6} };
	//findCriticalAndPseudoCriticalEdges(n, edges);

	//string s = "AABABBABABABABA";
	//int k = 1;
	//characterReplacement(s, k);
	//vector<int> A = { 0,1,0 };
	//int K = 1;
	//minKBitFlips(A, K);
	//vector<int> nums = { 1,3,-1,-3,5,3,6,7 };
	//int k = 3;
	//medianSlidingWindow(nums, k);
	

	//string s = "anryddgaqpjdw";
	//string t = "zjhotgdlmadcf";
	//int cost = 5;
	//equalSubstring(s, t, cost);


//int n = 5;
//vector<vector<int>> connections = { {0, 1},{0, 2},{3,4},{2,3} };
//makeConnected(n, connections);





//vector<int> source = { 1,2,3,4 };
	//vector<int> target = { 1,3,2,4 };
	//vector<vector<int>> allowedSwaps = { };
	//minimumHammingDistance(source, target, allowedSwaps);

//vector<vector<int>> isConnected = { {1, 1, 0},{1, 1, 0},{0, 0, 1} };
	//findCircleNum(isConnected);

/*
vector<int> a = { 0,0,0,0,0,0,0,0 };
	waysToSplit(a);
*/

/*
leetcode 399
图论
vector<vector<string>> equations = { {"a", "b"},{"b", "c"} };
	vector<double> values = {2.0, 3.0};
	vector<vector<string>> queries = { {"a", "c"},{"b", "a"},{"a", "e"},{"a", "a"},{"x", "x"} };

	calcEquation(equations, values, queries);

*/



/*
vector<int> a = { 2,14,11,5,1744,2352,0,1,1300,2796,0,4,376,1672,73,55,2006,42,10,6,0,2,2,0,0,1,0,1,0,2,271,241,1,63,1117,931,3,5,378,646,2,0,2,0,15,1 };
	countPairs(a);
*/

//vector<int> a = { 9,11 };
	//int k = 2;
	//maxSlidingWindow(a, k);


	//vector<int> a = { 0,0,1,0,1 };int n = 1;
	//canPlaceFlowers(a, n);

	//vector<int> ratings = { 1,2,5,4,3,2,1 };
	//candy(ratings);
	
	
	//string S = "aaaabbb";
	//reorganizeString(S);

	//vector<int> nums = { 2,2 };
	//int target = 2;
	//searchRange(nums, target);

	//countPrimes(1500000);

	//vector<char> tasks = {'A', 'A', 'A', 'B', 'B', 'B','C','C' ,'C' ,'D' ,'D' ,'E' };
	//int n = 2;
	//leastInterval(tasks, n);

	//generate(5);

	//vector<vector<int>> s = { {0, 0, 1, 1}, {1, 0, 1, 0}, {1, 1, 0, 0} };
	//matrixScore(s);

	//vector<int> vec;
	//string s = "123456579";
	//splitIntoFibonacci(s);

	//vector<int> num = { 5,2,9,1,6 };
	//containsDuplicate(num);

	
	//vector<string> a = { "eat", "tea", "tan", "ate", "nat", "bat" };
	//groupAnagrams(a);

	//vector<int> nums = { 7,90,5,1,100,10,10,2  };
	//PredictTheWinner(nums);
	
	//vector<int> stones = { 5,3,1,4,2 };
	//stoneGameVII(stones);
	
	
	//monotoneIncreasingDigits(120);
	//string  pattern = "abba", str = "dog cat cat dog";
	//wordPattern(pattern, str);
	
	//string s = "abcd", t = "abcde";
	//findTheDifference(s, t);

	//vector<vector<int>> matrix = {
	//							 {1,2,3},
	//							 {4,5,6},
	//							 {7,8,9} };
	//rotate(matrix);
	
	//vector<int> a = { 5,2,1,2,5,2,1,2,5 };
	//maximumUniqueSubarray(a);

	//string number = "9964-";
	//reformatNumber(number);



