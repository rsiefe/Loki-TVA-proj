/*
* Description: This assignment utilizing linked lists to simulate a timeline and various nexus events 
using their respective Timeline and createNexusEvent functions. The same branches are then pruned using
deallocated and redirecting the nodes in the linked list. The various stages of the timeline are shown 
using an illustration from the vprint function and a list from the print function. 
* Input: starting year of a timeline, size of timeline 
* Output: illustation of timeline, chronological list, location and size of nexus events, pruning of branches
*/

#include <iostream>
using namespace std;

class node
{
public:
  int year;
  int branches;
  node *next;

/*
* node: Default constructor to initialize year, branches, and next pointer
* parameters: None. 
* return value: None.
*/
  node()
  {
    year = 0;
    branches = 0;
    next = nullptr;
  }

/*
* node: Parameterized constructor to intialize integers for year and branches, and next pointer of node type
* parameters: int y 
* return value: None.
*/
  node(int y): year(y), branches(0), next(nullptr)
  {

  }
};

class Timeline
{
public:
/*
* Timeline: Constructor with start year and size. default param are 0 for both.
* parameters:int start, int size
* return value: None.
*/
  Timeline(int start = 0, int size = 0)
  {
    head = nullptr;
    head = new node(start);
    node *newNode = head;
    this->size = size;
      // for loop initalizes timeline list link 
    for (int i = 1; i < size; i++)
    {
      newNode->next = new node(start + i);
      newNode = newNode->next;
    }
    newNode->next = nullptr;
  }

/*
* createNexusEvent: 
* parameters: int amount  
* return value: int
*/
int createNexusEvent(int amount = 1)
{
  if (!head)
    return 0; // Can't create nexus event if timeline is empty!
  for (int i = 0; i < amount; i++)
  {
    int year = (rand() % size) + head->year;
  // branchsize is MaxSize minus years left give us how big branch can be before end of time
    int branchSize = rand() % (head->year + size - year) / 4; // divide by 4 to keep them smaller for visual reasons
    node *branch = new node[2];
    node *tmp = head;
    if (tmp)
    {
      while (tmp->year != year)
      {
        tmp = tmp->next;
      }
    }
  //increments branches to 1 now that nexus event has been created; 
    tmp->branches = tmp->branches + 1;
  //deep copies next year's node into first index of branch and the current year into the second
  //index to continue the nexus event 
    if (tmp->next)
    {
      branch[0].year = tmp->next->year;
      branch[0].branches = tmp->next->branches;
      branch[0].next = tmp->next->next;
      branch[1].year = tmp->year;
    }
    //redirects the current year to point to the nexus event branch
    node *newYear = &branch[1];
    for (int i = 1; i <= branchSize; i++)
    {
      newYear->next = new node(year + i);
      newYear = newYear->next;
    }

    newYear->next = nullptr;
   // delete the year after nexus since we replaced with non pointer type in next[0]
    delete tmp->next;
  // branch the timeline here.
    tmp->next = branch; // redirects original current node to point to the nexus event branch
    cout << "Created Nexus at " << year << ", Size: " << branchSize + 1 << endl;
  }
  return amount;
}

/*
* print: 
* parameters: None. 
* return value: None.
*/
void print()
{
  cout << "The Sacred Timeline" << endl;
  node *tmp = head;
  cout << "s->";
  while (tmp)
  { //this while loop prints while iterating through the timeline, including any brahces
    cout << tmp->year << "->";
    if (tmp->branches > 0) //checks for branch at year
    {
      node *tmp2 = &(tmp->next[1]);
      cout << "(";
      while (tmp2) //prints years in branched timeline 
      {
        cout << "*" << tmp2->year << ".1";

        tmp2 = tmp2->next;
        if (tmp2)
        {
          cout << "->";
        }
      }
      cout << ")~>";
    }
    tmp = tmp->next;
  }
  cout << "x\n";
  cout << endl;
}

// Visual Print Mode
/*
* vprint: 
* parameters: None. 
* return value: None.
*/
void vprint()
{
// creating a 2D array of chars
  const int row = 11;
  const int col = 70;
  int middle = 5;
  char arr[row][col];
  node *tmp = head;

  //filling the array with borders, dashes, and whitespace
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (i == 0 || i == 10)
      {
        arr[i][j] = '#';
      }
      else if (i == middle && j < size)
      {
        arr[i][j] = '-'; // in the sacred timeline each - represents 1 year.
      }
      else
      {
        arr[i][j] = ' ';
      }
    }
  }

  int yr = 0; //current year/column in the timeline
  bool up = true; //boolean to alternate where to write the branches 
  while (tmp != nullptr && yr < 70) //loop iterates through timeline up to column 70 of array
  {
    if (tmp->branches > 0 && up) 
    {
      arr[middle - 1][yr] = '/';
      arr[middle - 2][yr + 1] = '/';
      int nexusYears = 0;
      node *tmpHead = &tmp->next[1];
      while (tmpHead && (yr+2+nexusYears)<col) //loop writes tildas
      {
        arr[middle - 3][yr + 2 + nexusYears] = '~';
        tmpHead = tmpHead->next;
        nexusYears++;
      }
      up = false;
    }
    else if (tmp->branches > 0 && !up)
    {
      arr[middle + 1][yr] = '\\';
      arr[middle + 2][yr + 1] = '\\';
      int nexusYears = 0;
      node *tmpHead = &tmp->next[1];
      while (tmpHead && (yr+2+nexusYears)<col) //loop writes tildas for node in the branch
      {
        arr[middle + 3][yr + 2 + nexusYears] = '~';
        tmpHead = tmpHead->next;
        nexusYears++;
      }
      up = true;
    }
    yr++;
    tmp = tmp->next;
  }
