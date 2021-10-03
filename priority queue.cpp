
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
using namespace std;

struct Data {
	string name;
	int useful;
	float hours_for_task;

	Data() {
		name = "Task";
		useful = rand() % 10 + 1;
		hours_for_task = rand() % 148 + 1;
	}

	Data(string name_value, int usefulness_value, float hours_value) {
		name = name_value;
		useful = usefulness_value;
		hours_for_task = hours_value;
	}
};

bool operator<(const Data& h1, const Data& h2) {
	return h1.hours_for_task / 2 + h1.useful < h2.hours_for_task / 2 + h2.useful;
}

bool operator==(const Data& h1, const Data& h2) {
	return h1.hours_for_task / 2 + h1.useful == h2.hours_for_task / 2 + h2.useful;
}

struct DynamicArray {
	Data* start;
	int size_;
	int capacity;

	DynamicArray() {
		size_ = 0;
		capacity = 40;
		start = new Data[capacity];
	}

	~DynamicArray() {
		delete[] start;
	}

	int size() const {
		return size_;
	}

	void push_back(const Data& s) {
		if (size_ >= capacity) {
			capacity = (size_ + 1) * 2;
			Data* new_arr = new Data[capacity];

			for (int i = 0; i < size_; i++) {
				new_arr[i] = *(start + i);
			}
			delete[] start;
			start = new_arr;
		}

		*(start + size_++) = s;
	}

	Data* get(int n) {
		if (size_ > n)
			return (start + n);
		return NULL;
	}

	void print() const {
		cout << "---------Data:-----------\n";
		for (int i = 0; i < size_; i++) {
			Data hw = *(start + i);
			cout << hw.name << ": " << hw.hours_for_task << " useful-" << hw.useful << endl;
		}
		cout << endl;
	}
};

void swap(Data* hw1, Data* hw2) {
	Data copy = *hw1;
	*hw1 = *hw2;
	*hw2 = copy;
}

struct PriorityQueue {
	DynamicArray tree;

	int size() {
		return tree.size_; // return tree.size();
	}

	bool empty() {
		return size() == 0;

	}

	int getParent(int index) {
		return (index - 1) / 2;
	}

	int getLeftChild(int index) {
		return 2 * index + 1;
	}

	int getRightChild(int index) {
		return 2 * index + 2;
	}

	void siftUp(int index) {
		Data* hw = tree.get(index);
		while (index > 0) {
			hw = tree.get(index);
			int parent_index = getParent(index);
			Data* parent = tree.get(parent_index);
			if (*parent < *hw) {
				swap(parent, hw);
				index = parent_index;
			}
			else
				break;

		}
	}

	void siftDown(int index) {
		Data* hw = tree.get(index);
		int leftIndex, rightIndex;
		Data* leftChild, * rightChild;

		while (index < size() / 2) {
			leftIndex = getLeftChild(index);
			rightIndex = getRightChild(index);
			leftChild = tree.get(leftIndex);
			rightChild = tree.get(rightIndex);//leftChild != NULL && rightChild != NULL

			int maxI = leftIndex;
			if (rightIndex < size() && *leftChild < *rightChild) {
				maxI = rightIndex;
			}

			Data* maxChild = tree.get(maxI);
			if (*maxChild < *hw)
				break;
			swap(hw, maxChild);
			hw = maxChild;
			index = maxI;
		}

	}

	void push(const Data& hw) {
		tree.push_back(hw);
		siftUp(size() - 1);
	}

	void pop() {
		if (size() != 0) {
			swap(tree.get(0), tree.get(size() - 1));
			tree.size_--;
			siftDown(0);
		}
	}

	Data top() {
		return *tree.get(0);
	}
};


template <typename T>
float testPriorityQueueSpeed(T&& priorityQueue)
{
	const int iters = 100000;
	clock_t timeStart = clock();
	for (int i = 0; i < iters; i++)
	{
		int insertDataAmount = rand() % 6 + 5;
		for (int j = 0; j < insertDataAmount; j++)
		{
			priorityQueue.push(Data());
		}
		priorityQueue.top();
		priorityQueue.pop();
	}
	clock_t timeEnd = clock();
	float time = (float(timeEnd - timeStart)) / CLOCKS_PER_SEC;
	return time;
}
bool testPriorityQueue()
{
	srand(time(NULL));
	const int iters = 20000;
	PriorityQueue myPriorQueue;
	priority_queue<Data> stlPriorQueue;
	bool isDataEqual = true;
	for (int i = 0; i < iters; i++)
	{
		int insertDataAmount = rand() % 6 + 5;
		for (int j = 0; j < insertDataAmount; j++)
		{
			Data randData = Data();
			myPriorQueue.push(randData);
			stlPriorQueue.push(randData);
		}
		if (!(myPriorQueue.top() == stlPriorQueue.top()))
		{
			isDataEqual = false;
			cerr << "Comparing failed on iteration " << i << endl << endl;
			break;
		}
		int removeDataAmount = rand() % insertDataAmount;
		for (int j = 0; j < removeDataAmount; j++)
		{
			myPriorQueue.pop();
			stlPriorQueue.pop();
		}
	}
	int myQueueSize = myPriorQueue.size();
	int stlQueueSize = stlPriorQueue.size();
	float stlTime =
		testPriorityQueueSpeed<priority_queue<Data>>(priority_queue<Data>());
	float myTime = testPriorityQueueSpeed<PriorityQueue>(PriorityQueue());
	cout << "My PriorityQueue:" << endl;
	cout << "Time: " << myTime << ", size: " << myQueueSize << endl;
	cout << "STL priority_queue:" << endl;
	cout << "Time: " << stlTime << ", size: " << stlQueueSize << endl << endl;
	if (isDataEqual && myQueueSize == stlQueueSize)
	{
		cout << "The lab is completed" << endl << endl;
		return true;
	}
	cerr << ":(" << endl << endl;
	return false;
}

template <typename T>
void heapify(T* arr, int size, int i)
{
	int biggest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < size && arr[biggest] < arr[left])
		biggest = left;

	if (right < size && arr[biggest] < arr[right])
		biggest = right;

	if (biggest != i) {
		swap(arr[i], arr[biggest]);
		heapify(arr, size, biggest);
	}
}
template <typename T>
void build_heap(T* arr, int size) {
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify(arr, size, i);
}

template <typename T>
void heapSort(T* arr, int size) {
	build_heap(arr, size);

	for (int i = size - 1; i > 0; i--) {
		swap(arr[0], arr[i]);
		heapify(arr, i, 0);
	}
}


void testHeapSort() {
	const int iters = 1000;

	Data* mySort, * stlSort;
	mySort = new Data[iters];
	stlSort = new Data[iters];


	for (int j = 0; j < iters; j++)
	{
		Data randData = Data();
		mySort[j] = randData;
		stlSort[j] = randData;
	}

	clock_t timeStart = clock();
	sort(stlSort, stlSort + iters);
	clock_t timeEnd = clock();
	float time = (float(timeEnd - timeStart)) / CLOCKS_PER_SEC;
	cout << "STL sort: " << time << endl;

	timeStart = clock();
	heapSort(mySort, iters);
	timeEnd = clock();
	time = (float(timeEnd - timeStart)) / CLOCKS_PER_SEC;
	cout << "My HeapSort: " << time << endl;


	for (int j = 0; j < iters; j++)
	{
		cout << stlSort[j].hours_for_task / 2 + stlSort[j].useful << "=" << mySort[j].hours_for_task / 2 + mySort[j].useful << endl;
	}

}

int main() {
	testPriorityQueue();
	testHeapSort();

	system("pause");
	return 0;
}
