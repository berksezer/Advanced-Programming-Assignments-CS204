#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "strutils.h"




using namespace std;

struct metroStop {
  string metroStopName;
  metroStop *left;
  metroStop *right;  

  metroStop(){}

  metroStop(string name, metroStop* leftp, metroStop* rightp) {
	  metroStopName = name;
	  left = leftp;
	  right = rightp;
  }

};

struct metroLine {
  string metroNumber;
  metroLine *next;  
  metroStop *metroStops;

  vector<double>costs;

  metroLine(){}

  metroLine(string name, metroLine* nextp, metroStop* metroStopsp, vector<double> cost) {
	  metroNumber = name;
	  next = nextp;
	  metroStops = metroStopsp;
	  costs = cost;
  }

};


metroLine* metroLineMaker(string fnameLine, string fnameCost) { // will form a 2d dynamic data structure for all the metro lines
	ifstream in, in2;
	string line, lineCost;
	metroLine* Linehead = nullptr; // this is our main head which will we use for further actions
	
	in.open(fnameLine.c_str());
	
	

	

	if (!in.fail() && !in2.fail()){
	
		while(getline(in,line)) { // looks at the line txt lines
			string Mnumber;
			int index;
			vector<double> costs;
			index = line.find(",");
			Mnumber = line.substr(0,index);
			metroStop *stopHead = nullptr; // head of the stops

			in2.open(fnameCost.c_str());
			while(getline(in2,lineCost)) { // looks at the costs txt file
				string mnumberCheck;
				int indexCheck;
				indexCheck = lineCost.find(",");
				mnumberCheck = lineCost.substr(0,indexCheck); // checks if it is the metro line we are looking at
				lineCost = lineCost.substr(indexCheck+1); // there exist only the costs not the name

				if(mnumberCheck == Mnumber) { // if it the right line we add the costs
					while(lineCost.find(",") != string::npos) { // it will look for the cost in that line
						indexCheck = lineCost.find(",");
						double cost = stoi(lineCost.substr(0,indexCheck));
						costs.push_back(cost); // push the costs of that line into a vector
						
						lineCost = lineCost.substr(indexCheck+1); // erasing the useless part of the line
					}
					double cost = atof(lineCost); // there will be one cost left we add it too
					costs.push_back(cost);
				}
			}
			in2.close();

			metroLine *tempLine = new metroLine(Mnumber, nullptr,nullptr,costs); // temporary line ptr
			
			
				
			line = line.substr(index+1); // erased the line number from the line

			while(line.find(",") != string::npos || line != "#") {// it will look for the stops in the particular line
				if(line.find(",") == string::npos) { // if only one stop remains we will continue from here
					index = line.find("#");
					string stop = line.substr(0,index);
					line = line.substr(index);

					metroStop *ptr = stopHead; 
					metroStop *tail = nullptr; // we need to form a tail
					metroStop *temp = new metroStop(stop,nullptr,nullptr); // temporary stop ptr

					if(stopHead == nullptr) { // if there isn't any stops pointed (not very likely but need to check)
						stopHead = temp;
					}
					else {
							
						while(ptr->right != nullptr) { // will find the last place 
							ptr = ptr->right;
							
						}
						tail = ptr; 
						ptr->right = temp; // the place where the stop will be set
						temp->left = tail; // the tail will be pointing the previous stop
					
					}

				}
				else {
					index = line.find(",");
					string stop = line.substr(0,index); // found the stop name
						
					

					
					metroStop *ptr = stopHead;
					metroStop *tail = nullptr; // we need to form a tail
					metroStop *temp = new metroStop(stop,nullptr,nullptr); // temporary stop ptr

					if(stopHead == nullptr) { // if there isn't any stops pointed temp will be the head of stops
						stopHead = temp;
					}
					else {
							
						while(ptr->right != nullptr) { // will find the last place
							ptr = ptr->right;
							
						}
						tail = ptr;
						ptr->right = temp; // the place where the stop will be set
						temp->left = tail; // the tail will be pointing the previous stop
					
					}
					line = line.substr(index+1); //erasing the used stop
				}
				
					
			
			}

			metroLine *ptr = Linehead; // will look at the places with this ptr to protect the real head
			if(Linehead == nullptr) { // it is the first line 
				tempLine->metroStops = stopHead; // templine will be pointing  the stops
				Linehead = tempLine; // also our main head will be this temporary ptr
			}

			else { // it is not the first line

				while(ptr->next != nullptr) { 
					ptr = ptr->next;
				}

				ptr->next = tempLine; // it will be put in the end of the metrolines
				tempLine->metroStops = stopHead; // this tempLine will be pointing the stops
			}
					
			
		}

		
		return Linehead;

	}
	
	in.close();
	
}

