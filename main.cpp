#include <clang-c/Index.h>

#include <iostream>

std::string Convert(const CXString& s)
{
   std::string result = clang_getCString(s);
   clang_disposeString(s);
   return result;
}


CXChildVisitResult visitor(CXCursor cursor, CXCursor, CXClientData) {
  CXCursorKind cursor_kind = clang_getCursorKind(cursor);
  CXString cursor_kind_name = clang_getCursorKindSpelling(cursor_kind);
  std::string cursor_kind_str = Convert(cursor_kind_name);

  CXString cursor_spelling = clang_getCursorSpelling(cursor);
  std::string cursor_spelling_str = Convert(cursor_spelling);

  CXType cursor_type = clang_getCursorType(cursor);
  std::string cursor_type_str  = Convert(clang_getTypeSpelling(cursor_type));

  CXCursor parent = clang_getCursorLexicalParent(cursor);
  std::string parent_name_str = Convert(clang_getCursorDisplayName(parent));

  std::cout<< cursor_spelling_str <<"--------" << cursor_kind_str<<" >>>>>> " << parent_name_str << cursor_type_str << std::endl;
  return CXChildVisit_Recurse;
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
