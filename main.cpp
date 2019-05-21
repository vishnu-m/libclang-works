#include <clang-c/Index.h>
#include <iostream>

std::string convert(const CXString& s){
   std::string result = clang_getCString(s);
   clang_disposeString(s);
   return result;
}

std::string get_cursor_kind(CXCursor cursor){
  CXCursorKind cursor_kind = clang_getCursorKind(cursor);
  CXString cursor_kind_name = clang_getCursorKindSpelling(cursor_kind);
  std::string cursor_kind_str = convert(cursor_kind_name);
  return cursor_kind_str;
}

std::string get_cursor_type(CXCursor cursor){
  CXType cursor_type = clang_getCursorType(cursor);
  std::string cursor_type_str  = convert(clang_getTypeSpelling(cursor_type));
  return cursor_type_str;
}

std::string get_cursor_spelling(CXCursor cursor){
  CXString cursor_spelling = clang_getCursorSpelling(cursor);
  std::string cursor_spelling_str = convert(cursor_spelling);
  return cursor_spelling_str;
}

std::string get_parent(CXCursor cursor){
  CXCursor parent = clang_getCursorLexicalParent(cursor);
  std::string parent_name_str = convert(clang_getCursorDisplayName(parent));
  return parent_name_str;
}

CXChildVisitResult enum_visitor(CXCursor cursor, CXCursor parent, CXClientData) {
  if(get_cursor_kind(cursor) == "IntegerLiteral"){
    long long enum_const_value = clang_getEnumConstantDeclValue(parent);
    std::cout << "IntegerLiteral" << "   " << enum_const_value << "  Parent = " << get_cursor_spelling(parent) << std::endl;
  }
  else if(get_cursor_kind(cursor) == "EnumConstantDecl"){
    long long enum_const_value = clang_getEnumConstantDeclValue(cursor);
    std::cout << "EnumConstantDecl" << "   " << get_cursor_spelling(cursor) << "  Parent = " << get_parent(cursor) << std::endl;
  }
  
  return CXChildVisit_Recurse;
}

void enum_handler(CXCursor cursor){
  std::cout << "EnumDecl" << "   " << get_cursor_spelling(cursor) << std::endl;
  clang_visitChildren(cursor, enum_visitor, 0);
}

CXChildVisitResult visitor(CXCursor cursor, CXCursor, CXClientData) {
  if(get_cursor_kind(cursor) == "EnumDecl"){
    enum_handler(cursor);
  }
 
  return CXChildVisit_Continue;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  // Command line arguments required for parsing the TU
  constexpr const char *ARGUMENTS[] = {};

  // Create an index with excludeDeclsFromPCH = 1, displayDiagnostics = 0
  CXIndex index = clang_createIndex(1, 0);

  // Speed up parsing by skipping function bodies
  CXTranslationUnit translationUnit = clang_parseTranslationUnit(
      index, argv[1], ARGUMENTS, std::extent<decltype(ARGUMENTS)>::value,
      nullptr, 0, CXTranslationUnit_SkipFunctionBodies);

  // Visit all the nodes in the AST
  CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
  clang_visitChildren(cursor, visitor, 0);

  // Release memory
  clang_disposeTranslationUnit(translationUnit);
  clang_disposeIndex(index);

  return 0;
}
