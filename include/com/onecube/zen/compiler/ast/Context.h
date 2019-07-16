// Friday, November 24, 2017

#ifndef COM_ONECUBE_ZEN_COMPILER_COMPILER_CONTEXT_H
#define COM_ONECUBE_ZEN_COMPILER_COMPILER_CONTEXT_H

#include <jtk/collection/list/ArrayList.h>

#include <com/onecube/zen/Configuration.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>

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

/**
 * @class CompilationUnitContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CompilationUnitContext_t {
    /**
     * The node which represents this rule context within the AST.
     */
    zen_ASTNode_t* m_node;
	jtk_ArrayList_t* m_importDeclarations; /* <zen_ASTNode_t*> */
	jtk_ArrayList_t* m_annotatedComponentDeclarations; /* <zen_ASTNode_t*> */
};

typedef struct zen_CompilationUnitContext_t zen_CompilationUnitContext_t;

zen_CompilationUnitContext_t* zen_CompilationUnitContext_new(zen_ASTNode_t* node);
void zen_CompilationUnitContext_delete(zen_CompilationUnitContext_t* context);
void zen_CompilationUnitContext_getChildren(zen_CompilationUnitContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ImportDeclarationContext                                                    *
 *******************************************************************************/

/**
 * @class ImportDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ImportDeclarationContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_identifiers; /* <zen_ASTNode_t*> */
    bool m_wildcard;
};

typedef struct zen_ImportDeclarationContext_t zen_ImportDeclarationContext_t;

zen_ImportDeclarationContext_t* zen_ImportDeclarationContext_new(zen_ASTNode_t* node);
void zen_ImportDeclarationContext_delete(zen_ImportDeclarationContext_t* context);
void zen_ImportDeclarationContext_getChildren(zen_ImportDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AnnotatedComponentDeclarationContext                                        *
 *******************************************************************************/

/**
 * @class AnnotatedComponentDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotatedComponentDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_annotations;
    zen_ASTNode_t* m_componentDeclaration;
};

typedef struct zen_AnnotatedComponentDeclarationContext_t zen_AnnotatedComponentDeclarationContext_t;

zen_AnnotatedComponentDeclarationContext_t* zen_AnnotatedComponentDeclarationContext_new(zen_ASTNode_t* node);
void zen_AnnotatedComponentDeclarationContext_delete(zen_AnnotatedComponentDeclarationContext_t* context);
void zen_AnnotatedComponentDeclarationContext_getChildren(zen_AnnotatedComponentDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AnnotationsContext                                                          *
 *******************************************************************************/

/**
 * @class AnnotationsContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationsContext_t {
    zen_ASTNode_t* m_node;
	jtk_ArrayList_t* m_annotations; /* <zen_ASTNode_t*> */
};

typedef struct zen_AnnotationsContext_t zen_AnnotationsContext_t;

zen_AnnotationsContext_t* zen_AnnotationsContext_new(zen_ASTNode_t* node);
void zen_AnnotationsContext_delete(zen_AnnotationsContext_t* context);
void zen_AnnotationsContext_getChildren(zen_AnnotationsContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AnnotationContext                                                           *
 *******************************************************************************/

/**
 * @class AnnotationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationContext_t {
    zen_ASTNode_t* m_node;
	zen_ASTNode_t* m_annotationType;
	jtk_ArrayList_t* m_annotationAttributes; /* <zen_ASTNode_t*> */
};

typedef struct zen_AnnotationContext_t zen_AnnotationContext_t;

zen_AnnotationContext_t* zen_AnnotationContext_new(zen_ASTNode_t* node);
void zen_AnnotationContext_delete(zen_AnnotationContext_t* context);
void zen_AnnotationContext_getChildren(zen_AnnotationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AnnotationTypeContext                                                       *
 *******************************************************************************/

/**
 * @class AnnotationTypeContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationTypeContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_identifiers; /* <zen_ASTNode_t*> */
};

typedef struct zen_AnnotationTypeContext_t zen_AnnotationTypeContext_t;

