#ifndef _symbolTable_H
#define _symbolTable_H

#include <llvm/IR/BasicBlock.h>

#include <bits/stdc++.h>


class symbol_table_node
{
public:
    llvm::BasicBlock * block;
    std::map<std::string, llvm::Value *> localVariables;

    symbol_table_node(llvm::BasicBlock * block)
    {
        auto blk = block;
        this->block = blk;
    }

    ~symbol_table_node() {}
};

class symbol_table
{
private:
    std::list<symbol_table_node> table;
    std::stack<std::pair<llvm::BasicBlock *, llvm::BasicBlock *>> break_continue_stack;

public:
    symbol_table() {}
    ~symbol_table() {}
    std::map<std::string, llvm::Value *> get_local_variables()
    {
        auto lclVrbls = this->table.front().localVariables;
        return lclVrbls;
    }

    void set_local_variables(std::map<std::string, llvm::Value *> variables) {
        auto vrblsFrnt = variables.begin();
        auto vrblsBck = variables.end();
        this->table.front().localVariables.insert(vrblsFrnt, vrblsBck);
    }

    bool look_up_locals(std::string name) {
        auto lclvrblsNmeLcn = this->get_local_variables().find(name);
        bool ifPresent = lclvrblsNmeLcn == this->get_local_variables().end();
        return ifPresent;
    }

    void declare_locals(std::string name, llvm::Value * value) {
        auto lclLcn = this->look_up_locals(name);
        if (lclLcn) {
            std::cerr<<"Variable "<<name<<" already declared";
            exit(0);
        }
        else {
            auto insrtLclVar = std::pair<std::string, llvm::Value *>(name, value); 
            this->table.front().localVariables.insert(insrtLclVar);
        }
    }

    bool lookup_global_vars(std::string name)
    {
        auto lclVars = this->return_locals(name);
        bool ifPresent = lclVars != NULL;
        return ifPresent;
    }

    llvm::Value * return_locals(std::string name)
    {
        auto it = this->table.begin();
        while ( it != this->table.end())
        {
            auto lclVars = it->localVariables;
            if (lclVars.find(name) != lclVars.end())
            {
                return lclVars.find(name)->second;
            }
            it++;
        }
        if(it == this->table.end())
            return NULL;
    }

    void push_block(llvm::BasicBlock * block) {
        auto blkToPsh = symbol_table_node(block);
        this->table.push_front(blkToPsh);
    }

    void pop_block() {
        this->table.pop_front();
    }

    llvm::BasicBlock * top_block() {
        auto it = this->table.begin();
        while ( it != this->table.end()) {
            if (it->block) {
                return it->block;
            }
            it++;
        }
        auto blkFnd = this->table.front().block;
        return blkFnd;
    }

    llvm::BasicBlock * bottom_block()
    {
        auto btmBlk = this->table.back().block;
        return btmBlk;
    }

    void print_table()
    {
        auto i = this->table.front().localVariables;
        auto it =  i.begin() ;
        while ( it != i.end())
        {
            std::cout << it->first;
            it++;
        }
    }

    void push_break_continue_stack(llvm::BasicBlock * breakSt, llvm::BasicBlock * returnSt)
    {
        auto stckPr = std::make_pair(breakSt, returnSt);
        this->break_continue_stack.push(stckPr);
    }

    void pop_break_continue_stack()
    {
        this->break_continue_stack.pop();
    }

    llvm::BasicBlock * get_break_stack() {
        if (this->break_continue_stack.empty()) {
            return NULL;
        }
        else{
            auto tpElm = this->break_continue_stack.top();
            return tpElm.first;
        }
    }

    llvm::BasicBlock * get_continue_stack() {
        if (this->break_continue_stack.empty()) {
            return NULL;
        }
        else{
            auto tpElmBrkCnt = this->break_continue_stack.top();
            return tpElmBrkCnt.second;
        }
    }

};

#endif
