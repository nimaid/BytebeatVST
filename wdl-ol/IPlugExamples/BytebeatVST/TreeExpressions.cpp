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
	destroyTree();

	//do all the fun making stuff
	//tokenize
	//then reference that python script you made to turn to polish notation
	//then turn the polish notation into a tree


}