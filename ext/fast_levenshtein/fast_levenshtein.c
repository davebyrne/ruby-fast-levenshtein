
#include <ruby.h>
#include <string.h>

typedef struct StringPair { 
    const char* a;
    const char* b;
} StringPair;

void print_matrix(int ** matrix, int len_a, int len_b) { 
    for(int i = 0; i < (len_a + 1); i++) { 
        for(int j = 0; j < (len_b + 1); j++) { 
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
/*
 * the levenshtein distance between two strings that are equal will
 * always be zero, so to reduce the amount of computation involved,
 * we can trim both leading and trailing characters that are equal
 */
StringPair* trim_strings(const char* a, const char* b) { 

    //first make sure that the smallest string is 'a' so that
    //we dont have to worry about overrunning the buffers when 
    //looping through the chars
    if(strlen(a) > strlen(b)) { 
        const char* tmp = a;
        a = b;
        b = tmp;
    }

    const char* offset_start_a = a;
    const char* offset_start_b = b;
    //advance the pointers until the strings dont match.  this will 
    //reduce the table sizes needed to calculate the distances
    for(int i = 0; i < strlen(a); i++) { 
        if(*offset_start_a != *offset_start_b) { 
            break;
        }
        offset_start_a++;
        offset_start_b++;
    }

    const char* offset_end_a = a + strlen(a) -1;
    const char* offset_end_b = b + strlen(b) -1;
    //do the same from the end of the string
    for(int i = 0; i < strlen(a); i++) { 
        if(*offset_end_a != *offset_end_b) { 
            //move the pointer forward one char since the test failed
            offset_end_a++;
            offset_end_b++;
            break;
        }
        offset_end_a--;
        offset_end_b--;
    }


    StringPair* pair = ruby_xmalloc(sizeof(StringPair));

    int len = offset_end_a - offset_start_a;
    char* str_a = ruby_xmalloc(sizeof(char) *  len + 1);
    strncpy(str_a, offset_start_a, len);
    str_a[len] = '\0';
    pair->a = str_a;

    len = offset_end_b - offset_start_b;
    char* str_b = ruby_xmalloc(sizeof(char) * len + 1);
    strncpy(str_b, offset_start_b, len);
    str_b[len] = '\0';
    pair->b = str_b;
    
    return pair;

}

void free_pair(StringPair *pair) {
    ruby_xfree((void*) pair->a);
    ruby_xfree((void*) pair->b);
    ruby_xfree((void*) pair);
}

static VALUE fast_levensthein_distance(VALUE self, VALUE rb_a, VALUE rb_b) { 
    
    Check_Type(rb_a, T_STRING);
    Check_Type(rb_b, T_STRING);

    //if the strings are equal, return 0, no need to calculate
    if(strcmp(StringValueCStr(rb_a), StringValueCStr(rb_b)) == 0) { 
        return INT2FIX(0);
    }

    StringPair* pair = trim_strings(StringValueCStr(rb_a), StringValueCStr(rb_b));

    const char* str_a = pair->a;
    const char* str_b = pair->b;
    
    int len_a = strlen(str_a);
    int len_b = strlen(str_b);

    int** matrix = (int ** ) ruby_xmalloc(sizeof(int *) * (len_a + 1 ));
    for(int i = 0; i < (len_a + 1); i++) { 
        matrix[i] = (int *) ruby_xmalloc(sizeof(int *) * (len_b + 1));
    }

    //inialize matrix
    for(int i = 0; i < (len_a + 1); i++) { 
        for(int j = 0; j < (len_b + 1); j++) { 
            matrix[i][j] = 0;            
        }
    }

    for(int i = 1; i < (len_a + 1); i++) { 
        matrix[i][0] = i;
    }

    for(int j = 1; j < (len_b + 1); j++) { 
        matrix[0][j] = j;
    }

    int ret = 0;

    // loop through matrix and calculate costs
    for(int j = 1; j < (len_b + 1); j++) { 
        for(int i = 1; i < (len_a + 1); i++) { 
            int cost = str_a[i - 1] == str_b[j - 1] ? 0 : 1;

            int delete = matrix[i - 1][j] + 1;
            int insert = matrix[i][j - 1] + 1;
            int substitute = matrix[i - 1][j - 1] + cost;

            int tmp = (delete < insert ? delete : insert);
            
            ret = (substitute < tmp ? substitute : tmp);
            matrix[i][j] = ret;
        }
    }

    //debugging
    //print_matrix(matrix, len_a, len_b);

    free_pair(pair);

    for(int i = 0; i < (len_a + 1); i++) { 
        ruby_xfree(matrix[i]);
    }
    ruby_xfree(matrix);
    
    return INT2FIX(ret);
}

void Init_fast_levenshtein(void) {

  VALUE cFastLevenshtein = rb_define_module("FastLevenshtein");
  
  rb_define_module_function(cFastLevenshtein, "distance", fast_levensthein_distance, 2);

}
