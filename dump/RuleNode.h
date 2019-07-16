// Sunday, November 26, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_RULE_NODE_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_RULE_NODE_H

#include <com/onecube/zen/Configuration.h>

/*******************************************************************************
 * RuleNode                                                                    *
 *******************************************************************************/

/**
 * A rule node records a single rule invocation.
 */
struct zen_RuleNode_t {
    zen_ArrayList_t* m_children; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_node;
};

zen_RuleNode_t* zen_RuleNode_new(zen_ASTNode_t* node);
void zen_RuleNode_delete(zen_RuleNode_t* ruleNode);
void zen_RuleNode_addRule(zen_RuleNode_t* ruleNode, zen_ASTNode_t* rule);
void zen_RuleNode_addTerminal(zen_RuleNode_t* ruleNode, zen_ASTNode_t* terminal);
void zen_RuleNode_addErrorTerminal(zen_RuleNode_t* ruleNode, zen_ASTNode_t* terminal);
zen_ASTNode_t* zen_RuleNode_getChild(zen_RuleNode_t* ruleNode, int32_t index);
zen_ASTNode_t* zen_RuleNode_getChildCount(zen_RuleNode_t* ruleNode, int32_t index);
zen_ASTNode_t* zen_RuleNode_getToken(zen_RuleNode_t* ruleNode, zen_TokenType_t type, int32_t index);
zen_ArrayList_t* zen_RuleNode_getTokens(zen_RuleNode_t* ruleNode, zen_TokenType_t type);

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_RULE_NODE_H */