#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <jmorecfg.h>

#define MAX_LEXEM_LEN 120
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

const char * gettype_by_num[] = {"INT", "ID", "DEL", "KEY", "LIT"};

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

//todo make ftell; get cursor;
struct error{
    char message[1024];
};

struct lexeme * delim_list = NULL;
struct lexeme * key_list  = NULL ;
struct lexeme * id_list  = NULL;
struct lexeme * int_list = NULL;
struct lexeme * lit_list = NULL;
struct record * record_list = NULL;
struct error * global_error = NULL;
char current_char = NULL;

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

struct lexeme * iden(char first_char){
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
    current_char = ch;
    return lex;
}
struct lexeme * digit(char first_char){
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
    current_char = ch;
    return lex;
}

struct lexeme * delim(char first_char){
    int len = 0;
    char buffer_double[MAX_LEXEM_LEN];
    char buffer_single[MAX_LEXEM_LEN];
    char ch;
    buffer_single[len] = first_char;
    buffer_double[len++] = first_char;

    if (first_char == '\''){
        char buffer[MAX_LEXEM_LEN];
        int lit_len = 0;
        while ((ch = get_char()) != '\'' && ch != EOF){
            buffer[lit_len++] = ch;
        };
        if (ch == EOF){
            global_error = malloc(sizeof(struct error));
            char message[1024] = "Closing delimeter \' absent";
            strcpy(global_error->message, message);
        }
        buffer[lit_len] = '\0';
        struct lexeme * lex = lookup(buffer, lit_list);
        if (lex == NULL){
            lit_list = add_lexeme(LIT, lit_list, buffer);
            lex = lit_list;
        }
        add_record(lex);
        ch = get_char();
        current_char = ch;
        return lex;
    }


    buffer_single[len] = '\0';
    ch = get_char();
    buffer_double[len++] = ch;
    buffer_double[len] = '\0';
    struct lexeme * lex = lookup(buffer_double, delim_list);
    if (lex != NULL){
        add_record(lex);
        ch = get_char();
    } else {
        lex = lookup(buffer_single, delim_list);
        add_record(lex);
    }
    current_char = ch;
    return lex;

}



struct lexeme * scan(){
        if (current_char == NULL){
            current_char = get_char();
        }

        while (current_char == ' ' || current_char == '\n'){
            current_char = get_char();
        }

        if (current_char == EOF){
            return NULL;
        }

        if(isalpha(current_char)){
            return iden(current_char);
        }

        if isdigit(current_char){
            return digit(current_char);
        }

        if (isdelim(current_char)){
            return delim(current_char);
        }

        global_error = malloc(sizeof(struct error));
        char message[1024];
        sprintf(message, "ERROR. Unknown characher %c", current_char);
        strcpy(global_error->message, message);
        return NULL;

}


void print_lexeme_list(struct lexeme * last_lex){
    if (last_lex != NULL) {
        struct lexeme *iter = last_lex->first;
        printf("\n%s TABLE", gettype_by_num[iter->type]);
        do {
            printf("\n%d \t\t%s", iter->id, iter->value);
        } while ((iter = iter->next) != NULL);
        printf("\n");
    }
}

void print_record_list(){
    if (record_list != NULL) {
        struct record *iter = record_list->first;
        printf("\nRECORD TABLE");
        do {
            printf("\n%d\t%s\t%d\t%s", iter->id, gettype_by_num[iter->lex->type], iter->lex->id, iter->lex->value);
        } while ((iter = iter->next) != NULL);
        printf("\n");
    }
}


int main() {
    delim_list = read_values_from_file(DELIM_FILENAME, DELIM);
    key_list = read_values_from_file(KEYWORD_FILENAME, KEY);
    code_file_ptr = fopen(PROGRAM_FILENAME, "r");
    struct lexeme * lex;

    while ((lex = scan()) != NULL);


    fclose(code_file_ptr);

    print_lexeme_list(delim_list);
    print_lexeme_list(key_list);
    print_lexeme_list(id_list);
    print_lexeme_list(lit_list);
    print_lexeme_list(int_list);
    print_record_list();

    if (global_error->message != NULL){
        printf ("%s", global_error->message);
    }

    return 0;
}