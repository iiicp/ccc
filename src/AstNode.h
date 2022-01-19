/**********************************
 * File:     AstNode.h
 *
 * Author:   caipeng
 *
 * Email:    iiicp@outlook.com
 *
 * Date:     2022/1/3
 ***********************************/

#ifndef C100_ASTNODE_H
#define C100_ASTNODE_H

#include <memory>
#include <list>
#include <string_view>
#include <vector>

namespace C100
{
  class AstVisitor;
  class Type;
  class AstNode
  {
  public:
    std::shared_ptr<Type> Ty;
    virtual ~AstNode() {}
    virtual void Accept(AstVisitor *visitor) {};
  };

  class Var
  {
  public:
    std::string_view Name;
    std::shared_ptr<Type> Ty;
    int Offset;
  };

  class ProgramNode : public AstNode
  {
  public:
    std::list<std::shared_ptr<AstNode>> Funcs;
    void Accept(AstVisitor *visitor) override;
  };

  class FunctionNode : public AstNode
  {
  public:
    std::string_view FuncName;
    std::list<std::shared_ptr<Var>> Params{};
    std::list<std::shared_ptr<Var>> Locals{};
    std::list<std::shared_ptr<AstNode>> Stmts{};

    void Accept(AstVisitor *visitor) override;
  };

  class ExprStmtNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Lhs{nullptr};
    void Accept(AstVisitor *visitor) override;
  };

  class IfStmtNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Cond{nullptr};
    std::shared_ptr<AstNode> Then{nullptr};
    std::shared_ptr<AstNode> Else{nullptr};

    void Accept(AstVisitor *visitor) override;
  };

  class WhileStmtNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Cond{nullptr};
    std::shared_ptr<AstNode> Then{nullptr};

    void Accept(AstVisitor *visitor) override;
  };

  class DoWhileStmtNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Stmt{nullptr};
    std::shared_ptr<AstNode> Cond{nullptr};

    void Accept(AstVisitor *visitor) override;
  };

  class ForStmtNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Init{nullptr};
    std::shared_ptr<AstNode> Cond{nullptr};
    std::shared_ptr<AstNode> Inc{nullptr};
    std::shared_ptr<AstNode> Stmt{nullptr};

    void Accept(AstVisitor *visitor) override;
  };

  class BlockStmtNode : public AstNode
  {
  public:
    std::list<std::shared_ptr<AstNode>> Stmts;
    void Accept(AstVisitor *visitor) override;
  };

  class ReturnStmtNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Lhs;
    void Accept(AstVisitor *visitor) override;
  };

  class AssignExprNode : public AstNode
  {
  public:
    std::shared_ptr<AstNode> Lhs;
    std::shared_ptr<AstNode> Rhs;
    void Accept(AstVisitor *visitor) override;
  };

  class DeclarationStmtNode : public AstNode
  {
  public:
    std::list<std::shared_ptr<AssignExprNode>> AssignNodes;
    void Accept(AstVisitor *visitor) override;
  };

  enum class BinaryOperator
  {
    Add,
    Sub,
    Mul,
    Div,
    Equal,
    PipeEqual,
    Greater,
    GreaterEqual,
    Lesser,
    LesserEqual
  };

  class BinaryNode : public AstNode
  {
  public:
    BinaryOperator BinOp;
    std::shared_ptr<AstNode> Lhs;
    std::shared_ptr<AstNode> Rhs;
    void Accept(AstVisitor *visitor) override;
  };

  enum class UnaryOperator
  {
    Plus,
    Minus,
    Deref,
    Amp
  };
  class UnaryNode : public AstNode
  {
  public:
    UnaryOperator Uop;
    std::shared_ptr<AstNode> Lhs;
    void Accept(AstVisitor *visitor) override;
  };

  class ConstantNode : public AstNode
  {
  public:
    int Value;
    void Accept(AstVisitor *visitor) override;
  };

  class VarExprNode : public AstNode
  {
  public:
    std::shared_ptr<Var> VarObj;
    void Accept(AstVisitor *visitor) override;
  };

  class FuncCallNode : public AstNode
  {
  public:
    std::string_view FuncName;
    std::vector<std::shared_ptr<AstNode>> Args;

    void Accept(AstVisitor *visitor) override;
  };

  class StmtExprNode : public AstNode
  {
  public:
    std::list<std::shared_ptr<AstNode>> Stmts;
    void Accept(AstVisitor *visitor) override;
  };

  class AstVisitor
  {
  public:
    virtual ~AstVisitor(){}
    virtual void VisitorProgramNode(ProgramNode *node) = 0;
    virtual void VisitorFunctionNode(FunctionNode *node) = 0;
    virtual void VisitorFuncCallNode(FuncCallNode *node) = 0;
    virtual void VisitorExprStmtNode(ExprStmtNode *node) = 0;
    virtual void VisitorIfStmtNode(IfStmtNode *node) = 0;
    virtual void VisitorWhileStmtNode(WhileStmtNode *node) = 0;
    virtual void VisitorDoWhileStmtNode(DoWhileStmtNode *node) = 0;
    virtual void VisitorForStmtNode(ForStmtNode *node) = 0;
    virtual void VisitorBlockStmtNode(BlockStmtNode *node)  = 0;
    virtual void VisitorReturnStmtNode(ReturnStmtNode *node)  = 0;
    virtual void VisitorDeclarationStmtNode(DeclarationStmtNode *node) = 0;
    virtual void VisitorStmtExprNode(StmtExprNode *node) = 0;
    virtual void VisitorAssignExprNode(AssignExprNode *node) = 0;
    virtual void VisitorBinaryNode(BinaryNode *node) = 0;
    virtual void VisitorUnaryNode(UnaryNode *node) = 0;
    virtual void VisitorConstantNode(ConstantNode *node) = 0;
    virtual void VisitorVarExprNode(VarExprNode *node) = 0;
  };
}

#endif // C100_ASTNODE_H
