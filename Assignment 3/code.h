// Zohaib Adnan
// 19i-0741
// Data Structures 
// Assignment 3

#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//**************** Classes ****************/

class tree
{
	// Nested structures and classes
	struct treeNode // Each treeNode represents a directory
	{
		// Data members
		string name;
		treeNode* children;
		treeNode* sibling;
		treeNode* parent;

		treeNode() // Constructor 
		{
			name = "NULL";
			children = sibling = parent = NULL;
		}
	};

	class queue // Built-in queue to traverse the file directory tree
	{
		struct queueNode
		{
			// Data members
			treeNode* data;
			queueNode* next;

			queueNode() // Constructor
			{
				data = NULL;
				next = NULL;
			}
		};

		// Data members
		queueNode* front; // Always points to the first element in the queue
		int size; // Keeps track of the number of elements inside the queue

	public:
		queue() // Constructor
		{
			front = NULL;
			size = 0;
		}
		~queue() // Destructor
		{}

		// Methods
		void enqueue(treeNode* data) // Add an element to the queue
		{
			queueNode* newNode = new queueNode; // Create a new queueNode and save the received data inside it
			newNode->data = data;
			if (front == NULL) // If the queue is empty, add the new queueNode at the very front
				front = newNode;
			else // Otherwise, add it after the last element in the queue
			{
				queueNode* currNode = front;
				while (currNode->next != NULL)
					currNode = currNode->next;
				currNode->next = newNode;
			}
			size++;
			return;
		}
		void dequeue() // Remove an element from the queue
		{
			if (front == NULL) // The queue is already empty
				return;
			else
			{
				queueNode* temp = front;
				front = front->next; // The front pointer should now point to the second element in the queue
				delete temp; // Delete the first element
			}
			size--;
			return;
		}
		bool isEmpty() // Returns 'true' if the queue is empty
		{
			if (size)
				return false;
			else
				return true;
		}
		treeNode* peek() // Returns the data of the first element in the queue
		{
			return front->data;
		}
		int getSize() // Returns the size of the queue
		{
			return size;
		}
		void reverse()
		{
			if (this->isEmpty())
				return;
			treeNode* temp = this->peek();
			this->dequeue();
			this->reverse();
			this->enqueue(temp);
			return;
		}
	};

	// Data members
	treeNode* root; 
	bool created;

public:
	tree() // Constructor
	{
		created = false;
		root = new treeNode; // The root directory must be constructed as soon as a new file directory tree is made
		root->name = "/";
	}
	~tree() // Destructor
	{
	}

