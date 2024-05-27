#ifndef CEGEX_H
#define CEGEX_H

#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

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
    TOKEN_CARET,          // ^
    TOKEN_DOLAR,          // $
    TOKEN_QUESTION,       // ?
    TOKEN_L_ROUND_PAREN,  // (
    TOKEN_R_ROUND_PAREN,  // )
    TOKEN_PIPE,           // |
    TOKEN_DOT,            // .
    TOKEN_ESCAPE,         /* \  */
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
  size_t length = strlen(regex);
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
      case '^':
        token.type = TOKEN_CARET;
        break;
      case '&':
        token.type = TOKEN_DOLAR;
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

  return tokens;
}

// TOKENIZER END




// PARSE START

typedef enum {
  DEFAULT_QUANTIFIER,
  ZERO_OR_ONE,
  ZERO_OR_MANY,
  ONLY_ONE,
  ONE_OR_MANY,
} Quantifier;

typedef enum{
  DEFAULT_ELEMENT_TYPE,
  WILDCHARD,      // .
  ELEMENT,        // a b c
  GROUP_ELEMENT,  // ( )
  LINE_POSITION,  // ^ $
  ALTERNATION,    // |
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
  return (Element){
    .type= DEFAULT_ELEMENT_TYPE,
    .quantifier= DEFAULT_QUANTIFIER,
    .value= '\0',
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
  s.items =  malloc(sizeof(Element)*2);
  s.capacity =2;
  memset(s.items, 0, sizeof(Element));
  return s;
}

Stack stack_create(){
  Stack s;
  memset(&s, 0, sizeof(Stack));
  s.items =  malloc(sizeof(Scope)*1);
  s.capacity =1;
  memset(s.items, 0, sizeof(Scope));
  return s;
}

Scope* stack_get_last_scope(const Stack* s){
  return &s->items[s->count-1];
}
#ifdef CEGEX_DEBUG
void dump_stack(const Stack s, int indent);
void dump_el(const Element el, int indent){

  char indentSpcs[256]={0};
  char *type, *quantifier;
  for(int i=0;i<indent;i++){
    sprintf(indentSpcs,"    %s", indentSpcs); // create indent string
  }

  switch(el.type){
    case WILDCHARD:
      type = "WILDCHAR";
      break;
    case ELEMENT:
      type = "ELEMENT";
      break; 
    case GROUP_ELEMENT:
      type = "GROUP_ELEMENT";
      break;
    case ALTERNATION:
      type = "ALTERNATION";
      break;
    default:
      type = "DEFAULT";
  }

  switch(el.quantifier){
    case ZERO_OR_ONE:
    quantifier="ZERO_OR_ONE";
    break;
    case ZERO_OR_MANY:
    quantifier="ZERO_OR_MANY";
    break;
    case ONLY_ONE:
    quantifier="ONLY_ONE";
    break;
    case ONE_OR_MANY:
    quantifier="ONE_OR_MANY";
    break;
        default:
      type = "DEFAULT";
  }

  printf("%stype:%s\n%squantifier:%s\n%svalue:'%c'\n%sstack:\n",indentSpcs, type, indentSpcs, quantifier, indentSpcs, el.value >=32 ? el.value : '\0',indentSpcs);
  dump_stack(el.stack, indent+1);
  printf("\n");
}

void dump_stack(const Stack s, int indent){

  if(s.count==0){
    return printf("---- EMPTY STACK %i---- \n", indent);
  }

  for(int i=0;i<s.count;i++){
    if(0 == s.items[i].count){
      return printf("---- EMPTY SCOPE  %i ---\n", indent);
    }
    printf("== Scope start\n");
    for(int j=0;j<s.items[i].count;j++){
      dump_el(s.items[i].items[j], indent);
    }
    printf("== end\n");

  }
}
#endif

Stack parse(Tokens *tokens){
  Stack stack= stack_create();
  Scope fs = scope_create();
  DA_PUSH(stack, fs);

  bool normalize = false;
  for(int i=0; (size_t)i<tokens->count; i++){
    Token token = tokens->items[i];
    Scope* currentScope = stack_get_last_scope(&stack);

    Element el=element_init();
    switch(token.type){
      case TOKEN_DOLAR:
        el.type = LINE_POSITION;
        el.quantifier = ONLY_ONE;
        el.value = 0; // zero means start point
        break;
      case TOKEN_CARET:
        el.type = LINE_POSITION;
        el.quantifier = ONLY_ONE;
        el.value = 1; // one means end point
        break;
      case TOKEN_PIPE:
        if(currentScope->count ==0){
          CEGEX_ERROR("Parse Error: | can not first element\n",i);
          exit(1);
        }
        // push prev and next element to current element stack
        el.type = ALTERNATION;
        el.quantifier = ONLY_ONE;
        el.stack = stack_create();
        normalize = true;
        break;
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
        Scope groupScope = DA_POP(stack);
        currentScope--;
        DA_PUSH(el.stack, groupScope);
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
          lEl->quantifier = ZERO_OR_MANY;
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
    DA_PUSH(*currentScope, el);
  }

  // Normalize stacks
  // For alternation operation
  void normalize_element(Element* el, Scope* currentScope, size_t nth_element){
     if(el->type == ALTERNATION){
      // Push prev and next elements to alternation stack
      Scope subScope = scope_create();
      // Create wraper for simulate OR operation.
      Element prevWraper ={
        .type = GROUP_ELEMENT,
        .quantifier = ONLY_ONE,
        .stack = stack_create(),
      },
      nextWraper ={
        .type = GROUP_ELEMENT,
        .quantifier = ONLY_ONE,
        .stack = stack_create(),
      };
      DA_PUSH(nextWraper.stack, scope_create());
      DA_PUSH(prevWraper.stack, scope_create());

      // Push elements to wrapers
      DA_PUSH(DA_GET(prevWraper.stack, 0), *(el-1));
      DA_PUSH(DA_GET(nextWraper.stack, 0), *(el+1));

      // Push wrappers to alternation elements stack
      DA_PUSH(subScope, prevWraper);
      DA_PUSH(subScope, nextWraper);
      DA_PUSH(el->stack, subScope);

      // Remove that items from stack
      DA_REMOVE(*currentScope, nth_element+1);
      DA_REMOVE(*currentScope, nth_element-1);
    }
  }

  void normalize_stack(Stack* stack){
    for(int i=0; i<stack->count;i++){
        Scope* scope = &stack->items[i];
        for(int j=0; j<scope->count;j++){
          Element* el= &scope->items[j];
          normalize_element(el, scope, j);
          if(el->stack.count>0){
            normalize_stack(&el->stack);
          }
      }
    }
  }

  if(normalize){
    normalize_stack(&stack);
  }

  return stack;
}

// PERSER END

// MATCH START
CegexValue match(Stack *s, char* text);

int element_match_str(Element el, char* text, size_t index){
  if(index>strlen(text)){
    return -1;
  }

  if(el.type == ELEMENT){
    return el.value == text[index] ? 1 : -1;
  }

  if(el.type == ALTERNATION){
    Scope alterScope = DA_SHIFT(el.stack);

    Stack lStack = stack_create();
    Scope lScope = scope_create();
    DA_PUSH(lScope, DA_SHIFT(alterScope));
    DA_PUSH(lStack, lScope);
    int lIndex = match(&lStack, &text[index]).index;
    if(lIndex!=-1){
      return lIndex;
    }

    Stack rStack = stack_create();
    Scope rScope = scope_create();
    DA_PUSH(rScope, DA_SHIFT(alterScope));
    DA_PUSH(rStack, rScope);
    return match(&rStack, &text[index]).index;
  }

  if(el.type == WILDCHARD){
    return 1;
  }

  if(el.type == GROUP_ELEMENT){
    return match(&el.stack, &text[index]).index;
  }
  if(el.type == LINE_POSITION){
    return el.value == 0 ? index == 0 : index +1 == strlen(text);
  }
  CEGEX_ERROR("Unsupported element type!");
  return -1;
}

CegexValue match(Stack *s, char* text){
  CegexValue res ={0};
  int i=0;
  do{
    Scope currentScope = DA_SHIFT(*s);

    while(currentScope.count >=1){
      Element el = DA_SHIFT(currentScope);
      switch(el.quantifier){
        case ONLY_ONE:
          int in = element_match_str(el, text, i);
          if(-1 == in){
            res.index = -1;
            res.length = 0;
            return res;
          }
          i+=(in != 0? in:1);
          res.length++;
          break;
        case ZERO_OR_ONE:
          if(i>= (int)strlen(text)){
            res.length++;
            break;
          }
          int in2 = element_match_str(el, text, i);
          if(in2>0){ // step over
            i+=in2;
          }
          res.length++;
          break;
        case ZERO_OR_MANY:
          bool is_done = false;
          while(!is_done){
            int in3 = element_match_str(el, text, i);
            if(i>= (int)strlen(text)){
              break;
            }
            if(in3 == -1){ // zero
              // currentScope = DA_SHIFT(*s);
              break;
            }
            i+=in3;
            res.length++;
          }
          break;
        case ONE_OR_MANY:
          int in4 = element_match_str(el, text, i);
          if(-1 == in4){ // zero
            res.index= -1;
            res.length = 0;
            return res;
          }
          while(true){
            // if(in4 !=0){ // we increment it after the case
            //   res.length--;
            // }
            in4 = element_match_str(el, text, i);
            if(-1==in4){
              break;
            }
            i+=in4;
            res.length++;
          }
          break;
        case DEFAULT_QUANTIFIER:
          CEGEX_ERROR("DEFAULT_QUANTIFIER handled!");
          return (CegexValue){
            -1,0
          };
      }
      // el = DA_SHIFT(currentScope);
    }
  }while(s->count >=1);

  res.index =i;
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
  int index =0;

    res = match(&stack, text +index);
  // while(index<strlen(text + index)){
  //   if(res.index != -1){
  //     break;
  //   }
  //   index++;
  // }
  return res;
}

#endif
