//@ZainSra(zainsra.com) , 2016
//Top-Down Approach code is written according to the Pseudo code given in Algorithm textbook.
#include<iostream>
#include <fstream>
using namespace std;

//Global Counting Variables
//for Top Down
	int Number_of_SplitOP_TD;
	int Number_of_DiskReads_TD;
	int Number_of_DiskWrites_TD;

//for Bottom Up
	int Number_of_SplitOP_BU;
	int Number_of_DiskReads_BU;
	int Number_of_DiskWrites_BU;

// A BTree node
class BTreeNode
{
    int *elements;  // Array of key elements
    int t;      // Minimum degree 
    BTreeNode **Children; // Array of Child Pointers
    int n;     // Number of key elements
    bool isLeaf; // Is true when node is isLeaf. Otherwise false
	BTreeNode *parent; //Used to keep track of the parent , used in Bottom up Approach
public:
    BTreeNode(int _t, bool _isLeaf);   // Constructor
 
    //Function to insert a new key  k using Top Down Approach
    void InsertNonFull_TopDown(int k);

	//Function to insert a new key k using Bottom up Approach, rot is the pointer to Root and is used to update the root of B Tree if needed
	bool InsertNonFull_BottomUP(int k,BTreeNode *rot);
 
    // Function to split the child y of this node in Top Down Approach. i is index of y in
    // child array Children[].  
    void TopDownsplitChild(int i, BTreeNode *y);

	// Function to split the child of parent node in Bottom Up Approach(Recursively). k is used to
	//find the index of child in parent's Children[], root is the pointer to Main Root of B tree and updated if needed
	BTreeNode* BottomUPsplitChild(int k,BTreeNode *parent,BTreeNode *child,BTreeNode *root);
 
    // A function to traverse all nodes in a subtree rooted with this node
    void traverse();
 
    // A function to search a key in subtree rooted with this node.
    BTreeNode *search(int k);   // returns NULL if k is not present.

friend class BTree; //Made it Friend class so that we can use private members of BTreeNode class, using instead of making getters and setters
};
 
// A BTree
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t;  // Minimum degree

	



public:
    // Constructor (Creating an Empty Tree )
    BTree(int _t)
    {  root = NULL;  t = _t;

	//Incrementing Disk Writes
	Number_of_DiskWrites_BU=1;
	Number_of_DiskWrites_TD=1;

	Number_of_SplitOP_TD=0;
	Number_of_DiskReads_BU=0;
	Number_of_DiskReads_TD=0;
	Number_of_SplitOP_BU=0;

	}
 
    // function to traverse the tree
    void traverse()
    {  if (root != NULL) root->traverse(); }
 
    // function to search a key in this tree
    BTreeNode* search(int k)
	{  if(root!=NULL)
		{
		return root->search(k);
		}
		else return NULL;
	} 
	
    // The main functions that insert a new key in this B-Tree
    void InsertTopDown(int k);

	void InsertBottomUP(int k);

	//Getters for count variables ( don't need setters for this assingment as we have to calculate while running the algorithm)
	
	int get_n_of_splits_TD(){return Number_of_SplitOP_TD;}
	int get_n_of_splits_BU(){return Number_of_SplitOP_BU;}

	
	int get_n_of_dread_BU(){return Number_of_DiskReads_BU;}
	int get_n_of_dread_TD(){return Number_of_DiskReads_TD;}

	
	int get_n_of_dwrite_TD(){return Number_of_DiskWrites_TD;}
	int get_n_of_dwrite_BU(){return Number_of_DiskWrites_BU;}

	//To clear all the count variable values ,and set them to 0
	void clearCounts()
	{
		int Number_of_SplitOP_TD=0;
	int Number_of_DiskReads_TD=0;
	int Number_of_DiskWrites_TD=0;

	int Number_of_SplitOP_BU=0;
	int Number_of_DiskReads_BU=0;
	int Number_of_DiskWrites_BU=0;
	}
};
 
