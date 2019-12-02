#ifndef _expressionGenVisitor_H
#define _expressionGenVisitor_H

void * visit(ast_literal_expression * node) {
    if (dynamic_cast<ast_integer_literal_expression *>(node)) {
        auto toVst = dynamic_cast<ast_integer_literal_expression *>(node);
        return this->visit(toVst);
    }
    else if (dynamic_cast<ast_char_literal_expression *>(node)) {
        auto toVst = dynamic_cast<ast_char_literal_expression *>(node);
        return this->visit(toVst);
    }
    else if (dynamic_cast<ast_true_literal_expression *>(node)) {
        auto toVst = dynamic_cast<ast_true_literal_expression *>(node);
        return this->visit(toVst);
    }
    else if (dynamic_cast<ast_false_literal_expression *>(node)) {
        auto toVst = dynamic_cast<ast_false_literal_expression *>(node);
        return this->visit(toVst);
    }
    else{
        return ErrorHandler("Error, ast literal expression decision");
    }
}

void * visit(ast_integer_literal_expression * node) {
    auto ndeVal = node->get_value();
    auto dtTyp = llvm::Type::getInt64Ty(MyGlobalContext);
    return llvm::ConstantInt::get(dtTyp, ndeVal, true);
}

void * visit(ast_char_literal_expression * node) {
    auto ndeVal = node->get_value();
    auto dtTyp = llvm::Type::getInt64Ty(MyGlobalContext);
    return llvm::ConstantInt::get(dtTyp, ndeVal, true);
}

void * visit(ast_true_literal_expression * node) {
    auto ndeVal = node->get_value();
    auto dtTyp = llvm::Type::getInt64Ty(MyGlobalContext);
    return llvm::ConstantInt::get(dtTyp, ndeVal, true);
}

void * visit(ast_false_literal_expression * node) {
    auto ndeVal = node->get_value();
    auto dtTyp = llvm::Type::getInt64Ty(MyGlobalContext);
    return llvm::ConstantInt::get(dtTyp, ndeVal, true);
}

void * visit(ast_binary_operation_expression * node) {

    auto opTyp = node->get_op();
    
    if(opTyp == bin_op::plus_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::Add, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::minus_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::Sub, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::multiply_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::Mul, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::divide_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::SDiv, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::module_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::SRem, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::divide_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::SDiv, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::lessthan_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "zext";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLT, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::greaterthan_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "zext";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SGT, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::lessequal_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "zext";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLE, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::greaterequal_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "zext";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SGE, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::notequal_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "zext";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_NE, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::equalequal_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "zext";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_EQ, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::and_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLE, lftNdeVal, rtNdeVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::or_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::And, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else if(opTyp == bin_op::or_op){
        auto lftNdeVal = static_cast<llvm::Value*>(this->visit(node->get_left()));
        auto rtNdeVal  = static_cast<llvm::Value*>(this->visit(node->get_right()));
        string toArg = "tmp";
        return llvm::BinaryOperator::Create(llvm::Instruction::Or, lftNdeVal, rtNdeVal, toArg, symbol_table_obj.top_block());
    }
    else{
        return ErrorHandler("Error, did not match any known binary operator");
    }
}

void * visit(ast_unary_operation_expression * node) {
    auto ndeOp = node->get_op();
    if(ndeOp == un_op::minus_op){
        auto exrVal = static_cast<llvm::Value*>(this->visit(node->get_expr()));
        auto toArg = "tmp";
        auto dtTyp = llvm::Type::getInt64Ty(MyGlobalContext);
        return llvm::BinaryOperator::Create(llvm::Instruction::Sub, llvm::ConstantInt::get(dtTyp, 0, true), exrVal, toArg, symbol_table_obj.top_block());
    }
    else if(ndeOp == un_op::not_op){
        auto exrVal = static_cast<llvm::Value*>(this->visit(node->get_expr()));
        auto toArg = "zext";
        auto dtTyp = llvm::Type::getInt64Ty(MyGlobalContext);
        return new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_EQ, llvm::ConstantInt::get(dtTyp, 0, true), exrVal,"tmp", symbol_table_obj.top_block()), llvm::Type::getInt64Ty(MyGlobalContext), toArg, symbol_table_obj.top_block());
    }
    else{
        return ErrorHandler("Error, no known unary operator");
    }
}

#endif
