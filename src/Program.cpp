#include "Program.hpp"

#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() : programCounter_(-1), programEnd_(false) {}

void Program::addStmt(int line, Statement* stmt) { recorder_.add(line, stmt); }

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run() {
  // Start from the first line
  programCounter_ = -1;
  programEnd_ = false;

  // Get the first line
  int firstLine = recorder_.nextLine(programCounter_);
  if (firstLine == -1) {
    // No program lines
    resetAfterRun();
    return;
  }

  programCounter_ = firstLine;

  while (programCounter_ != -1 && !programEnd_) {
    const Statement* stmt = recorder_.get(programCounter_);
    if (!stmt) {
      throw BasicError("LINE NUMBER ERROR");
    }

    int currentLine = programCounter_;
    // Execute the statement
    stmt->execute(vars_, *this);

    // If PC wasn't changed by the statement (e.g., GOTO or IF), move to next
    // line
    if (programCounter_ == currentLine && !programEnd_) {
      programCounter_ = recorder_.nextLine(programCounter_);
    }
  }

  resetAfterRun();
}

void Program::list() const { recorder_.printLines(); }

void Program::clear() {
  recorder_.clear();
  vars_.clear();
}

void Program::execute(Statement* stmt) {
  // Execute a statement immediately (not part of the program)
  stmt->execute(vars_, *this);
}

int Program::getPC() const noexcept { return programCounter_; }

void Program::changePC(int line) {
  if (!recorder_.hasLine(line)) {
    throw BasicError("LINE NUMBER ERROR");
  }
  programCounter_ = line;
}

void Program::programEnd() { programEnd_ = true; }

void Program::resetAfterRun() noexcept {
  programCounter_ = -1;
  programEnd_ = false;
}