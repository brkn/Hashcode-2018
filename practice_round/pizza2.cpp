#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <windows.h>

using namespace std;

struct myArray {
	int arr[2];
};

struct slicePossibility {
	int x = 0, y = 0;
	double getSliceScore(int);
	slicePossibility* next = NULL;
	int rows[2];
	int columns[2];
	double sliceScore;
	int mCount;
	int tCount;
	int size;
	bool operator<(const slicePossibility &sp) const
	{
		return sliceScore < sp.sliceScore;
	}
	bool operator>(const slicePossibility &sp) const
	{
		return sliceScore > sp.sliceScore;
	}
};

struct eachCell {
	int x = 0, y = 0;
	bool occupied = false;
	char ingredient;
	int slicePossibilityCount = 0;
	void push(int, int, int, int, int, int);
	slicePossibility* head = NULL;
};

double slicePossibility::getSliceScore(int spc) {
	double result = 1 / spc;
	return result;
}

void eachCell::push(int x1, int x2, int y1, int y2, int tCount, int mCount) {
	slicePossibility* traverse = head;
	slicePossibility* sp = new slicePossibility;
	sp->rows[0] = x1;
	sp->rows[1] = x2;
	sp->columns[0] = y1;
	sp->columns[1] = y2;
	sp->size = (x2 - x1 + 1)*(y2 - y1 + 1);
	sp->tCount = tCount;
	sp->mCount = mCount;
	sp->next = NULL;
	
	if (head == NULL) {
		head = sp;
		return;
	}

	else {
		while (traverse->next) traverse = traverse->next;
		traverse->next = sp;
	}
}


int R, C, L, H, totalM = 0, totalT = 0;//4 inputs
list<slicePossibility> sliceRanks, output;
eachCell** readFile();


void pushMe(eachCell cl);
bool compare_ranks(const slicePossibility& first, const slicePossibility& second);
void posCounts(eachCell** cells);
void calculateScore(eachCell**);
void findAllSlices(eachCell**);
void solvePizza(eachCell**);
void printPizza(eachCell**);
void printOutput();

int main() {

	eachCell** cells = readFile();
	findAllSlices(cells);
	posCounts(cells);
	calculateScore(cells);
	sliceRanks.sort(compare_ranks);
	solvePizza(cells);
	printPizza(cells);
	printOutput();
	//system("pause");

	//free memory
	
	for (int i = 0; i < R; i++)
		delete[] cells[i];
	delete[] cells;
	return 0;
}

void calculateScore(eachCell** cells) {
	list<slicePossibility>::iterator itr;
	for (itr = sliceRanks.begin(); itr != sliceRanks.end(); ++itr) {
		double sum = 0;
		for (int i = itr->rows[0]; i <= itr->rows[1]; i++) {
			for (int j = itr->columns[0]; j <= itr->columns[1]; j++) {
				if (cells[i][j].slicePossibilityCount != 0)
					sum += (double)1/cells[i][j].slicePossibilityCount;
				else
					sum += 1 / 0.01; //preventing dividing to 0
			}
		}
		itr->sliceScore = sum;
		//if(cells[itr->x][itr->y].slicePossibilityCount != 0)
		//	itr->sliceScore = (double) 1/cells[itr->x][itr->y].slicePossibilityCount;

	}
}

bool compare_ranks(const slicePossibility& first, const slicePossibility& second) {
	return first.sliceScore > second.sliceScore;
}

void solvePizza(eachCell** cells) {
	slicePossibility temp;
	bool available = true;
	char ch = '0';
			
	while (!sliceRanks.empty()) {
		available = true;
		temp = sliceRanks.front();
		for (int i = temp.rows[0]; i <= temp.rows[1]; i++) {
			for (int j = temp.columns[0]; j <= temp.columns[1]; j++) {
				if (cells[i][j].occupied) {
					available = false;
					sliceRanks.pop_front();
					break;
				}
			}
			if (!available) break;
		}

		if (available) {
			for (int i = temp.rows[0]; i <= temp.rows[1]; i++) {
				for (int j = temp.columns[0]; j <= temp.columns[1]; j++) {
					cells[i][j].occupied = true;
					cells[i][j].ingredient = ch;
				}
			}
			output.push_back(temp);
			sliceRanks.pop_front();
			ch++;
			if (ch > '9') ch = '0';
		}
	}
	
}

void posCounts(eachCell** cells) {
	list<slicePossibility>::iterator itr;
	for(itr = sliceRanks.begin(); itr != sliceRanks.end(); ++itr){
		for (int i = itr->rows[0]; i <= itr->rows[1]; i++) {
			for (int j = itr->columns[0]; j <= itr->columns[1]; j++) {
				cells[i][j].slicePossibilityCount++;
			}
		}
	}

}

