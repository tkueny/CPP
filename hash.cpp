#include <iostream>
#include <unordered_set>

using namespace std;

/***************************
 * Define a Student class
 */
class Student
{
  int mId;
  string mName;

public:
  Student(int id, string name) :
      mId(id), mName(name)
  {
  }

  void displayInfo() const
  {
    cout << "Student ("<<mId << ", " << mName <<")";
  }

  bool operator ==(const Student & obj) const
  {
    if (mId == obj.mId)
      return true;
    else
      return false;
  }

  int getId() const
  {
    return mId;
  }

  string getName() const
  {
    return mName;
  }
};

/*******************************************
 * Below are several hash function and comparator function for Student class
 * (to be used for unsorted_set)
 *******************************************/

//Below we define "function objects" which package function as a class/struct type.
struct StudentHasherByName
{
  // overload operator () to implement a hash function on Student object
  //  can be called as follows: StudentHasher() (Student (11,"Mary"))
  //                                         ^^ refer to operator that is defined here of StudentHasher class 
  // demonstrated in main() 
  size_t operator()(const Student & obj) const
  {
    return hash<string>()(obj.getName());
           //Note: hash is a template class defined in C++ STL in which the operator() is overloaded 
           //Here we call operator() of hash<string> class, passing obj.GetName() as parameter.
	   // basically, this will use system provided hash function to hash a string into an unsigned int
  }
};

// Custom comparator that compares the Student objects by name
struct StudentComparatorByName
{
  bool operator()(const Student & obj1, const Student & obj2) const
  {
    if (obj1.getName() == obj2.getName())
      return true;
    return false;
  }
};


//Below we define "function objects" which package functions as a class/struct type.
struct StudentHasherByID
{
  static int a;  //here are configurable parameters 
  // overload operator (), so that StudentHash can be called as a function
  size_t operator()(const Student & obj) const
  {
    return hash<int>()(a*obj.getId());
           //Note: hash is a template class defined in C++ STL in which the operator() is overloaded 
           //Here we call operator() of hash<int> class, passing a*obj.getID() as parameter.
	   // basically, this will use system provided hash function to hash a int into an unsigned int
  }
};


/*********************************
 * Beginning main/driver
 *************************/
int StudentHasherByID::a; //static member variable needs to be defined (to get space) 

// declare two different sets of student, using different hash function and comparasion function
typedef unordered_set<Student, StudentHasherByName, StudentComparatorByName> SetOfStudentName;
     //use StudentHasherByName to hash a Student object 
     //Use StudentComparatorByName to test if two Student objects are duplicate 

typedef unordered_set<Student, StudentHasherByID> SetOfStudentID;
	//Use == operator to test if two Student objects are equivalent (duplicates) 

// a function to be used to examine the hash table details of the set 
void DisplayHashTable(SetOfStudentName &);


int main()
{
int id;
string name;
char op;

  cout <<"This is a demo program for unordered_set class\n";

  /************************************ 
   * Demonstrate usage of function object 
   ***********************************/ 
  cout <<"Enter value of a to be used by hasherByID (non-zero integer):";
  cin >> StudentHasherByID::a; 

  cout <<"Demo hashers\n";
  Student st (14, "Alice");
  st.displayInfo();
  cout <<" is hashed to "<<StudentHasherByName()(st)<<" by SduentHasherByName\n";
  //                                 ^() access the overloaded () operator 
  cout <<" is hashed to "<<StudentHasherByID()(st) <<" by StudentHasherByID\n";

  cout <<"Testing comparator\n";

  cout <<"Demo StudentComparatorByName:\n";
  Student st2 (12, "Alice");

  st.displayInfo();
  st2.displayInfo();
  if (StudentComparatorByName() (st, st2))
	cout <<"are same\n";
  else
	  cout <<"are different\n";

  /****************************************
  // Declaring unordered_set of Student
  //  specifying hash function and comparator to use by the parameters (both are function objects)
  //    using student's name to hash and compare two students object 
  ************************************/
  SetOfStudentName setOfStud; 
  //unordered_set<Student, StudentHasherByName, StudentComparatorByName> setOfStud;
  //Note: if we don't provide the third parameter, the == operator on Student class
  //      will be used 


  //We can query unordered_set to get the hash function being used 
  SetOfStudentName::hasher fn = setOfStud.hash_function(); 

  /************************************************
  //Display the hash policy
  ***********************************************/
  cout <<"When load factor reaches this value, the bucket size is increased and elements are rehashed.\n";
  cout <<"current Max_load_factor is" << setOfStud.max_load_factor()<<endl;
  cout <<"Do you want to change it?";
  //char op;
  float loadFactor;
  cin >> op;
  if (op=='y')
  {
	  	cout <<"Enter new load factor (a positive float value: ";
		cin >> loadFactor;
		setOfStud.max_load_factor(loadFactor);
  		cout <<"Max_load_factor is now " << setOfStud.max_load_factor()<<endl;
  }
  cout <<endl;

  /**********************************************
   * A load to insert elements into the set
   *  Student object that is a duplicate will fail to 
   *  be inserted... 
   ********************************************/
  while (true)
  {
	cout <<"Enter student info (ID, name)\n";
	cin >> id>> name;
	Student st (id, name);

	cout <<" This student is hashed to "<<fn(st)<<endl;

	auto res = setOfStud.insert (st);

  	if (res.second == false)
		//This happens due to the new object is equivalent to one aleady
		// in the set (according to comparator, here the name are same)
    		cout << "Failed to insert"<<endl;
	else 
	{
  		DisplayHashTable(setOfStud);
	}


	cout <<"Continue?(y/n)";
	cin >> op;
	if (op=='n')
		break;

  }

  cout <<"Search for student with key (name)\n";
  while (true){
  	cout <<"Enter a name:";
  	cin >> name;
  	Student st3(0,name); //just fill in the key field (i.e., the field
  			//used to hash the object 
  	SetOfStudentName::const_iterator got=setOfStud.find(st3);
  	if (got==setOfStud.end())
	  	cout <<"student "<<name<<" not found";
  	else
  	{
	  	cout <<"student "<<name<<" is in the set\n";
  	  	(*got).displayInfo();
  	}

	cout <<"continue(y/n)?";
	cin >> op;
	if (op=='n')
		break;
  }

  return 0;
}


void DisplayHashTable(SetOfStudentName & set)
{
  //Display size (number of elements), number of buckets, load factor 
  cout <<"\n---------------------------------------\n";
  unsigned size = set.size();
  cout << "There are " << size << " elements:\n";
  unsigned nbuckets = set.bucket_count();
  cout << "There are " << nbuckets << " buckets:\n";
  cout <<"Current load factor (size/bucket_count):"<<set.load_factor()<<endl;

  //Display the size of each bucket (i.e., the length of linked list in each entry of hash table) 
  // and elements stored in each bucket
  for (unsigned i=0; i<nbuckets; ++i) {
    std::cout << "	bucket #" << i << " has " << set.bucket_size(i) << " elements:";
    for (Student st : set)
    {
	    if (set.bucket(st)==i)
                        st.displayInfo();
    }
    cout <<endl;
  }

  cout <<"\n---------------------------------------\n";
}
