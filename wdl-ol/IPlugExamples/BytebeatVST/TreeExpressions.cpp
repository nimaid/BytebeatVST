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
		return( opFunc( first->evaluate(tVal), last->evaluate(tVal) ) );
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

TreeNode *ExpressionTree::insertNode(TreeNode *leaf)
{
	if (leaf->first == NULL)
	{
        leaf->first = new TreeNode;
        leaf->first->parent = leaf;
        return(leaf->first);
	}
	else
	{
        leaf->last = new TreeNode;
        leaf->last->parent = leaf;
        return(leaf->last);
	}
}

void ExpressionTree::destroyTree()
{
	destroyTree(treeRoot);
}

std::vector<std::string> ExpressionTree::build(std::string formulaStr)
{
	std::string::iterator stringIterator;
	std::vector<std::string>::iterator stringVectorIterator;

	std::vector<std::string> retVal;
	std::deque<std::string> formulaTokensToAppend;
	std::string formulaTokenToAppend;
	int lParems = 0;
	std::vector<std::string> formulaTokens;
	std::vector<std::string> pnTokens;
	std::stack<std::string> stringStack;
	std::string lastToken = "";
	std::string currentNumString = "";
	std::string currentTokenString = "";
	std::stack<TreeNode> nodeStack;
	TreeNode *currentNode;
	std::string currToken;
	bool treeIsDone;

	//tokenization

	for (stringIterator = formulaStr.begin(); stringIterator != formulaStr.end(); stringIterator++)
	{
		std::string currCharString(1, *stringIterator);

		currentTokenString.push_back(*stringIterator);

		if ((precedence.find(currentTokenString) != precedence.end()) || (currentTokenString == "(") || (currentTokenString == ")"))
		{
			//if the current token string is found in the precedence list (or is a parentheses)
			if (currentNumString != "")
			{
				formulaTokensToAppend.push_back(currentNumString);
				currentNumString = "";
			}

			formulaTokensToAppend.push_back(currentTokenString);
			currentTokenString = "";
		}
		else if (std::all_of(currCharString.begin(), currCharString.end(), ::isdigit))
		{
			//if the current char string is all digits

			if (!std::all_of(currentTokenString.begin(), currentTokenString.end(), ::isdigit))
			{
				//if the current token string is not all digits
				currentTokenString.pop_back();
				retVal.push_back(currentTokenString);
				retVal.push_back(ETBUILD_UNRECOTOK);
				return(retVal);
			}

			currentNumString.append(currentTokenString);
			currentTokenString = "";
		}
		else if (currCharString == "t")
		{
			//if the current token string is the t variable

			if (currentNumString != "")
			{
				formulaTokensToAppend.push_back(currentNumString);
				currentNumString = "";
			}

			if (currentTokenString != currCharString)
			{
				//if the current token string is not just the variable
				currentTokenString.pop_back();
				retVal.push_back(currentTokenString);
				retVal.push_back(ETBUILD_UNRECOTOK);
				return(retVal);
			}

			formulaTokensToAppend.push_back(currentTokenString);
			currentTokenString = "";
		}

		if (stringIterator + 1 == formulaStr.end())
		{
			//if this is the last char in the formula
			if (currentNumString.size() > 0)
			{
				formulaTokensToAppend.push_back(currentNumString);
				currentNumString = "";
			}
		}

		//here is the central location where we append tokens, if any
		//this is a good place to perform token checking
		while (formulaTokensToAppend.size() > 0)
		{
			formulaTokenToAppend = formulaTokensToAppend.front();
			formulaTokensToAppend.pop_front();

			if (formulaTokenToAppend == "t")
			{
				//if token is a variable
				if (formulaTokens.size() > 0)
				{
					if (formulaTokens.back() == ")")
					{
						retVal.push_back(ETBUILD_RPARENVAR);
						return(retVal);
					}

					if ((formulaTokens.back() == "t") || (std::all_of(formulaTokens.back().begin(), formulaTokens.back().end(), ::isdigit)))
					{
						retVal.push_back(formulaTokenToAppend);
						retVal.push_back(ETBUILD_VARMISSOP);
						return(retVal);
					}
				}
			}
			else if (std::all_of(formulaTokenToAppend.begin(), formulaTokenToAppend.end(), ::isdigit))
			{
				//if token is a number
				if (formulaTokens.size() > 0)
				{
					if (formulaTokens.back() == ")")
					{
						retVal.push_back(ETBUILD_RPARENNUM);
						return(retVal);
					}

					if ((formulaTokens.back() == "t") || (std::all_of(formulaTokens.back().begin(), formulaTokens.back().end(), ::isdigit)))
					{
						retVal.push_back(formulaTokenToAppend);
						retVal.push_back(ETBUILD_NUMMISSOP);
						return(retVal);
					}
				}
			}
			else if (precedence.find(formulaTokenToAppend) != precedence.end())
			{
				//if token is an operator
				if (stringIterator == formulaStr.begin())
				{
					retVal.push_back(formulaTokenToAppend);
					retVal.push_back(ETBUILD_OPATSTART);
					return(retVal);
				}

				if (stringIterator + 1 == formulaStr.end())
				{
					retVal.push_back(formulaTokenToAppend);
					retVal.push_back(ETBUILD_OPATEND);
					return(retVal);
				}

				if (formulaTokens.size() > 0)
				{
					if (precedence.find(formulaTokens.back()) != precedence.end())
					{
						retVal.push_back(formulaTokens.back());
						retVal.push_back(formulaTokenToAppend);
						retVal.push_back(ETBUILD_OPSTOUCH);
						return(retVal);
					}
				}
			}
			else if (formulaTokenToAppend == "(")
			{
				//if token is a left paren
				lParems++;

				if (formulaTokens.size() > 0)
				{
					if (formulaTokens.back() == ")")
					{
						//if there is a set of parens with no operator (or anything) between them
						retVal.push_back(ETBUILD_PARENSMISSOP);
						return(retVal);

					}

					if (formulaTokens.back() == "t")
					{
						retVal.push_back(ETBUILD_LPARENVAR);
						return(retVal);
					}

					if (std::all_of(formulaTokens.back().begin(), formulaTokens.back().end(), ::isdigit))
					{
						retVal.push_back(ETBUILD_LPARENNUM);
						return(retVal);
					}
				}
			}
			else if (formulaTokenToAppend == ")")
			{
				//if token is a right paren
				lParems--;

				if (formulaTokens.size() > 0)
				{
					if (formulaTokens.back() == "(")
					{
						//if there is a set of empty parens
						retVal.push_back(ETBUILD_PARENSEMPTY);
						return(retVal);
					}
				}
			}

			if (formulaTokenToAppend != " ")
			{
				//disregard spaces
				formulaTokens.push_back(formulaTokenToAppend);
			}
		}
	}

	if (currentTokenString.size() > 0)
	{
		//if there was something else in the input string at the end
		retVal.push_back(currentTokenString);
		retVal.push_back(ETBUILD_UNRECOTOK);
		return(retVal);
	}

	if (lParems < 0)
	{
		//if there were unmatched right parens
		retVal.push_back(ETBUILD_UNMATCHRPAREN);
		return(retVal);
	}
	else if (lParems > 0)
	{
		//if there were unmatched left parens
		retVal.push_back(ETBUILD_UNMATCHLPAREN);
		return(retVal);
	}

	//if it's gotten this far, the formula was valid

	//convert to polish notation
	std::reverse(formulaTokens.begin(), formulaTokens.end());

	stringStack.push(")");

	for (stringVectorIterator = formulaTokens.begin(); stringVectorIterator != formulaTokens.end(); stringVectorIterator++)
	{
		currToken = *stringVectorIterator;

		if ((precedence.find(currToken) == precedence.end()) && (currToken != "(") && (currToken != ")"))
		{
			//if the current token is not an operator or a paren
			//so, if it's variable or a number
			pnTokens.push_back(currToken);
		}
		else if (currToken == ")")
		{
			stringStack.push(currToken);
		}
		else if (precedence.find(currToken) != precedence.end())
		{
			//if the current token is an operator
			if (stringStack.size() > 0)
			{
				if (stringStack.top() != ")")
				{
					while (precedence[stringStack.top()] <= precedence[currToken])
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
			stringStack.push(currToken);
		}
		else if (currToken == "(")
		{
			//if the current token is a right paren (represented as a left paren)
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

				stringStack.pop();
			}
		}

		lastToken = currToken;
	}

	while (stringStack.size() > 0)
	{
		if (stringStack.top() != ")")
		{
			pnTokens.push_back(stringStack.top());
		}
		stringStack.pop();
	}

	std::reverse(pnTokens.begin(), pnTokens.end());

	//destroy the current tree to make room
	destroyTree();

	//parse polish notation into tree
	treeRoot = new TreeNode;
	currentNode = treeRoot;
	treeIsDone = false;

	for (stringVectorIterator = pnTokens.begin(); stringVectorIterator != pnTokens.end(); stringVectorIterator++)
	{
		currToken = *stringVectorIterator;
		if (precedence.find(currToken) != precedence.end())
		{
			//if the token is an operator
			currentNode->setOp(currToken);

			currentNode = insertNode(currentNode);
		}
		else
		{
			//if it's a number or a variable
			if (currentNode->first != NULL)
            {
                //if the current node has it's first branch initialized
                while (currentNode->last != NULL)
                {
                    //while the last branch is initialized
                    currentNode = currentNode->parent;
                }

                currentNode = insertNode(currentNode);
            }

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

			if (currentNode != treeRoot)
			{
				currentNode = currentNode->parent;
			}

			//since we just moved up, we know at least the first branch is used
			//so only check the last
			while (currentNode->last != NULL)
			{
				//if the last node is not free
				if (currentNode == treeRoot)
				{
					//if we're at the root, and it's totally populated
					treeIsDone = true;
					break;
				}

				currentNode = currentNode->parent;
			}

			if (!treeIsDone)
			{
				
				currentNode = insertNode(currentNode);
			}
		}
	}

	retVal.push_back(ETBUILD_SUCCESS);
	return(retVal);
}

uint8_t ExpressionTree::evaluate(uint32_t tVal)
{
	return((uint8_t)treeRoot->evaluate(tVal));
}
