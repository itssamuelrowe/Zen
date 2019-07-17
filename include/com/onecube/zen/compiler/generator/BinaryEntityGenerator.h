// Friday, March 20, 2018

#ifndef COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_GENERATOR_H
#define COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_GENERATOR_H

#include <com/onecube/zen/Configuration.h>

#include <com/onecube/zen/compiler/ast/ASTListener.h>
#include <com/onecube/zen/compiler/ast/ASTNode.h>
#include <com/onecube/zen/compiler/ast/ASTAnnotations.h>
#include <com/onecube/zen/compiler/symbol-table/SymbolTable.h>

#include <com/onecube/zen/virtual-machine/feb/ByteCode.h>

/*******************************************************************************
 * BinaryEntityGenerator                                                       *
 *******************************************************************************/

/**
 * @author Samuel Rowe
 * @since zen 1.0
 * @ingroup zen_compiler_generator
 */
struct zen_BinaryEntityGenerator_t {
    zen_ASTListener_t* m_astListener;
    zen_BinaryEntityBuilder_t* m_builder;
    zen_SymbolTable_t* m_symbolTable;
    zen_ASTAnnotations_t* m_scopes;
};

/**
 * @memberof BinaryEntityGenerator
 */
typedef struct zen_BinaryEntityGenerator_t zen_BinaryEntityGenerator_t;

// Constructor

/**
 * @memberof BinaryEntityGenerator
 */
zen_BinaryEntityGenerator_t* zen_BinaryEntityGenerator_new(zen_SymbolTable_t* symbolTable,
    zen_AstAnnotations_t* scopes);

// Destructor

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_delete(zen_BinaryEntityGenerator_t* generator);

// Generate

/**
 * @memberof BinaryEntityGenerator
 */
void zen_BinaryEntityGenerator_generate(zen_BinaryEntityGenerator_t* generator);

// Event Handlers

void zen_BinaryEntityGenerator_onEnterExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitAssignmentExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitCompilationUnit(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitFunctionDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitStatementSuite(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitClassDeclaration(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitConditionalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitLogicalOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitLogicalAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitInclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitExclusiveOrExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitAndExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitEqualityExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitRelationalExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitShiftExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitAdditiveExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitMultiplicativeExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitUnaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterPostfixExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitPostfixExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterPrimaryExpression(zen_ASTListener_t* listener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterListExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitListExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitMapExpression(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitMapEntries(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

void zen_BinaryEntityGenerator_onEnterMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node);
void zen_BinaryEntityGenerator_onExitMapEntry(zen_ASTListener_t* astListener, zen_ASTNode_t* node);

#endif /* COM_ONECUBE_ZEN_COMPILER_GENERATOR_BINARY_ENTITY_GENERATOR_H */