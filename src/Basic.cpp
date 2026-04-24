#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // Tokenize the line
      TokenStream tokens = lexer.tokenize(line);

      // Check if it's an interpreter command
      const Token* firstToken = tokens.peek();
      if (!firstToken) {
        continue;
      }

      // Handle interpreter commands (RUN, LIST, CLEAR, QUIT, HELP)
      if (firstToken->type == TokenType::RUN) {
        program.run();
        continue;
      } else if (firstToken->type == TokenType::LIST) {
        program.list();
        continue;
      } else if (firstToken->type == TokenType::CLEAR) {
        program.clear();
        continue;
      } else if (firstToken->type == TokenType::QUIT) {
        break;
      } else if (firstToken->type == TokenType::HELP) {
        // HELP command - print help information
        continue;
      }

      // Parse the line
      ParsedLine parsed = parser.parseLine(tokens, line);

      // Check if it has a line number
      auto lineNum = parsed.getLine();
      if (lineNum.has_value()) {
        // It's a program line
        Statement* stmt = parsed.fetchStatement();
        if (stmt == nullptr) {
          // Just a line number - delete that line
          program.removeStmt(lineNum.value());
        } else {
          // Add or replace the line
          program.addStmt(lineNum.value(), stmt);
        }
      } else {
        // It's an immediate command
        Statement* stmt = parsed.fetchStatement();
        if (stmt) {
          program.execute(stmt);
          delete stmt;
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}