zen_AnnotationTypeContext_t* zen_AnnotationTypeContext_new(zen_ASTNode_t* node);
void zen_AnnotationTypeContext_delete(zen_AnnotationTypeContext_t* context);
void zen_AnnotationTypeContext_getChildren(zen_AnnotationTypeContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AnnotationAttributeContext                                                  *
 *******************************************************************************/

/**
 * @class AnnotationAttributeContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AnnotationAttributeContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_literal;
};

typedef struct zen_AnnotationAttributeContext_t zen_AnnotationAttributeContext_t;

zen_AnnotationAttributeContext_t* zen_AnnotationAttributeContext_new(zen_ASTNode_t* node);
void zen_AnnotationAttributeContext_delete(zen_AnnotationAttributeContext_t* context);
void zen_AnnotationAttributeContext_getChildren(zen_AnnotationAttributeContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ComponentDeclarationContext                                                 *
 *******************************************************************************/

/**
 * @class ComponentDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ComponentDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_component;
};

typedef struct zen_ComponentDeclarationContext_t zen_ComponentDeclarationContext_t;

zen_ComponentDeclarationContext_t* zen_ComponentDeclarationContext_new(zen_ASTNode_t* node);
void zen_ComponentDeclarationContext_delete(zen_ComponentDeclarationContext_t* context);
void zen_ComponentDeclarationContext_getChildren(zen_ComponentDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * FunctionDeclarationContext                                                  *
 *******************************************************************************/

/**
 * @class FunctionDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_functionParameters;
    zen_ASTNode_t* m_functionBody;
};

typedef struct zen_FunctionDeclarationContext_t zen_FunctionDeclarationContext_t;

zen_FunctionDeclarationContext_t* zen_FunctionDeclarationContext_new(zen_ASTNode_t* node);
void zen_FunctionDeclarationContext_delete(zen_FunctionDeclarationContext_t* context);
void zen_FunctionDeclarationContext_getChildren(zen_FunctionDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * FunctionParametersContext                                                   *
 *******************************************************************************/

/**
 * @class FunctionParametersContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionParametersContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_fixedParameters; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_variableParameter;
};

typedef struct zen_FunctionParametersContext_t zen_FunctionParametersContext_t;

zen_FunctionParametersContext_t* zen_FunctionParametersContext_new(zen_ASTNode_t* node);
void zen_FunctionParametersContext_delete(zen_FunctionParametersContext_t* context);
void zen_FunctionParametersContext_getChildren(zen_FunctionParametersContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * FunctionBodyContext                                                         *
 *******************************************************************************/

/**
 * @class FunctionBodyContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FunctionBodyContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_FunctionBodyContext_t zen_FunctionBodyContext_t;

zen_FunctionBodyContext_t* zen_FunctionBodyContext_new(zen_ASTNode_t* node);
void zen_FunctionBodyContext_delete(zen_FunctionBodyContext_t* context);
void zen_FunctionBodyContext_getChildren(zen_FunctionBodyContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * StatementSuiteContext                                                       *
 *******************************************************************************/

/**
 * @class StatementSuiteContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_StatementSuiteContext_t {
    zen_ASTNode_t* m_node;
    /* In order to help the users read code easily, the simple statements were
     * removed from statement suites. Previously, a statement suite was either
     * a simple statement or a block consisting of a newline at the begining,
     * indentation and dedentation.
     */
    // zen_ASTNode_t* m_simpleStatement;
    jtk_ArrayList_t* m_statements;
    int32_t m_scope;
};

typedef struct zen_StatementSuiteContext_t zen_StatementSuiteContext_t;

