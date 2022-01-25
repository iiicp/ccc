/***********************************
* File:     Type.h
*
* Author:   caipeng
*
* Email:    iiicp@outlook.com
*
* Date:     2022/1/16
***********************************/

#ifndef C100_SRC_TYPE_H
#define C100_SRC_TYPE_H
#include <memory>
#include <vector>
#include "Lexer.h"
#include "AstNode.h"

namespace C100
{
  class BuildInType;
  class FunctionType;
  class PointerType;
  class ArrayType;
  class Type
  {
  public:
    enum class TypeClass
    {
      BuildInType,
      FunctionType,
      PointerType,
      ArrayType
    };
    int Size;
    int Align;
  private:
    TypeClass TC;

  public:
    static std::shared_ptr<BuildInType> CharType;
    static std::shared_ptr<BuildInType> ShortType;
    static std::shared_ptr<BuildInType> IntType;
    static std::shared_ptr<BuildInType> LongType;

  public:
    virtual ~Type() {}
    Type(TypeClass tc, int size, int align) : TC(tc), Size(size), Align(align) {}

    bool IsIntegerType() const;
    bool IsPointerType() const;
    bool IsFunctionType() const;
    bool IsArrayType() const;
  };

  class BuildInType : public Type
  {
  public:
    enum class Kind
    {
      Char,
      Short,
      Int,
      Long
    };
  private:
    Kind Kd;
  public:
    BuildInType(Kind kd, int size, int align)
    : Type(TypeClass::BuildInType, size, align), Kd(kd) {}

    Kind GetKind() const {
      return Kd;
    }
  };

  struct Param
  {
    std::shared_ptr<Type> Ty;
    std::shared_ptr<Token> Tok;
  };
  class FunctionType : public Type
  {
  private:
    std::shared_ptr<Type> ReturnType;
  public:
    std::list<std::shared_ptr<Param>> Params;
    FunctionType(std::shared_ptr<Type> returnTy)
        : Type(TypeClass::FunctionType, 8, 8), ReturnType(returnTy) {}
  };

  class PointerType : public Type
  {
  public:
    std::shared_ptr<Type> Base;
    PointerType(std::shared_ptr<Type> base)
        : Type(TypeClass::PointerType, 8, 8), Base(base) {}
  };

  class ArrayType : public Type
  {
  public:
    std::shared_ptr<Type> ElementType;
    int ArrayLen;
    ArrayType(std::shared_ptr<Type> elementType, int len)
    : Type(TypeClass::ArrayType, len * elementType->Size, elementType->Align),
    ElementType(elementType), ArrayLen(len){}
  };

  class TypeVisitor : public AstVisitor
  {
  public:
    void VisitorProgramNode(ProgramNode *node) override;
    void VisitorFunctionNode(FunctionNode *node) override;
    void VisitorFuncCallNode(FuncCallNode *node) override;
    void VisitorExprStmtNode(ExprStmtNode *node) override;
    void VisitorIfStmtNode(IfStmtNode *node) override;
    void VisitorWhileStmtNode(WhileStmtNode *node) override;
    void VisitorDoWhileStmtNode(DoWhileStmtNode *node) override;
    void VisitorForStmtNode(ForStmtNode *node) override;
    void VisitorBlockStmtNode(BlockStmtNode *node)  override;
    void VisitorReturnStmtNode(ReturnStmtNode *node)  override;
    void VisitorStmtExprNode(StmtExprNode *node) override;
    void VisitorDeclarationStmtNode(DeclarationStmtNode *node) override;
    void VisitorAssignExprNode(AssignExprNode *node) override;
    void VisitorSizeofExprNode(SizeofExprNode *node) override;
    void VisitorBinaryNode(BinaryNode *node) override;
    void VisitorUnaryNode(UnaryNode *node) override;
    void VisitorNumNode(NumNode *node) override;
    void VisitorVarExprNode(VarExprNode *node) override;

    static TypeVisitor *Visitor();
  };
}

#endif //C100_SRC_TYPE_H