bool consistencyCheck(metroLine *head) {
  metroLine* currBL = head;
  while(currBL) {
    metroStop* currBS = currBL->metroStops;
    while(currBS) {
      metroStop* rightBS = currBS->right;
      if(rightBS && rightBS->left != currBS) {
	cout << "Inconsistency for " 
	     << currBL->metroNumber 
	     << " " 
	     << currBS->metroStopName << endl;
	return false;
      }
      currBS = currBS->right;
    }
    currBL = currBL->next;
  }
  return true;
}

int metroLineLen(metroLine* head) { // finds the length of one metroline
	int lenCounter = 0;
	metroStop* tempStop = head->metroStops;

	while(tempStop) {
		tempStop = tempStop->right;
		lenCounter++;
	}
	return lenCounter;
}

void printMainMenu() {
  cout << endl;
  cout <<"I***********************************************I"<<endl
       <<"I               0 - EXIT PROGRAM                I"<<endl
       <<"I               1 - PRINT LINES                 I"<<endl
       <<"I               2 - ADD METRO STOP              I"<<endl
       <<"I               3 - DELETE METRO STOP           I"<<endl
       <<"I               4 - PATH FINDER                 I"<<endl
       <<"I***********************************************I"<<endl
       <<">>";
  cout << endl;
}  

bool metroLineExist(string metroLineName, metroLine* &currentLine) { // checks if the given metro line exists
	
	while(currentLine) {
		if(currentLine->metroNumber == metroLineName) {
			return true;
		}
		currentLine = currentLine->next;
	}

	return false;

}

bool metroStopExistsAnywhere(metroLine* head, string stopName){ // checks if there exist the particular stop in any line
	metroLine* temp = head;

	while(temp) {
		metroStop* tempStop = temp->metroStops;

		while(tempStop) {
			if(tempStop->metroStopName == stopName) {
				return true;
			}

			tempStop = tempStop->right;
		}

		temp = temp->next;
	}

	return false;


}

vector<string> findLines(metroLine* head, string stopName){ // finds the lines which include the given stops
	metroLine* temp = head;
	vector<string> lines;

	while(temp) {
		metroStop* tempStop = temp->metroStops;

		while(tempStop) {
			if(tempStop->metroStopName == stopName) {
				lines.push_back(temp->metroNumber);
			}

			tempStop = tempStop->right;
		}

		temp = temp->next;
	}

	return lines;


}

bool metroStopExist(metroLine* head, string metroStopName, string metroLineName) { // checks if the given metro stop exists in a line
	metroLine* currentLine = head;

	while(currentLine) {
		if(currentLine->metroNumber == metroLineName) {
			metroStop* currentStop = currentLine->metroStops;

			while(currentStop) {
				if(currentStop->metroStopName == metroStopName) {
					return true;
				}
				currentStop = currentStop->right;
			}

		}
		currentLine = currentLine->next;
		
	}
	return false;
}

void printmetroLinesDetailed(metroLine *head) { // printing the metro lines
	metroLine* currentLine = head;

	while(currentLine) { // while there is a line 
		metroStop* currentStop = currentLine->metroStops; 
		cout << currentLine->metroNumber <<": "; // writing the number of the metro line

		int length = metroLineLen(currentLine);
		int counter = 0; // counts the number of stops

		while(currentStop) { // while there are stops
			if(counter != length-1) { // The last stop must be printed differently
				cout << currentStop->metroStopName << "<-" << currentLine->costs[counter] << "->" ;
				currentStop = currentStop->right;
				counter++;
			}
			else {
				cout << currentStop->metroStopName;
				currentStop = currentStop->right;
				
			}
		}
			
		cout << endl;

		currentLine = currentLine->next;
		
	}
	
}

