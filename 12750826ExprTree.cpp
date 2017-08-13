#include "ExprTree.h"
#include <sstream>
#include <iostream>

/*
 * Helper function that tests whether a string is a non-negative integer.
 */

// Recursion used for _size to construct the ExprTree(TreeNode *r)
int size(TreeNode * node)
{
  int size = 0;
  if(node == NULL)
  {
    return 0;
  }else
  {
    size += 1 + ::size(node->getRightChild()) + ::size(node->getLeftChild()); // +1 because of root
    return size;
  }
}

int getOpPriority(string op)
{
	//Higher priority for / and *.
	if (op == "/") return 2;
	else if (op == "*") return 2;
	else if (op == "+") return 1;
	else if (op == "-") return 1;
	else return 0;
}

bool isdigit(const char & c){

  switch (c) {
  case '0' :
  case '1' :
  case '2' :
  case '3' :
  case '4' :
  case '5' :
  case '6' :
  case '7' :
  case '8' :
  case '9' : return true;
  }

  return false;

}

bool is_number(const std::string & s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/*
 * Helper function that converts a string to an int.
 */
int to_number(const std::string & s){
  return atoi(s.c_str());
}

/*
 * Helper function that converts a number to a string.
 */
string to_string(const int & n){
  std::stringstream stream;
  stream << n;
  return stream.str();
}

/*
 * Helper function that creates a TreeNode with the appropriate operator
 * when given a string that's "+", "-", "*" or "/". If the string is wrong
 * it gives a NoOp value.
 */
TreeNode * createOperatorNode(const string & op){

  if (op == "+") return new TreeNode(Plus);
  if (op == "-") return new TreeNode(Minus);
  if (op == "*") return new TreeNode(Times);
  if (op == "/") return new TreeNode(Divide);
  return new TreeNode(NoOp);

}

/*
 * Basic constructor that sets up an empty Expr Tree.
 */
ExprTree::ExprTree(){
  root = 0;
  _size = 0;
}

/*
 * Constructor that takes a TreeNode and sets up an ExprTree with that node at the root.
 */
ExprTree::ExprTree(TreeNode * r){
  root = r;
  _size = ::size(root);
}

/*
 * Destructor to clean up the tree.
 * Not sure how to do this so not going to touch it
 */
ExprTree::~ExprTree()
{
  TreeNode *n;
  while(n != NULL)
  {
    if(n->getRightChild != NULL)
    {
      delete n->getRightChild();
    }
  }

}

/*
 * This function takes a string representing an arithmetic expression and breaks
 * it up into components (number, operators, parentheses).
 * It returns the broken up expression as a vector of strings.

 * Reads in a string, then breaks it up into the individual components like
 * in the example above. Returns the components grouped together as a vector
 * of strings. The input string may or may not separate the components with white space. */
vector<string> ExprTree::tokenise(string expression)
{
  string field;
 	vector<string> tokens;
 	int posOpen;
 	int posClose;
  int posSpace;
  std::istringstream iss(expression);

  // std::cout << "\nStart tokenise" << std::endl;
  //getline(iss, field, ' ')
 	while (iss >> field) // iss automatically removes spaces
 	{
 		//Check if field contains '(' or ')' characters.
 		posOpen = field.find('(');
 		posClose = field.find(')');

    // If '(' != -1 (npos = non position)
 		if (posOpen != string::npos) //A '(' exists.
 		{
 		// 	std::cout << "\n(" << std::endl;
 		// 	std::cout << field.substr(posOpen + 1, string::npos) << std::endl;
 			tokens.push_back("(");
 			tokens.push_back(field.substr(posOpen + 1, string::npos));
      // substr(position, length of input)
      // So posOpen + 1 = next set of numbers and making sure that length is not -1 (npos)
      // This way we're not setting any length of the return substr
 		}
 		else if(posClose != string::npos)	//A ')' exists.
 		{
 		// 	std::cout << field.substr(0, posClose) << std::endl;
 		// 	std::cout << ")" << std::endl;
 			tokens.push_back(field.substr(0, posClose)); // posClose determines the length of the number reaching the end.
 			tokens.push_back(")");
 		}
 		else // Must be an operator
 		{
      tokens.push_back(field);
 			// std::cout << "[" << field << "]\n"<< std::endl;
      // std::cout << "End of tokenise\n\n";
 		}
   }

  return tokens;
}

/*
 * This function takes a vector of strings representing an expression (as produced
 * by tokenise(string), and builds an ExprTree representing the same expression.
 */
ExprTree ExprTree::buildTree(vector<string> tokens)
{
  	int i;
  	vector<string> postFixOp;
  	stack<string> strStack;
  	stack<TreeNode *> treeStack;
  	TreeNode *node, *leftNode, *rightNode;

  	// std::cout << "Tokens ";
  	// for (i = 0; i < tokens.size(); i++)
  	// {
  	// 	std::cout << "[" << tokens[i]<< "]\n";
  	// }

    // Convert infix vector to a postFixOp vector
  	for (i = 0; i < tokens.size(); i++)
  	{
  		//For all operands, add it to our postFixOp vector.
  		if (is_number(tokens[i]))
  		{
  			postFixOp.push_back(tokens[i]);
  		}
  		else if(tokens[i] == "(")
  		{
  			//Open bracket gets pushed onto stack.
  			strStack.push(tokens[i]); // strStack stack
  		}
  		else if(tokens[i] == ")") //If there's an open bracket, there's a closing.
  		{
  			//Once we get closing bracket, start taking it
  			//out of the stack we built when we got open bracket.
  			while (!strStack.empty())
  			{
  				if (strStack.top() == "(")
  				{
  					strStack.pop();
  					break;
  				}
  				postFixOp.push_back(strStack.top());
  				strStack.pop();
  			}
  		}
  		else
  		{
  			//We must have an operator type.
  			//In this case, if stack is already empty
  			{
  				//Take out operators from the stack and
  				//push to the postFixOp vector until we see
  				//operators with lower priority than current.
  				while (!strStack.empty() &&
  						strStack.top() != "("  && // Otherwise there's a mismatch
  						getOpPriority(tokens[i]) <= getOpPriority(strStack.top()))
              // Checking precedence - '*' and '/' has higher priority
  				{
  					postFixOp.push_back(strStack.top());
  				  strStack.pop();
  				}
  				strStack.push(tokens[i]);
  			}
  		}
  	}

  	//Any remaining operators on the stack built so far
  	//needs to be popped and pushed to postFixOp vector.
  	while (!strStack.empty())
  	{
  		postFixOp.push_back(strStack.top());
  		strStack.pop();
  	}
  	//std::cout << "Start ";
  	//for (i = 0; i < postFixOp.size(); i++)
  	//{
  	//	std::cout << "[" << postFixOp[i]<< "]";
  	//}
  	//std::cout << std::endl;

    // Now that everything is on a vector string (postFixOp),
    // now build the expression tree.
  	for (i = 0; i < postFixOp.size(); i++)
  	{
  		//Operands are pushed onto the stack first
  		if (is_number(postFixOp[i]))
  		{
  			node = new TreeNode(to_number(postFixOp[i]));
  			treeStack.push(node);
  		}
  		else
  		{
  			// For operators use createOperatorNode()
  			// 5 2 [-]
  			node = createOperatorNode(postFixOp[i]);

  			rightNode = treeStack.top();	//Save the right node.
  			treeStack.pop();
  			leftNode = treeStack.top();	//Save the left node.
  			treeStack.pop();

  			//Setup the children from this node.
  			node->setRightChild(rightNode);
  			node->setLeftChild(leftNode);

  			//Setup the node as common parent.
  			rightNode->setParent(node);
  			leftNode->setParent(node);

  			//Add the operator node into stack.
  			treeStack.push(node);
  		}
  	}

  	//Top of stack has the root.
  	ExprTree et(treeStack.top());
  	treeStack.pop();

  	// std::cout << "infix: " << infixOrder(et.root) << std::endl;

  	return et;
}

/*
 * This function takes a TreeNode and does the maths to calculate
 * the value of the expression it represents.
 */
int ExprTree::evaluate(TreeNode * n)
{
  // 1. If operator is plus, minus, times or divide then return the operator
  // 2. Depending on 1. (operator value) return that value
  // 3. result += recurse the left and rightChild
  // 4. Base case to check if operator is a value
  ExprTree t;
  char op = n->getOperator();
  int isOp = n->isOperator();
  int leftResult = 0;
  int rightResult = 0;
  int totalResult = 0;

  if(n == NULL) { return 0; }

  while(n != NULL)
  {
    if(!isOp)
    {
      return n->getValue();
    }

    leftResult += evaluate(n->getLeftChild());
    rightResult += evaluate(n->getRightChild());

    switch(op)
    {
      case Plus   : return leftResult + rightResult;
      case Minus  : return leftResult - rightResult;
      case Times  : return leftResult * rightResult;
      case Divide : return leftResult / rightResult;
      default     : return 0;
    }
  }

  return totalResult;
}

/*
 * When called on an ExprTree, this function calculates the value of the
 * expression represented by the whole tree.
 */
int ExprTree::evaluateWholeTree()
{
  return evaluate(root);
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * prefix notation.
 */

 /* Visit root, g§etLeftChild(), getRightChild() */
string ExprTree::prefixOrder(const ExprTree & t)
{
  string expression;
  // Start from the root and convert toString()
  expression += t.root->toString();

  if(t.root->getLeftChild() != NULL)
  {                                // Using ExprTree(TreeNode *n) instead of creating another function
    expression += " " + prefixOrder(ExprTree(t.root->getLeftChild()));
  }

  if(t.root->getRightChild() != NULL)
  {
    expression += " " + prefixOrder(ExprTree(t.root->getRightChild()));
  }

  return expression;
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * infix notation.
 */

 /* Visit getLeftChild(), root, getRightChild() */
string ExprTree::infixOrder(const ExprTree & t)
{
  string expression;

  if(t.root->getLeftChild() != NULL)
  {
    expression += infixOrder(ExprTree(t.root->getLeftChild())) + " ";
  }

  expression += t.root->toString();

  if(t.root->getRightChild() != NULL)
  {
    expression += " " + infixOrder(ExprTree(t.root->getRightChild()));
  }

  return expression;
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * postFixOp notation.
 */

 /* Visit getLeftChild(), getRightChild(), root */
string ExprTree::postfixOrder(const ExprTree & t)
{
  string expression;

  if(t.root->getLeftChild() != NULL)
  {
    expression += postfixOrder(ExprTree(t.root->getLeftChild())) + " ";
  }

  if(t.root->getRightChild() != NULL)
  {
    expression += postfixOrder(ExprTree(t.root->getRightChild())) + " ";
  }

  expression += t.root->toString();

  return expression;
}

/*
 * Returns the size of the tree. (i.e. the number of nodes in it)
 */
int ExprTree::size(){ return _size; }

/*
 * Returns true if the tree contains no nodes. False otherwise.
 */
bool ExprTree::isEmpty(){ return _size == 0; }

TreeNode * ExprTree::getRoot(){ return root; }
