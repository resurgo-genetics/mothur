/*
 *  weighted.cpp
 *  Mothur
 *
 *  Created by Sarah Westcott on 2/9/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

#include "weighted.h"

/**************************************************************************************************/

EstOutput Weighted::getValues(Tree* t) {
    try {
	
		int numGroups = tmap->getNumGroups();
		
		//calculate number of comparisons i.e. with groups A,B,C = AB, AC, BC = 3;
		int n = 1;
		for (int i=1; i<numGroups; i++) { 
			for (int l = n; l < numGroups; l++) {
				//initialize weighted scores
				WScore[tmap->namesOfGroups[i-1]+tmap->namesOfGroups[l]] = 0.0;
			}
		}

		data.clear(); //clear out old values
	
		double D = 0.0000;
	
		for(int i=0;i<t->getNumLeaves();i++){
			int index = i;
			double sum = 0.0000;
		
			//while you aren't at root
			while(t->tree[index].getParent() != -1){
		
				if(t->tree[index].pGroups.size() != 0){
					sum += t->tree[index].getBranchLength();
				}
			
				//old_index = you
				int old_index = index; 
				//index = your parent   
				index = t->tree[index].getParent();
			
				//if you grandparent is the root 
				if(t->tree[index].getParent() == -1 && t->tree[old_index].pGroups.size() != 0){
					int lc = t->tree[t->tree[index].getLChild()].pGroups.size();
					int rc = t->tree[t->tree[index].getRChild()].pGroups.size();
				
				
					if(lc == 0 || rc == 0){
						sum -= t->tree[old_index].getBranchLength();
					}
				}
			}
	
			if(t->tree[i].getGroup() != ""){
				sum /= (double)tmap->seqsPerGroup[t->tree[i].getGroup()];
				D += sum; 
			}
		}
	
	
		for(int i=0;i<t->getNumNodes();i++){
			//calculate weighted score for each of the group comb i.e. with groups A,B,C = AB, AC, BC.
			n = 1;
			for (int b=1; b<numGroups; b++) { 
				for (int l = n; l < numGroups; l++) {
					double u;
					//does this node have descendants from group b-1
					it = t->tree[i].pcount.find(tmap->namesOfGroups[b-1]);
					//if it does u = # of its descendants with a certain group / total number in tree with a certain group
					if (it != t->tree[i].pcount.end()) {
						u = (double) t->tree[i].pcount[tmap->namesOfGroups[b-1]] / (double) tmap->seqsPerGroup[tmap->namesOfGroups[b-1]];
					}else { u = 0.00; }
		
					//does this node have descendants from group l
					it = t->tree[i].pcount.find(tmap->namesOfGroups[l]);
					//if it does subtract their percentage from u
					if (it != t->tree[i].pcount.end()) {
						u -= (double) t->tree[i].pcount[tmap->namesOfGroups[l]] / (double) tmap->seqsPerGroup[tmap->namesOfGroups[l]];
					}
						
					u = abs(u) * t->tree[i].getBranchLength();
					
					//save groupcombs u value
					WScore[tmap->namesOfGroups[b-1]+tmap->namesOfGroups[l]] += u;

				}
				n++;
			}
		}
  
		//calculate weighted score for each group combination
		double UN;	
		n = 1;
		for (int i=1; i<numGroups; i++) { 
			for (int l = n; l < numGroups; l++) {
				UN = (WScore[tmap->namesOfGroups[i-1]+tmap->namesOfGroups[l]] / D);
				if (isnan(UN) || isinf(UN)) { UN = 0; } 
				data.push_back(UN);
			}
		}

		return data;
	}
	catch(exception& e) {
		cout << "Standard Error: " << e.what() << " has occurred in the Weighted class Function getValues. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}
	catch(...) {
		cout << "An unknown error has occurred in the Weighted class function getValues. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}

}

/**************************************************************************************************/