void printMetroLine(metroLine* head, string metroLineName) { // prints only one line
	metroLine* currentLine = head;
	while(currentLine) {
		if(currentLine->metroNumber == metroLineName) {
			
			cout << currentLine->metroNumber <<": ";

			metroStop* currentStop = currentLine->metroStops;
			int length = metroLineLen(currentLine);
			int counter = 0; // shows which stop we are on

			while(currentStop) {
				if(counter != length-1) { // The last stop must be printed differently
					cout << currentStop->metroStopName << "<-" << currentLine->costs[counter] << "->" ;
					currentStop = currentStop->right;
				counter++;
				}
				else {
					cout << currentStop->metroStopName << endl;
					currentStop = currentStop->right;
				
				}
			}
		}

		currentLine = currentLine->next;
	}
}

void addToBeginning(metroLine* currentLine, double cost1, string newStopName) { // adding the stop to the begining

	metroLine* tempLine = currentLine; 
	metroStop* firstStop = currentLine->metroStops; // this points to the first stop

	metroStop* temp = new metroStop(newStopName,nullptr,firstStop); // the new stop we want to add

	firstStop->left = temp; // we change the left of the first stop from nullptr to new stop
	tempLine->metroStops = temp; // also the head of the line now points to the new one
	currentLine = tempLine;

	vector<double> newCosts; 
	newCosts.push_back(cost1); // the first cost is pushed before others

	for (int i = 0; i < currentLine->costs.size(); i++) { // copying the past costs to the new vector
		newCosts.push_back(currentLine->costs[i]);
	}
	
	currentLine->costs = newCosts;

}

int findLocation(metroLine* currentLine, string previousStopName) { // finds the stops location in a given line
	metroStop* currStop = currentLine->metroStops;
	
	int index = 1;

	while(currStop) {
		if(currStop->metroStopName == previousStopName) {
			return index;
		}
		index++;
		currStop = currStop->right;
	}
}

int findLocationAnywhere(metroLine* head, string metroLineName, string stopName, metroLine* &currentLine) { // searches all the metrolines in order the find the line and the stop in it
	metroLine* current = head;
	
	while(current) {
		if(current->metroNumber == metroLineName) {
			currentLine = current;
			metroStop* currentStop = current->metroStops;
			int index = 1;

			while(currentStop) {
				if(currentStop->metroStopName == stopName) {
					return index;
				}
				index++;
				currentStop = currentStop->right;
			}

		}
		current = current->next;
	
	}
}

double calculateCost(metroLine* currentLine, int currentIndex, int targetIndex) { // calcualtes the cost between two stops
	currentIndex = currentIndex-1; // changing the values as vector indexes
	targetIndex = targetIndex -1; 
	vector<double> lineCosts = currentLine->costs;
	double cost = 0;

	if (currentIndex > targetIndex) {
		for(int i = targetIndex; i < currentIndex; i++) {
			
			cost = cost + lineCosts[i];
	
		}
		return cost;
	}
	else {
		for(int i = currentIndex; i < targetIndex; i++ ) {
			
			cost = cost + lineCosts[i];
		}
		return cost;
	
	}
	
}

void addToEnd(metroLine* currentLine, double cost1, string newStopName) { // adding the stop to the end of line 
	
	metroStop* tempStops = currentLine->metroStops; 
	metroStop* temp = new metroStop(newStopName,nullptr,nullptr); // the new stop we will add

	while(tempStops) {
		if(tempStops->right == nullptr) { // we are looking for the last element

			tempStops->right = temp; // past last element will poin to the new temp
			temp->left = tempStops; // the new stop temp will point to the past temp
			break; // if we find it we will break out 
		}
		tempStops = tempStops->right; 
	}

	currentLine->costs.push_back(cost1); // need to push cost1 to the costs vector




}

void addInBetween(metroLine* currentLine, double cost1, double cost2, string newStopName, int location) { // adds the stop somewhere which is not start or end
	metroStop* tempStops = currentLine->metroStops;
	metroStop* temp = new metroStop(newStopName,nullptr,nullptr);
	int counter = 0; // will count the number of stops seen
	metroStop* tail; // need a tail because we are putting the stop betweem two pointer 

	while(tempStops) {
		if(counter == location) {
			tempStops->left = temp;
			tail->right = temp;
			temp->right = tempStops;
			temp->left = tail;
		}

		counter++;
		tail = tempStops;
		tempStops = tempStops->right;
	}

	vector<double> newCosts;

	for(int i = 0; i <= currentLine->costs.size(); i++){
		if(i<location-1) { 
			newCosts.push_back(currentLine->costs[i]);
		}
		else if (i == location-1) { // pushing the first cost
			newCosts.push_back(cost1);
		}
		else if (i == location) {
			newCosts.push_back(cost2); // the second cost
		}
		else {
			newCosts.push_back(currentLine->costs[i-1]); // we will discard one cost which we don't need anymore
		}
	}

	currentLine->costs = newCosts;
	
}

