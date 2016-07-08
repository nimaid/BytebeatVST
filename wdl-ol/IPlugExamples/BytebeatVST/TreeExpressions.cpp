#include "TreeExpressions.h"

void TreeNode::setOp(std::string op)
{
	isVar = false;
	isOp = true;

	if (op == "^")
	{
		opFunc = &xorOp;
	}
	else if (op == "&")
	{
		opFunc = &andOp;
	}
	else if (op == "|")
	{
		opFunc = &orOp;
	}
	else if (op == "<<")
	{
		opFunc = &lshiftOp;
	}
	else if (op == ">>")
	{
		opFunc = &rshiftOp;
	}
	else if (op == "+")
	{
		opFunc = &addOp;
	}
	else if (op == "-")
	{
		opFunc = &subOp;
	}
	else if (op == "*")
	{
		opFunc = &multOp;
	}
	else if (op == "/")
	{
		opFunc = &divOp;
	}
	else if (op == "%")
	{
		opFunc = &modOp;
	}
}

void TreeNode::setVal(uint32_t val)
{
	isVar = false;
	isOp = false;

	value = val;
}

void TreeNode::setVar()
{
	isVar = true;
	isOp = false;
}

uint32_t TreeNode::evaluate(uint32_t tVal)
{
	if (isOp)
	{
		//if it's an op
		return( opFunc( (*first).evaluate(tVal), (*last).evaluate(tVal) ) );
	}
	else if (isVar)
	{
		//if it's a var
		return(tVal);
	}
	else
	{
		//if it's a number
		return(value);
	}
}



ExpressionTree::ExpressionTree()
{
	treeRoot = NULL;

	// http://en.cppreference.com/w/cpp/language/operator_precedence
	precedence["*"] = 5;
	precedence["/"] = 5;
	precedence["%"] = 5;
	precedence["+"] = 6;
	precedence["-"] = 6;
	precedence["<<"] = 7;
	precedence[">>"] = 7;
	precedence["&"] = 10;
	precedence["^"] = 11;
	precedence["|"] = 12;
}

ExpressionTree::~ExpressionTree()
{
	destroyTree();
}

void ExpressionTree::destroyTree(TreeNode *leaf)
{
	if (leaf != NULL)
	{
		destroyTree(leaf->first);
		destroyTree(leaf->last);
		delete leaf;
	}
}

bool ExpressionTree::insert(bool first, TreeNode *leaf)
{
	if (first)
	{
		if (leaf->first != NULL)
			return(false);
		else
		{
			leaf->first = new TreeNode;
			leaf->first->first = NULL;
			leaf->first->last = NULL;
		}
	}
	else
	{
		if (leaf->last != NULL)
			return(false);
		else
		{
			leaf->last = new TreeNode;
			leaf->last->first = NULL;
			leaf->last->last = NULL;
		}
	}
}

void ExpressionTree::destroyTree()
{
	destroyTree(treeRoot);
}