//Implementation
BTreeNode::BTreeNode(int t1, bool isLeaf1)
{
    t = t1;
    isLeaf = isLeaf1;
 
	Children = new BTreeNode *[2*t];
    elements = new int[2*t-1];
  
    n = 0;
}

void BTreeNode::traverse()
{
    // There are n elements and n+1 children, travers through n elements
    // and first n children
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not isLeaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (isLeaf == false)
            Children[i]->traverse();

		cout<<endl<<elements[i]<<" Parent: ";
		if(parent!=NULL)	
		{
			for(int i=0;i<parent->n;i++)
		cout<<parent->elements[i]<<"  ";
		}
		cout<<endl<<endl;
    }
 
    // Print the subtree rooted with last child
    if (isLeaf == false)
        Children[i]->traverse();
}
 
BTreeNode *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > elements[i])
        i++;
 
    // If the found key is equal to k, return this node
    if (elements[i] == k)
        return this;
 
    // If key is not found here and this is a isLeaf node
    if (isLeaf == true)
        return NULL;
 
	Number_of_DiskReads_BU++;
	Number_of_DiskReads_TD++;

    return Children[i]->search(k);
}
 
void BTree::InsertTopDown(int k)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->elements[0] = k;  // inserti key
        root->n = 1;  // Update number of elements in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t-1)
        {
            // Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);

			s->parent=NULL;
 
            // Make old root as child of new root
            s->Children[0] = root;
 
			root->parent=s;

			Number_of_SplitOP_TD++;

            // Split the old root and move 1 key to the new root
			s->TopDownsplitChild(0, root);
 
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->elements[0] < k)
                i++;
            s->Children[i]->InsertNonFull_TopDown(k);
 
            // Change root
            root = s;
        }
        else  // If root is not full, call InsertNonFull for root
            root->InsertNonFull_TopDown(k);
    }
} 

void BTree::InsertBottomUP(int k)
	{

		bool flag=true;

		if (root == NULL)
		{
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->elements[0] = k;  // inserti key
        root->n = 1;  // Update number of elements in root
		root->parent=NULL;
		 }
		else if((root->n==2*t-1) && root->isLeaf==true)
		{
			//Root is full
			// Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);

			s->parent=NULL;
 
            // Make old root as child of new root
            s->Children[0] = root;
 
			root->parent=s;

			Number_of_SplitOP_BU++;

            // Split the old root and move 1 key to the new root
            s->TopDownsplitChild(0,root);
 
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->elements[0] < k)
                i++;
            s->Children[i]->InsertNonFull_TopDown(k);
 
            // Change root
            root = s;
		}
		else 
			{
			//Tree is not empty

			flag=root->InsertNonFull_BottomUP(k,root);
			//Here Tree would have been splitted from Bottom to Top , so we need to call insert again so that the key would be insertied 

			if(flag!=true)
			root->InsertNonFull_BottomUP(k,root);
			
			//Updating the root
			while(root->parent!=NULL)
			{
				root=root->parent;
			}
		}
	
	}
 
void BTreeNode::InsertNonFull_TopDown(int k)
{
    // Initialize index as index of rightmost element
    int i = n-1;
 
    // If this is a isLeaf node
    if (isLeaf == true)
    {
        
        //Finding the location of new key to be insertied
        // and Moving all greater elements to one place ahead
        while (i >= 0 && elements[i] > k)
        {
            elements[i+1] = elements[i];
            i--;
        }
 
        // inserti the new key at found location
        elements[i+1] = k;
        n = n+1;

		Number_of_DiskWrites_TD++;
    }
    else // If this node is not isLeaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && elements[i] >= k)
            i--;
 
		Number_of_DiskReads_TD++;

        // See if the found child is full
        if (Children[i+1]->n == 2*t-1)
        {

			Number_of_SplitOP_TD++;

            // If the child is full, then split it
            TopDownsplitChild(i+1, Children[i+1]);
 
            // Finding child of splitted node to have the new key
            if (elements[i+1] < k)
                i++;
        }
        Children[i+1]->InsertNonFull_TopDown(k);
    }
} 

