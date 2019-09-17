/*
 * Copyright 2018-2019 OneCube
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <com/onecube/zen/RuleNode.h>

/*******************************************************************************
 * RuleNode                                                                    *
 *******************************************************************************/

zen_RuleNode_t* zen_RuleNode_new(zen_ASTNode_t* node) {
    zen_RuleNode_t* ruleNode = zen_Memory_allocate(zen_RuleNode_t, 1);
    ruleNode->m_children = NULL;
    ruleNode->m_start = NULL;
    ruleNode->m_stop = NULL;
    ruleNode->m_node = node;
    
    return ruleNode;
}

void zen_RuleNode_delete(zen_RuleNode_t* ruleNode) {
    jtk_Assert_assertObject(ruleNode, "The specified rule node is null.");
    
    if (ruleNode->m_children != NULL) {
        int32_t size = jtk_ArrayList_getSize(ruleNode->m_children);
        int32_t i;
        for (i = 0; i < size; i++) {
            zen_ASTNode_t* child = (zen_ASTNode_t*)jtk_ArrayList_getValue(ruleNode->m_children, i);
            zen_ASTNode_delete(child);
        }
        jtk_ArrayList_delete(ruleNode->m_children);
    }
    
    /* At this point, ruleNode->m_node should be destroyed.
     * Therefore, we ignore it.
     * We cannot destroy ruleNode->m_start and ruleNode->m_stop.
     * Because they belong to the TokenStream that demaned
     * their creation.
     */
    
    zen_Memory_deallocate(ruleNode);
}

void zen_RuleNode_addRule(zen_RuleNode_t* ruleNode, zen_ASTNode_t* rule) {
    jtk_Assert_assertObject(ruleNode, "The specified rule node is null.");
    
    if (ruleNode->m_children == NULL) {
        ruleNode->m_children = jtk_ArrayList_new();
    }
    jtk_ArrayList_add(ruleNode->m_children, rule);
}

void zen_RuleNode_addTerminal(zen_RuleNode_t* ruleNode, zen_ASTNode_t* terminal) {
    jtk_Assert_assertObject(ruleNode, "The specified rule node is null.");
    
    if (ruleNode->m_children == NULL) {
        ruleNode->m_children = jtk_ArrayList_new();
    }
    jtk_ArrayList_add(ruleNode->m_children, terminal);
}

