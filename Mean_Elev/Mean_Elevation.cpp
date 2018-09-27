#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

double min(double x[], int n);
double max(double x[], int n);

int main(){
	ifstream patchFile,demFile;

	char   str[50];
	int    rows, cols, tot_pits = 0, neighbors = 0,code=0;
	double nodata, xllcorner, yllcorner, cellsize, newvalue;
	double **pDem, **pPatch;

	//open and create files
	patchFile.open("D://ResearchData//Douyifeng//Inputimage//liuxihe.patch.asc", ios::in);
	demFile.open("D://ResearchData//Douyifeng//Inputimage//liuxihe.dem.asc", ios::in);

	if (!patchFile)
	{
		cerr << "fail to open patch file!" << endl;
		return 1;
	}

	if (!demFile)
	{
		cerr << "fail to open dem file!" << endl;
		return 2;
	}

	//read ArcINFO ascii DEM data file
	patchFile.seekg(0, ios::beg);
	patchFile >> str >> cols;
	patchFile >> str >> rows;
	patchFile >> str >> xllcorner;
	patchFile >> str >> yllcorner;
	patchFile >> str >> cellsize;
	patchFile >> str >> nodata;

	demFile.seekg(0, ios::beg);
	demFile >> str >> cols;
	demFile >> str >> rows;
	demFile >> str >> xllcorner;
	demFile >> str >> yllcorner;
	demFile >> str >> cellsize;
	demFile >> str >> nodata;

	//dynamically allocate two-dimensional array and read dem data;
	pDem = new double*[cols]; //开辟行 
	pPatch = new double*[cols];
	for (int i = 0; i <cols; i++)
	{
		pDem[i] = new double[rows];//开辟列 
		pPatch[i] = new double[rows];
	}

	for (int j = 0; j<rows; j++){
		for (int i = 0; i<cols; i++){
			demFile >> newvalue;
			pDem[i][j] = newvalue;
			patchFile >> code;
			pPatch[i][j] = code;

		}
	}

	//calculate mean elevation
	int m = 0;
	double meanElev = 0.;
	for (int j = 1; j<rows - 1; j++){
		for (int i = 1; i<cols - 1; i++){
			if (pPatch[i][j] != nodata){
				m++;
				meanElev+=pDem[i][j];
			}
		}
	}

	//output result
	std::cout<< "Average elevation for the wateshed is " << meanElev / (1.0*m) << endl;
	

	//释放开辟的二维空间  
	for (int i = 0; i < cols; i++){
		delete[] pDem[i];
		delete[] pPatch[i];
	}
	delete[] pPatch;
	delete[] pDem;

	return 0;
}

double max(double x[], int n)
{
	double y = 0;

	for (int i = 0; i < n; i++)
	{
		if (x[i] > y)
		{
			y = x[i];
		}
	}
	return y;
}

double min(double x[], int n)
{
	double y = 1000000000.;

	for (int i = 0; i < n; i++)
	{
		if (x[i] < y)
		{
			y = x[i];
		}
	}
	return y;
}