#include <cstdint>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <algorithm>

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
	bool build(std::string formulaStr);
	uint8_t evaluate(uint32_t tVal);
};
