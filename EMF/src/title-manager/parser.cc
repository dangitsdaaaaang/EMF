
#include "lexer.h"
#include "token.h"
#include "error.h"
#include "pdx.h"

#include <cstdio>
#include <cassert>
#include <cstring>

void print_block(int indent, const pdx::block*);
void print_stmt(int indent, const pdx::stmt&);
void print_obj(int indent, const pdx::obj&);


int main(int argc, char** argv) {
  
  try {
    if (argc < 2)
      throw va_error("You must supply a filename!");

    pdx::plexer lex( argv[1] );
    pdx::block* root = new pdx::block(lex, true, true);
    print_block(0, root);
  }
  catch (std::exception& e) {
    fprintf(stderr, "fatal: %s\n", e.what());
    return 1;
  }

  return 0;
}


void print_block(int indent, const pdx::block* p_b) {
  for (auto&& s : p_b->stmt_list)
    print_stmt(indent, s);
}


void print_stmt(int indent, const pdx::stmt& s) {
  printf("%*s", indent, "");
  print_obj(indent, s.key);
  printf(" = ");
  print_obj(indent, s.val);
  printf("\n");
}


void print_obj(int indent, const pdx::obj& o) {

  using namespace pdx;

  if (o.type == obj::STR) {
    if (!strchr(o.data.s, ' ')) // not the only time to quote, but whatever
      printf("%s", o.data.s);
    else
      printf("\"%s\"", o.data.s);
  }
  else if (o.type == obj::INT) {
    printf("%d", o.data.i);
  }
  else if (o.type == obj::DECIMAL || o.type == obj::DATE || o.type == obj::TITLE)
    printf("%s", o.data.s);
  else if (o.type == obj::BLOCK) {
    printf("{\n");
    print_block(indent+3, o.data.p_block);
    printf("%*s}", indent, "");
  }
  else if (o.type == obj::LIST) {
    printf("{ ");

    for (auto&& i : o.data.p_list->obj_list) {
      print_obj(indent, i);
      printf(" ");
    }

    printf("}");
  }
  else if (o.type == obj::COLOR) {
    printf("{ %u %u %u }", o.data.color.r, o.data.color.g, o.data.color.b);
  }
  else {
    assert(false);
  }
}

