#ifndef _statementGenVisitor_H
#define _statementGenVisitor_H

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
        symbol_table_obj.push_block(NULL);
        auto toVst = dynamic_cast<ast_block_statement *>(node);
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
    else{
        return ErrorHandler("Error, deciding ast statement");
    }
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
    if (static_cast<llvm::BasicBlock *>(symbol_table_obj.get_continue_stack())) {
        return llvm::BranchInst::Create(static_cast<llvm::BasicBlock *>(symbol_table_obj.get_continue_stack()), static_cast<llvm::BasicBlock *>(symbol_table_obj.top_block()));
    }
    else{
        return ErrorHandler("Error, incorrect use of continue statement");
    }
    
}

void * visit(ast_break_statement * node) {

    if (static_cast<llvm::BasicBlock *>(symbol_table_obj.get_break_stack())) {
        auto localVariables = symbol_table_obj.get_local_variables();
        symbol_table_obj.pop_block();
        symbol_table_obj.push_block(static_cast<llvm::BasicBlock *>(symbol_table_obj.get_break_stack()));
        symbol_table_obj.set_local_variables(localVariables);
        return llvm::BranchInst::Create(static_cast<llvm::BasicBlock *>(symbol_table_obj.get_break_stack()), static_cast<llvm::BasicBlock *> symbol_table_obj.top_block());
    }
    else{
        return ErrorHandler("Error, incorrect use of break statement");
    }
    
}

#endif
