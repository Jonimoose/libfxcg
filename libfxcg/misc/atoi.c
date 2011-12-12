int sys_atoi(const char * s)
{
    register int rv=0; 
    register char sign = 0;

    /* skip till we find either a digit or '+' or '-' */
    while (*s) {
        if (*s <= '9' && *s >= '0')
            break;
        if (*s == '-' || *s == '+') 
            break;
        s++;
    }    

    sign = (*s == '-');
    if (*s == '-' || *s == '+') s++;

    while (*s && *s >= '0' && *s <= '9') {
        rv = (rv * 10) + (*s - '0');
        s++;
    }

    return (sign ? -rv : rv);
}
