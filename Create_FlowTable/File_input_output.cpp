//-----------------------------------------------------------------------------
// NAME:                    FILEIO.C
// 
// Name:: input_prompt
// 
// Descriptions
//	contains all functions for handling ArcInfo input images files
//----------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include "file_io.h"
#include "blender.h"

using namespace std;

//--------------------------------------------------------------------------
//	input_header() - input information (row, col) from [root].header
//--------------------------------------------------------------------------
void input_header(int *maxr, int *maxc, char *fndem, int arc_flag)
{
	ifstream inFileDem;
	char ChCols[20], ChRows[20];

	inFileDem.open(fndem, ios::in);
	if (inFileDem.is_open()){
		inFileDem >> ChCols >> *maxc;
		std::cout << " cols are " << *maxc << std::endl;
		inFileDem >> ChRows >> *maxr;
		std::cout << " rows are " << *maxr << std::endl;
		inFileDem.close();
	}
	else {
		std::cout<<"cannot open ArcInfo DEM files \n"<<std::endl;
	}
}

//-------------------------------------------------------------------------------------
//	input_prompt() - input root filename, create full filenames
//-------------------------------------------------------------------------------------
void	input_prompt(int *maxr, int *maxc, char *filename, char *fndem, char *fnslope,
	char *fnpatch, char *fnstream, char *fnroads, char *fntable, int arc_flag)
{
	// copy the root filename into the specific filenames
	strcpy(fndem, filename);
	strcpy(fnslope, filename);
	strcpy(fnpatch, filename);
	strcpy(fnstream, filename);
	strcpy(fnroads, filename);
	strcpy(fntable, filename);

	// append '.' extensions to each filename (these should be generalized)
	strcat(fndem, ".dem");
	strcat(fnslope, ".slope");
	strcat(fnpatch, ".patch");
	strcat(fnstream, ".stream");
	strcat(fnroads, ".road");
	strcat(fntable, ".table");

	input_header(maxr, maxc, fndem, arc_flag);

	return;
}

//-------------------------------------------------------------------------
//input_ascii_int() - input an ascii image into an interger array using the
//      (row, col) coordinates maxr and maxc.
//-------------------------------------------------------------------------
void input_ascii_int(int *aray,char *filename, int mc, int mr, int arc_flag)
{
    ifstream InPutImage;
    int  r;
    int max;
	char STR[70];
    max = 0;

	InPutImage.open(filename,ios::in);
	if(InPutImage.is_open()){
		// skip header
		if (arc_flag == 0)
			for (r=0; r < LEN_GRASSHEADER; r++)
				InPutImage>>STR;
		else
			for (r=0; r < LEN_ARCHEADER; r++)
				InPutImage>>STR;

		for (r=0; r < mr*mc; r++) {
			InPutImage>>aray[r];
			if (aray[r] > max) 
				max = aray[r];
		}
		std::cout << "\n Max for " << filename << " is " << max << std::endl;
		InPutImage.close();

	}
	else{
		std::cout << "cannot open file %s\n" << filename << std::endl;
		exit(0);
	}

    return;
}
 
//---------------------------------------------------------------------------------------------
// input_ascii_double() - input an ascii image into an double array using the
//                        (row, col) coordinates maxr and maxc.
//---------------------------------------------------------------------------------------------
void input_ascii_double(double *aray, char *filename, int mc, int mr, int arc_flag)
{
	ifstream InPutImage;
	int  r;
	double max; 
	char STR[70];
	
	max = 0;
	InPutImage.open(filename,ios::in);
	if (InPutImage.is_open()){
		/* skip header */
		if (arc_flag == 0)
			for (r=0; r < LEN_GRASSHEADER; r++)
				InPutImage>>STR;
		else
			for (r=0; r < LEN_ARCHEADER; r++)
				InPutImage>>STR;

		for (r=0; r < mr*mc; r++) {
			InPutImage>>aray[r];
			aray[r] = (double)(aray[r]); 
			if (aray[r] > max) 
			max = aray[r];
		}
		cout<<"\n Max for "<<filename<<" is "<<max<<endl;
		InPutImage.close();
	}
	else {
		cout<<"cannot open file "<<filename<<endl;
		exit (0);
    }

	return;
}