bool BTreeNode::InsertNonFull_BottomUP(int k,BTreeNode *rot)
{
	// Initialize index as index of rightmost element
    int i = n-1;
 
	if(isLeaf==true)
	{
		//It's a Leaf Node
		if(n==2*t-1)
		{
			//Leaf is full , so we need to traverse back while splitting full nodes

			//1) Split from Bottom to Top
			//2) Call inserti again from main inserti function

			Number_of_SplitOP_BU++;
			
			rot=BottomUPsplitChild(k,this->parent,this,rot);
			return false;	//No InsertBottomUPon was made and we need to traverse the tree again
		}
		else 
		{
			//Only need to insert and update Number of Disk Writes

			// The following loop does two things
        // a) Finds the location of new key to be insertied
        // b) Moves all greater elements to one place ahead
        while (i >= 0 && elements[i] > k)
        {
            elements[i+1] = elements[i];
            i--;
        }
 
        // inserti the new key at found location
        elements[i+1] = k;
        n = n+1;

		Number_of_DiskWrites_BU++;
		}
		return true;
	}
	else 
	{
		//It's Not leaf Node, so we need to find appropriate child 

		// Find the child which is going to have the new key

		Number_of_DiskReads_BU++;

        while (i >= 0 && elements[i] >= k)
            i--;
		Children[i+1]->InsertNonFull_BottomUP(k,rot);
	}
}

void BTreeNode::TopDownsplitChild(int i, BTreeNode *y)
{
    // Create a new node which is going to store (t-1) elements
    // of y
    BTreeNode *z = new BTreeNode(y->t, y->isLeaf);
    z->n = t - 1;
 
    // Copy the last (t-1) elements of y to z
    for (int j = 0; j < t-1; j++)
        z->elements[j] = y->elements[j+t];
 
    // Copy the last t children of y to z
    if (y->isLeaf == false)
    {
        for (int j = 0; j < t; j++)
            {
				z->Children[j] = y->Children[j+t];
				z->Children[j]->parent=z;
		}
    }
 
    // Reduce the number of elements in y
    y->n = t - 1;
 
    // Since "this" node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        Children[j+1] = Children[j];
 
    // Link the new child to this node
    Children[i+1] = z;

	z->parent=this;
 
    // A key of y will move to this node. Find location of
    // new key and move all greater elements one space ahead
    for (int j = n-1; j >= i; j--)
        elements[j+1] = elements[j];
 
    // Copy the middle key of y to this node
    elements[i] = y->elements[t-1];
 
    // Increment count of elements in this node
    n = n + 1;

	//Number of DiskWrites =3 (for y , z and this)

	Number_of_DiskWrites_BU+=3;	//As i am using this Split CHild function in the Bottom Up Approach as well
	Number_of_DiskWrites_TD+=3;	
	
}
 
BTreeNode* BTreeNode::BottomUPsplitChild(int k,BTreeNode *parent,BTreeNode *child,BTreeNode *root)
{

	if(parent!=NULL && (child->n==2*t-1))
	{
		//Child is Full and It's not Root 
		
		//Calculating the Position of Child realteive to Parent
		int i=parent->n-1;
			 while (i >= 0 && parent->elements[i] > k)
            i--;

			 Number_of_SplitOP_BU++ ; //As it is recursively Splitting nodes up towards root

			 //Split the Child
			 parent->TopDownsplitChild(i+1,child);

			 if(parent->n==2*t-1)
			 {
				 //Now if Parent becomes full we need to split that too

				 //Checking whether it's Root or not
				 if(parent->parent!=NULL)
				 {
					 BTreeNode *temp_parent=parent->parent;
					 BTreeNode *temp_child=parent;

					  Number_of_SplitOP_BU++ ;

					 BottomUPsplitChild(k,temp_parent,temp_child,root);
				 }
				 else 
				 {
					
					 //Root is full
			// Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);

			s->parent=NULL;
 
            // Make old root as child of new root
            s->Children[0] = root;
 
			root->parent=s;

			 Number_of_SplitOP_BU++ ;

            // Split the old root and move 1 key to the new root
            s->TopDownsplitChild(0, root);

			// Change root
           root = s;
				 }

			 }
			 return root;

	}
	else if(parent==NULL && (child->n==2*t-1))
	{
		//Root is full
			// Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);

			s->parent=NULL;
 
            // Make old root as child of new root
            s->Children[0] = root;
 
			root->parent=s;

			 Number_of_SplitOP_BU++ ;

            // Split the old root and move 1 key to the new root
            s->TopDownsplitChild(0, root);

			// Change root
            root = s;	
		}
	return root;
} 


