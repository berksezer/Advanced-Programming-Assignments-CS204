#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>


using namespace std;


void rowFinder(string fname, int &rowNum){ // finds the criteria regarding the rownumber 
	fstream in;
	in.open(fname.c_str());
	if(!in.fail()){ //check if the file could be opened 
		in >> rowNum;
	}
	
}

vector<vector<string> > generateMat (string fname, bool &checkOpen) { // makes the data in the txt files matrixes
	int counter = 0; //tracks the row of txt file
	ifstream in;
	string line;
	vector<vector<string> > finalVec;
	vector<string> row;

	in.open(fname.c_str());
	if(in.fail()){ //check if the file could be opened 
		cout << "could not open the file " << fname << endl;
		checkOpen = false;

	}
	
	while(getline(in,line)) {

		if(counter != 0) {
			
			if (line != "") {
				
				stringstream ss(line);
			
				string num;
				row.clear();

				while(ss >> num) {
					row.push_back(num);
				}

				finalVec.push_back(row);
			}
		}
		else {
			counter++;
		}
		
	}

	in.close();
	

	return finalVec;

}  

void Print(const vector<vector<string> > & mat) // prints our vector
{
    for (int j=0; j < mat.size(); j++)
    {   
		for (int k=0; k < mat[0].size(); k++)
        {   
			cout <<" " <<  mat[j][k];
        }
        cout << endl;
    }
}



bool checkSize(const vector<vector<string> > &mat, int rownum) { // checks if the rownumber is satisfied
	if (mat.size() == rownum) {
		return true;
	}
	else {
		return false;
	}
} 

bool checkValue(const vector<vector<string> > &mat, int rownum, int &outOfMatValue) { // checks if the values in the matrix are smaller than n
	for(int i = 0; i < mat.size(); i++) {

		for(int k = 0; k < mat[0].size(); k++) {
			if(mat[i][k] != "X"){
				if(stoi(mat[i][k]) > rownum) { // change the type string into ineteger and check if it is bigger than the number n
					outOfMatValue = stoi(mat[i][k]);
					return false;
				}	
			}
		}
	}
	return true;
}

bool checkDuplicateColumn(const vector<vector<string> > &mat) { // checks if there are any duplicates in a column of the board
	vector<string> seen; // stores the integers which are seen in the column

	for(int i = 0; i < mat[0].size(); i++) {
		for(int k = 0; k < mat.size();  k++) {
			string current; // current element
			current = mat[k][i];
			if (current != "X" ){ // if the element is X we are not considering it as a repetition
				for(int j = 0; j < seen.size(); j++){ 
					if(seen[j] == current) { // checks if the element is seen before in the dups vector
						return false; 
					}
				
				}
				seen.push_back(current); 
		}
			
		}
		seen.clear(); // clearing the elements of the vector before getting into the other column
	}
	return true;
}

bool checkDuplicateRow(const vector<vector<string> > &mat) { // checks if there are any duplicates in a row of the board
	vector<string> seen; // stores the integers which are seen in the row

	for(int i = 0; i < mat.size(); i++) {
		for(int k = 0; k < mat[0].size(); k++) {
			string current; // current element
			current = mat[i][k];
			if(current != "X") { // if the element is X we are not considering it as a repetition
				for(int j = 0; j < seen.size(); j++) {
					if(seen[j] == current) {
						return false;
					}
				}
				seen.push_back(current);
			}
		}
		seen.clear(); // clearing the elements of the vector before getting into the other row
	}
	return true;
}

bool checkConstraints (const vector<vector<string> > &consMat, const vector<vector<string> > &boardMat) { // checks if the constraints holds with the values given in the puzzle
	int counter = 0; // tracks the amount of checks made
	pair <int,int> firstCoor; // the one should be bigger 
	pair <int,int> secondCoor; // the one should be smaller 
	string firstElem, secondElem;
	int rowNum = consMat.size(); 
	
	
	for (int s = 0; s < consMat.size(); s++) { // storing the locations of the constraints in so called coordinate pairs
		
		firstCoor.first = stoi(consMat[s][0])-1; // we need to decrease the values by 1 because they are coordinates and don't start from 0 like index
		firstCoor.second = stoi(consMat[s][1])-1;
		secondCoor.first = stoi(consMat[s][2])-1;
		secondCoor.second = stoi(consMat[s][3])-1;



		firstElem = boardMat[firstCoor.first][firstCoor.second];
		secondElem = boardMat[secondCoor.first][secondCoor.second];

		if(firstElem != "X" && secondElem != "X") { // if they are not X we can compare them
			int firstInt,secondInt;
			firstInt = stoi(firstElem);
			secondInt = stoi(secondElem);

			if(!(firstInt > secondInt)) {
				return false;
			}
		}
			
	}
	
	return true;
	
}

