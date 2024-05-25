#ifndef CEGEX_H
#define CEGEX_H

#include<string.h>
#include<stdlib.h>

#include "da.h"

#define CEGEX_ENABLE_LOG
#ifdef CEGEX_ENABLE_LOG
#define CEGEX_ERROR(fmt, ...) printf("[CEGEX ERROR] %s", fmt, ##__VA_ARGS__) 
#else 
#define CEGEX_ERROR(fmt, ...) 
#endif // DEBUG



typedef struct{
  int index; // if not found index is -1
  size_t length;
}CegexValue;

// TOKENIZER START
// Usable Regex item types like *, ? or plain text 
typedef enum{
    TOKEN_CHAR,           // a
    TOKEN_STAR,           // *
    TOKEN_PLUS,           // +
    TOKEN_QUESTION,       // ?
    TOKEN_L_ROUND_PAREN,  // (
    TOKEN_R_ROUND_PAREN,  // )
    TOKEN_PIPE,           // |
    TOKEN_DOT,            // .
    TOKEN_ESCAPE,         /* \  */
    TOKEN_END,            // End of regex "\0"
} TokenType;


typedef struct{
  TokenType type;
  char value;
} Token;

typedef struct {
  Token *items;
  size_t count;
  size_t capacity;
}Tokens;

// Convert given regex to Token dynamic array
Tokens* tokenize(char* regex){
  int length = strlen(regex);
  Tokens* tokens= malloc(sizeof(Tokens));
  if(NULL == tokens){
    CEGEX_ERROR("Regex tokenizer tokens malloc error!");
    return NULL;
  }
  tokens->items= malloc(sizeof(Tokens) * length);

  if(NULL == tokens->items){
    CEGEX_ERROR("Regex tokenizer tokens.item malloc error!");
    return NULL;
  }
  tokens->count= 0;

  for(size_t i=0; i<length; i++){
    Token token={0};
    switch(regex[i]) {
      case '*':
        token.type = TOKEN_STAR;
        break;
      case '+':
        token.type = TOKEN_PLUS;
        break;
      case '?':
        token.type = TOKEN_QUESTION;
        break;
      case '(':
        token.type = TOKEN_L_ROUND_PAREN;
        break;
      case ')':
        token.type = TOKEN_R_ROUND_PAREN;
        break;
      case '|':
        token.type = TOKEN_PIPE;
        break;
      case '.':
        token.type = TOKEN_DOT;
        break;
      case '\\':
        token.type = TOKEN_CHAR;
        token.value = regex[++i];
        break;
      default:
        token.type=TOKEN_CHAR;
        token.value = regex[i];
        break;
    }
    DA_PUSH(*tokens, token);
  }
  Token t = {TOKEN_END, 0};
  DA_PUSH(*tokens, t);
  return tokens;
}

// TOKENIZER END




// PARSE START

typedef enum {
  ZERO_OR_ONE,
  ONLY_ONE,
  ONE_OR_MANY,
} Quantifier;

typedef enum{
  WILDCHARD,      // .
  ELEMENT,        // a b c
  GROUP_ELEMENT,  // ( )
} ElementType;


typedef struct Element_t Element;

typedef struct{
  Element* items;
  size_t count;
  size_t capacity;
}Scope;

typedef struct{
  Scope* items;
  size_t count;
  size_t capacity;
}Stack;

struct Element_t{
  ElementType type;
  Quantifier quantifier;
  char value;
  Stack stack;
};

Element element_init(){
  Element e;
  return (Element){
    .type= ELEMENT,
    .quantifier= ZERO_OR_ONE,
    .value= '\n',
    .stack = (Stack){
      NULL,
      0,
      0
    }
  };
}

Scope scope_create(){
  Scope s;
  memset(&s, 0, sizeof(Scope));
  s.items =  malloc(sizeof(ELEMENT)*1);
  memset(s.items, 0, sizeof(ELEMENT));
  return s;
}

Stack stack_create(){
  Stack s;
  memset(&s, 0, sizeof(Stack));
  s.items =  malloc(sizeof(Scope)*1);
  memset(s.items, 0, sizeof(Scope));
  return s;
}

Scope* stack_get_last_scope(const Stack* s){
  return &s->items[s->count-1];
}

Stack parse(Tokens *tokens){
  Stack stack= stack_create();
  Scope fs = scope_create();
  DA_PUSH(stack, fs);

  for(int i=0; i<tokens->count; i++){
    Token token = tokens->items[i];
    Scope* currentScope = stack_get_last_scope(&stack);

    Element el=element_init();
    switch(token.type){
      case TOKEN_DOT:
        el.type = WILDCHARD;
        el.quantifier = ONLY_ONE;
        break;
      case TOKEN_L_ROUND_PAREN: // add everything until )
        Scope s = scope_create();
        DA_PUSH(stack, s);
        continue;
      case TOKEN_R_ROUND_PAREN:
        if(stack.count<=1){
          CEGEX_ERROR("Parse Error: No group to close at index %i\n",i);
          exit(1);
        }
        el.type = GROUP_ELEMENT;
        el.quantifier = ONLY_ONE;
        el.stack = stack_create();
        DA_PUSH(el.stack,  DA_POP(stack));
        break;
      case TOKEN_QUESTION:
          Element *lastEl = &DA_GET_LAST(*currentScope);
          if(lastEl->quantifier != ONLY_ONE){
            CEGEX_ERROR("Parse Error: ? operator must be using after element or group\n",i);
            exit(1);
          }
          lastEl->quantifier = ZERO_OR_ONE;
          continue;
      case TOKEN_STAR:
          Element *lEl = &DA_GET_LAST(*currentScope);
          if(lEl->quantifier != ONLY_ONE){
            CEGEX_ERROR("Parse Error: * operator must be using after element or group\n",i);
            exit(1);
          }
          lEl->quantifier = ZERO_OR_ONE;
          continue;
      case TOKEN_PLUS:
          Element *lEl2 = &DA_GET_LAST(*currentScope);
          if(lEl2->quantifier != ONLY_ONE){
            CEGEX_ERROR("Parse Error: + operator must be using after element or group\n",i);
            exit(1);
          }
          lEl2->quantifier = ONE_OR_MANY;
          continue;
      default:
        el.type = ELEMENT;
        el.value = token.value;
        el.quantifier = ONLY_ONE;
    }
    DA_PUSH(*currentScope, el)
  }
  return stack;
}

// PERSER END


CegexValue match(Stack*s, char* text){
  CegexValue res ={0};

  do{
    Scope currentScope = DA_SHIFT(*s);

    while(currentScope.count >=1){
      Element el = DA_SHIFT(currentScope);

      // match conditions here
    }
  }while(s->count >=1);
  return res;
}
//
// Regex matcher
//
CegexValue cegex_match(char* regex, char* text){
  CegexValue res={0};
  if(0==strlen(regex)){
    // add optiization
  }

  Tokens *tokens = tokenize(regex);
  if(NULL == tokens){
    res.index = -1;
    return res;
  }

  Stack stack = parse(tokens);

  return match(&stack, text);
}

#endif