zen_StatementSuiteContext_t* zen_StatementSuiteContext_new(zen_ASTNode_t* node);
void zen_StatementSuiteContext_delete(zen_StatementSuiteContext_t* context);
void zen_StatementSuiteContext_getChildren(zen_StatementSuiteContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * SimpleStatementContext                                                      *
 *******************************************************************************/

/**
 * @class SimpleStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SimpleStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statement;
};

typedef struct zen_SimpleStatementContext_t zen_SimpleStatementContext_t;

zen_SimpleStatementContext_t* zen_SimpleStatementContext_new(zen_ASTNode_t* node);
void zen_SimpleStatementContext_delete(zen_SimpleStatementContext_t* context);
void zen_SimpleStatementContext_getChildren(zen_SimpleStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * StatementContext                                                            *
 *******************************************************************************/

/**
 * @class StatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_StatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_simpleStatement;
    zen_ASTNode_t* m_compoundStatement;
};

typedef struct zen_StatementContext_t zen_StatementContext_t;

zen_StatementContext_t* zen_StatementContext_new(zen_ASTNode_t* node);
void zen_StatementContext_delete(zen_StatementContext_t* context);
void zen_StatementContext_getChildren(zen_StatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * EmptyStatementContext                                                       *
 *******************************************************************************/

/**
 * @class EmptyStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EmptyStatementContext_t {
    zen_ASTNode_t* m_node;
};

typedef struct zen_EmptyStatementContext_t zen_EmptyStatementContext_t;

zen_EmptyStatementContext_t* zen_EmptyStatementContext_new(zen_ASTNode_t* node);
void zen_EmptyStatementContext_delete(zen_EmptyStatementContext_t* context);
void zen_EmptyStatementContext_getChildren(zen_EmptyStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * VariableDeclarationContext                                                  *
 *******************************************************************************/

/**
 * @class VariableDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VariableDeclarationContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_variableDeclarators;
};

typedef struct zen_VariableDeclarationContext_t zen_VariableDeclarationContext_t;

zen_VariableDeclarationContext_t* zen_VariableDeclarationContext_new(zen_ASTNode_t* node);
void zen_VariableDeclarationContext_delete(zen_VariableDeclarationContext_t* context);
void zen_VariableDeclarationContext_getChildren(zen_VariableDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * VariableDeclaratorContext                                                   *
 *******************************************************************************/

/**
 * @class VariableDeclaratorContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_VariableDeclaratorContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_VariableDeclaratorContext_t zen_VariableDeclaratorContext_t;

zen_VariableDeclaratorContext_t* zen_VariableDeclaratorContext_new(zen_ASTNode_t* node);
void zen_VariableDeclaratorContext_delete(zen_VariableDeclaratorContext_t* context);
void zen_VariableDeclaratorContext_getChildren(zen_VariableDeclaratorContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ConstantDeclarationContext                                                  *
 *******************************************************************************/

/**
 * @class ConstantDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConstantDeclarationContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_constantDeclarators;
};

typedef struct zen_ConstantDeclarationContext_t zen_ConstantDeclarationContext_t;

zen_ConstantDeclarationContext_t* zen_ConstantDeclarationContext_new(zen_ASTNode_t* node);
void zen_ConstantDeclarationContext_delete(zen_ConstantDeclarationContext_t* context);
void zen_ConstantDeclarationContext_getChildren(zen_ConstantDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ConstantDeclaratorContext                                                   *
 *******************************************************************************/

/**
 * @class ConstantDeclaratorContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConstantDeclaratorContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_ConstantDeclaratorContext_t zen_ConstantDeclaratorContext_t;

zen_ConstantDeclaratorContext_t* zen_ConstantDeclaratorContext_new(zen_ASTNode_t* node);
void zen_ConstantDeclaratorContext_delete(zen_ConstantDeclaratorContext_t* context);
void zen_ConstantDeclaratorContext_getChildren(zen_ConstantDeclaratorContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AssertStatementContext                                                      *
 *******************************************************************************/

/**
 * @class AssertStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AssertStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_AssertStatementContext_t zen_AssertStatementContext_t;

zen_AssertStatementContext_t* zen_AssertStatementContext_new(zen_ASTNode_t* node);
void zen_AssertStatementContext_delete(zen_AssertStatementContext_t* context);
void zen_AssertStatementContext_getChildren(zen_AssertStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * BreakStatementContext                                                       *
 *******************************************************************************/

/**
 * @class BreakStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_BreakStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_BreakStatementContext_t zen_BreakStatementContext_t;

zen_BreakStatementContext_t* zen_BreakStatementContext_new(zen_ASTNode_t* node);
void zen_BreakStatementContext_delete(zen_BreakStatementContext_t* context);
void zen_BreakStatementContext_getChildren(zen_BreakStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ContinueStatementContext                                                    *
 *******************************************************************************/

/**
 * @class ContinueStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ContinueStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_ContinueStatementContext_t zen_ContinueStatementContext_t;

zen_ContinueStatementContext_t* zen_ContinueStatementContext_new(zen_ASTNode_t* node);
void zen_ContinueStatementContext_delete(zen_ContinueStatementContext_t* context);
void zen_ContinueStatementContext_getChildren(zen_ContinueStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ReturnStatementContext                                                      *
 *******************************************************************************/

/**
 * @class ReturnStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ReturnStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_ReturnStatementContext_t zen_ReturnStatementContext_t;

zen_ReturnStatementContext_t* zen_ReturnStatementContext_new(zen_ASTNode_t* node);
void zen_ReturnStatementContext_delete(zen_ReturnStatementContext_t* context);
void zen_ReturnStatementContext_getChildren(zen_ReturnStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ThrowStatementContext                                                       *
 *******************************************************************************/

/**
 * @class ThrowStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ThrowStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
};

typedef struct zen_ThrowStatementContext_t zen_ThrowStatementContext_t;

zen_ThrowStatementContext_t* zen_ThrowStatementContext_new(zen_ASTNode_t* node);
void zen_ThrowStatementContext_delete(zen_ThrowStatementContext_t* context);
void zen_ThrowStatementContext_getChildren(zen_ThrowStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * CompoundStatementContext                                                    *
 *******************************************************************************/

/**
 * @class CompoundStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CompoundStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statement;
};

typedef struct zen_CompoundStatementContext_t zen_CompoundStatementContext_t;

zen_CompoundStatementContext_t* zen_CompoundStatementContext_new(zen_ASTNode_t* node);
void zen_CompoundStatementContext_delete(zen_CompoundStatementContext_t* context);
void zen_CompoundStatementContext_getChildren(zen_CompoundStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * IfStatementContext                                                          *
 *******************************************************************************/

/**
 * @class IfStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_IfStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_ifClause;
    jtk_ArrayList_t* m_elseIfClauses; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_elseClause;
};

typedef struct zen_IfStatementContext_t zen_IfStatementContext_t;

zen_IfStatementContext_t* zen_IfStatementContext_new(zen_ASTNode_t* node);
void zen_IfStatementContext_delete(zen_IfStatementContext_t* context);
void zen_IfStatementContext_getChildren(zen_IfStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * IfClauseContext                                                             *
 *******************************************************************************/

/**
 * @class IfClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_IfClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_IfClauseContext_t zen_IfClauseContext_t;

zen_IfClauseContext_t* zen_IfClauseContext_new(zen_ASTNode_t* node);
void zen_IfClauseContext_delete(zen_IfClauseContext_t* context);
void zen_IfClauseContext_getChildren(zen_IfClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ElseIfClauseContext                                                         *
 *******************************************************************************/

/**
 * @class ElseIfClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ElseIfClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_ElseIfClauseContext_t zen_ElseIfClauseContext_t;

zen_ElseIfClauseContext_t* zen_ElseIfClauseContext_new(zen_ASTNode_t* node);
void zen_ElseIfClauseContext_delete(zen_ElseIfClauseContext_t* context);
void zen_ElseIfClauseContext_getChildren(zen_ElseIfClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ElseClauseContext                                                           *
 *******************************************************************************/

/**
 * @class ElseClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ElseClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_ElseClauseContext_t zen_ElseClauseContext_t;

zen_ElseClauseContext_t* zen_ElseClauseContext_new(zen_ASTNode_t* node);
void zen_ElseClauseContext_delete(zen_ElseClauseContext_t* context);
void zen_ElseClauseContext_getChildren(zen_ElseClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * IterativeStatementContext                                                   *
 *******************************************************************************/

/**
 * @class IterativeStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_IterativeStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_labelClause;
    zen_ASTNode_t* m_statement;
};

typedef struct zen_IterativeStatementContext_t zen_IterativeStatementContext_t;

zen_IterativeStatementContext_t* zen_IterativeStatementContext_new(zen_ASTNode_t* node);
void zen_IterativeStatementContext_delete(zen_IterativeStatementContext_t* context);
void zen_IterativeStatementContext_getChildren(zen_IterativeStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * LabelClauseContext                                                          *
 *******************************************************************************/

/**
 * @class LabelClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LabelClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
};

typedef struct zen_LabelClauseContext_t zen_LabelClauseContext_t;

zen_LabelClauseContext_t* zen_LabelClauseContext_new(zen_ASTNode_t* node);
void zen_LabelClauseContext_delete(zen_LabelClauseContext_t* context);
void zen_LabelClauseContext_getChildren(zen_LabelClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * WhileStatementContext                                                       *
 *******************************************************************************/

/**
 * @class WhileStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_WhileStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
    zen_ASTNode_t* m_elseClause;
};

typedef struct zen_WhileStatementContext_t zen_WhileStatementContext_t;

zen_WhileStatementContext_t* zen_WhileStatementContext_new(zen_ASTNode_t* node);
void zen_WhileStatementContext_delete(zen_WhileStatementContext_t* context);
void zen_WhileStatementContext_getChildren(zen_WhileStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ForStatementContext                                                         *
 *******************************************************************************/

/**
 * @class ForStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
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
void zen_ForStatementContext_getChildren(zen_ForStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ForParametersContext                                                        *
 *******************************************************************************/

/**
 * @class ForParametersContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ForParametersContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_declarator;
    jtk_ArrayList_t* m_identifiers;
};

typedef struct zen_ForParametersContext_t zen_ForParametersContext_t;

zen_ForParametersContext_t* zen_ForParametersContext_new(zen_ASTNode_t* node);
void zen_ForParametersContext_delete(zen_ForParametersContext_t* context);
void zen_ForParametersContext_getChildren(zen_ForParametersContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * TryStatementContext                                                         *
 *******************************************************************************/

/**
 * @class TryStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_TryStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_tryClause;
    jtk_ArrayList_t* m_catchClauses; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_finallyClause;
};

typedef struct zen_TryStatementContext_t zen_TryStatementContext_t;

zen_TryStatementContext_t* zen_TryStatementContext_new(zen_ASTNode_t* node);
void zen_TryStatementContext_delete(zen_TryStatementContext_t* context);
void zen_TryStatementContext_getChildren(zen_TryStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * TryClauseContext                                                            *
 *******************************************************************************/

/**
 * @class TryClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_TryClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_TryClauseContext_t zen_TryClauseContext_t;

zen_TryClauseContext_t* zen_TryClauseContext_new(zen_ASTNode_t* node);
void zen_TryClauseContext_delete(zen_TryClauseContext_t* context);
void zen_TryClauseContext_getChildren(zen_TryClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * CatchClauseContext                                                          *
 *******************************************************************************/

/**
 * @class CatchClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CatchClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_catchFilter;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_CatchClauseContext_t zen_CatchClauseContext_t;

zen_CatchClauseContext_t* zen_CatchClauseContext_new(zen_ASTNode_t* node);
void zen_CatchClauseContext_delete(zen_CatchClauseContext_t* context);
void zen_CatchClauseContext_getChildren(zen_CatchClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * CatchFilterContext                                                          *
 *******************************************************************************/

/**
 * @class CatchFilterContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_CatchFilterContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_typeNames; /* <zen_ASTNode_t*> */
};

