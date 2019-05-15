#include <clang-c/Index.h>

#include <iostream>

std::string Convert(const CXString& s)
{
   std::string result = clang_getCString(s);
   clang_disposeString(s);
   return result;
}

CXChildVisitResult visitor(CXCursor cursor, CXCursor, CXClientData) {
  CXCursorKind kind = clang_getCursorKind(cursor);
  CXString kindName = clang_getCursorKindSpelling( kind );
  CXCursor parent = clang_getCursorLexicalParent (cursor);
  auto type = clang_getCursorType(cursor);
  auto typeStr  = Convert(clang_getTypeSpelling(type));
  std::string result = clang_getCString( kindName );
  std::string parentNameStr = clang_getCString(clang_getCursorDisplayName(parent));

  CXString cursorSpelling = clang_getCursorSpelling( cursor );
  std::string curSpellingStr = clang_getCString( cursorSpelling );
  std::cout << typeStr <<std::endl;
  //std::cout<< curSpellingStr <<"--------" << result<<" >>>>>> " << parentNameStr << typeStr << std::endl;
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
