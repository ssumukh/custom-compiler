#ifndef _codeGenVisitor_H
#define _codeGenVisitor_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <bits/stdc++.h>


#include "ast.h"
#include "symbol_table.h"
#include "visitor.h"

static llvm::LLVMContext MyGlobalContext;

class codegen_visitor : public Visitor
{
private:
    llvm::Module * module;
    ast_program * start;
    llvm::Function * mainFunction;
    symbol_table symbol_table_obj;
public:
    codegen_visitor(ast_program * start)
    {
        auto strtId = start->get_id();
        module = new llvm::Module(strtId, MyGlobalContext);
        this->start = start;
        auto vdTyp = llvm::Type::getVoidTy(MyGlobalContext);
        string toArg = "main";
        mainFunction = llvm::Function::Create(static_cast<llvm::FunctionType *>(llvm::FunctionType::get(vdTyp, false)), llvm::GlobalValue::ExternalLinkage, toArg, module);
    }

    ~codegen_visitor() {}

    void code_gen()
    {
        llvm::BasicBlock *block = llvm::BasicBlock::Create(MyGlobalContext, "entry", mainFunction, 0);
        symbol_table_obj.push_block(block);
        this->visit(start);
        block = symbol_table_obj.top_block();
        symbol_table_obj.pop_block();
        llvm::ReturnInst::Create(MyGlobalContext, block);
        llvm::verifyModule(*module);
        llvm::legacy::PassManager PM;
        PM.add(llvm::createPrintModulePass(llvm::outs()));
        PM.run(*module);
    }

    llvm::Value * ErrorHandler(const char * error)
    {
        std::cerr << error <<std::endl;
            // return 0;
        exit(-1);
    }

    void * visit(ast_program * node)
    {
        auto funcDclLst = node->getFdl();
        if (funcDclLst)
        {
            auto itrtr = funcDclLst->begin();
            while(itrtr != (node->getFdl())->end())
            {
                this->visit(*itrtr);
                itrtr++;
            }
        }
        llvm::Function * userMain = NULL;
        auto mainDecl = node->getMdl();
        if (mainDecl) {
            auto itrtr = mainDecl->begin();
            while(itrtr != (node->getMdl())->end())
            {
                auto itrtrId = (*itrtr)->get_id();
                if(module->getFunction(itrtrId) && itrtrId != "main")
                {
                    return ErrorHandler("Error, multiple Declaration of main function");
                }
                else if (itrtrId == "main" && userMain)
                {
                    return ErrorHandler("Error, multiple Declaration of main");
                }
                auto iterator= static_cast<llvm::Function *>(this->visit(*itrtr));
                if (itrtrId == "main" && !userMain)
                {
                    userMain = iterator;
                }
                
                if (itrtrId == "main" && (*itrtr)->get_arguments())
                {
                    return ErrorHandler("Error, main func cannot have any arguments");
                }
                itrtr++;
            }
        }
        if (!userMain)
            return ErrorHandler("No main Found");
        else {
            llvm::CallInst::Create(userMain, "", symbol_table_obj.top_block());
        }
        return NULL;
    }

    void * visit(ast_field_decl * node) {
        auto idLst = node->get_id_list();
        if (idLst) {
            auto itrtr = idLst->begin();
            while(itrtr != (node->get_id_list())->end()) {
                this->visit(*itrtr);
                itrtr++;
            }
        }
        return NULL;
    }

    void * visit(ast_var_decl * node) {
        auto idLst = node->get_id_list();
        if (idLst) {
            auto itrtr = idLst->begin();
            while(itrtr != (node->get_id_list())->end()) {
                this->visit(*itrtr);
                itrtr++;
            }
        }
        return NULL;
    }