void deleteFirst(metroLine* currentLine) {
	
	metroLine* tempLine = currentLine;
	metroStop* firstStop = tempLine->metroStops; // first stop (to be deleted)
	metroStop* secondStop = tempLine->metroStops->right; // second stop
	
	delete firstStop; // deleted the first one
	currentLine->metroStops = secondStop; // stop points the second
	secondStop->left = nullptr; // left of second is nullptr

	vector<double> newCosts;

	for(int i=1; i < currentLine->costs.size();i++) { // pushing all the cost except the first cost
		newCosts.push_back(currentLine->costs[i]);
	}

	currentLine->costs = newCosts;
} 

void deleteLast(metroLine* currentLine) { // deletes the last stop

	metroLine* tempLine = currentLine;
	metroStop* temp = currentLine->metroStops;
	metroStop* tail; 

	while(temp) {

		if(temp->right == nullptr) {
			tail->right = nullptr; // the new last stop is pointng nullptr on right
			delete temp; // deleting temp
			break; // after we delete we can break out
		}
		tail = temp;
		temp = temp->right;
	}

	vector<double> newCosts;

	for(int i = 0; i < currentLine->costs.size()-1;i++) { // pushing cost except last one
		newCosts.push_back(currentLine->costs[i]);
	}

	currentLine->costs = newCosts;
	
}

void deleteInBetween(metroLine* currentLine, int location) { // deletes the stops which are not the first nor the last
	
	metroLine* tempLine = currentLine;
	metroStop* temp = tempLine->metroStops;
	int counter = 1;

	while(temp) {
		if(counter == location) {

			temp->left->right = temp->right; // letting the pointers point each other 
			temp->right->left = temp->left;

			delete temp;
			break; // after delelting will break out
		}
		counter++;
		temp = temp->right;
	}

	vector<double> newCosts;
	double sum = 0; //will merge the costs
	 
	for(int i=0; i < currentLine->costs.size();i++) {
		
		if(i == location-2) {
			sum = currentLine->costs[i];
		}
		else if (i == location-1) {
			sum = sum + currentLine->costs[i];
			newCosts.push_back(sum);
		}
		else {
			newCosts.push_back(currentLine->costs[i]);
		}

	}

	currentLine->costs = newCosts;


}

void deletTheLast(metroLine* currentLine) { // deletes the last stop in a line
	metroLine* tempLine = currentLine;
	metroStop* theLast = tempLine->metroStops;
	currentLine->metroStops = nullptr;
	delete theLast;
	cout << "No stops left" << endl;

}

void checkEmptyLine(metroLine* head) { // checks if there is a empty line
	metroLine* tempLine = head;
	metroLine* tail = nullptr;


	while(tempLine) { // looks all the lines
		if(tempLine->metroStops == nullptr) { // if there are no stops
			if(tail != nullptr){ // if it is not the first metro line
				tail->next = tempLine->next; 
				delete tempLine; // deletes and breakes out
				break;
			}
			else if(tail == nullptr) { // if it is the first metro line the tail will be nullptr
				delete tempLine;
				break;
			}
			else if(!tempLine->next) { // the empty line is the last one
				tail->next = nullptr;
				delete tempLine;
				break;
			}
			
		}

		tail =  tempLine;
		tempLine = tempLine->next;
	}
}