typedef struct zen_CatchFilterContext_t zen_CatchFilterContext_t;

zen_CatchFilterContext_t* zen_CatchFilterContext_new(zen_ASTNode_t* node);
void zen_CatchFilterContext_delete(zen_CatchFilterContext_t* context);
void zen_CatchFilterContext_getChildren(zen_CatchFilterContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * TypeNameContext                                                             *
 *******************************************************************************/

/**
 * @class TypeNameContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_TypeNameContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_identifiers;
};

typedef struct zen_TypeNameContext_t zen_TypeNameContext_t;

zen_TypeNameContext_t* zen_TypeNameContext_new(zen_ASTNode_t* node);
void zen_TypeNameContext_delete(zen_TypeNameContext_t* context);
void zen_TypeNameContext_getChildren(zen_TypeNameContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * FinallyClauseContext                                                        *
 *******************************************************************************/

/**
 * @class FinallyClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_FinallyClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_FinallyClauseContext_t zen_FinallyClauseContext_t;

zen_FinallyClauseContext_t* zen_FinallyClauseContext_new(zen_ASTNode_t* node);
void zen_FinallyClauseContext_delete(zen_FinallyClauseContext_t* context);
void zen_FinallyClauseContext_getChildren(zen_FinallyClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * SynchronizeStatementContext                                                 *
 *******************************************************************************/

/**
 * @class SynchronizeStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_SynchronizeStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expression;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_SynchronizeStatementContext_t zen_SynchronizeStatementContext_t;

zen_SynchronizeStatementContext_t* zen_SynchronizeStatementContext_new(zen_ASTNode_t* node);
void zen_SynchronizeStatementContext_delete(zen_SynchronizeStatementContext_t* context);
void zen_SynchronizeStatementContext_getChildren(zen_SynchronizeStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * WithStatementContext                                                        *
 *******************************************************************************/

/**
 * @class WithStatementContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_WithStatementContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_expressions;
    zen_ASTNode_t* m_statementSuite;
};

typedef struct zen_WithStatementContext_t zen_WithStatementContext_t;

zen_WithStatementContext_t* zen_WithStatementContext_new(zen_ASTNode_t* node);
void zen_WithStatementContext_delete(zen_WithStatementContext_t* context);
void zen_WithStatementContext_getChildren(zen_WithStatementContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ClassDeclarationContext                                                     *
 *******************************************************************************/

/**
 * @class ClassDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_classExtendsClause;
    zen_ASTNode_t* m_classSuite;
};

typedef struct zen_ClassDeclarationContext_t zen_ClassDeclarationContext_t;

zen_ClassDeclarationContext_t* zen_ClassDeclarationContext_new(zen_ASTNode_t* node);
void zen_ClassDeclarationContext_delete(zen_ClassDeclarationContext_t* context);
void zen_ClassDeclarationContext_getChildren(zen_ClassDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ClassExtendsClauseContext                                                        *
 *******************************************************************************/

/**
 * @class ClassExtendsClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassExtendsClauseContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_typeNames; /* <zen_ASTNode_t*> */
};

