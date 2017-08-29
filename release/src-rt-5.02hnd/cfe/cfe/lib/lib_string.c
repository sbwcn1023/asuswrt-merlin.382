/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  String routines				File: lib_string.c
    *  
    *  Some standard routines for messing with strings.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001,2002,2003
    *  Broadcom Corporation. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions 
    *     as they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of Broadcom Corporation.  The "Broadcom Corporation" 
    *     name may not be used to endorse or promote products derived 
    *     from this software without the prior written permission of 
    *     Broadcom Corporation.
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT,
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */




#include "lib_types.h"
#define _LIB_NO_MACROS_
#include "lib_string.h"

char *lib_strcpy(char *dest,const char *src)
{
    char *ptr = dest;

    while (*src) *ptr++ = *src++;
    *ptr = '\0';

    return dest;
}

char *lib_strncpy(char *dest,const char *src,size_t cnt)
{
    char *ptr = dest;

    while (*src && (cnt > 0)) {
	*ptr++ = *src++;
	cnt--;
	}
    if (cnt > 0) *ptr = '\0';

    return dest;
}


size_t lib_xstrncpy(char *dest,const char *src,size_t cnt)
{
    char *ptr = dest;
    size_t copied = 0;

    while (*src && (cnt > 1)) {
	*ptr++ = *src++;
	cnt--;
	copied++;
	}
    *ptr = '\0';

    return copied;
}

size_t lib_strlen(const char *str)
{
    size_t cnt = 0;

    while (*str) {
	str++;
	cnt++;
	}

    return cnt;
}


int lib_strcmp(const char *dest,const char *src)
{
    while (*src && *dest) {
	if (*dest < *src) return -1;
	if (*dest > *src) return 1;
	dest++;	
	src++;
	}

    if (*dest && !*src) return 1;
    if (!*dest && *src) return -1;
    return 0;
}

int lib_strncmp(const char *s1, const char *s2, size_t n)
{
	while (*s2 && *s1 && n) {
		if (*s1 < *s2)
			return -1;
		if (*s1 > *s2)
			return 1;
		s1++;
		s2++;
		n--;
	}

	if (!n)
		return 0;
	if (*s1 && !*s2)
		return 1;
	if (!*s1 && *s2)
		return -1;
	return 0;
}

int lib_strcmpi(const char *dest,const char *src)
{
    char dc,sc;

    while (*src && *dest) {
	dc = lib_toupper(*dest);
	sc = lib_toupper(*src);
	if (dc < sc) return -1;
	if (dc > sc) return 1;
	dest++;	
	src++;
	}

    if (*dest && !*src) return 1;
    if (!*dest && *src) return -1;
    return 0;
}


char *lib_strchr(const char *dest,int c)
{
    while (*dest) {
	if (*dest == c) return (char *) dest;
	dest++;
	}
    return NULL;
}

char *lib_strnchr(const char *dest,int c,size_t cnt)
{
    while (*dest && (cnt > 0)) {
	if (*dest == c) return (char *) dest;
	dest++;
	cnt--;
	}
    return NULL;
}

char *lib_strrchr(const char *dest,int c)
{
    char *ret = NULL;

    while (*dest) {
	if (*dest == c) ret = (char *) dest;
	dest++;
	}

    return ret;
}


int memcmp(const void *dest,const void *src,size_t cnt)
{
   return lib_memcmp(dest, src, cnt);
}

int lib_memcmp(const void *dest,const void *src,size_t cnt)
{
    const unsigned char *d;
    const unsigned char *s;

    d = (const unsigned char *) dest;
    s = (const unsigned char *) src;

    while (cnt) {
	if (*d < *s) return -1;
	if (*d > *s) return 1;
	d++; s++; cnt--;
	}

    return 0;
}

void *memcpy(void *dest,const void *src,size_t cnt)
{
    return lib_memcpy(dest, src, cnt);
}

void *lib_memcpy(void *dest,const void *src,size_t cnt)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (cnt >= sizeof(*dl)) {
			*dl++ = *sl++;
			cnt -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (cnt--)
		*d8++ = *s8++;

    return dest;
}

void *memmove(void *dest,const void *src,size_t cnt)
{
    return lib_memmove(dest, src, cnt);
}