void addmetroStop(metroLine *head){
	string metroLineName;
	bool check0 = false;


	cout << "Enter the name of the metro line to insert a new metro stop (0 for main menu)" << endl;
	cin >> metroLineName;

	metroLine* currentLine = head; // we will need this 

	if(metroLineName == "0") { // if the user enter 0 we go back to the main menu
		check0 = true;
	}
	
	else if(!metroLineExist(metroLineName, currentLine))  { // checking if metroline exists and also passes the pointer of the current line
		cout << "Metro line cannot be found. Going back to previous menu." << endl;
	}

	else {
		cout << "The metro line information is shown below" << endl;
		printMetroLine(head, metroLineName); // printing the line 
		string newMetroStopName;

		cout << "Enter the name of the new metro stop" << endl; 
		cin >> newMetroStopName;

		if(metroStopExist(head, newMetroStopName, metroLineName))  { // checking if the metro stop exists
			cout << "Metro stop already exits. Going back to previoes menu." << endl;
		}

		else {
			string previousStop;
			double cost1, cost2;
			
			cout << "Enter the name of the previous metro stop for the new one (0 to put the new one as the first metro stop)" << endl;
			cin >> previousStop;

			cout << "Enter new cost 1" << endl;
			cin >> cost1;

			cout << "Enter new cost 2" << endl;
			cin >> cost2;

			bool mainMenu = false; // will be used at going back to main menu after a typo 

			while(!metroStopExist(head,previousStop,metroLineName) && previousStop != "0") {
				cout << "Metro stop does not exist. Typo? Enter again (0 for main menu)" << endl;
				cin >> previousStop;
				if(previousStop == "0") {
					mainMenu = true;
				}
			}

			int lineLen = 0; // len of the line
			int previousLocation = 0; // the location of the previous stop we are looking for

			if(previousStop != "0" && !mainMenu) { 
				lineLen = metroLineLen(currentLine);
				previousLocation = findLocation(currentLine, previousStop);
			}


			if(previousStop == "0" && !mainMenu) {
				addToBeginning(currentLine,cost1, newMetroStopName);
				printMetroLine(head, metroLineName);
			}
			
			else if (lineLen == previousLocation && !mainMenu) {
				addToEnd(currentLine,cost1,newMetroStopName);
				printMetroLine(head,metroLineName);
			}
			else if(!mainMenu) {
				cout << previousLocation << endl;
				previousLocation = findLocation(currentLine,previousStop);
				addInBetween(currentLine, cost1,cost2, newMetroStopName,previousLocation);
				printMetroLine(head,metroLineName);
			}
		}

	}

}

void deletemetroStop(metroLine *head){

	string metroLineName;
	bool check0 =  false;

	cout << "Enter the name of the metro line to delete a new metro stop (0 for main menu)" << endl;
	cin >> metroLineName;

	metroLine* currentLine = head; // we will need this 

	if(metroLineName == "0") { // if the user enter 0 we go back to the main menu
		check0 = true;
	}

	else if(!metroLineExist(metroLineName, currentLine))  { // checking if metroline exists and also passes the pointer of the current line
		cout << "Metro line cannot be found. Going back to previous menu." << endl;
	}
	else {
		cout << "The metro line information is shown below" << endl;
		printMetroLine(head, metroLineName); // printing the line 
		string toBeDeleted;

		cout << "Enter the name of the metro stop to delete (0 for main menu)" << endl;
		cin >> toBeDeleted;
		
		while(!metroStopExist(head, toBeDeleted, metroLineName) && toBeDeleted != "0")  { // checking if the metro stop exists
			cout << "Metro stop cannot be found. Enter the name of the metro stop to delete (0 for main menu)" << endl;
			cin >> toBeDeleted;
		}

		int lineLen = 0; // len of the line
		int deleteLocation = 0; // the location of the to be deletedwe are looking for

		if(toBeDeleted != "0") {
			lineLen = metroLineLen(currentLine);
			deleteLocation = findLocation(currentLine, toBeDeleted);
			
		}

		if(lineLen == 1) {
			deletTheLast(currentLine);
			checkEmptyLine(head); // if there are empty lines deleting them
		}

		else if(deleteLocation == 1) {
			deleteFirst(currentLine);
			printMetroLine(head,metroLineName);
			checkEmptyLine(head);

		}

		else if(deleteLocation == lineLen && toBeDeleted != "0") {
			deleteLast(currentLine);
			printMetroLine(head,metroLineName);
			checkEmptyLine(head);

		}
		else if(toBeDeleted != "0" ) {
			deleteInBetween(currentLine,deleteLocation);
			printMetroLine(head, metroLineName);
			checkEmptyLine(head);

		}


	}
	
}