	// Methods
	bool insert(string path) // Inserts a new directory or file in the file directory tree
	{
		if (path[0] != '/') // The first character in the path must be the root
		{
			cout << "Error! The provided path is invalid." << endl;
			return false;
		}
		treeNode* currNode = root; 
		string subStr = "";
		int index = 1;
		while (path[index] != '\0') // Process the path until the null character is read
		{
			if (path[index] == '/') // Move to a child directory
			{
				bool flag = false;
				if (currNode->children != NULL) // Check whether the current directory has children
				{
					treeNode* child = currNode->children;
					while (child != NULL) // Check whether the child directory exists
					{
						if (child->name == subStr) // If the child directory is found, exit the loop
						{
							flag = true;
							currNode = child;
							break;
						}
						child = child->sibling;
					}
				}
				if (flag) // The child directory exists
					subStr = ""; // Clear the subString
				else // The child directory does not exist and must be created
				{
					// Create a new child directory
					treeNode* newChild = new treeNode;
					newChild->name = subStr;
					newChild->parent = currNode; // The new child directory's parent is the current directory
					if (currNode->children == NULL) // If the parent node does not have any children
					{
						currNode->children = newChild; // Add the new directory directly
						currNode = currNode->children; // Move to the child directory
					}
					else
					{
						treeNode* child = currNode->children;
						while (child->sibling != NULL) // Find the last child directory
							child = child->sibling;
						child->sibling = newChild; // Add the new directory to the right of the last child directory
						currNode = child->sibling;
					}
					subStr = "";
				}
			}
			else
				subStr += path[index];
			index++;
		}
		if (path[index - 1] != '/') // The last file needs to be added
		{
			treeNode* newChild = new treeNode;
			newChild->name = subStr;
			newChild->parent = currNode;
			if (currNode->children == NULL) // If the current directory does not have any child directories, add the new file directly
				currNode->children = newChild;
			else
			{
				treeNode* child = currNode->children;
				while (child->sibling != NULL) // Otherwise, find the last child directory
					child = child->sibling;
				child->sibling = newChild; // Add the new file at the very end
			}
		}
		return true;
	}
	string levelOrderTraversal()
	{
		if (root == NULL)
		{
			cout << "The file directory tree is currently empty." << endl;
			return "NULL";
		}
		string output = ""; 
		queue q;
		q.enqueue(root);
		while (!q.isEmpty())
		{
			if (!output.empty())
				output += '\n';
			int inQueue = q.getSize();
			while(inQueue > 0)
			{
				treeNode* temp1 = q.peek();
				q.dequeue();
				output += temp1->name;
				if (temp1->children != NULL)
				{
					treeNode* temp2 = temp1->children;
					while (temp2 != NULL)
					{
						q.enqueue(temp2);
						temp2 = temp2->sibling;
					}
				}
				if(inQueue != 1)
					output += " ";
				inQueue--;
			}
		}
		return output;
	}
	string search(string name)
	{
		if (root == NULL)
		{
			cout << "The file directory tree is currently empty." << endl;
			return "NULL";
		}
		string path = "NULL";
		queue q;
		q.enqueue(root);
		while (!q.isEmpty())
		{
			int inQueue = q.getSize();
			while (inQueue > 0)
			{
				treeNode* temp1 = q.peek();
				q.dequeue();

				if (temp1->name == name)
				{
					treeNode* temp2 = temp1;
					queue found;
					while (temp2->name != "/")
					{
						found.enqueue(temp2);
						temp2 = temp2->parent;
					}
					found.reverse();
					string temp = "";
					while (!found.isEmpty())
					{
						temp += '/';
						temp2 = found.peek();
						temp += temp2->name;
						found.dequeue();
					}
					if (path == "NULL")
						path = "";
					if (!path.empty())
						path += '\n';
					path += temp;
				}

				if (temp1->children != NULL)
				{
					treeNode* temp2 = temp1->children;
					while (temp2 != NULL)
					{
						q.enqueue(temp2);
						temp2 = temp2->sibling;
					}
				}
				inQueue--;
			}
		}
		return path;
	}
	bool del(string path)
	{
		if (path == "/") // Make sure that the root directory is not deleted
		{
			cout << "Error! The root directory cannot be deleted." << endl;
			return false;
		}
		int index = 0;
		string subStr = "";
		// Extract the name of the directory or file that needs to be deleted
		while (path[index] != '\0')
		{
			if (path[index] == '/')
			{
				subStr = "";
				index++;
			}
			subStr += path[index];
			index++;
		}
		string temp = search(subStr);
		if (temp != path)
		{
			cout << "Error! The provided path is invalid" << endl;
			return false;
		}
		index = 1;
		string toDel = subStr;
		subStr = "";
		treeNode* currNode = root->children;
		while (path[index] != '\0')
		{
			if (currNode->name == toDel)
				break;
			if (path[index] == '/')
			{
				while (currNode->name != subStr)
					currNode = currNode->sibling;
				currNode = currNode->children;
				subStr = "";
				index++;
			}
			subStr += path[index];
			index++;
		}
		while (currNode->name != subStr)
			currNode = currNode->sibling;
		treeNode* parent = currNode->parent;
		if (parent->children->name == currNode->name)
			parent->children = NULL;
		else
		{
			treeNode* temp = parent->children;
			while (temp->sibling->name != currNode->name)
				temp = temp->sibling;
			temp->sibling = NULL;
		}
		delete currNode;
		return true;
	}
	bool merge(string source, string target)
	{
		if (source[0] != '/' || target[0] != '/')
		{
			cout << "Error! The provided path(s) are invalid." << endl;
			return false;
		}
		string sourceName = "", targetName = "";
		int index = 0;
		// Extract the source directory's name from the source path
		while (source[index] != '\0')
		{
			sourceName += source[index];
			index++;
			if (source[index] == '/')
			{
				sourceName = "";
				index++;
			}
		}
		index = 0;
		// Extract the target directory's name from the directory path
		while (target[index] != '\0')
		{
			targetName += target[index];
			index++;
			if (target[index] == '/')
			{
				targetName = "";
				index++;
			}
		}

		/******************************************************************/
		index = 1;
		string subStr = "";
		treeNode* currNode = root->children;
		while (source[index] != '\0')
		{
			if (currNode->name == sourceName)
				break;
			if (source[index] == '/')
			{
				while (currNode->name != subStr)
					currNode = currNode->sibling;
				currNode = currNode->children;
				subStr = "";
				index++;
			}
			subStr += source[index];
			index++;
		}
		while (currNode->name != sourceName)
			currNode = currNode->sibling;
		treeNode* sourceNode = currNode;

		index = 1;
		subStr = "";
		currNode = root->children;
		while (target[index] != '\0')
		{
			if (currNode->name == targetName)
				break;
			if (target[index] == '/')
			{
				while (currNode->name != subStr)
					currNode = currNode->sibling;
				currNode = currNode->children;
				subStr = "";
				index++;
			}
			subStr += target[index];
			index++;
		}
		while (currNode->name != targetName)
			currNode = currNode->sibling;
		if (currNode->children == NULL)
			currNode->children = sourceNode->children;
		else
		{
			treeNode* temp = currNode->children;
			while (temp->sibling != NULL)
				temp = temp->sibling;
			temp->sibling = sourceNode->children;
		}
		treeNode* children = currNode->children;
		while (children != NULL)
		{
			children->parent = currNode;
			children = children->sibling;
		}
		return true;
	}
	void clearTree()
	{
		root->children = NULL;
		return;
	}
};