//////////////////////////////////////////Main//////////////////////////////////////////////////////
int main()
{
	///////////////////////////////////Testing B Tree of Minimum Degree=2//////////////////////////////////////

	int Test[100000],index=0;

	ifstream  inputfile;
	inputfile.open("lakh.txt");
		if (inputfile.is_open())
	{
		while(!inputfile.eof() && index<100000){
		inputfile>>Test[index];
		index++;
		}

	}
		else 
		{
			cout<<endl<<"The Input file lakh.txt is not found , please enter that file in the same directory and rerun ! "<<endl;
			system("pause");
			return 1;
		}

		BTree t(2); // A B-Tree with minium degree 2
    
		cout<<endl<<"////////////////USING B-Tree of Minimum Degree = 2///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom Up Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;




		t.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 2///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;

		t.clearCounts();

		BTree t2(3);	//Minimum Degree 3

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 3///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t2.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t2.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 3///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t2.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;


		t.clearCounts();

		BTree t3(4);	//Minimum Degree 4

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 4///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t3.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t3.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 4///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t3.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;


		t.clearCounts();

		BTree t4(7);	//Minimum Degree 7

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 7///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t4.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t4.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 7///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t4.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;




		t.clearCounts();

		BTree t5(12);	//Minimum Degree 12

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 12///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t5.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t4.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 12///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t5.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;



		t.clearCounts();

		BTree t6(26);	//Minimum Degree 26

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 26///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t6.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t6.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 26///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t6.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;

		t6.clearCounts();

		BTree t7(100);	//Minimum Degree 100

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 100///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t7.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t7.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 100///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t7.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;


		t6.clearCounts();

		BTree t8(1000);	//Minimum Degree 1000

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 1000///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t8.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t7.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 1000///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t8.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;


		
		t6.clearCounts();

		BTree t9(5000);	//Minimum Degree 5000

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 5000///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Bottom up Approach//////////////////////////"<<endl;


		index=0;
		while(index<100000)
		{
			t9.InsertBottomUP(Test[index]);
			index++;
		}
 
		

		cout << "No of Split Operations : "<<Number_of_SplitOP_BU<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_BU<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_BU<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_BU+Number_of_DiskWrites_BU<<endl<<endl;


		t7.clearCounts();

		cout<<endl<<endl<<"////////////////USING B-Tree of Minimum Degree = 5000///////////////////////////////"<<endl;

		cout<<endl<<"////////////////////Top Down Approach//////////////////////////"<<endl;

		index=0;
		while(index<100000)
		{
			t9.InsertTopDown(Test[index]);
			index++;
		}
		cout << "No of Split Operations : "<<Number_of_SplitOP_TD<<endl;
		cout<<"No of Disk Reads : "<<Number_of_DiskReads_TD<<endl;
		cout<<"No of Disk Writes : "<<Number_of_DiskWrites_TD<<endl;
		cout<<"No of Total Disk Accesses (Reads+Writes) "<<Number_of_DiskReads_TD+Number_of_DiskWrites_TD<<endl<<endl;



 
	system("PAUSE");
    return 0;
}