void *lib_memmove(void *dest,const void *src,size_t cnt)
{
    char *tmp, *s;

    if (dest <= src) {
        tmp = (char *) dest;
        s = (char *) src;
        while (cnt--)
            *tmp++ = *s++;
        }
    else {
        tmp = (char *) dest + cnt;
        s = (char *) src + cnt;
        while (cnt--)
            *--tmp = *--s;
    }

    return dest;
}

void *memset(void *dest,int c,size_t cnt)
{
   return lib_memset(dest, c, cnt);
}

void *lib_memset(void *dest,int c,size_t cnt)
{
    unsigned char *d;

    d = dest;

    while (cnt) {
	*d++ = (unsigned char) c;
	cnt--;
	}

    return d;
}

void *lib_memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    while (n-- != 0) {
        if ((unsigned char)c == *p++) {
            return (void *)(p - 1);
        }
    }
    return NULL;
}

char lib_toupper(char c)
{
    if ((c >= 'a') && (c <= 'z')) c -= 32;
    return c;
}

void lib_strupr(char *str)
{
    while (*str) {
	*str = lib_toupper(*str);
	str++;
	}
}

char *lib_strcat(char *dest,const char *src)
{
    char *ptr = dest;

    while (*ptr) ptr++;
    while (*src) *ptr++ = *src++;
    *ptr = '\0';

    return dest;
}

char *lib_strncat(char *dest,const char *src, unsigned int max_size)
{
    char *ptr = dest, *max_ptr;
    if (max_size == 0) {
        return lib_strcat(dest,src);
    }
    max_ptr = (char*)((uintptr_t)dest + max_size - sizeof(char));
    while (*ptr) ptr++;
    while (*src && ptr < max_ptr) *ptr++ = *src++;
    *ptr = '\0';

    return dest;
}

#define isspace(x) (((x) == ' ') || ((x) == '\t'))

char *lib_gettoken(char **ptr)
{
    char *p = *ptr;
    char *ret;

    /* skip white space */

    while (*p && isspace(*p)) p++;
    ret = p;

    /* check for end of string */

    if (!*p) {
	*ptr = p;
	return NULL;
	}

    /* skip non-whitespace */

    while (*p) {
	if (isspace(*p)) break;

	/* do quoted strings */

	if (*p == '"') {
	    p++;
	    ret = p;
	    while (*p && (*p != '"')) p++;
	    if (*p == '"') *p = '\0';
	    }

        p++;

	}

    if (*p) {
        *p++ = '\0';
        }
    *ptr = p;

    return ret;
}


int lib_atoi(const char *dest)
{
    int x = 0;
    int digit;

    if ((*dest == '0') && (*(dest+1) == 'x')) {
	return lib_xtoi(dest+2);
	}

    while (*dest) {
	if ((*dest >= '0') && (*dest <= '9')) {
	    digit = *dest - '0';
	    }
	else {
	    break;
	    }
	x *= 10;
	x += digit;
	dest++;
	}

    return x;
}

unsigned long lib_xtoul(const char *dest)
{
    unsigned long x = 0;
    unsigned int digit;

    if ((*dest == '0') && (*(dest+1) == 'x')) dest += 2;

    while (*dest) {
	if ((*dest >= '0') && (*dest <= '9')) {
	    digit = *dest - '0';
	    }
	else if ((*dest >= 'A') && (*dest <= 'F')) {
	    digit = 10 + *dest - 'A';
	    }
	else if ((*dest >= 'a') && (*dest <= 'f')) {
	    digit = 10 + *dest - 'a';
	    }
	else {
	    break;
	    }
	x *= 16;
	x += digit;
	dest++;
	}

    return x;
}

int lib_xtoi(const char *dest)
{
    int x = 0;
    int digit;

    if ((*dest == '0') && (*(dest+1) == 'x')) dest += 2;

    while (*dest) {
	if ((*dest >= '0') && (*dest <= '9')) {
	    digit = *dest - '0';
	    }
	else if ((*dest >= 'A') && (*dest <= 'F')) {
	    digit = 10 + *dest - 'A';
	    }
	else if ((*dest >= 'a') && (*dest <= 'f')) {
	    digit = 10 + *dest - 'a';
	    }
	else {
	    break;
	    }
	x *= 16;
	x += digit;
	dest++;
	}

    return x;
}