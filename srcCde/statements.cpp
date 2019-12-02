#include "ast.h"

#include <bits/stdc++.h>


ast_block_statement::ast_block_statement(vector<ast_statement *> * stmtlist, vector<ast_var_decl *> * id_list)
{
    this->stmtlist = stmtlist, this->id_list = id_list;
}

vector<ast_var_decl *> * ast_block_statement::get_id_list() {
    auto idLst = this->id_list;
    return idLst;
}

vector<ast_statement *> * ast_block_statement::getStmtlist() {
    auto stmntLst = this->stmtlist;
    return stmntLst;
}

ast_assignment_statement::ast_assignment_statement(AssignOp op, ast_location * location, ast_expression * expr)
{
   this->op = op, this->location = location, this->expr = expr;
}

AssignOp ast_assignment_statement::get_op() {
    auto opVal = this->op;
    return opVal;
}

ast_location * ast_assignment_statement::getLocation(){
    auto assgnStLcn = this->location;
    return assgnStLcn;
}

ast_expression * ast_assignment_statement::get_expr(){
    auto astStExpr = this->expr;
    return astStExpr;
}

ast_if_statement::ast_if_statement(ast_expression * condition, ast_block_statement * if_block, ast_block_statement * else_block)
{
    this->condition = condition, this->if_block = if_block, this->else_block = else_block;
}

ast_expression * ast_if_statement::get_condition(){
    auto ifCndtn = this->condition;
    return ifCndtn;
}

ast_block_statement * ast_if_statement::getIf_block(){
    auto ifBlck = this->if_block;
    return ifBlck;
}

ast_block_statement * ast_if_statement::getElse_block(){
    auto elsBlk = this->else_block;
    return elsBlk;
}


ast_for_statement::ast_for_statement(ast_expression * init_condition, ast_expression * end_condition, ast_block_statement * block, string id)
{
    this->init_condition = init_condition, this->end_condition = end_condition, this->block = block, this->id = id;
}
string ast_for_statement::get_id()
{
    auto stmntId = this->id;
    return stmntId;
}

ast_expression * ast_for_statement::getInit_condition(){
    auto forInitCndtn = this->init_condition;
    return forInitCndtn;
}
ast_expression * ast_for_statement::getEnd_condition()
{
    auto forEndCndtn = this->end_condition;
    return forEndCndtn;
}
ast_block_statement * ast_for_statement::get_block()
{
    auto forBlk = this->block;
    return forBlk;
}

ast_return_statement::ast_return_statement(ast_expression * expr)
{
    auto rtrnStmnt = expr;
    this->expr = rtrnStmnt;
}
ast_expression * ast_return_statement::get_expr()
{
    auto rtrnStmnt = this->expr;
    return rtrnStmnt;
}