bool checkX(vector<vector<string> > &boardMat) { //checks if the proccess is finished in another word are there any X left in our main board
	for(int i = 0; i < boardMat.size(); i++) {
		for(int k = 0; k < boardMat.size(); k++) {
			if(boardMat[i][k] == "X") { 
				return false;
			}
		}
	}
	return true;
}


void solvePuzzle(vector<vector<string> > &boardMat, const vector<vector<string> > &consMat, bool &moreValue) {
	int totalSum = 0; // the sum of columns and rows
	
	for(int i = 1; i < boardMat.size()+1; i++) { // calculating the sum of one column or row in this board
		totalSum = totalSum + i;
	}

	while(!checkX(boardMat) && !moreValue ){ // continue untill there are no X left or our flag says that there has been no changes after a full loop
		vector<vector<string> > temp = boardMat; // will temporarily hold our board
		for (int j = 0; j < boardMat.size(); j++) {
			for(int k = 0; k < boardMat[0].size(); k++) {
				if(boardMat[j][k] == "X") {
					bool isSolved = false; // will check if first two steps could solve the problem or not. Which these steps are basic sumation with the sum of row and columns
					int currentColumnSum = 0;
					int currenRowSum = 0;
					int Xcounter = 0; // stores how much time X is seen in that row/column

					for(int s = 0; s < boardMat.size(); s++) { // checks all of the columns our X is in
						
						if(boardMat[s][k] == "X"){
							Xcounter++;
						}
						
						else { 
							currentColumnSum = currentColumnSum + stoi(boardMat[s][k]);
						}

					}
					if(Xcounter <= 1) { // if there are less than 1 X values on that column we can solve that X
						int oneValue;
						oneValue = totalSum - currentColumnSum; // solution of X
						boardMat[j][k] = to_string(oneValue); // Writing our solution into the X in the board
						Xcounter = 0;
						isSolved = true; // we can say that we solved this particular X

					}
					
					
					else { // if there are more X values in that previous column we should check the row whether it has only 1 X in it
						Xcounter = 0;
						for(int s= 0; s < boardMat[0].size(); s++) {

							if(boardMat[j][s] == "X") {
								Xcounter++;
							}
							else {
								currenRowSum = currenRowSum + stoi(boardMat[j][s]);
							}
						}
						if(Xcounter <= 1) { // if there are less than 1 X values on that column we can solve that X
							int oneValue;
							oneValue = totalSum - currenRowSum; // solution of X
							boardMat[j][k] = to_string(oneValue); // Writing our solution into the X in the board
							Xcounter = 0;
							isSolved = true;
							
						}
					}

					if (isSolved == false) { // will look at the constraints to help us solve the problem hence summation did not help us
						int xRow = j; // current row which has X value in it
						int xColumn = k; // current column which has X value in it
						vector<string> seenInColumn; // vector to store the values other than X's in the column 
						vector<string> seenInRow; // vector to store the values other than X's in the row 
						

						for(int g = 0; g < boardMat.size(); g++) {
							string current = boardMat[g][k];
							if(current != "X") {	
								seenInColumn.push_back(current); // pushing the elements we see in  the column
							}
						}


						for(int f = 0; f < boardMat[0].size(); f++) {
							string current = boardMat[j][f];
							if(current != "X") {	
								seenInRow.push_back(current); // pushing the elements we see in the row
							}
						}

						vector<int> notSeenInColumn; // it will be better to store the ones we did not see
						vector<int> notSeenInRow;
						vector<int> notSeenInBoth; // takes the unseen common elements of row and column

						for(int g = 1; g < boardMat.size()+1; g++) { // looking up all the numbers whihc could be written in our puzzle
							bool flag = false; // this flag informs if the number is seen in the board matrix
							for(int s = 0; s < seenInColumn.size(); s++) {
								if(g == stoi(seenInColumn[s])) {
									flag = true;
								}
							}
							if(flag != true) {
								notSeenInColumn.push_back(g); // stores the values which are possible X solutions regarding its column
							}
						}

						for(int g=1; g < boardMat.size()+1; g++) { // same as above but now we consider the row which X belongs
							bool flag = false;
							for(int s = 0; s < seenInRow.size(); s++) {
								if(g == stoi(seenInRow[s])) {
									flag = true;
								}
							}
							if (flag != true) {
								notSeenInRow.push_back(g); // stores the values which are possible X solutions regarding its row
							} 
						}

						for(int g= 0; g < notSeenInColumn.size(); g++) { // it will be better to combine every possible answer in a single vector
							for(int s = 0; s < notSeenInRow.size(); s++) {
								if(notSeenInColumn[g] == notSeenInRow[s]) {
									notSeenInBoth.push_back(notSeenInColumn[g]); // this part stores all the possible elements which could be the answer
								}
							}
						}

						

						
						pair <int,int> firstCoor; // both will store the constraint values
						pair <int,int> secondCoor; 

						for(int l = 0; l < consMat.size(); l++) { // storing the coordinates from every row of const txt files
							firstCoor.first = stoi(consMat[l][0]) -1;
							firstCoor.second = stoi(consMat[l][1]) -1;
							secondCoor.first = stoi(consMat[l][2]) -1;
							secondCoor.second = stoi(consMat[l][3]) -1;

							if(j == firstCoor.first && k == firstCoor.second) { // X value is the one which is bigger as constraint file points it
								string otherElem = boardMat[secondCoor.first][secondCoor.second]; // this one is the one we will compare our X
								if(otherElem != "X"){
									int intOtherElem = stoi(otherElem); // making it an integer so that we can compare it
									if(intOtherElem == boardMat.size()-1) { // if the second biggest element is smaller than X it means X must be the biggest element
										boardMat[j][k] = to_string(boardMat.size());
									}
									else {
										vector<int> possibleSolutions; // an empty vector for solutions
										for(int v = 0; v < notSeenInBoth.size(); v++) {
											if(notSeenInBoth[v] > intOtherElem) { // the elements which were absent are looked up. To be considered as a possible solution it should be bigger 
												possibleSolutions.push_back(notSeenInBoth[v]);
											}
										}
										if(possibleSolutions.size() == 1) { // if the size is 1 than we can say that there aren't any other solutions than only one
											boardMat[j][k] = to_string(possibleSolutions[0]);
										}
										else {
											notSeenInBoth = possibleSolutions; // updating our first vector as a result of constraint helps
										}
									}
								}
							}

							else if(j == secondCoor.first && k == secondCoor.second) { // X value is the one which is smaller as constraint file points it
								string otherElem = boardMat[firstCoor.first][firstCoor.second];
								if(otherElem != "X") {
									int intElem = stoi(otherElem);
									if(intElem == 2) { // if the second smallest element is bigger than X, X must be 1 
										boardMat[j][k] = "1";
									}
									else {
										vector<int> possibleSolutions;
										for(int v = 0; v < notSeenInBoth.size(); v++) {
											if(notSeenInBoth[v] < intElem) { // the elements which were absent are looked up. To be considered as a possible solution it should be smaller
												possibleSolutions.push_back(notSeenInBoth[v]);
											}
										}
										if(possibleSolutions.size() == 1) { // if the size is 1 than we can say that there aren't any other solutions than only one
											boardMat[j][k] = to_string(possibleSolutions[0]);
										}
										else {
											notSeenInBoth = possibleSolutions; // updating our first vector as a result of constraint helps
										}
									}
								}
							}
						}

					}
							
					
				}
			}
		}
		if (boardMat == temp) { // checking if our board was able to change after a full loop
			moreValue = true; // if it did not change it means there are some X values that have more than 1 solution
		}
	}

	
}

