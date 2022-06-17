#ifndef BASIC_ALGORITHM_H
#define BASIC_ALGORITHM_H
void quicksort(DataType list[], int l, int r) {
	if (l >= r)return;
	int lb = l, rb = r;// left/right bound
	while (l < r) {
		while (list[l] <= list[lb])l++;//choose lb as the pivot
		while (list[r] > list[lb])r--;// greater than
		if (l < r) {
			std::swap(list[l], list[r]);
		}
	}
	std::swap(list[lb], list[r]);// in the end, list[r] will be the seperator.
	quicksort(list, lb, r - 1);
	quicksort(list, l, rb);
}
int binSearch(int num[], int x, int n) {
	int l = 0, r = n;
	while (l < r) {
		int mid = l + (r - l) / 2; //计算中点的下标
		if (num[mid] >= x) {//如果中点大于或等于要查找的元素，则右边界更新为中点
			r = mid;
		} else {//否则左边界更新为中点的下一个元素
			l = mid + 1;
		}
	}
	if (num[l] == x)return l + 1;
	else return l + 1;
}
#endif
