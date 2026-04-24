#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// LetStatement implementation
LetStatement::LetStatement(std::string source, std::string varName,
                           Expression* expr)
    : Statement(std::move(source)),
      varName_(std::move(varName)),
      expr_(expr) {}

LetStatement::~LetStatement() { delete expr_; }

void LetStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  state.setValue(varName_, value);
}

// PrintStatement implementation
PrintStatement::PrintStatement(std::string source, Expression* expr)
    : Statement(std::move(source)), expr_(expr) {}

PrintStatement::~PrintStatement() { delete expr_; }

void PrintStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  std::cout << value << "\n";
}

// InputStatement implementation
InputStatement::InputStatement(std::string source, std::string varName)
    : Statement(std::move(source)), varName_(std::move(varName)) {}

void InputStatement::execute(VarState& state, Program& program) const {
  int value;
  while (true) {
    std::cout << " ? " << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) {
      throw BasicError("INPUT ERROR");
    }
    
    // Try to parse the line as an integer
    std::istringstream iss(line);
    if (!(iss >> value)) {
      std::cout << "INVALID NUMBER\n";
      continue;
    }
    
    // Check if there's any non-whitespace character after the number
    std::string remaining;
    if (iss >> remaining) {
      std::cout << "INVALID NUMBER\n";
      continue;
    }
    
    break;
  }
  state.setValue(varName_, value);
}

// GotoStatement implementation
GotoStatement::GotoStatement(std::string source, int targetLine)
    : Statement(std::move(source)), targetLine_(targetLine) {}

void GotoStatement::execute(VarState& state, Program& program) const {
  program.changePC(targetLine_);
}

// IfStatement implementation
IfStatement::IfStatement(std::string source, Expression* leftExpr, char op,
                         Expression* rightExpr, int targetLine)
    : Statement(std::move(source)),
      leftExpr_(leftExpr),
      rightExpr_(rightExpr),
      op_(op),
      targetLine_(targetLine) {}

IfStatement::~IfStatement() {
  delete leftExpr_;
  delete rightExpr_;
}

void IfStatement::execute(VarState& state, Program& program) const {
  int leftValue = leftExpr_->evaluate(state);
  int rightValue = rightExpr_->evaluate(state);

  bool condition = false;
  switch (op_) {
    case '=':
      condition = (leftValue == rightValue);
      break;
    case '<':
      condition = (leftValue < rightValue);
      break;
    case '>':
      condition = (leftValue > rightValue);
      break;
    default:
      throw BasicError("INVALID OPERATOR");
  }

  if (condition) {
    program.changePC(targetLine_);
  }
}

// RemStatement implementation
RemStatement::RemStatement(std::string source)
    : Statement(std::move(source)) {}

void RemStatement::execute(VarState& state, Program& program) const {
  // REM statements do nothing
}

// EndStatement implementation
EndStatement::EndStatement(std::string source)
    : Statement(std::move(source)) {}

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}