int main(){
	string boardf , constf; // both filenames 
	bool checkOpenBoard = true;
	bool checkOpenCons = true;
	string s;
	int rowNumBoard, rowNumCons;
	int outOfMatValue = 0; // stores the value which falls out of the matrix if there is any
	vector<vector<string> > boardMat, consMat;
	cout << "Enter the board file: ";
	cin >> boardf;

	cout << "Enter the constraint file: ";
	cin >> constf;

	
	rowFinder(boardf,rowNumBoard);
	rowFinder(constf,rowNumCons);
	
	boardMat = generateMat(boardf,checkOpenBoard);
	consMat = generateMat(constf,checkOpenCons);
	
	

	
	if(checkOpenBoard == false || checkOpenCons == false) {
		return 0; // if oen of the files could not be opened we do nt continue with the other check directly terminate
	}	
		
	else if (!checkSize(boardMat, rowNumBoard)) {
		cout << "File contains more lines than size" << endl;
	}

	else if (!checkSize(consMat, rowNumCons)) {
		cout << "There are more constraints then previously stated" << endl;
	}
	else if (!checkValue(boardMat, rowNumBoard, outOfMatValue)) {
		cout << "Value " << outOfMatValue << " is out of matrix size" << endl;
	}
	else if (!checkValue(consMat, rowNumBoard, outOfMatValue)) {
		cout << "Coordinate value " << outOfMatValue << " is out of matrix size" << endl;
	}
	else if (!checkDuplicateColumn(boardMat) && !checkDuplicateRow(boardMat)){
		cout << "There are values repeating in same column and same row" << endl;
	}
	else if (!checkDuplicateColumn(boardMat)){
		cout << "There is a value repeating in same column" << endl;
	}
	else if (!checkDuplicateRow(boardMat)) {
		cout << "There is a value repeating in same row" << endl;
	}
	else if (!checkConstraints(consMat,boardMat)) {
		cout << "Constraints do not match board" << endl;
	}
	else {
		bool moreValue = false; // will inform if some particular X's can have more tan 1 value
		solvePuzzle(boardMat,consMat,moreValue);
		if(moreValue) {
			cout << "X's can have more than 1 value" << endl;
		}
		else {
			Print(boardMat);
		}
	}

	

	

	


	return 0;
}

//Berk Sezer