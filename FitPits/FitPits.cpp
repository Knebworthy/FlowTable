//This program aims to fix pits to assure all water in a watershed will flow out of a point
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

double min(double x[], int n);
double max(double x[], int n);

int main() {
	ifstream inFile;
	ofstream outFile;

	char   str[50];
	long int    i, j, k, m, rows, cols, tot_pits = 0, neighbors = 0, count;
	double nodata, xllcorner, yllcorner, cellsize, minDem, maxDem, VALUE;
	double DEM[8];
	double **pDem = { nullptr }, **pIndex = { nullptr };

	//open and create files
	inFile.open("I://NSEC//Dongjiang//flowtable//dem.txt", ios::in);
	outFile.open("I://NSEC//Dongjiang//flowtable//dj.fill.txt", ios::out);
	const double fits = 0.01;


	if (!inFile)
	{
		cerr << "fail to open input file!" << endl;
		return 1;
	}

	if (!outFile)
	{
		cerr << "fail to open output file!" << endl;
		return 2;
	}

	//read ArcINFO ascii DEM data file
	inFile.seekg(0, ios::beg);
	inFile >> str >> cols;
	inFile >> str >> rows;
	inFile >> str >> xllcorner;
	inFile >> str >> yllcorner;
	inFile >> str >> cellsize;
	inFile >> str >> nodata;

	//dynamically allocate two-dimensional array and read dem data;
	pDem = new double*[rows]; //开辟行 
	pIndex = new double*[rows];
	for (int i = 0; i <rows; i++)
		pDem[i] = new double[cols];//开辟列 

	for (int i = 0; i<rows; i++) {
		for (int j = 0; j<cols; j++) {
			inFile >> VALUE;
			pDem[i][j] = VALUE;
		}
	}

	count = 0;
	do {
		//first, judge if there are sinks
		tot_pits = 0;
		for (int i = 1; i<rows - 1; i++) {
			for (int j = 1; j<cols - 1; j++) {
				//if (pDem[i][j] != nodata){
				//if (pDem[i - 1][j - 1] != nodata && pDem[i][j - 1] != nodata && pDem[i + 1][j - 1] != nodata
				//	&& pDem[i - 1][j] != nodata && pDem[i + 1][j] != nodata
				//	&& pDem[i - 1][j + 1] != nodata && pDem[i][j + 1] != nodata && pDem[i + 1][j + 1] != nodata) {

				DEM[0] = pDem[i - 1][j - 1];
				DEM[1] = pDem[i - 1][j];
				DEM[2] = pDem[i - 1][j + 1];
				DEM[3] = pDem[i][j - 1];
				DEM[4] = pDem[i][j + 1];
				DEM[5] = pDem[i + 1][j - 1];
				DEM[6] = pDem[i + 1][j];
				DEM[7] = pDem[i + 1][j + 1];
				minDem = min(DEM, 8);
				maxDem = max(DEM, 8);

				//rectified by xu
				//skip cells -9999
				int pits_flag = 1;
				if (fabs(pDem[i][j] + 9999) < 0.001) pits_flag = 0;

				if ((pDem[i][j] - minDem)<0.0001 && pits_flag == 1) {
					pDem[i][j] = minDem + fits;
					tot_pits++;
				}
				//}
				//}
			}
		}
		if (tot_pits > 0.)
			count++;
		cout << "tot_pits is " << tot_pits  << " " << count << endl;

	} while (tot_pits>0);

	//output files
	outFile << setiosflags(ios::left) << setw(14) << "ncols" << setw(12) << cols << endl;
	outFile << setiosflags(ios::left) << setw(14) << "nrows" << setw(12) << rows << endl;
	outFile << setiosflags(ios::left) << setw(14) << "xllcorner" << setw(14) << fixed << setprecision(5) << xllcorner << endl;
	outFile << setiosflags(ios::left) << setw(14) << "yllcorner" << setw(14) << fixed << setprecision(5) << yllcorner << endl;
	outFile << setiosflags(ios::left) << setw(14) << "cellsize" << setw(12) << cellsize << endl;
	outFile << setiosflags(ios::left) << setw(14) << "NODATA_value" << setw(12) << nodata << endl;

	for (int i = 0; i<rows; i++) {
		for (int j = 0; j<cols; j++) {
			outFile << right << setw(12) << setprecision(4) << pDem[i][j];
		}
		outFile << endl;
	}

	//释放开辟的二维空间  
	for (int i = 0; i < rows; i++)
		delete[] pDem[i];
	delete[] pDem;

	return 0;
}

double max(double x[], int n)
{
	double y = x[0];
	for (int i = 1; i < n; i++)
		if (x[i] > y)
			y = x[i];
	return y;
}


//rectificed by xu
//considering when there is -9999
double min(double x[], int n)
{
	int real_neigh_flag = 0;
	for (int i = 0; i < n; i++) {
		if (fabs(x[i] + 9999) > 0.001) {
			real_neigh_flag = 1; break;
		}
	}
	double y = 100000;
	if (real_neigh_flag == 1) {
		for (int i = 0; i < n; i++) {
			if (x[i]<y)
				y = x[i];
		}
	}
	else y = -9999;
	//rectified when all neighbor == -9999


	return y;
}