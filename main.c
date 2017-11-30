#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <jmorecfg.h>

#define MAX_LEXEM_LEN 64
#define DELIM_FILENAME "./delim"
#define KEYWORD_FILENAME "./keywords"
#define PROGRAM_FILENAME "./program_code"

enum table_type{
    INT,
    ID,
    DELIM,
    KEY,
    LIT
};

struct lexeme{
    char value[MAX_LEXEM_LEN];
    enum table_type type;
    int id;
    struct lexeme * first;
    struct lexeme * next;
};

struct record{
    int id;
    struct lexeme * lex;
    struct record * first;
    struct record * next;
};

struct lexeme * delim_list = NULL;
struct lexeme * key_list  = NULL ;
struct lexeme * id_list  = NULL;
struct lexeme * int_list = NULL;
struct lexeme * lit_list = NULL;
struct record * record_list = NULL;

FILE * code_file_ptr;

int get_line(FILE * fileptr, char buffer[]){
    int ch;
    int len = 0;

    while ((ch = fgetc(fileptr)) != '\n' && ch != EOF && len < MAX_LEXEM_LEN){
        buffer[len++] = toupper(ch);
    }
    buffer[len] = '\0';
    return len;
}

struct lexeme * read_values_from_file(char * filename, enum table_type type){
    FILE * fileptr;
    fileptr = fopen(filename, "r");
    char buffer[MAX_LEXEM_LEN];
    int len, id = 0;
    struct lexeme * first = NULL;
    struct lexeme * last = NULL;

    while((len = get_line(fileptr, buffer)) > 0){
        struct lexeme * lex = malloc(sizeof(struct lexeme));
        lex->type = type;
        lex->id = id++;
        strncpy(lex->value, buffer, (size_t) len);
        if (lex->id == 0){
            first = lex;
            last = lex;
        } else {
            last->next = lex;
            last = lex;
            lex->first = first;
            lex->next = NULL;
        }
    }
    fclose(fileptr);
    return last;
}

char get_char(){
    //todo: Ignore whitespace, newLine, Tab, Backspace and so on;
    return toupper(fgetc(code_file_ptr));
}

struct lexeme * lookup(char value[], struct  lexeme * lexeme_list){
    if (lexeme_list != NULL){
        struct lexeme * iter = lexeme_list->first;
        do{
            if (strcmp(value, iter->value) == 0){
                return iter;
            };

        } while ((iter = iter->next) != NULL);
    }
    return NULL;
}

struct lexeme * add_lexeme(enum table_type type, struct lexeme * lexeme_list, char value[]){
    struct lexeme * lex = malloc(sizeof(struct lexeme));
    lex->type = type;
    lex->next = NULL;
    strcpy(lex->value, value);
    if (lexeme_list == NULL){
        lex->id=0;
        lex->first = lex;
        lexeme_list = lex;
    } else {
        lexeme_list->next = lex;
        lex->id = lexeme_list->id + 1;
        lex->first = lexeme_list->first;
        lexeme_list = lex;
    }
    return lexeme_list;
}

struct record * add_record(struct lexeme * lexeme){
    struct record * record = malloc(sizeof(struct record));
    record->lex = lexeme;
    record->next = NULL;
    //  LOG
    printf("\nvalue %s", lexeme->value);
    //
    if (record_list == NULL){
        record->id = 0;
        record->first = record;
        record_list = record;
    } else {
        record->id = record_list->id + 1;
        record->first = record_list->first;
        record_list->next = record;
        record_list = record;
    }
}

boolean isdelim(char ch){
    char buffer[MAX_LEXEM_LEN];
    buffer[0] = ch;
    buffer[1] = '\0';
    if (lookup(buffer, delim_list) != NULL) return TRUE;
    else return FALSE;
}

char iden(char first_char){
    int len = 0;
    char buffer[MAX_LEXEM_LEN];
    buffer[len++] = first_char;
    char ch = get_char();
    while ((isalpha(ch) || isdigit(ch)) && len < MAX_LEXEM_LEN){
        buffer[len++] = toupper(ch);
        ch = get_char();
    }
    buffer[len] = '\0';
    struct lexeme * lex = lookup(buffer, key_list);
    if (lex == NULL){
        lex = lookup(buffer, id_list);
        if (lex == NULL){
            id_list = add_lexeme(ID, id_list, buffer);
            lex = id_list;
        }
    }

    add_record(lex);
    return ch;
}
char digit(char first_char){
    int len = 0;
    char buffer[MAX_LEXEM_LEN];
    buffer[len++] = first_char;
    char ch = get_char();
    while (isdigit(ch) && len < MAX_LEXEM_LEN){
        buffer[len++] = ch;
        ch = get_char();
    }
    buffer[len] = '\0';
    struct lexeme * lex = lookup(buffer, int_list);
    if (lex == NULL){
        int_list = add_lexeme(INT, int_list, buffer);
        lex = int_list;
    }
    add_record(lex);
    return ch;
}

char delim(char first_char){
    int len = 0;
    char buffer_double[MAX_LEXEM_LEN];
    char buffer_single[MAX_LEXEM_LEN];
    buffer_single[len] = first_char;
    buffer_double[len++] = first_char;
    buffer_single[len] = '\0';
    char ch = get_char();
    buffer_double[len++] = ch;
    buffer_double[len] = '\0';
    struct lexeme * double_delim = lookup(buffer_double, delim_list);
    //todo: add LITERALS
    if (double_delim != NULL){
        add_record(double_delim);
        ch = get_char();
    } else {
        add_record(lookup(buffer_single, delim_list));
    }
    return ch;
}



void scan(){
    char ch;
    ch = get_char();
    while (ch != EOF){
        if(isalpha(ch)){
            ch = iden(ch);
            continue;
        }

        if isdigit(ch){
            ch = digit(ch);
            continue;
        }

        if (isdelim(ch)){
            ch = delim(ch);
            continue;
        }


        printf("Error!!! Unknown construction: '%c'", ch);
        break;
    }
}





int main() {
    delim_list = read_values_from_file(DELIM_FILENAME, DELIM);
    key_list = read_values_from_file(KEYWORD_FILENAME, KEY);
    code_file_ptr = fopen(PROGRAM_FILENAME, "r");
    delim_list = add_lexeme(DELIM, delim_list, "\n");
    delim_list = add_lexeme(DELIM, delim_list, " ");
    scan();
    fclose(code_file_ptr);

    return 0;
}