typedef struct zen_ClassExtendsClauseContext_t zen_ClassExtendsClauseContext_t;

zen_ClassExtendsClauseContext_t* zen_ClassExtendsClauseContext_new(zen_ASTNode_t* node);
void zen_ClassExtendsClauseContext_delete(zen_ClassExtendsClauseContext_t* context);
void zen_ClassExtendsClauseContext_getChildren(zen_ClassExtendsClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ClassSuiteContext                                                           *
 *******************************************************************************/

/**
 * @class ClassSuiteContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassSuiteContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_classMembers; /* <zen_ASTNode_t*> */
};

typedef struct zen_ClassSuiteContext_t zen_ClassSuiteContext_t;

zen_ClassSuiteContext_t* zen_ClassSuiteContext_new(zen_ASTNode_t* node);
void zen_ClassSuiteContext_delete(zen_ClassSuiteContext_t* context);
void zen_ClassSuiteContext_getChildren(zen_ClassSuiteContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ClassMemberContext                                                          *
 *******************************************************************************/

/**
 * @class ClassMemberContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ClassMemberContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_modifiers; /* <zen_ASTNode_t*> */
    zen_ASTNode_t* m_declaration;
};

typedef struct zen_ClassMemberContext_t zen_ClassMemberContext_t;

