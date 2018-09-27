//---------------------------------------------------------------------------------------------
//  NAME                                                        
//		 print_flow_table										
//                                                              
//  DESCRIPTION                                                 
//		- locates a patch based on ID value and print flow path						
//---------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "blender.h"
#include <math.h>
#include <iomanip>

using namespace std; 

void	print_flow_table(int num_patches, struct flow_struct * flow_table, 
	int sc_flag, double cell, char *input_prefix, double width)
{
	int i,j, cnt;
	struct	adj_struct *adj_ptr;
	ofstream FlowTable;
	double mult, tmp,unidirectional;
	static int count=0;
	char name[256];

	strcpy(name,input_prefix);
	strcat(name,"_flow_table.dat");
	FlowTable.open(name,ios::out);

	if (!FlowTable)	{
		cout<<"\n opening flow_table output file failed"<<endl;
		exit(1);
	}

	FlowTable<<num_patches<<endl; 
	for (i=1; i<= num_patches; i++) {
		count=0;
		
		// this is a temporary patch, so that streams immediately produces outflow
		// below are for stream patch
		if ((flow_table[i].land == 1) && (sc_flag > 0)) {
			if (flow_table[i].land == 1 ) {
				if(flow_table[i].area >1 ) {
				  std::cout<<"flow_table[i].area is "<<flow_table[i].area<<std::endl;
			      getchar();
				}
				
				//mult = ( double )( flow_table[i].K * flow_table[i].m_par * sqrt(flow_table[i].area) ); 	
	            //mult = ( double )(0.12*2* sqrt(flow_table[i].area) );
				mult = 1.;
				if (sc_flag == 2)	
					tmp = ( double )( rand()/(pow(2.0,15.0)-1) );	
				else
				  	tmp = flow_table[i].internal_slope;
		
				if (sc_flag == 3) {
				 	adj_ptr = flow_table[i].adj_list;
					flow_table[i].internal_slope = 0.0;
					cnt = 0;
					for (j=1; j<= flow_table[i].num_adjacent; j++) {
						if (adj_ptr->gamma <= 0)
							flow_table[i].internal_slope += adj_ptr->slope;
						cnt += 1;
						adj_ptr = adj_ptr->next;
					}
					flow_table[i].internal_slope = flow_table[i].internal_slope / cnt;
					tmp =( double )( -1.0*flow_table[i].internal_slope );
				}
				flow_table[i].total_gamma = (double)( mult * tmp);
			}   
		}
	    
		unidirectional=0.001;
		adj_ptr = flow_table[i].adj_list;
		for (j=1; j<= flow_table[i].num_adjacent; j++) {
			if(adj_ptr->gamma==0.)
				count++;
			
			//Below if for judge unidirectional flow
			if (adj_ptr->gamma > unidirectional)
		    {
				unidirectional = adj_ptr->gamma;
		    }
			adj_ptr = adj_ptr->next;
		}
		
		FlowTable<<fixed<<right<<setw(8)<<flow_table[i].patchID
			<<setw(12)<<setprecision(1)<<flow_table[i].x
			<<setw(12)<<setprecision(1)<<flow_table[i].y
			<<setw(12)<<setprecision(1)<<flow_table[i].z
			<<setw(10)<<setprecision(2)<<flow_table[i].acc_area
			<<setw(4)<<flow_table[i].land
			<<setw(12)<<setprecision(5)<<flow_table[i].total_gamma
			<<setw(4)<<flow_table[i].num_adjacent-count<<endl;
			
		/*
		fprintf(outfile,"\n %-8d %-8.1f %-8.1f %-7.1f %-10.2f %-4d %-12.5f %-4d",  
						flow_table[i].patchID,  
						flow_table[i].x,
						flow_table[i].y,
						flow_table[i].z,
						flow_table[i].acc_area,
						flow_table[i].land,
						flow_table[i].total_gamma,
						1);
		*/
		
		adj_ptr = flow_table[i].adj_list;
		for (j=1; j<= flow_table[i].num_adjacent; j++) {
			if(adj_ptr->gamma>0.)
				FlowTable<<setw(13)<<adj_ptr->patchID<<setw(13)<<setprecision(8)<<adj_ptr->gamma<<endl;
			//if(adj_ptr->gamma ==unidirectional)
			//	fprintf(outfile, "\n%13d %13.8f ",adj_ptr->patchID, 1.);

			adj_ptr = adj_ptr->next;
		}
		
		if (flow_table[i].land == 2) {
			FlowTable<<setw(16)<<flow_table[i].stream_ID.patch<<setw(1)<<width<<endl;
		}
	}

    FlowTable.close();

	return;
}