    void * visit(ast_id * node) {
        auto ndeSz = node->getSize();
        if (ndeSz < 0) {
            return ErrorHandler("Error, invalid Array Size, can not be negative");
        }
        else if (!ndeSz) {
            auto ndeId = node->get_id();
            if(symbol_table_obj.top_block() == symbol_table_obj.bottom_block()) { // Which means that it is a global variable.
                 auto dtType = llvm::Type::getInt64Ty(MyGlobalContext);
                 llvm::GlobalVariable * globalInteger = new llvm::GlobalVariable(*module, dtType, false, llvm::GlobalValue::CommonLinkage, NULL, ndeId);
                 auto stVrbl = llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true);
                 globalInteger->setInitializer(stVrbl);
                 symbol_table_obj.declare_locals(ndeId, globalInteger);
                 return globalInteger;
             }
             else {
                 llvm::AllocaInst * allocaInst = new llvm::AllocaInst(llvm::Type::getInt64Ty(MyGlobalContext), 0, ndeId, symbol_table_obj.top_block());
                 new llvm::StoreInst(llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true), allocaInst, false, symbol_table_obj.top_block());
                 symbol_table_obj.declare_locals(ndeId, allocaInst);
                 return allocaInst;
             }
         }
        else {
            auto ndeId = node->get_id();
            auto ndeSz = node->getSize();
             llvm::GlobalVariable* variable = new llvm::GlobalVariable(*module, llvm::ArrayType::get(llvm::Type::getInt64Ty(MyGlobalContext), ndeSz), false, llvm::GlobalValue::CommonLinkage, NULL, ndeId);
             auto vrblInit = llvm::ConstantAggregateZero::get(llvm::ArrayType::get(llvm::Type::getInt64Ty(MyGlobalContext), ndeSz));
             variable->setInitializer(vrblInit);
             symbol_table_obj.declare_locals(ndeId, variable);
             return variable;
         }
     }

     llvm::Type * parseType(Datatype type) {
        if(type == Datatype::int_type){
            auto idntfdType = llvm::Type::getInt64Ty(MyGlobalContext);
            return idntfdType;
        }
        else if(type == Datatype::void_type){
            auto idntfdType = llvm::Type::getVoidTy(MyGlobalContext);
            return idntfdType;
        }
        else if(type == Datatype::bool_type){
            auto idntfdType = llvm::Type::getInt64Ty(MyGlobalContext);
            return idntfdType;
        }
        else{
            return NULL;
        }
    }

    void * visit(ast_method_decl * node) {
        std::vector<llvm::Type*> argTypes;

        llvm::Function *function;

        auto argLst = node->get_arguments();
        if (argLst) {
            auto itrtr = (node->get_arguments())->begin();
            while (itrtr != argLst->end()) {
                auto currType = (*itrtr)->getType();
                argTypes.push_back(parseType(currType));
                itrtr++;
            }
        }

        function = llvm::Function::Create(static_cast<llvm::FunctionType *>(llvm::FunctionType::get(parseType(node->getReturnType()), llvm::makeArrayRef(argTypes), false)), llvm::GlobalValue::InternalLinkage, node->get_id(), module);
        string toArg = "entry";
        symbol_table_obj.push_block(static_cast<llvm::BasicBlock *>(llvm::BasicBlock::Create(MyGlobalContext, toArg, function, 0)));
        auto argsLst = node->get_arguments();
        if (argsLst) {
            auto itrtr = argsLst->begin();
            auto itrtr2 = function->arg_begin();
            while (itrtr != (node->get_arguments())->end()) {
                llvm::Value * arg = &(*itrtr2);
                auto itrtrId = (*itrtr)->get_id();
                arg->setName(itrtrId);
                llvm::AllocaInst * allocaInst = NULL;
                allocaInst = new llvm::AllocaInst(llvm::Type::getInt64Ty(MyGlobalContext), 0, (*itrtr)->get_id(), symbol_table_obj.top_block());
                new llvm::StoreInst(arg, allocaInst, false, symbol_table_obj.top_block());
                symbol_table_obj.declare_locals((*itrtr)->get_id(), allocaInst);
                itrtr ++;
                itrtr2 ++;
            }
        }
            // symbol_table_obj.push_block();
        auto toVst = node->get_block();
        this->visit(toVst);
        auto ifTrmntr = symbol_table_obj.top_block()->getTerminator();
        if(!ifTrmntr) {
            auto ndeRtrnType = node->getReturnType();
            if(ndeRtrnType == Datatype::void_type)
                llvm::ReturnInst::Create(MyGlobalContext, symbol_table_obj.top_block());
            else{
                auto dtType = llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true);
                llvm::ReturnInst::Create(MyGlobalContext, dtType, symbol_table_obj.top_block());
            }
        }

        symbol_table_obj.pop_block();
        return function;
    }

    void * visit(ast_type_identifier * node) {
        return NULL;
    }

    void * visit(ast_statement * node) {
        llvm::BasicBlock * block = symbol_table_obj.top_block();
        if (block->getTerminator()) {
                // terminate any and all instructions which end the current block when there are still instructions to do
            llvm::Instruction * terminator = block->getTerminator();
            terminator->eraseFromParent();
        }
        
        if (dynamic_cast<ast_assignment_statement *>(node)) {
            auto toVst = dynamic_cast<ast_assignment_statement *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_block_statement *>(node)) {
            auto toVst = dynamic_cast<ast_block_statement *>(node);
            symbol_table_obj.push_block(NULL);
            this->visit(toVst);
            symbol_table_obj.pop_block();
            return NULL;
        }
        else if (dynamic_cast<ast_method_call *>(node)) {
            auto toVst = dynamic_cast<ast_method_call *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_if_statement *>(node)) {
            auto toVst = dynamic_cast<ast_if_statement *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_for_statement *>(node)) {
            auto toVst = dynamic_cast<ast_for_statement *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_return_statement *>(node)) {
            auto toVst = dynamic_cast<ast_return_statement *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_continue_statement *>(node)) {
            auto toVst = dynamic_cast<ast_continue_statement *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_break_statement *>(node)) {
            auto toVst = dynamic_cast<ast_break_statement *>(node);
            return this->visit(toVst);
        }
        else
            return ErrorHandler("Should Never Be Called"); // Should never be called.
    }

    void * visit(ast_expression * node) {
        if (dynamic_cast<ast_binary_operation_expression *>(node)){
            auto toVst = dynamic_cast<ast_binary_operation_expression *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_literal_expression *>(node)){
            auto toVst = dynamic_cast<ast_literal_expression *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_method_call *>(node)){
            auto toVst = dynamic_cast<ast_method_call *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_unary_operation_expression *>(node)){
            auto toVst = dynamic_cast<ast_unary_operation_expression *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_location *>(node)){
            auto toVst = dynamic_cast<ast_location *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_prnt_arg *>(node)){
            auto toVst = dynamic_cast<ast_prnt_arg *>(node);
            return this->visit(toVst);
        }
        else
            return ErrorHandler("Error with ast expression decision");
    }

    void * visit(ast_block_statement * node) {
        auto vldIdLst = node->get_id_list();
        
        if(vldIdLst){
            auto itrtr = vldIdLst->begin();
            while(itrtr != (node->get_id_list())->end()){
                this->visit(*itrtr);
                itrtr++;
            }
        }
        auto vldStmntLst = node->getStmtlist();
        if(vldStmntLst){
            auto itrtr = vldStmntLst->begin();
            while(itrtr != (node->getStmtlist())->end()) {
                this->visit(*itrtr);
                if (dynamic_cast<ast_return_statement *>(*itrtr) or dynamic_cast<ast_break_statement *>(*itrtr) or dynamic_cast<ast_continue_statement *>(*itrtr))
                    break;
                itrtr++;
            }
        }
        return NULL;
    }

    void * visit(ast_assignment_statement * node) {
        if(dynamic_cast<ast_array_location *>(node->getLocation()) && (!symbol_table_obj.lookup_global_vars(dynamic_cast<ast_array_location *>(node->getLocation())->get_id()))){
            return ErrorHandler("Variable Not Declared");
        }
        else{
            llvm::Value *location = NULL, *existingValue = NULL;
            string toArg;
            ast_array_location * arrayLocation = dynamic_cast<ast_array_location *>(node->getLocation());
            if (arrayLocation) {
                std::vector <llvm::Value *> index{llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true), static_cast<llvm::Value *>(this->visit(arrayLocation->get_index()))};
                toArg = "tmp";
                location = llvm::GetElementPtrInst::CreateInBounds(static_cast<llvm::Value *>(symbol_table_obj.return_locals(arrayLocation->get_id())), index, toArg, symbol_table_obj.top_block());
            }
            
            if (dynamic_cast<ast_var_location *>(node->getLocation())) {
                ast_var_location * varLocation = dynamic_cast<ast_var_location *>(node->getLocation());
                if (!symbol_table_obj.lookup_global_vars(varLocation->get_id())) {
                    return ErrorHandler("Variable Not Declared");
                }
                location = symbol_table_obj.return_locals(varLocation->get_id());
            }
            llvm::Value * expr = static_cast<llvm::Value *>(this->visit(node->get_expr()));
            if(node->get_op() == AssignOp::plus_equal){
                toArg = "load";
                existingValue = new llvm::LoadInst(location, toArg, symbol_table_obj.top_block());
                toArg = "tmp";
                expr = llvm::BinaryOperator::Create(llvm::Instruction::Add, existingValue, expr, toArg, symbol_table_obj.top_block());
            }
            else if(node->get_op() == AssignOp::minus_equal){
                toArg = "load";
                existingValue = new llvm::LoadInst(location, toArg, symbol_table_obj.top_block());
                toArg = "tmp";
                expr = llvm::BinaryOperator::Create(llvm::Instruction::Sub, existingValue, expr, toArg, symbol_table_obj.top_block());
            }

            if (!expr->getType()->isIntegerTy(64)) {
                return ErrorHandler("RHS of assignment statement is invalid");
            }
            else if (!location->getType()->isPointerTy()) {
                return ErrorHandler("LHS of assignment statement is invalid");
            }
            else{
                return new llvm::StoreInst(expr, location, false, symbol_table_obj.top_block());
            }
        }
    }

    void * visit(ast_method_call * node) {
        
        if (dynamic_cast<ast_prnt_method *>(node))
            return this->visit(dynamic_cast<ast_prnt_method *>(node));
        else if (dynamic_cast<ast_normal_method *>(node))
            return this->visit(dynamic_cast<ast_normal_method *>(node));
        else
            return ErrorHandler("Error, ast method call");
    }

    void * visit(ast_normal_method * node) {
        if (node->get_id() == "main") {
            return ErrorHandler("main function can not be called");
            
        }
        else if(!static_cast<llvm::Function *>(module->getFunction(node->get_id()))) {
            return ErrorHandler("No Function Defined");
        }
        else{
            llvm::Function * function = module->getFunction(node->get_id());
            if (!function->isVarArg() && (node->get_arguments()) && (function->arg_size() != node->get_arguments()->size())) {
                return ErrorHandler("Invalid Number of Arguments");
            }
            std::vector<llvm::Value *> args;
            auto vldArgmntLst = node->get_arguments();
            if (vldArgmntLst) {
                auto itrtr = vldArgmntLst->begin();
                while (itrtr != (node->get_arguments())->end()){
                    llvm::Value * toVst = static_cast<llvm::Value *>(this->visit(*itrtr));
                    args.push_back(toVst);
                    itrtr++;
                }
            }
            if (function->getReturnType()->isVoidTy()) {
                string toArg = "";
                return llvm::CallInst::Create(function, llvm::makeArrayRef(args), toArg, symbol_table_obj.top_block());
            }
            return llvm::CallInst::Create(function, llvm::makeArrayRef(args), node->get_id(), symbol_table_obj.top_block());
        }
    }

    void * visit(ast_prnt_method * node) {
        llvm::Function * function = module->getFunction(node->get_method_name());
        std::vector<llvm::Value *> args;
        if (function){
        }
        else
        {
            function = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getInt64Ty(MyGlobalContext), true), llvm::GlobalValue::ExternalLinkage, node->get_method_name(), module);
        }
        
        if(node->get_arguments())
        {
            auto it = (node->get_arguments())->begin();
            while (it != (node->get_arguments())->end()) {
                args.push_back(static_cast<llvm::Value *>(this->visit(*it)));
                it++;
            }
        }
        return static_cast<llvm::CallInst *>(llvm::CallInst::Create(function, llvm::makeArrayRef(args), node->get_method_name(), symbol_table_obj.top_block()));
    }

    void * visit(ast_prnt_arg * node) {
        if (dynamic_cast<ast_string_prnt_arg *>(node)){
            auto toVst = dynamic_cast<ast_string_prnt_arg *>(node);
            return this->visit(toVst);
        }
        else if (dynamic_cast<ast_expression_prnt_arg *>(node)){
            auto toVst = dynamic_cast<ast_expression_prnt_arg *>(node);
            return this->visit(toVst);
        }
        else{
            return ErrorHandler("Error, decision between string and expression prnt");
        }
    }

    void * visit(ast_string_prnt_arg * node)
    {
        string toArg = "string";
        int cntxtCnt = 8;
        bool blVl = true;
        llvm::GlobalVariable* variable = new llvm::GlobalVariable(*module, llvm::ArrayType::get(llvm::IntegerType::get(MyGlobalContext, cntxtCnt), node->get_argument().size() + 1), blVl, llvm::GlobalValue::InternalLinkage, NULL, toArg);
         bool addnll=true; 
        variable->setInitializer(llvm::ConstantDataArray::getString(MyGlobalContext, node->get_argument(), addnll));
        return variable;
    }

    void * visit(ast_expression_prnt_arg * node) {
        auto toVst = node->get_argument();
        auto rtrnVal = this->visit(toVst); 
        return rtrnVal;
    }

    void * visit(ast_if_statement * node) {
        llvm::BasicBlock * entryBlock = symbol_table_obj.top_block(), *returnedBlock = NULL, *ifBlock, *mergeBlock;
        llvm::Value * condition = static_cast<llvm::Value *>(this->visit(node->get_condition()));
        auto cmprsnNE = llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true);
        string toArg = "tmp";
        llvm::ICmpInst * comparison = new llvm::ICmpInst(*entryBlock, llvm::ICmpInst::ICMP_NE, condition, cmprsnNE, toArg);
        auto prntBlk = entryBlock->getParent();
        toArg = "ifBlock";
        ifBlock = llvm::BasicBlock::Create(MyGlobalContext, toArg, prntBlk);
        toArg = "mergeBlock";
        mergeBlock = llvm::BasicBlock::Create(MyGlobalContext, toArg, prntBlk);


        symbol_table_obj.push_block(ifBlock);
        auto ifBlkVst = node->getIf_block();
        this->visit(ifBlkVst);
        returnedBlock = symbol_table_obj.top_block();
        symbol_table_obj.pop_block();
        if (returnedBlock->getTerminator()) {
            auto prntBlck = entryBlock->getParent();
        }
        else{
            llvm::BranchInst::Create(mergeBlock, returnedBlock);
        }
        if (!node->getElse_block()) {
            llvm::BranchInst::Create(ifBlock, mergeBlock, comparison, entryBlock);
            
        } else {
            toArg = "elseBlock";
            llvm::BasicBlock * elseBlock = llvm::BasicBlock::Create(MyGlobalContext, toArg, prntBlk);

            symbol_table_obj.push_block(elseBlock);
            auto elseBlkVst = node->getElse_block();
            this->visit(elseBlkVst);
            returnedBlock = symbol_table_obj.top_block();
            symbol_table_obj.pop_block();
            if (returnedBlock->getTerminator()){
                llvm::BranchInst::Create(ifBlock, elseBlock, comparison, entryBlock);
            }
            else{
                llvm::BranchInst::Create(mergeBlock, returnedBlock);
                llvm::BranchInst::Create(ifBlock, elseBlock, comparison, entryBlock);
            }
        }
        auto localVariables = symbol_table_obj.get_local_variables();
        auto prntBlock = entryBlock->getParent();
        symbol_table_obj.pop_block();
        symbol_table_obj.push_block(mergeBlock);
        symbol_table_obj.set_local_variables(localVariables);
        return NULL;
    }

    void * visit(ast_for_statement * node) {
        auto ndeId = node->get_id();
        if (symbol_table_obj.lookup_global_vars(ndeId)) {
            bool clVal;
            llvm::BasicBlock * entryBlock = symbol_table_obj.top_block();
            auto entryPrntBlk = entryBlock->getParent();
            llvm::BasicBlock *headerBlock = llvm::BasicBlock::Create(MyGlobalContext, "loop_header", entryPrntBlk, 0), *bodyBlock = llvm::BasicBlock::Create(MyGlobalContext, "loop_body", entryPrntBlk, 0), *afterLoopBlock = llvm::BasicBlock::Create(MyGlobalContext, "after_loop", entryPrntBlk, 0);

            symbol_table_obj.push_break_continue_stack(afterLoopBlock, headerBlock);
            auto vstdNde = static_cast<llvm::Value *>(this->visit(node->getInit_condition()));
            clVal = 0;
            new llvm::StoreInst(vstdNde, symbol_table_obj.return_locals(ndeId), false, entryBlock);
            string toArg = "load";
            llvm::Value * val = new llvm::LoadInst(symbol_table_obj.return_locals(ndeId), toArg, headerBlock);
            toArg = "tmp";
            llvm::ICmpInst * comparison = new llvm::ICmpInst(*headerBlock, llvm::ICmpInst::ICMP_NE, val, static_cast<llvm::Value *>(this->visit(node->getEnd_condition())), toArg);
            llvm::BranchInst::Create(bodyBlock, afterLoopBlock, comparison, headerBlock), llvm::BranchInst::Create(headerBlock, entryBlock);
            // llvm::BranchInst::Create(headerBlock, entryBlock);
            symbol_table_obj.push_block(bodyBlock);
            this->visit(node->get_block());
            bodyBlock = symbol_table_obj.top_block();
            symbol_table_obj.pop_block();
            if (bodyBlock->getTerminator()){
                auto localVariables = symbol_table_obj.get_local_variables();
                symbol_table_obj.pop_block();
                symbol_table_obj.push_block(afterLoopBlock);
                symbol_table_obj.set_local_variables(localVariables);
                symbol_table_obj.pop_break_continue_stack();
                return NULL;
            }
            else {
                llvm::BranchInst::Create(headerBlock, bodyBlock);
                auto localVariables = symbol_table_obj.get_local_variables();
                symbol_table_obj.pop_block();
                symbol_table_obj.push_block(afterLoopBlock);
                symbol_table_obj.set_local_variables(localVariables);

                symbol_table_obj.pop_break_continue_stack();
                return NULL;
            }
        }
        else
            return ErrorHandler("Variable Not Declared");
    }

    void * visit(ast_return_statement * node) {
        llvm::Function * function;
        llvm::Type * type;
        function = symbol_table_obj.top_block()->getParent();
        type = function->getReturnType();
        if (type->isVoidTy() && (node->get_expr())){
            return ErrorHandler("Unknown Return for Void Type");
        }
        else if(type->isVoidTy()){
            return llvm::ReturnInst::Create(MyGlobalContext,symbol_table_obj.top_block());
        }
        else if (node->get_expr()){
            return llvm::ReturnInst::Create(MyGlobalContext, static_cast<llvm::Value *>(this->visit(node->get_expr())), symbol_table_obj.top_block());
        }
        return ErrorHandler("Void type return for non void function");
        
    }

    void * visit(ast_continue_statement * node) {
        // llvm::BasicBlock * block = symbol_table_obj.get_continue_stack();
        if (static_cast<llvm::BasicBlock *>(symbol_table_obj.get_continue_stack())) {
            llvm::BasicBlock * curBlock = symbol_table_obj.top_block();
            return llvm::BranchInst::Create(static_cast<llvm::BasicBlock *>(symbol_table_obj.get_continue_stack()), curBlock);
        }
        else{
            return ErrorHandler("Incorrect break usage");
        }
    }

    void * visit(ast_break_statement * node) {
        llvm::BasicBlock *block = symbol_table_obj.get_break_stack(), *curBlock = symbol_table_obj.top_block();
        if (block) {
            auto localVariables = symbol_table_obj.get_local_variables();
            symbol_table_obj.pop_block();
            symbol_table_obj.push_block(block);
            symbol_table_obj.set_local_variables(localVariables);
            return llvm::BranchInst::Create(block, curBlock);
        }
        else{
            return ErrorHandler("Incorrect break usage");
        }
        
    }

    void * visit(ast_location * node) {
        if (dynamic_cast<ast_array_location *>(node)){
            auto toVst = dynamic_cast<ast_array_location *>(node);
            return this->visit(toVst);
        }

        else if (dynamic_cast<ast_var_location *>(node)){
            auto toVst = dynamic_cast<ast_var_location *>(node);
            return this->visit(toVst);
        }
        else
            return ErrorHandler("Error with ast location decision between array and var");
    }

    void * visit(ast_var_location * node) {
        if (symbol_table_obj.lookup_global_vars(node->get_id())) {

            if (static_cast<llvm::Value *>(symbol_table_obj.return_locals(node->get_id()))){
                string toArg = "tmp";
                return new llvm::LoadInst(static_cast<llvm::Value *>(symbol_table_obj.return_locals(node->get_id())), toArg, symbol_table_obj.top_block());
            }
            else{
                return ErrorHandler("Variable Not Initilized");
            }
        }
        else{
            return ErrorHandler("Variable Not Declared");
        }
    }

    void * visit(ast_array_location * node) {
        if (symbol_table_obj.lookup_global_vars(node->get_id())) {
            std::vector <llvm::Value *> index{llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true), static_cast<llvm::Value *>(this->visit(node->get_index()))};

            string toArg = "tmp";
            llvm::Value * offset = llvm::GetElementPtrInst::CreateInBounds(symbol_table_obj.return_locals(node->get_id()), index, toArg, symbol_table_obj.top_block());
            if (symbol_table_obj.return_locals(node->get_id())) {
                return static_cast<llvm::LoadInst *>(new llvm::LoadInst(offset, "tmp", symbol_table_obj.top_block()));
            }
            else{
                return ErrorHandler("Variable Not Initilized");
            }
        }
        else{
            return ErrorHandler("Variable Not Declared");
        }
    }

    void * visit(ast_literal_expression * node) {
        
        if (dynamic_cast<ast_integer_literal_expression *>(node) != NULL) {
            return this->visit(dynamic_cast<ast_integer_literal_expression *>(node));
        }
        else if (dynamic_cast<ast_char_literal_expression *>(node) != NULL) {
            return this->visit(dynamic_cast<ast_char_literal_expression *>(node));
        }
        else if (dynamic_cast<ast_true_literal_expression *>(node) != NULL) {
            return this->visit(dynamic_cast<ast_true_literal_expression *>(node));
        }
        else if (dynamic_cast<ast_false_literal_expression *>(node) != NULL) {
            return this->visit(dynamic_cast<ast_false_literal_expression *>(node));
        }
        else{
            return ErrorHandler("Error, ast literal decision");
        }
        
    }

    void * visit(ast_integer_literal_expression * node) {
        auto ndeVal = node->get_value();
        bool sgnd = true;
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), ndeVal, sgnd);
    }

    void * visit(ast_char_literal_expression * node) {
        auto ndeVal = node->get_value();
        bool sgnd = true;
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), ndeVal, sgnd);
    }

    void * visit(ast_true_literal_expression * node) {
        auto ndeVal = node->get_value();
        bool sgnd = true;
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), ndeVal, sgnd);
    }

    void * visit(ast_false_literal_expression * node) {
        auto ndeVal = node->get_value();
        bool sgnd = true;
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), ndeVal, sgnd);
    }

    void * visit(ast_binary_operation_expression * node) {
        if(node->get_op() == bin_op::plus_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::Add, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::minus_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::Sub, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::multiply_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::Mul, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::divide_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::SDiv, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::modulo_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::SRem, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::lessthan_op)
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLT, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())),"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::greaterthan_op)
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SGT, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())),"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::lessequal_op)
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLE, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())),"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::greaterequal_op)
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SGE, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())),"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::notequal_op)
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_NE, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())),"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::equalequal_op)
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_EQ, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())),"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::and_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::And, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else if(node->get_op() == bin_op::or_op)
            return llvm::BinaryOperator::Create(llvm::Instruction::Or, static_cast<llvm::Value*>(this->visit(node->get_left())), static_cast<llvm::Value*>(this->visit(node->get_right())), "tmp", symbol_table_obj.top_block());

        else{
            return ErrorHandler("No Known BinaryOperator");
        }

    }

    void * visit(ast_unary_operation_expression * node) {
        if(node->get_op() == un_op::minus_op){
            llvm::Value* sndVal = static_cast<llvm::Value*>(this->visit(node->get_expr()));
            string toArg = "tmp";
            return llvm::BinaryOperator::Create(llvm::Instruction::Sub, llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true), sndVal, toArg, symbol_table_obj.top_block());
        }
        else if(node->get_op() == un_op::not_op){
            llvm::Value* sndVal = static_cast<llvm::Value*>(this->visit(node->get_expr()));
            string toArg = "tmp";
            return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_EQ, llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyGlobalContext), 0, true), sndVal,toArg, symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), "zext", symbol_table_obj.top_block());
        }

        return ErrorHandler("No Known UnaryOperator");
    }

};

#endif
