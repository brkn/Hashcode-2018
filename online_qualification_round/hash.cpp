#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct Rides {
	int a, b, x, y, s, f;
	int rideNum;
	int distance;
	bool taken = false;
};

struct Vehicles {
	int curr_x = 0, curr_y = 0;
	int remainingD = 0;
	bool onWork = false;
	Rides* curr_route = NULL;
	Rides* next_route = NULL;
	list<int> rideNums;
};

int R, C, F, N, B, T, minRemainingD, step = 0;
int takenD;

list<Rides> rideList;
Vehicles* vehicles;

void readFile();
void assignVehicles();
void checkVehicles();
bool compare_distances(const Rides& first, const Rides& second);
void printFile();
Vehicles* findBestDriver(list<Rides>::iterator itr);

int main() {
	readFile();
	rideList.sort(compare_distances);
	while (step < T) {
		checkVehicles();
		assignVehicles();
		if(minRemainingD > 0 && minRemainingD < T){
			for (int i = 0; i < F; i++) {
				if (vehicles[i].remainingD != 0) {
					vehicles[i].remainingD -= minRemainingD;
				}
			}
			step += minRemainingD;
			minRemainingD = T;
		}
		else {
			step++;
			for (int i = 0; i < F; i++) {
				if (vehicles[i].remainingD != 0) {
					vehicles[i].remainingD -= 1;
				}
			}
		}

	}
	printFile();
	
	list<Rides>::iterator itr;
	int totalD = 0;
	for (itr = rideList.begin(); itr != rideList.end(); ++itr) {
		totalD += itr->distance;
	}
	cout << "Total distance can be covered: " << totalD << endl << "Total distance that covered: " << takenD << endl <<
		"Success: %" << ((double)takenD / totalD) * 100 << endl;
	system("pause");

	return 0;
}

void checkVehicles() {
	if (step == 0)
		return;

	int tempRemainingD = 0;
	for (int i = 0; i < F; i++) {
		if (vehicles[i].onWork) {
			if (vehicles[i].remainingD == 0) {
				if (vehicles[i].curr_route->f >= step) {
					vehicles[i].rideNums.push_back(vehicles[i].curr_route->rideNum);
					takenD += vehicles[i].curr_route->distance;
				}
				vehicles[i].curr_route = NULL;
				if (vehicles[i].next_route) {
					vehicles[i].curr_route = vehicles[i].next_route;
					vehicles[i].next_route = NULL;
					vehicles[i].remainingD = vehicles[i].curr_route->distance +
						abs(vehicles[i].curr_route->a - vehicles[i].curr_x) +
						abs(vehicles[i].curr_route->b - vehicles[i].curr_y);
					vehicles[i].curr_x = vehicles[i].curr_route->x;
					vehicles[i].curr_y = vehicles[i].curr_route->y;
				}
				if(!vehicles[i].curr_route) vehicles[i].onWork = false;
			}
		}
		tempRemainingD = vehicles[i].remainingD;
		if (minRemainingD > tempRemainingD)
			minRemainingD = tempRemainingD;
	}
}

Vehicles* findBestDriver(list<Rides>::iterator itr) {
	Vehicles* best = NULL;
	double bestscore = -9999, cscore = -10000;

	for (int i = 0; i < F; i++) {
		if (vehicles[i].curr_route && vehicles[i].next_route)
			continue;
		else if (vehicles[i].curr_route && T - step >= itr->distance + abs(itr->a - vehicles[i].curr_x) + abs(itr->b - vehicles[i].curr_y) + vehicles[i].remainingD)
			//cscore = (double)(abs(itr->a - vehicles[i].curr_x) + abs(itr->b - vehicles[i].curr_y)) / itr->distance;
			cscore = abs(itr->s - step - (vehicles[i].remainingD + abs(vehicles[i].curr_route->x - itr->a) + abs(vehicles[i].curr_route->y - itr->b)));
		else if (!vehicles[i].curr_route && T - step >= itr->distance + abs(itr->a - vehicles[i].curr_x) + abs(itr->b - vehicles[i].curr_y))
			//cscore = (double)(abs(itr->a - vehicles[i].curr_x) + abs(itr->b - vehicles[i].curr_y)) / itr->distance;
			cscore = abs(itr->s - step - (vehicles[i].remainingD + abs(vehicles[i].curr_x - itr->a) + abs(vehicles[i].curr_y - itr->b)));
		else
			continue;
		if (cscore == 0)
			return &vehicles[i];
		cscore = 1 / cscore;
		if (cscore>bestscore) {
			bestscore = cscore;
			best = &vehicles[i];
		}
		//next route
	}
	return best; //test if it's null
}

void assignVehicles() {
	list<Rides>::iterator itr;
	Vehicles* driver;
	for (itr = rideList.begin(); itr != rideList.end(); ++itr) {
		if (!itr->taken) {
			driver = findBestDriver(itr);
			if (driver == NULL) {
				continue;
			}
			if (driver->curr_route != NULL) {
				driver->next_route = &(*itr);
				driver->next_route->taken = true;
			}
			else {
				driver->curr_route = &(*itr);
				driver->remainingD = itr->distance + abs(itr->a - driver->curr_x) + abs(itr->b - driver->curr_y);
				driver->curr_route->taken = true;
				driver->onWork = true;
				driver->curr_x = driver->curr_route->x;
				driver->curr_y = driver->curr_route->y;
			}
		}
	}
}

void readFile() {
	ifstream infile;//file object
	string data;//string d
	Rides rideData;
	//infile.open("e_high_bonus.in");
	infile.open("d_metropolis.in");
	//infile.open("c_no_hurry.in");
	//infile.open("b_should_be_easy.in");
	//infile.open("a_example.in");
	//reading text file word by word
	infile >> data;
	R = stoi(data);

	infile >> data;
	C = stoi(data);

	infile >> data;
	F = stoi(data);

	infile >> data;
	N = stoi(data);

	infile >> data;
	B = stoi(data);

	infile >> data;
	T = stoi(data);
	minRemainingD = T;

	//reading ingredients from file to store into pizzaToTest array
	int tempRemainingD = 0;
	for (int i = 0; i < N; i++) {
		infile >> data;
		rideData.a = stoi(data);
		infile >> data;
		rideData.b = stoi(data);
		infile >> data;
		rideData.x = stoi(data);
		infile >> data;
		rideData.y = stoi(data);
		infile >> data;
		rideData.s = stoi(data);
		infile >> data;
		rideData.f = stoi(data);
		rideData.distance = abs(rideData.a - rideData.x) + abs(rideData.b - rideData.y);
		rideData.rideNum = i;
		rideList.push_back(rideData);
		tempRemainingD = rideData.distance + rideData.a + rideData.b;
		if (tempRemainingD < minRemainingD)
			minRemainingD = tempRemainingD;
	}

	vehicles = new Vehicles[F];

	infile.close();//close file
	return;
}

void printFile() {
	ofstream outfile;
	outfile.open("output.out");
	for (int i = 0; i < F; i++) {
		if (!vehicles[i].rideNums.empty()) {
			outfile << vehicles[i].rideNums.size();
			while (!vehicles[i].rideNums.empty()) {
				outfile << " " << vehicles[i].rideNums.front();
				vehicles[i].rideNums.pop_front();
			}
			if (i != F - 1) outfile << endl;
		}
	}

	delete [] vehicles;
	outfile.close();
}

bool compare_distances(const Rides& first, const Rides& second) {
	return first.distance > second.distance;
}