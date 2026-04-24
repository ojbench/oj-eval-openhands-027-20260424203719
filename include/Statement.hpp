#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// LET statement: LET var = expr
class LetStatement : public Statement {
 public:
  LetStatement(std::string source, std::string varName, Expression* expr);
  ~LetStatement();
  void execute(VarState& state, Program& program) const override;

 private:
  std::string varName_;
  Expression* expr_;
};

// PRINT statement: PRINT expr
class PrintStatement : public Statement {
 public:
  PrintStatement(std::string source, Expression* expr);
  ~PrintStatement();
  void execute(VarState& state, Program& program) const override;

 private:
  Expression* expr_;
};

// INPUT statement: INPUT var
class InputStatement : public Statement {
 public:
  InputStatement(std::string source, std::string varName);
  void execute(VarState& state, Program& program) const override;

 private:
  std::string varName_;
};

// GOTO statement: GOTO line
class GotoStatement : public Statement {
 public:
  GotoStatement(std::string source, int targetLine);
  void execute(VarState& state, Program& program) const override;

 private:
  int targetLine_;
};

// IF statement: IF expr1 op expr2 THEN line
class IfStatement : public Statement {
 public:
  IfStatement(std::string source, Expression* leftExpr, char op,
              Expression* rightExpr, int targetLine);
  ~IfStatement();
  void execute(VarState& state, Program& program) const override;

 private:
  Expression* leftExpr_;
  Expression* rightExpr_;
  char op_;
  int targetLine_;
};

// REM statement: REM comment
class RemStatement : public Statement {
 public:
  RemStatement(std::string source);
  void execute(VarState& state, Program& program) const override;
};

// END statement: END
class EndStatement : public Statement {
 public:
  EndStatement(std::string source);
  void execute(VarState& state, Program& program) const override;
};
