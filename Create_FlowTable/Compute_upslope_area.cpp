//--------------------------------------------------------------
//  NAME:: compute_upslope_area									
//                                                              
//  DESCRIPTION                                                 
//	sorts pches by elevation									
//	computes upslope_area for neighbours of each pch			
//--------------------------------------------------------------

#include "blender.h"

void compute_upslope_area(struct flow_struct *flow_table, int num_patches, int rflag,double cell)
{
 	// local fuction declarations
	//struct ID_struct  sort_flow_table();
	int	find_patch();

 	// local variable declarations
	int inx;
	int neigh;
	int pch;

	struct adj_struct *aptr;

	// max_ID = sort_flow_table(flow_table, num_patches); 
	// send area to each upslope neighbour for each patch in the sort list

	for (pch = 1; pch <= num_patches; pch++) {
		aptr = flow_table[pch].adj_list;
		flow_table[pch].acc_area += flow_table[pch].area;
		//if(flow_table[pch].patchID==15157) {
		//	printf("\n %d %f \n",flow_table[pch].patchID,flow_table[pch].acc_area);
		//	getchar();
		//}
		// process roads as all accumulated area going to stream (or receiving patch)
		if ( (flow_table[pch].land == 2) && (rflag == 1) ) {
			inx = flow_table[pch].stream_inx;
			flow_table[inx].acc_area +=  flow_table[pch].acc_area;
		}

		//otherwise distribute accumulated area to downstream neighbours
		else {
			for (neigh = 1; neigh <= flow_table[pch].num_adjacent; neigh ++) {
				inx = aptr->inx;
				flow_table[inx].acc_area += (flow_table[pch].acc_area) * aptr->gamma;
				aptr = aptr->next;
			}
		}	
	}
	
	return;
}



