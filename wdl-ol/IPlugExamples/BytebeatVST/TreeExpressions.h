#include <cstdint>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <deque>
#include <algorithm>

//define ExpressionTree::build error codes

//non-error
#define ETBUILD_SUCCESS "SUCCESS" //succussfuly built tree

//tokenization errors
#define ETBUILD_UNRECOTOK "UNRECOTOK" //unrecognized token

//operator errors
#define ETBUILD_OPSTOUCH "OPSTOUCH" //two operators are immediately next to each other
#define ETBUILD_OPATSTART "OPATSTART" //the first first token is an operator
#define ETBUILD_OPATEND "OPATEND" //the last first token is an operator

//operand errors
#define ETBUILD_VARMISSOP "VARMISSOP" //variable has no operator between it and another operand before it
#define ETBUILD_NUMMISSOP "NUMMISSOP" //number has no operator between it and another operand before it

//paren errors
#define ETBUILD_PARENSMISSOP "PARENSMISSOP" //right and left parens have no operator between them
#define ETBUILD_PARENSEMPTY "PARENSEMPTY" //left and right parens have nothing between them
#define ETBUILD_LPARENVAR "LPARENVAR" //left paren has variable before it instead of another left paren or an operator
#define ETBUILD_LPARENNUM "LPARENNUM" //left paren has number before it instead of another left paren or an operator
#define ETBUILD_RPARENVAR "RPARENVAR" //right paren has variable after it instead of another right paren or an operator
#define ETBUILD_RPARENNUM "RPARENNUM" //right paren has number after it instead of another right paren or an operator
#define ETBUILD_UNMATCHLPAREN "UNMATCHLPAREN" //unmatched left paren
#define ETBUILD_UNMATCHRPAREN "UNMATCHRPAREN" //unmatched right paren

struct TreeNode {
private:
	bool isOp = false;
	bool isVar = false;
	uint32_t value = 0;
	uint32_t(*opFunc)(uint32_t, uint32_t) = NULL;

	static inline uint32_t xorOp(uint32_t a, uint32_t b) { return(a ^ b); };
	static inline uint32_t andOp(uint32_t a, uint32_t b) { return(a & b); };
	static inline uint32_t orOp(uint32_t a, uint32_t b) { return(a | b); };
	static inline uint32_t lshiftOp(uint32_t a, uint32_t b) { return(a << b); };
	static inline uint32_t rshiftOp(uint32_t a, uint32_t b) { return(a >> b); };
	static inline uint32_t addOp(uint32_t a, uint32_t b) { return(a + b); };
	static inline uint32_t subOp(uint32_t a, uint32_t b) { return(a - b); };
	static inline uint32_t multOp(uint32_t a, uint32_t b) { return(a * b); };
	static inline uint32_t divOp(uint32_t a, uint32_t b) { if (b == 0) { return(0); } else { return(a / b); } };
	static inline uint32_t modOp(uint32_t a, uint32_t b) { if (b == 0) { return(0); } else { return(a % b); } };
public:
	TreeNode *first = NULL;
	TreeNode *last = NULL;
	TreeNode *parent = NULL;
	uint32_t evaluate(uint32_t tVal);
	void setOp(std::string op);
	void setVal(uint32_t val);
	void setVar();
};

class ExpressionTree
{
private:
	std::map<std::string, int> precedence;
	TreeNode *treeRoot;
	TreeNode *insertNode(TreeNode *leaf);
	void destroyTree(TreeNode *leaf);
public:
	ExpressionTree();
	~ExpressionTree();
	void destroyTree();
	std::vector<std::string> build(std::string formulaStr);
	uint8_t evaluate(uint32_t tVal);
};
