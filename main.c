//
// Created by rea on 8/16/2018.
//



void print_all_arg(int argc, char * argv[] )
{
    int i ;
    printf("the call to the function is:\n");
    for (i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
}


/*
 * paramaters:
 * argc: the number of argument from the shell
 * argv: array that hold pointers to the arguments.
 *
 * arg_1: file name to check
 * arg_2: string to check for hers permutation
 *
 * program action: it print all the permutation of the string arg_2 in the file arg_1
 *
 */
int main( int argc, char * argv[] )
{
    char sorted_string [MAX_LINE_SIZE];
    int string_length;

    print_all_arg( argc , argv );
    if(argc == 1)
        fprintf( stderr , "%s\n" , ERROR_NO_ARGUMENTS );
    else if( argc == 2 )
    {
        fprintf( stderr , "%s\n", ERROR_MISSING_ARG);
    }
    else if( argc > 3 ) {
        fprintf( stderr , "%s\n" , ERROR_TO_MANY_ARGUMENTS);
    }
    else    /* which means that: (argc = 3) */
    {
        printf("argument received are:\nfile-name: " GRN "%s" RESET "\nstring: " GRN "%s" RESET "\n" , argv[1] , argv[2] );


    }

    return 0;
}