void pathFormer(metroLine* head, int hopNum ,int hopCounter, int &minHop , int cost, int &minCost, string pathStr, string &minPathStr, string currPlace, string targetPlace, vector<string> currLines, vector<string> targetLines, vector<string>alreadyChecked) { // finds the most efficient path
	string tempPathString = pathStr;
	hopCounter++;
	bool isFirst = true;

	for(int i = 0; i < currLines.size(); i++) { // looks for the possible starting lines
		pathStr = tempPathString;
		
		string startLine = currLines[i];
		pathStr = pathStr + "/" + startLine + "," + currPlace; // this will form the main key for the path


		int currIndex, lineLen;
		metroLine* currentLine;
		bool isChecked = false;

		if(alreadyChecked.size() == 0) { // checking if the line is already checked before
			alreadyChecked.push_back(startLine);
		}
		else {
			for(int o = 0; o < alreadyChecked.size(); o++) {
				if(alreadyChecked[o] == startLine) {
					isChecked = true;
				}
			}
		}


		if(!isChecked) {

			currIndex = findLocationAnywhere(head, startLine, currPlace, currentLine);

			metroStop* currStop = currentLine->metroStops;

			lineLen = metroLineLen(currentLine);

			while(currStop) {
				if (currStop->metroStopName == targetPlace) { // found the target
					pathStr = pathStr + "," + currStop->metroStopName + "#"; 

					int targetIndex = findLocationAnywhere(head, startLine, currStop->metroStopName, currentLine);
					cost = cost + calculateCost(currentLine, currIndex, targetIndex);	
					
					if(minCost == 0 || minCost > cost) {
						minCost = cost;
						minPathStr = pathStr; // the string key for the minPath but it needs to be cleaned
						minHop = hopCounter -1;
					}
				
			
				}
				else if(currStop->metroStopName != currPlace) {
					string currentStopName = currStop->metroStopName;
					string tempPathStr = pathStr;
					pathStr = pathStr + "," + currentStopName;

					int targetIndex = findLocationAnywhere(head, startLine, currentStopName, currentLine);
					int tempCost = cost;

					cost = cost + calculateCost(currentLine, currIndex, targetIndex);

					vector<string> newCurrLines = findLines(head, currentStopName); // finding the lines which have the new stop in int
					vector<string> checkedCurrentLines; // they also need to be checked if they previously processed in order to prevent a loop
	
					for(int j = 0; j < newCurrLines.size(); j++) {
						if (!(newCurrLines[j] == startLine)) { // it shouldn't go back to the line it started
							checkedCurrentLines.push_back(newCurrLines[j]);
						}
					}

					if(checkedCurrentLines.size() == 0 ) { // if there are no other lines we go back to searching
						cost = tempCost;
						
					}
				
					else if(hopCounter <= hopNum) {  // if we can still hop will enter a recursive process
						
						pathFormer(head, hopNum, hopCounter, minHop, cost, minCost, pathStr, minPathStr, currentStopName, targetPlace, checkedCurrentLines, targetLines, alreadyChecked);
						cost = tempCost;
						
					}
					else { // other wise continue looking for other stops in that line
						cost = tempCost;
					}
				}

				currStop = currStop->right;
			}
		}
		
	}
	

	

}

void printInBetween(metroLine* head, string start, string end, string LineNum) { // printing the stops between two given stops
	metroLine* currentLine;
	
	int startLoc, endLoc;

	

	startLoc = findLocationAnywhere(head,LineNum,start,currentLine);
	endLoc = findLocationAnywhere(head,LineNum,end,currentLine);
	metroStop* tempStop = currentLine->metroStops;


	if(endLoc > startLoc) { 
		int indexCounter = 1;
		while(tempStop) { // starting from the beggining and looking for the given indexes
			if(indexCounter >= startLoc && indexCounter <= endLoc) { 
				cout << tempStop->metroStopName << " ";
			}
			indexCounter++;
			tempStop = tempStop->right;

		}
		cout << endl;
	}

	else if (startLoc > endLoc) {
		while(tempStop->right != nullptr) { // going to the end of the line 
			tempStop = tempStop->right;
		}
		int lenCounter = metroLineLen(currentLine);
		while(tempStop) { // now looking backwards and printing that way
			if(lenCounter <= startLoc && lenCounter >= endLoc) {
				cout << tempStop->metroStopName << " ";
			}
			lenCounter--;
			tempStop = tempStop->left;
		}
		cout << endl;
	
	}

}

