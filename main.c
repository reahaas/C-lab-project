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
 * this function come from permut maman
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
int print_all_args( int argc, char * argv[] )
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

/*
 * set main
 */
int main()
{
    set * SETA = new_set();
    set * SETB = new_set();
    set * SETC = new_set();
    set * SETD = new_set();
    set * SETE = new_set();
    set * SETF = new_set();
    set * SETTEMP = new_set();  /* will be used for the command read_set */

    char line_input [MAX_INPUT];
    int arg_num ;
    unsigned long num_to_add_to_set;  /* will be used in the read_set function */
    unsigned char all_good_while_read_set;  /* flag to know if the read_set command is valid */
    unsigned char index ;   /* will be used in the read_set function */
    char  command [MAX_INPUT] ;
    char  str_arg_2 [MAX_INPUT] ;
    char  str_arg_3 [MAX_INPUT] ;
    char  str_arg_4 [MAX_INPUT] ;
    char  str_arg_n [MAX_INPUT] ;   /* represent the last argument recived */
    char  str_arg_i [MAX_INPUT] ;   /* will be use if the command read_set will be recived */

    set * set_arg_2 ;
    set * set_arg_3 ;
    set * set_arg_4 ;

    set * set_names[SET_POINTERS] ;

    *( set_names + 1) = SETA ;
    *( set_names + 2) = SETB ;
    *( set_names + 3) = SETC ;
    *( set_names + 4) = SETD ;
    *( set_names + 5) = SETE ;
    *( set_names + 6) = SETF ;


    printf("welcome to REA's myset program! when there is >>> you can type your command. \nto use REA's tester type \"test\"\n");
    printf("now you can type any command\n");

    while (TRUE)
    {
        printf(">>>");
        scanf (" %[^\n]", line_input);
        printf("\nline entered is \"%s\"\n" , line_input);  /* print to the screen so the command recived is visiable even if the input comminig from a file */
        trim(line_input); /* trim the spaces */

        arg_num = count_arg(line_input);
        get_arg_n(line_input, command, 1);

        get_arg_n(line_input, str_arg_2, 2);
        get_arg_n(line_input, str_arg_3, 3);
        get_arg_n(line_input, str_arg_4, 4);
        get_arg_n(line_input, str_arg_n, arg_num);

        /* for debugging:
         * print the four firsts arguments:
         * printf("argument number %d is \"%s\"\n", 1 , command);
         * printf("argument number %d is \"%s\"\n", 2 , str_arg_2);
         * printf("argument number %d is \"%s\"\n", 2 , str_arg_2);
         * printf("argument number %d is \"%s\"\n", 3 , str_arg_3);
         * printf("argument number %d is \"%s\"\n", 4 , str_arg_4);
         * printf("argument number %d is \"%s\"\n", arg_num , str_arg_n);
         */

        if ( strcmp(command, "stop" ) == 0 )  /* active the command stop,  then exit the program */
        {
            if(arg_num == 1)
            {
                printf("thank you for useing REA's \"myset\" program :) program now turn off.\n");
                return 0;
            }
            else
                printf("%s\n" , ERROR_ARG_NUM);
        }
        else if( ! memcmp(command, "print_set" , strlen("print_set")) )   /* active the command print_set */
        {
            if(arg_num == 2)
            {
                if( ( set_arg_2 = is_valid_set_name(str_arg_2 , set_names) ) )
                    print_set(set_arg_2);
                else
                    printf("%s\n" , ERROR_ARG_NAME);
            }
            else
                printf("%s\n" , ERROR_ARG_NUM);
        }
        else if(! memcmp( command, "union_set" , strlen("union_set")) )  /* active the command union_set */
        {
            if(arg_num == 4)
            {
                if( ( set_arg_2 = is_valid_set_name(str_arg_2 , set_names) ) && ( set_arg_3 = is_valid_set_name(str_arg_3 , set_names) ) && ( set_arg_4 = is_valid_set_name(str_arg_4 , set_names) )   )
                    union_set(set_arg_2, set_arg_3, set_arg_4);
                else
                    printf("%s\n" , ERROR_ARG_NAME);
            }
            else
                printf("%s\n" , ERROR_ARG_NUM);
        }
        else if(! memcmp(command, "intersect_set" , strlen("intersect_set")) )   /* active the command inersect_set */
        {
            if(arg_num == 4)
            {
                if( ( set_arg_2 = is_valid_set_name(str_arg_2 , set_names) ) && ( set_arg_3 = is_valid_set_name(str_arg_3 , set_names) ) && ( set_arg_4 = is_valid_set_name(str_arg_4 , set_names) )   )
                    intersect_set(set_arg_2, set_arg_3, set_arg_4);
                else
                    printf("%s\n" , ERROR_ARG_NAME);
            }
            else
                printf("%s\n" , ERROR_ARG_NUM);
        }
        else if(! memcmp(command, "sub_set" , strlen("sub_set")) )  /* active the command sub_set */
        {
            if(arg_num == 4)
            {
                if( ( set_arg_2 = is_valid_set_name(str_arg_2 , set_names) ) && ( set_arg_3 = is_valid_set_name(str_arg_3 , set_names) ) && ( set_arg_4 = is_valid_set_name(str_arg_4 , set_names) )   )
                    sub_set(set_arg_2, set_arg_3, set_arg_4);
                else
                    printf("%s\n" , ERROR_ARG_NAME);
            }
            else
                printf("%s\n" , ERROR_ARG_NUM);
        }
        else if(! memcmp(command, "read_set" , strlen("read_set")) )  /* active the command read_set */
        {
            if(arg_num > 2)  /* read_set must  get at least 3 arguments: 1=command , 2=set_name , 3 =terminator (-1)  */
            {
                if(( set_arg_2 = is_valid_set_name(str_arg_2 , set_names ) )) /* check for valid set_name (the second argument) */
                {
                    all_good_while_read_set = 1; /* flag to know if the read_set command is valid */
                    if( strcmp( str_arg_n , "-1" ))  /* check for invalid terminator  */
                    {
                        printf ("%s\n" , ERROR_READ_MISSING_TERMINATOR);
                        all_good_while_read_set = 0 ;
                    }
                    else  /* terminator is valid */
                    {
                        for( index = 3 ; index < arg_num ; index++)  /* check for validation of each of the arguments */
                        {
                            /* for debugging:
                            printf("all_good_while_read_set before for loop is: %d\n" , all_good_while_read_set);     */
                            get_arg_n ( line_input , str_arg_i ,  index);

                            if( ! strcmp(str_arg_i , "0" ))
                            {
                                add_num_to_set(SETTEMP , 0 ) ;  /* add 0 to the temp set */
                            }
                            else
                            {
                                num_to_add_to_set = atoi ( str_arg_i ) ;
                                /* for debugging:
                                 * printf("arg_n_as_str is : \"%s\"     num_to_add_to_set after strtol is : \"%ld\"\n" , str_arg_i , num_to_add_to_set );
                                 */
                                if((unsigned char)num_to_add_to_set < 1 || (unsigned char)num_to_add_to_set > (SET_SIZE -1) )   /* check if argument is a number, in the valid range of (0-127) */
                                {
                                    all_good_while_read_set = 0;
                                    printf("%s\n" , ERROR_ARG_NAME_FOR_READ);
                                }
                                else
                                    add_num_to_set(SETTEMP,  ( unsigned char )  num_to_add_to_set );
                            }
                            /* for debugging:
                             * printf("all_good_while_read_set after for loop is: %d\n" , all_good_while_read_set);
                             */
                        }
                        if(all_good_while_read_set)  /* after read all the command, its ok, and we can copy the temp set to the set that recived as paramater */
                        {
                            copy_set( set_arg_2 , SETTEMP );
                            clear_set(SETTEMP);
                            printf("set load with the numbers\n");
                        }
                    }
                } /* if(( arg2 = is_valid_set_name(get_arg_n(line_input, 2 ) ) )) */
                else
                {
                    printf("%s\n" , ERROR_ARG_NAME);
                    all_good_while_read_set = 0;
                }
            } /* if(arg_num > 2) */  /* there where less then 3 argument for the read_set command  */
            else
                printf("%s\n" , ERROR_ARG_NUM);

        }/* end of read_set block */
            /* good program till here */
        else if(! memcmp(command, "test" , strlen("test")) )  /* active the command test */
        {
            if(arg_num == 1 )
                test( SETA ,  SETB ,  SETC ,  SETD ,  SETE , SETF);
            else
                printf("%s\n" , ERROR_ARG_NUM);
        }
        else  /* the first argument is not any of the command that exists in this program */
            printf("%s\n" , ERROR_COMMAND_NOT_EXIST);

        clear_str(line_input);
        clear_str(str_arg_2);
        clear_str(str_arg_3);
        clear_str(str_arg_4);
        clear_str(str_arg_n);
        clear_str(str_arg_i);


    }

    return 0;
}