void printPizza(eachCell** cells) {

	system("cls");
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			cout << cells[i][j].ingredient << " ";
		}
		cout << endl;
	}
	//Sleep(300);
	ofstream myPizza;
	myPizza.open("myPizza.txt");
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			myPizza << cells[i][j].ingredient << " ";
		}
		myPizza << endl;
	}
	myPizza.close();
}

eachCell** readFile() {
	ifstream infile;//file object
	string data;//string data we get from pizza.txt for constraints
	infile.open("example.in");
	//reading text file word by word
	infile >> data;
	R = stoi(data);

	infile >> data;
	C = stoi(data);

	infile >> data;
	L = stoi(data);

	infile >> data;
	H = stoi(data);

	//reading ingredients from file to store into pizzaToTest array


	eachCell** cells = new eachCell*[R];
	for (int i = 0; i < R; i++)
		cells[i] = new eachCell[C];

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			infile >> cells[i][j].ingredient;
			if (cells[i][j].ingredient == 'M') totalM++;
			if (cells[i][j].ingredient == 'T') totalT++;
			if (infile.eof()) break;
		}
	}
	infile.close();//close file
	return cells;
}

void findAllSlices(eachCell** cells) {
	vector<myArray> indexes;
	vector<myArray>::iterator it;
	myArray depo;
	bool flag;
	int a = 1, b = 2 * L;
	

	while (true) {
		while(a*b > H) {
			a++;
			b = a;
			if (a >= H) break;
		}
		if (a == H) break;
		depo.arr[0] = a;
		depo.arr[1] = b;
		indexes.push_back(depo);
		b++;
	}


	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			for (it = indexes.begin(); it < indexes.end(); ++it){
				flag = false;
				int tCount = 0, mCount = 0;
				for (int k = 0; k < it->arr[0]; k++) {
					if ((i + it->arr[0] - 1) >= R) break;
					for (int p = 0; p < it->arr[1]; p++) {
						if ((j + it->arr[1] - 1) >= C) break;
						if (cells[i + k][j + p].ingredient == 'M') mCount++;
						else tCount++;
					}
					if (tCount >= L && mCount >= L && tCount + mCount == it->arr[0]*it->arr[1]) {
						cells[i][j].push(i, i + it->arr[0]-1, j, j + it->arr[1] - 1, tCount, mCount);
						flag = true;
					}

				}
				if (it->arr[0] == it->arr[1]) {
					//if (flag) pushMe(cells[i][j]); //score calc here
					continue;
				}
				
				tCount = 0, mCount = 0;
				for (int k = 0; k < it->arr[0]; k++) {
					if ((j + it->arr[0] - 1) >= C) break;
					for (int p = 0; p < it->arr[1]; p++) {
						if ((i + it->arr[1] - 1) >= R) break;
						if (cells[i + p][j + k].ingredient == 'M') mCount++;
						else tCount++;
					}
					if (tCount >= L && mCount >= L && tCount + mCount == it->arr[0] * it->arr[1]) {
						cells[i][j].push(i, i + it->arr[1] - 1, j, j + it->arr[0] - 1, tCount, mCount);
						flag = true;
					}
				}
				
			}
			if (cells[i][j].head) {
				cells[i][j].x = i;
				cells[i][j].y = j;
				pushMe(cells[i][j]);
			}
		}
	}
}

void pushMe(eachCell cl) {
	slicePossibility* traverse;
	slicePossibility data;
	traverse = cl.head;
	if (traverse == NULL) return;
	while (traverse != NULL) {
		data.x = cl.x;
		data.y = cl.y;
		data.columns[0] = traverse->columns[0];
		data.columns[1] = traverse->columns[1];
		data.rows[0] = traverse->rows[0];
		data.rows[1] = traverse->rows[1];
		data.mCount = traverse->mCount;
		data.tCount = traverse->tCount;
		data.sliceScore = traverse->sliceScore;
		data.size = traverse->size;
		data.next = traverse->next;

		//traverse->sliceScore = traverse->getSliceScore(cl.slicePossibilityCount);
		sliceRanks.push_back(data);
		traverse = traverse->next;
	}
}

void printOutput() {
	list<slicePossibility>::iterator itr;
	ofstream myPizza;
	myPizza.open("solution.out");
	myPizza << output.size();
	for (itr = output.begin(); itr != output.end(); ++itr) {
		myPizza << endl << itr->rows[0] << " " << itr->columns[0] << " " << itr->rows[1] << " " << itr->columns[1];
	}
	myPizza.close();
}