void pathParser(string minPathStr, metroLine* head, string startingLoc) { // will clean the path string obtained from pathFormer and print it
	
	minPathStr = minPathStr.substr(1); // deleting the first "/" at the start

	while(minPathStr != "#") {
		int index, index2;
		string firstStop, lastStop, LineName; // the first and the last stops in the line
		bool isLast = false;
		index = minPathStr.find(",");
		LineName = minPathStr.substr(0,index);

		
		
		

		minPathStr = minPathStr.substr(index+1);
		index2 = minPathStr.find(",");

		firstStop = minPathStr.substr(0,index2); // found the first stop of the line
		index = minPathStr.find("/");
		index2 = minPathStr.rfind(",",index);
		
		lastStop = minPathStr.substr(index2+1 , index-index2-1); // found the last stop of the line 
		
		cout << LineName << ": ";
		

		if(lastStop.find("#") != string::npos) { // if there is only one stop left it will have "#"
			lastStop = lastStop.substr(0,lastStop.length()-1); // removing the "#"
			isLast = true;
		}
		printInBetween(head,firstStop,lastStop,LineName);
		
		if (isLast) { // if it was the last element assigning the minpathstr to "#"
			minPathStr = "#";
		}
		else {
			minPathStr = minPathStr.substr(index+1);
		}
		
	}
	


	
}

void pathfinder(metroLine* head, int hopNum){ // finds the path with the least cost
	string currPlace, targetPlace;
	vector<string> currLines, targetLines;
	int minCost = 0, cost = 0, minHop = 0;
	string pathStr = "", minPathStr = "";
	int hopCounter = 0;

	cout << "Where are you now" << endl;
	cin >> currPlace;

	cout << "Where do you want to go?" << endl;
	cin >> targetPlace;

	auto start = chrono::steady_clock::now();

	if(!metroStopExistsAnywhere(head,currPlace) || !metroStopExistsAnywhere(head,targetPlace)) {
		cout <<	"Metro stop does not exist in the table. Going back to previous menu." << endl;
	}
	else {
		currLines = findLines(head, currPlace);
		targetLines = findLines(head, targetPlace);
		vector<string> alreadyChecked;
		pathFormer(head, hopNum, hopCounter, minHop, cost, minCost, pathStr, minPathStr, currPlace, targetPlace, currLines, targetLines, alreadyChecked); // enter the recursion

		auto end = chrono::steady_clock::now();
		
		
		if (minCost == 0) { // if mincost is 0 then there isn't a possible path 
			cout << "No path found between " << currPlace << " and " << targetPlace << endl;
			cout << "Elapsed time: " << chrono::duration_cast<chrono::nanoseconds>(end-start).count() * 0.000000001 << " seconds" << endl;
		}
		else {
			cout << "Best cost path:" << endl;
			pathParser(minPathStr, head, currPlace);
			cout << "Hops: " << minHop << endl;
			cout << "Cost: "<< minCost << endl;
			cout << "Elapsed time: " << chrono::duration_cast<chrono::nanoseconds>(end-start).count() * 0.000000001 << " seconds" <<  endl;
		}
	}
}

void deleteLines(metroLine* head) { // deletes the metrostops and lines in order to free the memory

	while(head) {
		metroLine* temp = head;

		metroStop* currentStop = temp->metroStops;

		while(currentStop) {
			metroStop* tempStop = currentStop;
			currentStop = currentStop->right;
			delete tempStop;
		}
		head = head->next;
		delete temp;

	}


}

void processMainMenu(metroLine *head, int hopNum) {
  char input;
  do{
    if(!consistencyCheck(head)) {
      cout << "There are inconsistencies. Exit." << endl; 
      return;
    }
    printMainMenu();
    cout << "Please enter your option " << endl;
    cin >> input;
	
    switch (input) {
    case '0':
      cout << "Thanks for using our software" << endl;
      return;
    case '1':
      printmetroLinesDetailed(head);
      break;
    case '2':
      addmetroStop(head);
      break;
    case '3':
      deletemetroStop(head);
      break;
    case '4':
      pathfinder(head, hopNum);
      break;
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}


int main() {
	string fnameLine, fnameCost;
	metroLine *head;
	int hopNum;

	cout << "Enter the name of the metro line data file:" << endl;
	cin >> fnameLine;

	cout << "Enter the name of costs file:" << endl;
	cin >> fnameCost;

	cout << "Enter maximum number of hops:"<< endl;
	cin >> hopNum;

	head = metroLineMaker(fnameLine,fnameCost);

	processMainMenu(head,hopNum);

	deleteLines(head);



	return 0;
}

// Berk Sezer