//loop prints map of timeline
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      cout << arr[i][j];
    }
    cout << endl;
  }
}

/*
* prune: prunes a branch and returns location it pruned. For all time. always.
* parameters: None. 
* return value: int
*/
int prune()
{
  int yearWithBranch = -1;
  int branchSize = 1;

  node *tmp = head;
  while (tmp)
  {
    if (tmp->branches > 0)
    { //if there is a nexus event the branch will be pruned
      tmp->branches = 0;
      yearWithBranch = tmp->year; //year with branch is updated
      node *tmp2 = tmp->next[1].next;
        node *tmp3 = tmp2;
      while (tmp2) 
      {
        branchSize++;
        tmp2 = tmp2->next;
        delete tmp3;
        tmp3 = tmp2;
      }
      //redirects original node away the branch back to the original timeline 
      tmp2= new node(tmp->next[0].year); 
      tmp2->next=tmp->next[0].next; 
      tmp2->branches=tmp->next[0].branches; 
      delete [] tmp->next;
      tmp->next=tmp2;
      cout << "Pruned Brach at: " << yearWithBranch << " of size " << branchSize << endl;
      return yearWithBranch;
    }
    tmp = tmp->next;
  }
  return yearWithBranch;
}

/*
* ~Timelime: Destructor for timeline 
* parameters: None. 
* return value: None.
*/
~Timeline()
{
  while (head) //while loop deallocates each node in the linked list
  {
    node *ptr = head;
    head = head->next;
    if (ptr->branches > 0) // if a branch was not pruned 
    {
      cout << "Branch was not pruned!" << endl;
    }
    delete ptr;
    ptr = nullptr;
  }
}

  // Don't modify any code beneath this line or the TVA will prune you!!!
private:
  int size;
  node *head;

};

// FOR ALL TIME. ALWAYS
int main(int argc, char *argv[])
{ // ./a.out 40 4 1970 50 (40 is seed, 4 is number of nexus events to
  // create 1970 is starting year of the sacred timeline, and 50 is the length of the sacred timeline).
  int seed = 40;               // for testing with no cmd args
  int nexusEventsToCreate = 4; // for testing with no cmd args
  int startYear = 1970;        // for testing with no cmd args
  int timelineLength = 50;     // for testing with no cmd args
  if (argc == 5)
  { // for codegrading
    seed = atoi(argv[1]);
    nexusEventsToCreate = atoi(argv[2]);
    startYear = atoi(argv[3]);
    timelineLength = atoi(argv[4]);
  }
  srand(seed);
  Timeline sacredTimeline(startYear, timelineLength);
  sacredTimeline.print();
  sacredTimeline.createNexusEvent(nexusEventsToCreate);
  // sacredTimeline.print();
  sacredTimeline.wtf();
  sacredTimeline.vprint();
  cout << "Nexus Event Detected! Sending Minute Men to Prune the Branches!\n"
       << endl;
  // on behalf of the time variance authority, I hereby arrest you for crimes against the sacred timeline
  while (sacredTimeline.prune() != -1)
  {
    sacredTimeline.print();
  }
  cout << "\nSacred Timeline Restored :)" << endl;
  sacredTimeline.print();
  sacredTimeline.vprint();

  cout << "___________   ____\n| /|    \\  \\ /  / \\\n|/ |    |\\  Y \\/   \\\n   "
       << "|    | \\   /\\    \\\n   |____|  \\_/_/\\____\\\n FOR ALL TIME.  ALWAYS.\n"
       << endl;
  return 0;
}
