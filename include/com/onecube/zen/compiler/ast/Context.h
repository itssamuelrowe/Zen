// Friday, November 24, 2017

#ifndef ZEN_CONTEXT_H
#define ZEN_CONTEXT_H

#include <zen/collection/ArrayList.h>
#include <zen/ASTNode.h>
#include <com/onecube/zen/Configuration.h>

/*
 * According to my knowledge, I had two options to implement the
 * contexts.
 *
 * 1. A generic class which represents all contexts. Each subrule
 *    or token in the rule is stored in a list. Accessing would
 *    require a linear search. Usually, children of a context do
 *    not exceed more than, say, 10. Thus, the performance would
 *    be affected in a negligible way. Further, each rule is
 *    associated with an helper classes which helps us in the
 *    linear search.
 *    This technique provides a common ground for destruction of
 *    nodes with error checking, abstract syntax tree (AST) walking,
 *    and other such generic manipulative tasks.
 *
 * 2. A custom class represents a particular rule. Each subrule
 *    or token in the rule is stored in its class. Accessing would
 *    not require any special methods. You just access with the
 *    member access operator.
 *
 * Here, I have implemented the latter technique. Yes, I know the
 * former technique is more robust. Unfortunately, the code base is
 * too big to modify now without hours of typing, compiling, and
 * debugging.
 */

/*******************************************************************************
 * CompilationUnitContext                                                      *
 *******************************************************************************/

struct zen_CompilationUnitContext_t {
    /**
     * The node which represents this rule context within the AST.
     */
    zen_ASTNode_t* m_node;
	zen_ArrayList_t* m_importDeclarations; /* <zen_ASTNode_t*> */
	zen_ArrayList_t* m_annotatedComponentDeclarations; /* <zen_ASTNode_t*> */
};

typedef struct zen_CompilationUnitContext_t zen_CompilationUnitContext_t;

zen_CompilationUnitContext_t* zen_CompilationUnitContext_new(zen_ASTNode_t* node);
void zen_CompilationUnitContext_delete(zen_CompilationUnitContext_t* context);
void zen_CompilationUnitContext_getChildren(zen_CompilationUnitContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ImportDeclarationContext                                                    *
 *******************************************************************************/

struct zen_ImportDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_identifiers; /* <zen_ASTNode_t*> */
    bool m_wildcard;
};

typedef struct zen_ImportDeclarationContext_t zen_ImportDeclarationContext_t;

zen_ImportDeclarationContext_t* zen_ImportDeclarationContext_new(zen_ASTNode_t* node);
void zen_ImportDeclarationContext_delete(zen_ImportDeclarationContext_t* context);
void zen_ImportDeclarationContext_getChildren(zen_ImportDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AnnotatedComponentDeclarationContext                                        *
 *******************************************************************************/

struct zen_AnnotatedComponentDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_annotations;
    zen_ASTNode_t* m_componentDeclaration;
};

typedef struct zen_AnnotatedComponentDeclarationContext_t zen_AnnotatedComponentDeclarationContext_t;

