compilationUnit
:	NEWLINE*
    importDeclaration*
    annotatedComponentDeclaration*
    EOF
; 
importDeclaration
:     'import' importTarget NEWLINE
; 
importTarget
:     IDENTIFIER ('.' IDENTIFIER)* ('.' '*' | ('as' IDENTIFIER))?
; 
annotatedComponentDeclaration
:     annotations? componentDeclaration
; 
annotations
:     annotation+
; 
annotation
:     '@' annotationType annotationAttribute* NEWLINE
; 
annotationType
:     IDENTIFIER ('.' IDENTIFIER)*
; 
annotationAttribute
:     IDENTIFIER '=' literal
; 
componentDeclaration
:     functionDeclaration
|     classDeclaration
|     enumerationDeclaration
; 
functionDeclaration
:     'function' functionIdentifier functionParameters? (functionBody | NEWLINE)
;
functionIdentifier
:     IDENTIFIER
// |     'static'
|     'new'
;
 functionParameters
 :    '(' actualFunctionParameters? ')'
 ;
actualFunctionParameters
:     IDENTIFIER (',' IDENTIFIER)* (',' '...' IDENTIFIER)?
|     '...' IDENTIFIER
; 
functionBody
:     statementSuite
; 
statementSuite
|     NEWLINE INDENT statement+ DEDENT
; 
simpleStatement
:     unterminatedSimpleStatement NEWLINE
;
unterminatedSimpleStatement
:     expressionStatement
|     emptyStatement
|     variableDeclaration
|     constantDeclaration
|     assertStatement
|     breakStatement
|     continueStatement
|     returnStatement
|     throwStatement
;
expressionStatement
:     expression
; 
statement
:     simpleStatement
|     compoundStatement
; 
emptyStatement
:     ';'
; 
variableDeclaration
:     'var' variableDeclarator (',' variableDeclarator)*
; 
variableDeclarator
:     IDENTIFIER ('=' expression)?
; 
constantDeclaration
:     'final' constantDeclarator (',' constantDeclarator)*
; 
constantDeclarator
:     IDENTIFIER '=' expression
; 
assertStatement
:     'assert' expression (':' expression)?
; 
breakStatement
:     'break' IDENTIFIER?
; 
continueStatement
:     'continue' IDENTIFIER?
; 
returnStatement
:     'return' expression
; 
throwStatement
:     'throw' expression?
; 
compoundStatement
:     ifStatement
|     iterativeStatement
|     tryStatement
|     synchronizeStatement
|     withStatement
// |     functionDeclaration
// |     classDeclaration
; 
ifStatement
:     ifClause elseIfClause* elseClause?
; 
ifClause
:     'if' expression statementSuite
; 
ifClause
:     'if' expression statementSuite
; 
elseClause
:     'else' statementSuite
; 
iterativeStatement
:     labelClause? (whileStatement | forStatement)
; 
labelClause
:     '#' IDENTIFIER
; 
whileStatement
:     'while' expression statementSuite
; 
forParameter
:     ('var' | 'final')? IDENTIFIER
; 
forStatement
:     'for' forParameters 'in' expression statementSuite
;
tryStatement
:     tryClause catchClause* finallyClause?
; 
tryClause
:     'try' statementSuite
; 
catchClause
:	'catch' catchFilter IDENTIFIER statementSuite'
; 
catchFilter
:	typeName ('|' typeName)*
; 
typeName
:     IDENTIFIER ('.' IDENTIFIER)*
; 
finallyClause
:	'finally' statementSuite
; 
synchronizeStatement
:	'synchronize' expression statementSuite
; 
withStatement
:	'with' withParameters statementSuite
; 
withParameters
:     withParameter (',' withParameter)*
; 
withParameter
:     (('var' | 'final') IDENTIFIER '=' )? expression
; 
classDeclaration
:	'class' IDENTIFIER classClassExtendsClause? classSuite
; 
classClassExtendsClause
:	'<=' typeName (',' typeName)*
; 
classSuite
:	NEWLINE INDENT classMember+ DEDENT
; 
classMember
:	annotations? classMemberModifier* unmodifiedClassMember
; 
unmodifiedClassMember
:	variableDeclaration
|	constantDeclaration
|	functionDeclaration
// |	constructorDeclaration
|	classDeclaration
|	enumerationDeclaration
; 
classMemberModifier
:     'static'
|     'native'
|     'abstract'
|     'public'
|     'private'
|     'secret'
; 
constructorDeclaration
:	IDENTIFIER functionParameters? statementSuite
; 
enumerationDeclaration
:    'enum' IDENTIFIER enumerationBaseClause? enumerationSuite
; 
enumerationBaseClause
:    ':' typeName
; 
enumerationSuite
:    NEWLINE INDENT enumerate+ DEDENT
; 
enumerate
:    IDENTIFIER functionArguments? NEWLINE
; 
expression
:     expression (',' expression)*
; 
expression
:	assignmentExpression
; 
assignmentExpression
:	conditionalExpression (assignmentOperator assignmentExpression)?
; 
assignmentOperator
:     '='
|     '*='
|     '/='
|     '%='
|     '+='
|     '-='
|     '<<='
|     '>>='
|     '>>>='
|     '&='
|     '^='
|     '|='
; 
conditionalExpression
:	logicalOrExpression ('then' expression 'else' conditionalExpression)?
; 
logicalOrExpression
:	logicalAndExpression ('or' logicalAndExpression)*
; 
logicalAndExpression
:	inclusiveOrExpression ('and' logicalAndExpression)?
; 
inclusiveOrExpression
:	exclusiveOrExpression ('|' exclusiveOrExpression)*
; 
exclusiveOrExpression
:	andExpression ('^' andExpression)*
; 
andExpression
:	equalityExpression ('&' equalityExpression)*
; 
equalityExpression
:	relationalExpression (equalityOperator relationalExpression)*
; 
equalityOperator
:	'=='
|	'!='
; 
relationalExpression
:	shiftExpression (relationalOperator shiftExpression)*
; 
relationalOperator
:	'<'
|	'>'
|	'<='
|	'>='
|	'is'
; 
shiftExpression
:	additiveExpression (shiftOperator additiveExpression)*
; 
shiftOperator
:	'<<'
|	'>>'
|	'>>>'
; 
additiveExpression
:	multiplicativeExpression (multiplicativeOperator multiplicativeExpression)*
; 
additiveOperator
:	'+'
|	'-'
; 
multiplicativeExpression
:	unaryExpression (multiplicativeOperator unaryExpression)*
; 
multiplicativeOperator
:	'*'
|	'/'
|	'%'
; 
unaryExpression
:	unaryOperator unaryExpression
|	postfixExpression
; 
unaryOperator
:     '+'
|     '-'
|     '~'
|     '!'
// |     '++'
// |     '--'
; 
postfixExpression
:	primaryExpression postfixPart*
;
postfixPart
:     subscript
|     functionArguments
|     memberAccess
|     postfixOperator
;
subscript
:	'[' expression ']'
; 
functionArguments
:	'(' expressions? ')'
; 
memberAccess
:	'.' IDENTIFIER
; 
postfixOperator
:	'++'
|	'--'
; 
primaryExpression
:	IDENTIFIER
|	literal
|	'(' expression ')'
|	mapExpression
|	listExpression
;
literal
:	INTEGER_LITERAL
|	FLOATING_POINT_LITERAL
|	'true'
|	'false'
|	STRING_LITERAL
|	'null'
|     'this'
;
mapExpression
:	'{' mapEntries? '}'
; 
mapEntries
:	mapEntry (',' mapEntry)*
; 
mapEntry
:	expression ':' expression
; 
listExpression
:     '[' expressions ']'
; 
newExpression
:    'new' typeName functionArguments?
;