zen_ClassMemberContext_t* zen_ClassMemberContext_new(zen_ASTNode_t* node);
void zen_ClassMemberContext_delete(zen_ClassMemberContext_t* context);
void zen_ClassMemberContext_getChildren(zen_ClassMemberContext_t* context, jtk_ArrayList_t* children);

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
// void zen_ConstructorDeclarationContext_getChildren(zen_ConstructorDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * EnumerationDeclarationContext                                               *
 *******************************************************************************/

/**
 * @class EnumerationDeclarationContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationDeclarationContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_enumerationBaseClause;
    zen_ASTNode_t* m_enumerationSuite;
};

typedef struct zen_EnumerationDeclarationContext_t zen_EnumerationDeclarationContext_t;

zen_EnumerationDeclarationContext_t* zen_EnumerationDeclarationContext_new(zen_ASTNode_t* node);
void zen_EnumerationDeclarationContext_delete(zen_EnumerationDeclarationContext_t* context);
void zen_EnumerationDeclarationContext_getChildren(zen_EnumerationDeclarationContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * EnumerationBaseClauseContext                                                *
 *******************************************************************************/

/**
 * @class EnumerationBaseClauseContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationBaseClauseContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_typeName;
};

typedef struct zen_EnumerationBaseClauseContext_t zen_EnumerationBaseClauseContext_t;

zen_EnumerationBaseClauseContext_t* zen_EnumerationBaseClauseContext_new(zen_ASTNode_t* node);
void zen_EnumerationBaseClauseContext_delete(zen_EnumerationBaseClauseContext_t* context);
void zen_EnumerationBaseClauseContext_getChildren(zen_EnumerationBaseClauseContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * EnumerationSuiteContext                                                     *
 *******************************************************************************/

/**
 * @class EnumerationSuiteContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerationSuiteContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_enumerates; /* <zen_ASTNode_t*> */
};

typedef struct zen_EnumerationSuiteContext_t zen_EnumerationSuiteContext_t;

zen_EnumerationSuiteContext_t* zen_EnumerationSuiteContext_new(zen_ASTNode_t* node);
void zen_EnumerationSuiteContext_delete(zen_EnumerationSuiteContext_t* context);
void zen_EnumerationSuiteContext_getChildren(zen_EnumerationSuiteContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * EnumerateContext                                                            *
 *******************************************************************************/

/**
 * @class EnumerateContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EnumerateContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_identifier;
    zen_ASTNode_t* m_functionArguments;
};

typedef struct zen_EnumerateContext_t zen_EnumerateContext_t;

zen_EnumerateContext_t* zen_EnumerateContext_new(zen_ASTNode_t* node);
void zen_EnumerateContext_delete(zen_EnumerateContext_t* context);
void zen_EnumerateContext_getChildren(zen_EnumerateContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ExpressionsContext                                                          *
 *******************************************************************************/

/**
 * @class ExpressionsContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExpressionsContext_t {
    zen_ASTNode_t* m_node;
    jtk_ArrayList_t* m_expressions; /* <zen_ASTNode_t*> */
};

typedef struct zen_ExpressionsContext_t zen_ExpressionsContext_t;

