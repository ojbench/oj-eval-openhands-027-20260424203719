#include "Recorder.hpp"

#include <iostream>

Recorder::~Recorder() {
  for (auto& pair : lines_) {
    delete pair.second;
  }
}

void Recorder::add(int line, Statement* stmt) {
  // If line already exists, delete the old statement
  if (lines_.count(line)) {
    delete lines_[line];
  }
  lines_[line] = stmt;
}

void Recorder::remove(int line) {
  auto it = lines_.find(line);
  if (it != lines_.end()) {
    delete it->second;
    lines_.erase(it);
  }
}

const Statement* Recorder::get(int line) const noexcept {
  auto it = lines_.find(line);
  if (it != lines_.end()) {
    return it->second;
  }
  return nullptr;
}

bool Recorder::hasLine(int line) const noexcept {
  return lines_.count(line) > 0;
}

void Recorder::clear() noexcept {
  for (auto& pair : lines_) {
    delete pair.second;
  }
  lines_.clear();
}

void Recorder::printLines() const {
  for (const auto& pair : lines_) {
    std::cout << pair.first << " " << pair.second->text() << "\n";
  }
}

int Recorder::nextLine(int line) const noexcept {
  auto it = lines_.upper_bound(line);
  if (it != lines_.end()) {
    return it->first;
  }
  return -1;
}