//
// Created by rea on 8/16/2018.
//


/*
 * trim any spaces characters of the string from both sides (left and right).
 */
void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;   /* remove spaces from the right side */
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);  /* copy the non-space part to the begin of the string */
}


/*
 * return the first word before any space character, which is the command.
 */
void get_command (char * line , char * arg_1)
{
    int i ;
    for ( i = 0 ; *( line + i ) && ! (isspace((unsigned char) *( line + i ) ) ) ; i++ )  /* looking for the first space, that end the first argument */
        ;
    strncpy(arg_1, line , i ); /* put the first argument (everything before the first space, and the space) into command */
    *(arg_1 + i ) = '\0';
    trim(arg_1); /* trim all the spaces around the command */
}


/*
 * return the second argument, after the first space, and before any comma character.
 */
void get_arg_2 (char * line, char * arg_2)
{
    int start ;
    int i ;
    for ( i = 0 ;  *( line + i ) && ! (isspace((unsigned char) *( line + i ) ) ) ; i++ )  /* skip the first argument */
        ;

    for ( ;   *( line + i ) && (isspace((unsigned char) *( line + i ) ) ) ; i++ )  /* skip all the spaces between arg_1 and arg_2 */
        ;
    start = i ;
    /* start now pointing to the start of arg_2, or to the end of the line */
    for(  ; *(line + i ) &&  *( line + i ) != ',' ; i++   )  /* looking for the first comma, that end the second argument, or the end of the line */
        ;
    /* i now pointing to the comma after arg_2, or to the end of the line */
    strncpy(arg_2, line + start , i - start ); /* put the second argument (everything after the first space, and before the first comma ) into arg_2 */
    *(arg_2 + i - start  ) = '\0';
    trim(arg_2);/* trim all the spaces around the command */
}




/*
 * return a string that is the n argument. only good for n>=1.
 */
void get_arg_n(char *(line ) , char * arg_n , int num )
{
    int start ;
    int i ;
    int j ;

    if(num == 1)   /* return arg_1 */
    {
        get_command(line, arg_n);
        return ;
    }


    if(num == 2)   /* return arg_2 */
    {
        get_arg_2(line, arg_n);
        return ;
    }

    /* else, return the n argument: */

    for ( i = 0 ;  *( line + i ) && ! (isspace((unsigned char) *( line + i ) ) ) ; i++ )  /* skip the first argument */
        ;
    for( j = 2 ; j < num ; j++)  /* go to the start of the n argument */  /* move i one forward, that it wont point on the comma */
    {
        for(  ; *(line +i) &&  *( line + i ) != ',' ; i++   )  /* looking for the next comma, that after it start the next  argument, or to the end of the line */
            ;
        if(*(line +i)) /* the for loop stopped by comma not by end of line */
        {
            i++;
        }
    }



    start = i ; /* start now is the index of the start of the n argument */
    for( ; *(line +i) &&  *( line + i ) != ',' ; i++)  /* looking for the "n" comma, that end the "n" argument, or the end of the line */
        ;

    strncpy(arg_n, line + start, i-start ); /* put into arg_n the string that comes after the (n-1) comma, which is the n argument */
    *(arg_n + i - start  ) = '\0';
    trim(arg_n); /* trim all the spaces around the argument */
}

/*
 * return the number of arguments that in the line recived.
 * the format is this:
 * arg1 arg2,arg3, ... , argn
 */
int count_arg (char * line)
{
    int count ;
    int i ;

    count = 1;
    i = 0 ;

    while(*(line +i) && ! (isspace((unsigned char) *( line + i ) ) ) )  /* looking for the first space, that end the first argument */
    {
        i++;
    }
    if(*(line +i) ) /* find space, so there is more argument */
        count ++;
    while(*(line + i ))
    {
        if(*(line + i ) == ',')
            count++;
        i++;
    }
    return count;
}


paras_arg( char line )
{

}