zen_AnnotatedComponentDeclarationContext_t* zen_AnnotatedComponentDeclarationContext_new(zen_ASTNode_t* node);
void zen_AnnotatedComponentDeclarationContext_delete(zen_AnnotatedComponentDeclarationContext_t* context);
void zen_AnnotatedComponentDeclarationContext_getChildren(zen_AnnotatedComponentDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AnnotationsContext                                                          *
 *******************************************************************************/

struct zen_AnnotationsContext_t {
    zen_ASTNode_t* m_node;
	zen_ArrayList_t* m_annotations; /* <zen_ASTNode_t*> */
};

typedef struct zen_AnnotationsContext_t zen_AnnotationsContext_t;

zen_AnnotationsContext_t* zen_AnnotationsContext_new(zen_ASTNode_t* node);
void zen_AnnotationsContext_delete(zen_AnnotationsContext_t* context);
void zen_AnnotationsContext_getChildren(zen_AnnotationsContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AnnotationContext                                                           *
 *******************************************************************************/

struct zen_AnnotationContext_t {
    zen_ASTNode_t* m_node;
	zen_ASTNode_t* m_annotationType;
	zen_ArrayList_t* m_annotationAttributes; /* <zen_ASTNode_t*> */
};

typedef struct zen_AnnotationContext_t zen_AnnotationContext_t;

zen_AnnotationContext_t* zen_AnnotationContext_new(zen_ASTNode_t* node);
void zen_AnnotationContext_delete(zen_AnnotationContext_t* context);
void zen_AnnotationContext_getChildren(zen_AnnotationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AnnotationTypeContext                                                       *
 *******************************************************************************/

struct zen_AnnotationTypeContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_identifiers; /* <zen_ASTNode_t*> */
};

typedef struct zen_AnnotationTypeContext_t zen_AnnotationTypeContext_t;

zen_AnnotationTypeContext_t* zen_AnnotationTypeContext_new(zen_ASTNode_t* node);
void zen_AnnotationTypeContext_delete(zen_AnnotationTypeContext_t* context);
void zen_AnnotationTypeContext_getChildren(zen_AnnotationTypeContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AnnotationAttributeContext                                                  *
 *******************************************************************************/

struct zen_AnnotationAttributeContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_literal;
};

typedef struct zen_AnnotationAttributeContext_t zen_AnnotationAttributeContext_t;

zen_AnnotationAttributeContext_t* zen_AnnotationAttributeContext_new(zen_ASTNode_t* node);
void zen_AnnotationAttributeContext_delete(zen_AnnotationAttributeContext_t* context);
void zen_AnnotationAttributeContext_getChildren(zen_AnnotationAttributeContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ComponentDeclarationContext                                                 *
 *******************************************************************************/

struct zen_ComponentDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_component;
};

typedef struct zen_ComponentDeclarationContext_t zen_ComponentDeclarationContext_t;

zen_ComponentDeclarationContext_t* zen_ComponentDeclarationContext_new(zen_ASTNode_t* node);
void zen_ComponentDeclarationContext_delete(zen_ComponentDeclarationContext_t* context);
void zen_ComponentDeclarationContext_getChildren(zen_ComponentDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * FunctionDeclarationContext                                                  *
 *******************************************************************************/

struct zen_FunctionDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_functionParameters;
    zen_ASTNode_t* m_functionBody;
};

typedef struct zen_FunctionDeclarationContext_t zen_FunctionDeclarationContext_t;

zen_FunctionDeclarationContext_t* zen_FunctionDeclarationContext_new(zen_ASTNode_t* node);
void zen_FunctionDeclarationContext_delete(zen_FunctionDeclarationContext_t* context);
void zen_FunctionDeclarationContext_getChildren(zen_FunctionDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * FunctionParametersContext                                                   *
 *******************************************************************************/

struct zen_FunctionParametersContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_fixedParameters; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_variableParameter;
};

typedef struct zen_FunctionParametersContext_t zen_FunctionParametersContext_t;

zen_FunctionParametersContext_t* zen_FunctionParametersContext_new(zen_ASTNode_t* node);
void zen_FunctionParametersContext_delete(zen_FunctionParametersContext_t* context);
void zen_FunctionParametersContext_getChildren(zen_FunctionParametersContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * FunctionBodyContext                                                         *
 *******************************************************************************/

struct zen_FunctionBodyContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_FunctionBodyContext_t zen_FunctionBodyContext_t;

zen_FunctionBodyContext_t* zen_FunctionBodyContext_new(zen_ASTNode_t* node);
void zen_FunctionBodyContext_delete(zen_FunctionBodyContext_t* context);
void zen_FunctionBodyContext_getChildren(zen_FunctionBodyContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * StatementSuiteContext                                                       *
 *******************************************************************************/

struct zen_StatementSuiteContext_t {
    zen_ASTNode_t* m_node;
    /* In order to help the users read code easily, the simple statements were
     * removed from statement suites. Previously, a statement suite was either
     * a simple statement or a block consisting of a newline at the begining,
     * indentation and dedentation.
     */
    // zen_ASTNode_t* m_simpleStatement;
    zen_ArrayList_t* m_statements;
    int32_t m_scope;
};

typedef struct zen_StatementSuiteContext_t zen_StatementSuiteContext_t;

zen_StatementSuiteContext_t* zen_StatementSuiteContext_new(zen_ASTNode_t* node);
void zen_StatementSuiteContext_delete(zen_StatementSuiteContext_t* context);
void zen_StatementSuiteContext_getChildren(zen_StatementSuiteContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * SimpleStatementContext                                                      *
 *******************************************************************************/

struct zen_SimpleStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statement;
};

typedef struct zen_SimpleStatementContext_t zen_SimpleStatementContext_t;

zen_SimpleStatementContext_t* zen_SimpleStatementContext_new(zen_ASTNode_t* node);
void zen_SimpleStatementContext_delete(zen_SimpleStatementContext_t* context);
void zen_SimpleStatementContext_getChildren(zen_SimpleStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * StatementContext                                                            *
 *******************************************************************************/

struct zen_StatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_simpleStatement;
    zen_ASTNode_t* m_compoundStatement;
};

typedef struct zen_StatementContext_t zen_StatementContext_t;

zen_StatementContext_t* zen_StatementContext_new(zen_ASTNode_t* node);
void zen_StatementContext_delete(zen_StatementContext_t* context);
void zen_StatementContext_getChildren(zen_StatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * EmptyStatementContext                                                       *
 *******************************************************************************/

struct zen_EmptyStatementContext_t {
    zen_ASTNode_t* m_node;
};

typedef struct zen_EmptyStatementContext_t zen_EmptyStatementContext_t;

zen_EmptyStatementContext_t* zen_EmptyStatementContext_new(zen_ASTNode_t* node);
void zen_EmptyStatementContext_delete(zen_EmptyStatementContext_t* context);
void zen_EmptyStatementContext_getChildren(zen_EmptyStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * VariableDeclarationContext                                                  *
 *******************************************************************************/

struct zen_VariableDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_variableDeclarators;
};

typedef struct zen_VariableDeclarationContext_t zen_VariableDeclarationContext_t;

zen_VariableDeclarationContext_t* zen_VariableDeclarationContext_new(zen_ASTNode_t* node);
void zen_VariableDeclarationContext_delete(zen_VariableDeclarationContext_t* context);
void zen_VariableDeclarationContext_getChildren(zen_VariableDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * VariableDeclaratorContext                                                   *
 *******************************************************************************/

struct zen_VariableDeclaratorContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_VariableDeclaratorContext_t zen_VariableDeclaratorContext_t;

zen_VariableDeclaratorContext_t* zen_VariableDeclaratorContext_new(zen_ASTNode_t* node);
void zen_VariableDeclaratorContext_delete(zen_VariableDeclaratorContext_t* context);
void zen_VariableDeclaratorContext_getChildren(zen_VariableDeclaratorContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ConstantDeclarationContext                                                  *
 *******************************************************************************/

struct zen_ConstantDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_constantDeclarators;
};

typedef struct zen_ConstantDeclarationContext_t zen_ConstantDeclarationContext_t;

zen_ConstantDeclarationContext_t* zen_ConstantDeclarationContext_new(zen_ASTNode_t* node);
void zen_ConstantDeclarationContext_delete(zen_ConstantDeclarationContext_t* context);
void zen_ConstantDeclarationContext_getChildren(zen_ConstantDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ConstantDeclaratorContext                                                   *
 *******************************************************************************/

struct zen_ConstantDeclaratorContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_ConstantDeclaratorContext_t zen_ConstantDeclaratorContext_t;

zen_ConstantDeclaratorContext_t* zen_ConstantDeclaratorContext_new(zen_ASTNode_t* node);
void zen_ConstantDeclaratorContext_delete(zen_ConstantDeclaratorContext_t* context);
void zen_ConstantDeclaratorContext_getChildren(zen_ConstantDeclaratorContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AssertStatementContext                                                      *
 *******************************************************************************/

struct zen_AssertStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_AssertStatementContext_t zen_AssertStatementContext_t;

zen_AssertStatementContext_t* zen_AssertStatementContext_new(zen_ASTNode_t* node);
void zen_AssertStatementContext_delete(zen_AssertStatementContext_t* context);
void zen_AssertStatementContext_getChildren(zen_AssertStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * BreakStatementContext                                                       *
 *******************************************************************************/

struct zen_BreakStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_BreakStatementContext_t zen_BreakStatementContext_t;

zen_BreakStatementContext_t* zen_BreakStatementContext_new(zen_ASTNode_t* node);
void zen_BreakStatementContext_delete(zen_BreakStatementContext_t* context);
void zen_BreakStatementContext_getChildren(zen_BreakStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ContinueStatementContext                                                    *
 *******************************************************************************/

struct zen_ContinueStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_ContinueStatementContext_t zen_ContinueStatementContext_t;

zen_ContinueStatementContext_t* zen_ContinueStatementContext_new(zen_ASTNode_t* node);
void zen_ContinueStatementContext_delete(zen_ContinueStatementContext_t* context);
void zen_ContinueStatementContext_getChildren(zen_ContinueStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ReturnStatementContext                                                      *
 *******************************************************************************/

struct zen_ReturnStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_ReturnStatementContext_t zen_ReturnStatementContext_t;

zen_ReturnStatementContext_t* zen_ReturnStatementContext_new(zen_ASTNode_t* node);
void zen_ReturnStatementContext_delete(zen_ReturnStatementContext_t* context);
void zen_ReturnStatementContext_getChildren(zen_ReturnStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ThrowStatementContext                                                       *
 *******************************************************************************/

struct zen_ThrowStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_ThrowStatementContext_t zen_ThrowStatementContext_t;

zen_ThrowStatementContext_t* zen_ThrowStatementContext_new(zen_ASTNode_t* node);
void zen_ThrowStatementContext_delete(zen_ThrowStatementContext_t* context);
void zen_ThrowStatementContext_getChildren(zen_ThrowStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * CompoundStatementContext                                                    *
 *******************************************************************************/

struct zen_CompoundStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statement;
};

typedef struct zen_CompoundStatementContext_t zen_CompoundStatementContext_t;

zen_CompoundStatementContext_t* zen_CompoundStatementContext_new(zen_ASTNode_t* node);
void zen_CompoundStatementContext_delete(zen_CompoundStatementContext_t* context);
void zen_CompoundStatementContext_getChildren(zen_CompoundStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * IfStatementContext                                                          *
 *******************************************************************************/

struct zen_IfStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_ifClause;
    zen_ArrayList_t* m_elseIfClauses; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_elseClause;
};

typedef struct zen_IfStatementContext_t zen_IfStatementContext_t;

zen_IfStatementContext_t* zen_IfStatementContext_new(zen_ASTNode_t* node);
void zen_IfStatementContext_delete(zen_IfStatementContext_t* context);
void zen_IfStatementContext_getChildren(zen_IfStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * IfClauseContext                                                             *
 *******************************************************************************/

struct zen_IfClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_IfClauseContext_t zen_IfClauseContext_t;

zen_IfClauseContext_t* zen_IfClauseContext_new(zen_ASTNode_t* node);
void zen_IfClauseContext_delete(zen_IfClauseContext_t* context);
void zen_IfClauseContext_getChildren(zen_IfClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ElseIfClauseContext                                                         *
 *******************************************************************************/

struct zen_ElseIfClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_ElseIfClauseContext_t zen_ElseIfClauseContext_t;

zen_ElseIfClauseContext_t* zen_ElseIfClauseContext_new(zen_ASTNode_t* node);
void zen_ElseIfClauseContext_delete(zen_ElseIfClauseContext_t* context);
void zen_ElseIfClauseContext_getChildren(zen_ElseIfClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ElseClauseContext                                                           *
 *******************************************************************************/

struct zen_ElseClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_ElseClauseContext_t zen_ElseClauseContext_t;

zen_ElseClauseContext_t* zen_ElseClauseContext_new(zen_ASTNode_t* node);
void zen_ElseClauseContext_delete(zen_ElseClauseContext_t* context);
void zen_ElseClauseContext_getChildren(zen_ElseClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * IterativeStatementContext                                                   *
 *******************************************************************************/

struct zen_IterativeStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_labelClause;
    zen_ASTNode_t* m_statement;
};

typedef struct zen_IterativeStatementContext_t zen_IterativeStatementContext_t;

zen_IterativeStatementContext_t* zen_IterativeStatementContext_new(zen_ASTNode_t* node);
void zen_IterativeStatementContext_delete(zen_IterativeStatementContext_t* context);
void zen_IterativeStatementContext_getChildren(zen_IterativeStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * LabelClauseContext                                                          *
 *******************************************************************************/

struct zen_LabelClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_LabelClauseContext_t zen_LabelClauseContext_t;

zen_LabelClauseContext_t* zen_LabelClauseContext_new(zen_ASTNode_t* node);
void zen_LabelClauseContext_delete(zen_LabelClauseContext_t* context);
void zen_LabelClauseContext_getChildren(zen_LabelClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * WhileStatementContext                                                       *
 *******************************************************************************/

struct zen_WhileStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
    zen_ASTNode_t* m_elseClause;
};

typedef struct zen_WhileStatementContext_t zen_WhileStatementContext_t;

zen_WhileStatementContext_t* zen_WhileStatementContext_new(zen_ASTNode_t* node);
void zen_WhileStatementContext_delete(zen_WhileStatementContext_t* context);
void zen_WhileStatementContext_getChildren(zen_WhileStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ForStatementContext                                                         *
 *******************************************************************************/

struct zen_ForStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_forParameters;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
    zen_ASTNode_t* m_elseClause;
};

typedef struct zen_ForStatementContext_t zen_ForStatementContext_t;

zen_ForStatementContext_t* zen_ForStatementContext_new(zen_ASTNode_t* node);
void zen_ForStatementContext_delete(zen_ForStatementContext_t* context);
void zen_ForStatementContext_getChildren(zen_ForStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ForParametersContext                                                        *
 *******************************************************************************/

struct zen_ForParametersContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_declarator;
    zen_ArrayList_t* m_identifiers;
};

typedef struct zen_ForParametersContext_t zen_ForParametersContext_t;

zen_ForParametersContext_t* zen_ForParametersContext_new(zen_ASTNode_t* node);
void zen_ForParametersContext_delete(zen_ForParametersContext_t* context);
void zen_ForParametersContext_getChildren(zen_ForParametersContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * TryStatementContext                                                         *
 *******************************************************************************/

struct zen_TryStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_tryClause;
    zen_ArrayList_t* m_catchClauses; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_finallyClause;
};

typedef struct zen_TryStatementContext_t zen_TryStatementContext_t;

zen_TryStatementContext_t* zen_TryStatementContext_new(zen_ASTNode_t* node);
void zen_TryStatementContext_delete(zen_TryStatementContext_t* context);
void zen_TryStatementContext_getChildren(zen_TryStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * TryClauseContext                                                            *
 *******************************************************************************/

struct zen_TryClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_TryClauseContext_t zen_TryClauseContext_t;

zen_TryClauseContext_t* zen_TryClauseContext_new(zen_ASTNode_t* node);
void zen_TryClauseContext_delete(zen_TryClauseContext_t* context);
void zen_TryClauseContext_getChildren(zen_TryClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * CatchClauseContext                                                          *
 *******************************************************************************/

struct zen_CatchClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_catchFilter;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_CatchClauseContext_t zen_CatchClauseContext_t;

zen_CatchClauseContext_t* zen_CatchClauseContext_new(zen_ASTNode_t* node);
void zen_CatchClauseContext_delete(zen_CatchClauseContext_t* context);
void zen_CatchClauseContext_getChildren(zen_CatchClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * CatchFilterContext                                                          *
 *******************************************************************************/

struct zen_CatchFilterContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_typeNames; /* <zen_ASTNode_t*> */
};

typedef struct zen_CatchFilterContext_t zen_CatchFilterContext_t;

zen_CatchFilterContext_t* zen_CatchFilterContext_new(zen_ASTNode_t* node);
void zen_CatchFilterContext_delete(zen_CatchFilterContext_t* context);
void zen_CatchFilterContext_getChildren(zen_CatchFilterContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * TypeNameContext                                                             *
 *******************************************************************************/

struct zen_TypeNameContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_identifiers;
};

typedef struct zen_TypeNameContext_t zen_TypeNameContext_t;

zen_TypeNameContext_t* zen_TypeNameContext_new(zen_ASTNode_t* node);
void zen_TypeNameContext_delete(zen_TypeNameContext_t* context);
void zen_TypeNameContext_getChildren(zen_TypeNameContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * FinallyClauseContext                                                        *
 *******************************************************************************/

struct zen_FinallyClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_FinallyClauseContext_t zen_FinallyClauseContext_t;

zen_FinallyClauseContext_t* zen_FinallyClauseContext_new(zen_ASTNode_t* node);
void zen_FinallyClauseContext_delete(zen_FinallyClauseContext_t* context);
void zen_FinallyClauseContext_getChildren(zen_FinallyClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * SynchronizeStatementContext                                                 *
 *******************************************************************************/

struct zen_SynchronizeStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_SynchronizeStatementContext_t zen_SynchronizeStatementContext_t;

zen_SynchronizeStatementContext_t* zen_SynchronizeStatementContext_new(zen_ASTNode_t* node);
void zen_SynchronizeStatementContext_delete(zen_SynchronizeStatementContext_t* context);
void zen_SynchronizeStatementContext_getChildren(zen_SynchronizeStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * WithStatementContext                                                        *
 *******************************************************************************/

struct zen_WithStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_WithStatementContext_t zen_WithStatementContext_t;

zen_WithStatementContext_t* zen_WithStatementContext_new(zen_ASTNode_t* node);
void zen_WithStatementContext_delete(zen_WithStatementContext_t* context);
void zen_WithStatementContext_getChildren(zen_WithStatementContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ClassDeclarationContext                                                     *
 *******************************************************************************/

struct zen_ClassDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_classExtendsClause;
    zen_ASTNode_t* m_classSuite;
};

typedef struct zen_ClassDeclarationContext_t zen_ClassDeclarationContext_t;

zen_ClassDeclarationContext_t* zen_ClassDeclarationContext_new(zen_ASTNode_t* node);
void zen_ClassDeclarationContext_delete(zen_ClassDeclarationContext_t* context);
void zen_ClassDeclarationContext_getChildren(zen_ClassDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ClassExtendsClauseContext                                                        *
 *******************************************************************************/

struct zen_ClassExtendsClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_typeNames; /* <zen_ASTNode_t*> */
};

typedef struct zen_ClassExtendsClauseContext_t zen_ClassExtendsClauseContext_t;

zen_ClassExtendsClauseContext_t* zen_ClassExtendsClauseContext_new(zen_ASTNode_t* node);
void zen_ClassExtendsClauseContext_delete(zen_ClassExtendsClauseContext_t* context);
void zen_ClassExtendsClauseContext_getChildren(zen_ClassExtendsClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ClassSuiteContext                                                           *
 *******************************************************************************/

struct zen_ClassSuiteContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_classMembers; /* <zen_ASTNode_t*> */
};

typedef struct zen_ClassSuiteContext_t zen_ClassSuiteContext_t;

zen_ClassSuiteContext_t* zen_ClassSuiteContext_new(zen_ASTNode_t* node);
void zen_ClassSuiteContext_delete(zen_ClassSuiteContext_t* context);
void zen_ClassSuiteContext_getChildren(zen_ClassSuiteContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ClassMemberContext                                                          *
 *******************************************************************************/

struct zen_ClassMemberContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_declaration;
};

typedef struct zen_ClassMemberContext_t zen_ClassMemberContext_t;

zen_ClassMemberContext_t* zen_ClassMemberContext_new(zen_ASTNode_t* node);
void zen_ClassMemberContext_delete(zen_ClassMemberContext_t* context);
void zen_ClassMemberContext_getChildren(zen_ClassMemberContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ConstructorDeclarationContext                                               *
 *******************************************************************************/

// struct zen_ConstructorDeclarationContext_t {
//     zen_ASTNode_t* m_node;
//     zen_ASTNode_t* m_identifier;
//     zen_ASTNode_t* m_functionParameters;
//     zen_ASTNode_t* m_statementSuite;
// };

// typedef struct zen_ConstructorDeclarationContext_t zen_ConstructorDeclarationContext_t;

// zen_ConstructorDeclarationContext_t* zen_ConstructorDeclarationContext_new(zen_ASTNode_t* node);
// void zen_ConstructorDeclarationContext_delete(zen_ConstructorDeclarationContext_t* context);
// void zen_ConstructorDeclarationContext_getChildren(zen_ConstructorDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * EnumerationDeclarationContext                                               *
 *******************************************************************************/

struct zen_EnumerationDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_enumerationBaseClause;
    zen_ASTNode_t* m_enumerationSuite;
};

typedef struct zen_EnumerationDeclarationContext_t zen_EnumerationDeclarationContext_t;

zen_EnumerationDeclarationContext_t* zen_EnumerationDeclarationContext_new(zen_ASTNode_t* node);
void zen_EnumerationDeclarationContext_delete(zen_EnumerationDeclarationContext_t* context);
void zen_EnumerationDeclarationContext_getChildren(zen_EnumerationDeclarationContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * EnumerationBaseClauseContext                                                *
 *******************************************************************************/

struct zen_EnumerationBaseClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_typeName;
};

typedef struct zen_EnumerationBaseClauseContext_t zen_EnumerationBaseClauseContext_t;

zen_EnumerationBaseClauseContext_t* zen_EnumerationBaseClauseContext_new(zen_ASTNode_t* node);
void zen_EnumerationBaseClauseContext_delete(zen_EnumerationBaseClauseContext_t* context);
void zen_EnumerationBaseClauseContext_getChildren(zen_EnumerationBaseClauseContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * EnumerationSuiteContext                                                     *
 *******************************************************************************/

struct zen_EnumerationSuiteContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_enumerates; /* <zen_ASTNode_t*> */
};

typedef struct zen_EnumerationSuiteContext_t zen_EnumerationSuiteContext_t;

zen_EnumerationSuiteContext_t* zen_EnumerationSuiteContext_new(zen_ASTNode_t* node);
void zen_EnumerationSuiteContext_delete(zen_EnumerationSuiteContext_t* context);
void zen_EnumerationSuiteContext_getChildren(zen_EnumerationSuiteContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * EnumerateContext                                                            *
 *******************************************************************************/

struct zen_EnumerateContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_functionArguments;
};

typedef struct zen_EnumerateContext_t zen_EnumerateContext_t;

zen_EnumerateContext_t* zen_EnumerateContext_new(zen_ASTNode_t* node);
void zen_EnumerateContext_delete(zen_EnumerateContext_t* context);
void zen_EnumerateContext_getChildren(zen_EnumerateContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ExpressionsContext                                                          *
 *******************************************************************************/

struct zen_ExpressionsContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_expressions; /* <zen_ASTNode_t*> */
};

typedef struct zen_ExpressionsContext_t zen_ExpressionsContext_t;

zen_ExpressionsContext_t* zen_ExpressionsContext_new(zen_ASTNode_t* node);
void zen_ExpressionsContext_delete(zen_ExpressionsContext_t* context);
void zen_ExpressionsContext_getChildren(zen_ExpressionsContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ExpressionContext                                                           *
 *******************************************************************************/

struct zen_ExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_assignmentExpression;
};

typedef struct zen_ExpressionContext_t zen_ExpressionContext_t;

zen_ExpressionContext_t* zen_ExpressionContext_new(zen_ASTNode_t* node);
void zen_ExpressionContext_delete(zen_ExpressionContext_t* context);
void zen_ExpressionContext_getChildren(zen_ExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AssignmentExpressionContext                                                 *
 *******************************************************************************/

struct zen_AssignmentExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_conditionalExpression;
    zen_ASTNode_t* m_assignmentOperator;
    zen_ASTNode_t* m_assignmentExpression;
};

typedef struct zen_AssignmentExpressionContext_t zen_AssignmentExpressionContext_t;

zen_AssignmentExpressionContext_t* zen_AssignmentExpressionContext_new(zen_ASTNode_t* node);
void zen_AssignmentExpressionContext_delete(zen_AssignmentExpressionContext_t* context);
void zen_AssignmentExpressionContext_getChildren(zen_AssignmentExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ConditionalExpressionContext                                                *
 *******************************************************************************/

struct zen_ConditionalExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_logicalOrExpression;
    zen_ASTNode_t* m_thenExpression;
    zen_ASTNode_t* m_elseExpression;
};

typedef struct zen_ConditionalExpressionContext_t zen_ConditionalExpressionContext_t;

zen_ConditionalExpressionContext_t* zen_ConditionalExpressionContext_new(zen_ASTNode_t* node);
void zen_ConditionalExpressionContext_delete(zen_ConditionalExpressionContext_t* context);
void zen_ConditionalExpressionContext_getChildren(zen_ConditionalExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * LogicalOrExpressionContext                                                  *
 *******************************************************************************/

struct zen_LogicalOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_logicalAndExpression;
    zen_ASTNode_t* m_logicalOrExpression;
};

typedef struct zen_LogicalOrExpressionContext_t zen_LogicalOrExpressionContext_t;

zen_LogicalOrExpressionContext_t* zen_LogicalOrExpressionContext_new(zen_ASTNode_t* node);
void zen_LogicalOrExpressionContext_delete(zen_LogicalOrExpressionContext_t* context);
void zen_LogicalOrExpressionContext_getChildren(zen_LogicalOrExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * LogicalAndExpressionContext                                                 *
 *******************************************************************************/

struct zen_LogicalAndExpressionContext_t {
    zen_ASTNode_t* m_node;

    /**
     * The expression on the left of the operator.
     */
    zen_ASTNode_t* m_inclusiveOrExpression;

    /**
     * The logicalAnd expression on the right of the operator.
     */
    zen_ASTNode_t* m_logicalAndExpression;
};

typedef struct zen_LogicalAndExpressionContext_t zen_LogicalAndExpressionContext_t;

zen_LogicalAndExpressionContext_t* zen_LogicalAndExpressionContext_new(zen_ASTNode_t* node);
void zen_LogicalAndExpressionContext_delete(zen_LogicalAndExpressionContext_t* context);
void zen_LogicalAndExpressionContext_getChildren(zen_LogicalAndExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * InclusiveOrExpressionContext                                                *
 *******************************************************************************/

struct zen_InclusiveOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    /**
     * The expression on the left of the operator.
     */
    zen_ASTNode_t* m_exclusiveOrExpression;

    /**
     * The exclusiveOrExpressions on the right of the operator.
     */
    zen_ASTNode_t* m_inclusiveOrExpression;
};

typedef struct zen_InclusiveOrExpressionContext_t zen_InclusiveOrExpressionContext_t;

zen_InclusiveOrExpressionContext_t* zen_InclusiveOrExpressionContext_new(zen_ASTNode_t* node);
void zen_InclusiveOrExpressionContext_delete(zen_InclusiveOrExpressionContext_t* context);
void zen_InclusiveOrExpressionContext_getChildren(zen_InclusiveOrExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ExclusiveOrExpressionContext                                                *
 *******************************************************************************/

struct zen_ExclusiveOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_andExpression;
    zen_ASTNode_t* m_exclusiveOrExpression;
};

typedef struct zen_ExclusiveOrExpressionContext_t zen_ExclusiveOrExpressionContext_t;

zen_ExclusiveOrExpressionContext_t* zen_ExclusiveOrExpressionContext_new(zen_ASTNode_t* node);
void zen_ExclusiveOrExpressionContext_delete(zen_ExclusiveOrExpressionContext_t* context);
void zen_ExclusiveOrExpressionContext_getChildren(zen_ExclusiveOrExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AndExpressionContext                                                        *
 *******************************************************************************/

struct zen_AndExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_equalityExpression;
    zen_ASTNode_t* m_andExpression;
};

typedef struct zen_AndExpressionContext_t zen_AndExpressionContext_t;

zen_AndExpressionContext_t* zen_AndExpressionContext_new(zen_ASTNode_t* node);
void zen_AndExpressionContext_delete(zen_AndExpressionContext_t* context);
void zen_AndExpressionContext_getChildren(zen_AndExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * EqualityExpressionContext                                                   *
 *******************************************************************************/

struct zen_EqualityExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_relationalExpression;
    zen_ASTNode_t* m_equalityOperator;
    zen_ASTNode_t* m_equalityExpression;
};

typedef struct zen_EqualityExpressionContext_t zen_EqualityExpressionContext_t;

zen_EqualityExpressionContext_t* zen_EqualityExpressionContext_new(zen_ASTNode_t* node);
void zen_EqualityExpressionContext_delete(zen_EqualityExpressionContext_t* context);
void zen_EqualityExpressionContext_getChildren(zen_EqualityExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * RelationalExpressionContext                                                 *
 *******************************************************************************/

struct zen_RelationalExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_shiftExpression;
    zen_ASTNode_t* m_relationalOperator;
    zen_ASTNode_t* m_relationalExpression;
};

typedef struct zen_RelationalExpressionContext_t zen_RelationalExpressionContext_t;

zen_RelationalExpressionContext_t* zen_RelationalExpressionContext_new(zen_ASTNode_t* node);
void zen_RelationalExpressionContext_delete(zen_RelationalExpressionContext_t* context);
void zen_RelationalExpressionContext_getChildren(zen_RelationalExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ShiftExpressionContext                                                      *
 *******************************************************************************/

struct zen_ShiftExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_additiveExpression;
    zen_ASTNode_t* m_shiftOperator;
    zen_ASTNode_t* m_shiftExpression;
};

typedef struct zen_ShiftExpressionContext_t zen_ShiftExpressionContext_t;

zen_ShiftExpressionContext_t* zen_ShiftExpressionContext_new(zen_ASTNode_t* node);
void zen_ShiftExpressionContext_delete(zen_ShiftExpressionContext_t* context);
void zen_ShiftExpressionContext_getChildren(zen_ShiftExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * AdditiveExpressionContext                                                   *
 *******************************************************************************/

struct zen_AdditiveExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_multiplicativeExpression;
    zen_ASTNode_t* m_additiveOperator;
    zen_ASTNode_t* m_additiveExpression;
};

typedef struct zen_AdditiveExpressionContext_t zen_AdditiveExpressionContext_t;

zen_AdditiveExpressionContext_t* zen_AdditiveExpressionContext_new(zen_ASTNode_t* node);
void zen_AdditiveExpressionContext_delete(zen_AdditiveExpressionContext_t* context);
void zen_AdditiveExpressionContext_getChildren(zen_AdditiveExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * MultiplicativeExpressionContext                                             *
 *******************************************************************************/

struct zen_MultiplicativeExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_unaryExpression;
    zen_ASTNode_t* m_multiplicativeOperator;
    zen_ASTNode_t* m_multiplicativeExpression;
};

typedef struct zen_MultiplicativeExpressionContext_t zen_MultiplicativeExpressionContext_t;

zen_MultiplicativeExpressionContext_t* zen_MultiplicativeExpressionContext_new(zen_ASTNode_t* node);
void zen_MultiplicativeExpressionContext_delete(zen_MultiplicativeExpressionContext_t* context);
void zen_MultiplicativeExpressionContext_getChildren(zen_MultiplicativeExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * UnaryExpressionContext                                                      *
 *******************************************************************************/

struct zen_UnaryExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_unaryOperator;
    zen_ASTNode_t* m_unaryExpression;
    zen_ASTNode_t* m_postfixExpression;
};

typedef struct zen_UnaryExpressionContext_t zen_UnaryExpressionContext_t;

zen_UnaryExpressionContext_t* zen_UnaryExpressionContext_new(zen_ASTNode_t* node);
void zen_UnaryExpressionContext_delete(zen_UnaryExpressionContext_t* context);
void zen_UnaryExpressionContext_getChildren(zen_UnaryExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * PostfixExpressionContext                                                    *
 *******************************************************************************/

struct zen_PostfixExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_primaryExpression;
    zen_ArrayList_t* m_postfixParts; /* <zen_ASTNode_t*> */
};

typedef struct zen_PostfixExpressionContext_t zen_PostfixExpressionContext_t;

zen_PostfixExpressionContext_t* zen_PostfixExpressionContext_new(zen_ASTNode_t* node);
void zen_PostfixExpressionContext_delete(zen_PostfixExpressionContext_t* context);
void zen_PostfixExpressionContext_getChildren(zen_PostfixExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * SubscriptContext                                                            *
 *******************************************************************************/

struct zen_SubscriptContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_SubscriptContext_t zen_SubscriptContext_t;

zen_SubscriptContext_t* zen_SubscriptContext_new(zen_ASTNode_t* node);
void zen_SubscriptContext_delete(zen_SubscriptContext_t* context);
void zen_SubscriptContext_getChildren(zen_SubscriptContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * FunctionArgumentsContext                                                    *
 *******************************************************************************/

struct zen_FunctionArgumentsContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
};

typedef struct zen_FunctionArgumentsContext_t zen_FunctionArgumentsContext_t;

zen_FunctionArgumentsContext_t* zen_FunctionArgumentsContext_new(zen_ASTNode_t* node);
void zen_FunctionArgumentsContext_delete(zen_FunctionArgumentsContext_t* context);
void zen_FunctionArgumentsContext_getChildren(zen_FunctionArgumentsContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * MemberAccessContext                                                         *
 *******************************************************************************/

struct zen_MemberAccessContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_MemberAccessContext_t zen_MemberAccessContext_t;

zen_MemberAccessContext_t* zen_MemberAccessContext_new(zen_ASTNode_t* node);
void zen_MemberAccessContext_delete(zen_MemberAccessContext_t* context);
void zen_MemberAccessContext_getChildren(zen_MemberAccessContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * PostfixOperatorContext                                                      *
 *******************************************************************************/

struct zen_PostfixOperatorContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_postfixOperator;
};

typedef struct zen_PostfixOperatorContext_t zen_PostfixOperatorContext_t;

zen_PostfixOperatorContext_t* zen_PostfixOperatorContext_new(zen_ASTNode_t* node);
void zen_PostfixOperatorContext_delete(zen_PostfixOperatorContext_t* context);
void zen_PostfixOperatorContext_getChildren(zen_PostfixOperatorContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * PrimaryExpressionContext                                                    *
 *******************************************************************************/

struct zen_PrimaryExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_PrimaryExpressionContext_t zen_PrimaryExpressionContext_t;

zen_PrimaryExpressionContext_t* zen_PrimaryExpressionContext_new(zen_ASTNode_t* node);
void zen_PrimaryExpressionContext_delete(zen_PrimaryExpressionContext_t* context);
void zen_PrimaryExpressionContext_getChildren(zen_PrimaryExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * MapExpressionContext                                                        *
 *******************************************************************************/

struct zen_MapExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_mapEntries;
};

typedef struct zen_MapExpressionContext_t zen_MapExpressionContext_t;

zen_MapExpressionContext_t* zen_MapExpressionContext_new(zen_ASTNode_t* node);
void zen_MapExpressionContext_delete(zen_MapExpressionContext_t* context);
void zen_MapExpressionContext_getChildren(zen_MapExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * MapEntriesContext                                                           *
 *******************************************************************************/

struct zen_MapEntriesContext_t {
    zen_ASTNode_t* m_node;
    zen_ArrayList_t* m_mapEntries;
};

typedef struct zen_MapEntriesContext_t zen_MapEntriesContext_t;

zen_MapEntriesContext_t* zen_MapEntriesContext_new(zen_ASTNode_t* node);
void zen_MapEntriesContext_delete(zen_MapEntriesContext_t* context);
void zen_MapEntriesContext_getChildren(zen_MapEntriesContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * MapEntryContext                                                             *
 *******************************************************************************/

struct zen_MapEntryContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_keyExpression;
    zen_ASTNode_t* m_valueExpression;
};

typedef struct zen_MapEntryContext_t zen_MapEntryContext_t;

zen_MapEntryContext_t* zen_MapEntryContext_new(zen_ASTNode_t* node);
void zen_MapEntryContext_delete(zen_MapEntryContext_t* context);
void zen_MapEntryContext_getChildren(zen_MapEntryContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * ListExpressionContext                                                       *
 *******************************************************************************/

struct zen_ListExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
};

typedef struct zen_ListExpressionContext_t zen_ListExpressionContext_t;

zen_ListExpressionContext_t* zen_ListExpressionContext_new(zen_ASTNode_t* node);
void zen_ListExpressionContext_delete(zen_ListExpressionContext_t* context);
void zen_ListExpressionContext_getChildren(zen_ListExpressionContext_t* context, zen_ArrayList_t* children);

/*******************************************************************************
 * NewExpressionContext                                                        *
 *******************************************************************************/

struct zen_NewExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_typeName;
    zen_ASTNode_t* m_functionArguments;
};

typedef struct zen_NewExpressionContext_t zen_NewExpressionContext_t;

zen_NewExpressionContext_t* zen_NewExpressionContext_new(zen_ASTNode_t* node);
void zen_NewExpressionContext_delete(zen_NewExpressionContext_t* context);
void zen_NewExpressionContext_getChildren(zen_NewExpressionContext_t* context, zen_ArrayList_t* children);

#endif /* ZEN_CONTEXT_H */