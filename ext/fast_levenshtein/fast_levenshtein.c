
#include <ruby.h>
#include <string.h>

void print_matrix(int ** matrix, int len_a, int len_b) { 
    for(int i = 0; i < (len_a + 1); i++) { 
        for(int j = 0; j < (len_b + 1); j++) { 
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

static VALUE fast_levensthein_distance(VALUE self, VALUE rb_a, VALUE rb_b) { 
    
    Check_Type(rb_a, T_STRING);
    Check_Type(rb_b, T_STRING);

    const char* src_str_a = StringValueCStr(rb_a);
    const char* src_str_b = StringValueCStr(rb_b);

    //if the strings are equal, return 0, no need to calculate
    if(strcmp(src_str_a, src_str_b) == 0) { 
        return INT2FIX(0);
    }

    const char* str_a = src_str_a;
    const char* str_b = src_str_b;
    
    int len_a = strlen(str_a);
    int len_b = strlen(str_b);

    //make src_a the shortest string to avoid overrunning a buffer 
    //while checking for matching prefixes
    if(len_a > len_b) { 
        str_a = src_str_b;
        len_a = strlen(str_a);
        str_b = src_str_a;
        len_b = strlen(str_b);        
    }

    //advance the pointers untill the strings dont match.  this will 
    //reduce the table sizes needed to calculate the distances
    for(;;) { 
        if(*str_a != *str_b) { 
            break;
        }
        str_a++;
        str_b++;
    }

    // calculate new lengths for matrix
    len_a = strlen(str_a);
    len_b = strlen(str_b);

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

    print_matrix(matrix, len_a, len_b);

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
