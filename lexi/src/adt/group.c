/*
 * Copyright (c) 2002-2005 The TenDRA Project <http://www.tendra.org/>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of The TenDRA Project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific, prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
/*
    		 Crown Copyright (c) 1997

    This TenDRA(r) Computer Program is subject to Copyright
    owned by the United Kingdom Secretary of State for Defence
    acting through the Defence Evaluation and Research Agency
    (DERA).  It is made available to Recipients with a
    royalty-free licence for its use, reproduction, transfer
    to other parties and amendment for any purpose not excluding
    product development provided that any such use et cetera
    shall be deemed to be acceptance of the following conditions:-

        (1) Its Recipients shall ensure that this Notice is
        reproduced upon any copies or amended versions of it;

        (2) Any amended version of it shall be clearly marked to
        show both the nature of and the organisation responsible
        for the relevant amendment or amendments;

        (3) Its onward transfer from a recipient to another
        party shall be deemed to be that party's acceptance of
        these conditions;

        (4) DERA gives no warranty or assurance as to its
        quality or suitability for any purpose and DERA accepts
        no liability whatsoever in relation to any use to which
        it may be put.
*/

#include <string.h>

#include "xalloc/xalloc.h"
#include "error/error.h"

#include "group.h"
#include "zone.h"
#include "tree.h"


/*
    ALLOCATE A NEW GROUP

    This routine allocates a new group
*/

static char_group * 
new_group (char* grid, zone* z) 
{
    char_group *p;
    static int groups_left = 0;
    static char_group *groups_free = NULL;
    if (groups_left == 0) {
	groups_left = 100;
	groups_free = xmalloc_nof(char_group, groups_left);
    }
    p = groups_free + (--groups_left);
    p->name=grid;
    p->defn=NULL;
    p->next=NULL;
    p->z=z;

    return p;
}



/*
    CREATE A NEW GROUP

    This routine creates a new character group with name nm and
    definition s.
*/

char_group*
make_group(zone* z,char *nm, letter *s)
{
   char_group* grp;
   lexer_parse_tree* top_level=z->top_level;
   letter_translation* trans; 
   letter_translation* reverse_trans; 
   unsigned int hash_key=hash_cstring(nm);

   for (grp = z->groups_hash_table[hash_key].head; 
	grp !=NULL; grp=grp->next) {
	if (!strcmp(nm, grp->name)) {
	    error(ERROR_SERIOUS, "Group '%s' already defined", nm);
	    return NULL;
	}
    }
    if (top_level->no_total_groups >= MAX_GROUPS) {
	error(ERROR_SERIOUS, "Too many groups defined (%d)", top_level->no_total_groups);
	return NULL;
    }
    grp=new_group(nm,z);
    grp->name = nm;
    grp->defn = s;
    grp->z = z;
    grp->group_code=top_level->no_total_groups++;

    trans=add_group_letter_translation(grp,0); /* In group match */
    reverse_trans=add_group_letter_translation(grp,1); /* Not in group match */
    letters_table_add_translation(trans, top_level->letters_table);
    letters_table_add_translation(reverse_trans, top_level->letters_table);

    grp->letter_code = trans->letter_code;
    grp->notin_letter_code = reverse_trans->letter_code;

    *(z->groups_hash_table[hash_key].tail)=grp;
    z->groups_hash_table[hash_key].tail=&(grp->next);

    *(z->top_level->groups_list.tail)=grp;
    z->top_level->groups_list.tail=&(grp->next_in_groups_list);
    return grp;
}


/*
    IS A LETTER IN A GROUP?

    This routine checks whether the letter c is in the list p.
*/

int
in_group(char_group *grp, letter c)
{
    letter a;
    letter_translation* atrans;
    letter* p= grp->defn;
    letter_translation* ctrans;
    ctrans=letters_table_get_translation(c,grp->z->top_level->letters_table);
    if (p == NULL) return 0;
    while (a = *(p++), a != grp->z->top_level->last_letter_code) {
        atrans=letters_table_get_translation(a,grp->z->top_level->letters_table);
	if (atrans->type==char_letter && atrans->u.ch == ctrans->u.ch) {
	    return 1;
	} else if (atrans->type==group_letter) {
	    if (in_group(atrans->u.grp, c)) return 1;
	} else if (atrans->type==notin_group_letter) {
	    if (!in_group(atrans->u.grp, c)) return 1;
	}
    }
    return 0;
}


/*
	IS A GROUP EMPTY?
*/

int is_group_empty(char_group *grp)
{
    letter *p = grp->defn;
    if (p == NULL) return 1;

    return *p == grp->z->top_level->last_letter_code;
}


/*
	ARE ALL GROUPS EMPTY?
*/

int all_groups_empty(char_group *list)
{
	char_group *g;

	for (g = list; g != NULL; g = g->next_in_groups_list) {
		if (!is_group_empty(g)) {
			return 0;
		}
	}

	return 1;
}