zen_ExpressionsContext_t* zen_ExpressionsContext_new(zen_ASTNode_t* node);
void zen_ExpressionsContext_delete(zen_ExpressionsContext_t* context);
void zen_ExpressionsContext_getChildren(zen_ExpressionsContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ExpressionContext                                                           *
 *******************************************************************************/

/**
 * @class ExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_assignmentExpression;
};

typedef struct zen_ExpressionContext_t zen_ExpressionContext_t;

zen_ExpressionContext_t* zen_ExpressionContext_new(zen_ASTNode_t* node);
void zen_ExpressionContext_delete(zen_ExpressionContext_t* context);
void zen_ExpressionContext_getChildren(zen_ExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AssignmentExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class AssignmentExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AssignmentExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_conditionalExpression;
    zen_ASTNode_t* m_assignmentOperator;
    zen_ASTNode_t* m_assignmentExpression;
};

typedef struct zen_AssignmentExpressionContext_t zen_AssignmentExpressionContext_t;

zen_AssignmentExpressionContext_t* zen_AssignmentExpressionContext_new(zen_ASTNode_t* node);
void zen_AssignmentExpressionContext_delete(zen_AssignmentExpressionContext_t* context);
void zen_AssignmentExpressionContext_getChildren(zen_AssignmentExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ConditionalExpressionContext                                                *
 *******************************************************************************/

/**
 * @class ConditionalExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ConditionalExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_logicalOrExpression;
    zen_ASTNode_t* m_thenExpression;
    zen_ASTNode_t* m_elseExpression;
};

typedef struct zen_ConditionalExpressionContext_t zen_ConditionalExpressionContext_t;

zen_ConditionalExpressionContext_t* zen_ConditionalExpressionContext_new(zen_ASTNode_t* node);
void zen_ConditionalExpressionContext_delete(zen_ConditionalExpressionContext_t* context);
void zen_ConditionalExpressionContext_getChildren(zen_ConditionalExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * LogicalOrExpressionContext                                                  *
 *******************************************************************************/

/**
 * @class LogicalOrExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_LogicalOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_logicalAndExpression;
    zen_ASTNode_t* m_logicalOrExpression;
};

typedef struct zen_LogicalOrExpressionContext_t zen_LogicalOrExpressionContext_t;

zen_LogicalOrExpressionContext_t* zen_LogicalOrExpressionContext_new(zen_ASTNode_t* node);
void zen_LogicalOrExpressionContext_delete(zen_LogicalOrExpressionContext_t* context);
void zen_LogicalOrExpressionContext_getChildren(zen_LogicalOrExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * LogicalAndExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class LogicalAndExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
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
void zen_LogicalAndExpressionContext_getChildren(zen_LogicalAndExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * InclusiveOrExpressionContext                                                *
 *******************************************************************************/

/**
 * @class InclusiveOrExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
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
void zen_InclusiveOrExpressionContext_getChildren(zen_InclusiveOrExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ExclusiveOrExpressionContext                                                *
 *******************************************************************************/

/**
 * @class ExclusiveOrExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ExclusiveOrExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_andExpression;
    zen_ASTNode_t* m_exclusiveOrExpression;
};

typedef struct zen_ExclusiveOrExpressionContext_t zen_ExclusiveOrExpressionContext_t;

zen_ExclusiveOrExpressionContext_t* zen_ExclusiveOrExpressionContext_new(zen_ASTNode_t* node);
void zen_ExclusiveOrExpressionContext_delete(zen_ExclusiveOrExpressionContext_t* context);
void zen_ExclusiveOrExpressionContext_getChildren(zen_ExclusiveOrExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AndExpressionContext                                                        *
 *******************************************************************************/

/**
 * @class AndExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AndExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_equalityExpression;
    zen_ASTNode_t* m_andExpression;
};

typedef struct zen_AndExpressionContext_t zen_AndExpressionContext_t;

zen_AndExpressionContext_t* zen_AndExpressionContext_new(zen_ASTNode_t* node);
void zen_AndExpressionContext_delete(zen_AndExpressionContext_t* context);
void zen_AndExpressionContext_getChildren(zen_AndExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * EqualityExpressionContext                                                   *
 *******************************************************************************/

/**
 * @class EqualityExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_EqualityExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_relationalExpression;
    zen_ASTNode_t* m_equalityOperator;
    zen_ASTNode_t* m_equalityExpression;
};

typedef struct zen_EqualityExpressionContext_t zen_EqualityExpressionContext_t;

zen_EqualityExpressionContext_t* zen_EqualityExpressionContext_new(zen_ASTNode_t* node);
void zen_EqualityExpressionContext_delete(zen_EqualityExpressionContext_t* context);
void zen_EqualityExpressionContext_getChildren(zen_EqualityExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * RelationalExpressionContext                                                 *
 *******************************************************************************/

/**
 * @class RelationalExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_RelationalExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_shiftExpression;
    zen_ASTNode_t* m_relationalOperator;
    zen_ASTNode_t* m_relationalExpression;
};

typedef struct zen_RelationalExpressionContext_t zen_RelationalExpressionContext_t;

zen_RelationalExpressionContext_t* zen_RelationalExpressionContext_new(zen_ASTNode_t* node);
void zen_RelationalExpressionContext_delete(zen_RelationalExpressionContext_t* context);
void zen_RelationalExpressionContext_getChildren(zen_RelationalExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * ShiftExpressionContext                                                      *
 *******************************************************************************/

/**
 * @class ShiftExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_ShiftExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_additiveExpression;
    zen_ASTNode_t* m_shiftOperator;
    zen_ASTNode_t* m_shiftExpression;
};

typedef struct zen_ShiftExpressionContext_t zen_ShiftExpressionContext_t;

zen_ShiftExpressionContext_t* zen_ShiftExpressionContext_new(zen_ASTNode_t* node);
void zen_ShiftExpressionContext_delete(zen_ShiftExpressionContext_t* context);
void zen_ShiftExpressionContext_getChildren(zen_ShiftExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * AdditiveExpressionContext                                                   *
 *******************************************************************************/

/**
 * @class AdditiveExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_AdditiveExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_multiplicativeExpression;
    zen_ASTNode_t* m_additiveOperator;
    zen_ASTNode_t* m_additiveExpression;
};

typedef struct zen_AdditiveExpressionContext_t zen_AdditiveExpressionContext_t;

zen_AdditiveExpressionContext_t* zen_AdditiveExpressionContext_new(zen_ASTNode_t* node);
void zen_AdditiveExpressionContext_delete(zen_AdditiveExpressionContext_t* context);
void zen_AdditiveExpressionContext_getChildren(zen_AdditiveExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * MultiplicativeExpressionContext                                             *
 *******************************************************************************/

/**
 * @class MultiplicativeExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_MultiplicativeExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_unaryExpression;
    zen_ASTNode_t* m_multiplicativeOperator;
    zen_ASTNode_t* m_multiplicativeExpression;
};

typedef struct zen_MultiplicativeExpressionContext_t zen_MultiplicativeExpressionContext_t;

zen_MultiplicativeExpressionContext_t* zen_MultiplicativeExpressionContext_new(zen_ASTNode_t* node);
void zen_MultiplicativeExpressionContext_delete(zen_MultiplicativeExpressionContext_t* context);
void zen_MultiplicativeExpressionContext_getChildren(zen_MultiplicativeExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * UnaryExpressionContext                                                      *
 *******************************************************************************/

/**
 * @class UnaryExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_UnaryExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_unaryOperator;
    zen_ASTNode_t* m_unaryExpression;
    zen_ASTNode_t* m_postfixExpression;
};

typedef struct zen_UnaryExpressionContext_t zen_UnaryExpressionContext_t;

zen_UnaryExpressionContext_t* zen_UnaryExpressionContext_new(zen_ASTNode_t* node);
void zen_UnaryExpressionContext_delete(zen_UnaryExpressionContext_t* context);
void zen_UnaryExpressionContext_getChildren(zen_UnaryExpressionContext_t* context, jtk_ArrayList_t* children);

/*******************************************************************************
 * PostfixExpressionContext                                                    *
 *******************************************************************************/

/**
 * @class PostfixExpressionContext
 * @ingroup zen_compiler_ast
 * @author Samuel Rowe
 * @since zen 1.0
 */
struct zen_PostfixExpressionContext_t {
    zen_ASTNode_t* m_node;
    zen_ASTNode_t* m_primaryExpression;
    jtk_ArrayList_t* m_postfixParts; /* <zen_ASTNode_t*> */
};

typedef struct zen_PostfixExpressionContext_t zen_PostfixExpressionContext_t;

zen_PostfixExpressionContext_t* zen_PostfixExpressionContext_new(zen_ASTNode_t* node);
void zen_PostfixExpressionContext_delete(zen_PostfixExpressionContext_t* context);
void zen_PostfixExpressionContext_getChildren(zen_PostfixExpressionContext_t* context, jtk_ArrayList_t* children);

#include <com/onecube/zen/compiler/ast/context/SubscriptContext.h>
#include <com/onecube/zen/compiler/ast/context/FunctionArgumentsContext.h>
#include <com/onecube/zen/compiler/ast/context/MemberAccessContext.h>
#include <com/onecube/zen/compiler/ast/context/PostfixOperatorContext.h>
#include <com/onecube/zen/compiler/ast/context/PrimaryExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/MapExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/MapEntriesContext.h>
#include <com/onecube/zen/compiler/ast/context/MapEntryContext.h>
#include <com/onecube/zen/compiler/ast/context/ListExpressionContext.h>
#include <com/onecube/zen/compiler/ast/context/NewExpressionContext.h>

#endif /* COM_ONECUBE_ZEN_COMPILER_COMPILER_CONTEXT_H */