//**************** Global Objects ****************//

tree t;

//**************** Functions ****************//

void createTree(string inputFilePath) 
{
	t.clearTree();
	ifstream iFile;
	iFile.open(inputFilePath);
	if (iFile.is_open())
	{
		string str;
		while (getline(iFile, str))
		{
			bool flag = t.insert(str);
			if (!flag)
			{
				cout << "Unexpected error! The file directory tree could not be created." << endl;
				break;
			}
		}
		iFile.close();
	}
	else
		cout << "Error! The input file could not be opened." << endl;
	return;
}

void levelOrderFunction(string outputFilePath) 
{
	ofstream oFile;
	oFile.open(outputFilePath, ios::out);
	if (oFile.is_open())
	{
		string output = t.levelOrderTraversal();
		int index = 0;
		while (output[index] != '\0')
		{
			oFile << output[index];
			index++;
		}
		oFile.close();
	}
	else
		cout << "Error! The output file could not be opened." << endl;
	return;
}

void insertFileFolder(string source, string targetPath, string outputFilePath) 
{
	int index = 0;
	while (targetPath[index] != '\0')
		index++;
	if (!(targetPath[index - 1] == 't' && targetPath[index - 2] == 'x' && targetPath[index - 3] == 't'))
	{
		if (targetPath[index - 1] != '/')
			targetPath += '/';
		index = 0;
		targetPath += source;
		t.insert(targetPath);
	}
	levelOrderFunction(outputFilePath);
	return;
}

void deleteFileFolder(string targetPath, string outputFilePath) 
{
	t.del(targetPath);
	levelOrderFunction(outputFilePath);
	return;
}

void mergeFolder(string sourcePath, string targetPath, string outputFilePath) 
{
	t.merge(sourcePath, targetPath);
	levelOrderFunction(outputFilePath);
	return;
}

void searchFileFolder(string targetFileFolder, string outputFilePath) 
{
	ofstream oFile;
	oFile.open(outputFilePath, ios::out);
	if (oFile.is_open())
	{
		string path = t.search(targetFileFolder);
		oFile << path;
		oFile.close();
	}
	else
		cout << "Error! The output file could not be opened." << endl;
	return;
}
/*
int main()
{
	createTree("input.txt");
	//levelOrderFunction("C:/Users/zohai/Desktop/output.txt");
	insertFileFolder("testcase.txt", "/root/bin/apt-get", "C:/Users/zohai/Desktop/output2.txt");
	return 0;
}*/