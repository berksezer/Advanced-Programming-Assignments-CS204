struct metroStop {
  string metroStopName;
  metroStop *left;
  metroStop *right;  
};
  

struct metroLine {
  string metroNumber;
  metroLine *next;  
  metroStop *metroStops;

  vector<double>costs;
};

bool consistencyCheck() {
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


void processMainMenu() {
  char input;
  do{
    if(!consistencyCheck()) {
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
      printmetroLinesDetailed();
      break;
    case '2':
      addmetroStop();
      break;
    case '3':
      deletemetroStop();
      break;
    case '4':
      pathfinder();
      break;
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}