bool ExpressionTree::build(std::string formulaStr)
{
	std::string::iterator stringIterator;
	std::vector<std::string>::iterator stringVectorIterator;
	
	std::vector<std::string> formulaTokens;
	std::vector<std::string> pnTokens;
	std::stack<std::string> stringStack;
	std::string currentNumString = "";
	std::string currentTokenString = "";
	std::stack<TreeNode> nodeStack;
	TreeNode *currentNode = treeRoot;
	std::string currToken;

	//tokenization
	for (stringIterator = formulaStr.begin(); stringIterator != formulaStr.end(); stringIterator++)
	{
		std::string currCharString(1, *stringIterator);

		/*
		if ((currentTokenString.length() > 0) && !((precedence.find(currCharString) != precedence.end()) || (currCharString == "(") || (currCharString == ")") || std::all_of(currCharString.begin(), currCharString.end(), ::isdigit) || (currCharString == "t")))
		{
			//if there is something in the current token string, and the  current char string is not any other case

		}
		*/

		currentTokenString.push_back(*stringIterator);

		if ((precedence.find(currentTokenString) != precedence.end()) || (currentTokenString == "(") || (currentTokenString == ")"))
		{
			//if the current token string is found in the precedence list (or is a parentheses)
			formulaTokens.push_back(currentTokenString);
			currentTokenString = "";

			if (currentNumString != "")
			{
				formulaTokens.push_back(currentNumString);
				currentNumString = "";
			}
		}
		else if (std::all_of(currentTokenString.begin(), currentTokenString.end(), ::isdigit))
		{
			//if the current token string is all digits
			currentNumString.append(currentTokenString);
			currentTokenString = "";
		}
		else if (currentTokenString == "t")
		{
			//if the current token string is the t variable
			formulaTokens.push_back(currentTokenString);
			currentTokenString = "";
		}
	}

	//convert to polish notation
	std::reverse(formulaTokens.begin(), formulaTokens.end());

	stringStack.push(")");

	for (stringVectorIterator = formulaTokens.begin(); stringVectorIterator != formulaTokens.end(); stringVectorIterator++)
	{
		if ((precedence.find(*stringVectorIterator) == precedence.end()) && (*stringVectorIterator != "(") && (*stringVectorIterator != ")"))
		{
			pnTokens.push_back(*stringVectorIterator);
		}
		else if (*stringVectorIterator == ")")
		{
			stringStack.push(*stringVectorIterator);
		}
		else if (precedence.find(*stringVectorIterator) != precedence.end())
		{
			if (stringStack.size() > 0)
			{
				if (stringStack.top() != ")")
				{
					while (precedence[stringStack.top()] <= precedence[*stringVectorIterator])
					{
						if (stringStack.top() != ")")
						{
							pnTokens.push_back(stringStack.top());
							stringStack.pop();
						}

						if (stringStack.size() <= 0)
						{
							break;
						}

						if (stringStack.top() == ")")
						{
							break;
						}
					}
				}
			}
			stringStack.push(*stringVectorIterator);
		}
		else if (*stringVectorIterator == "(")
		{
			if (stringStack.size() > 0)
			{
				while (stringStack.top() != ")")
				{
					pnTokens.push_back(stringStack.top());
					stringStack.pop();

					if (stringStack.size() <= 0)
					{
						break;
					}
				}
			}
		}
	}

	while (stringStack.size() > 0)
	{
		if (stringStack.top() != ")")
		{
			pnTokens.push_back(stringStack.top());
			stringStack.pop();
		}
	}

	std::reverse(pnTokens.begin(), pnTokens.end());

	//if it's gotten this far, the formula was valid
	//destory the current tree to make room
	destroyTree();

	//parse polish notation into tree
	for (stringVectorIterator = pnTokens.begin(); stringVectorIterator != pnTokens.end(); stringVectorIterator++)
	{
		currToken = *stringVectorIterator;
		if (precedence.find(currToken) != precedence.end())
		{
			//if the token is an operator
			currentNode->setOp(currToken);
			
			TreeNode *newNode;
			TreeNode *parentNode = currentNode;
			
			//a new operator node will always have the first branch free
			currentNode->first = newNode;
			currentNode = newNode;
			currentNode->parent = parentNode;
		}
		else
		{
			//if it's a number or a variable
			if (std::all_of(currToken.begin(), currToken.end(), ::isdigit))
			{
				//if it's a number
				currentNode->setVal((uint32_t)atoi(currToken.c_str()));
			}
			else
			{
				//if it's something else, a variable
				currentNode->setVar();
			}
			
			currentNode = currentNode->parent;
			
			//since we just moved up, we know at least the first branch is used
			//so only check the last
			while (currentNode->last != NULL)
			{
				//if the last node is not free
				if (currentNode == treeRoot)
				{
					//if we're at the root, and it's totally populated
					break;
				}

				currentNode = currentNode->parent;
			}
		}
	}
	return(true);
}

uint8_t ExpressionTree::evaluate(uint32_t tVal)
{
	return((uint8_t)treeRoot->evaluate(tVal));
}