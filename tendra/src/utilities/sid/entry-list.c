/*
 * Copyright (c) 2002-2004, The Tendra Project <http://www.ten15.org/>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *    		 Crown Copyright (c) 1997
 *
 *    This TenDRA(r) Computer Program is subject to Copyright
 *    owned by the United Kingdom Secretary of State for Defence
 *    acting through the Defence Evaluation and Research Agency
 *    (DERA).  It is made available to Recipients with a
 *    royalty-free licence for its use, reproduction, transfer
 *    to other parties and amendment for any purpose not excluding
 *    product development provided that any such use et cetera
 *    shall be deemed to be acceptance of the following conditions:-
 *
 *        (1) Its Recipients shall ensure that this Notice is
 *        reproduced upon any copies or amended versions of it;
 *
 *        (2) Any amended version of it shall be clearly marked to
 *        show both the nature of and the organisation responsible
 *        for the relevant amendment or amendments;
 *
 *        (3) Its onward transfer from a recipient to another
 *        party shall be deemed to be that party's acceptance of
 *        these conditions;
 *
 *        (4) DERA gives no warranty or assurance as to its
 *        quality or suitability for any purpose and DERA accepts
 *        no liability whatsoever in relation to any use to which
 *        it may be put.
 *
 * $TenDRA$
 */


/*** entry-list.c --- Identifier table entry list ADT.
 *
 ** Author: Steve Folkes <smf@hermes.mod.uk>
 *
 *** Commentary:
 *
 * This file implements the SID table entry list routines.
 */

/****************************************************************************/

#include "entry-list.h"
#include "action.h"
#include "basic.h"
#include "name.h"
#include "rule.h"
#include "type.h"

/*--------------------------------------------------------------------------*/

typedef struct EntrySortListT {
	EntryListEntryP		head;
	EntryListEntryP	       *tail;
} EntrySortListT, *EntrySortListP;

/*--------------------------------------------------------------------------*/

static EntryListEntryP
entry_list_find(EntryListP list, EntryP entry)
{
	EntryListEntryP link = list->head;
	
	while (link) {
		if (link->entry == entry) {
			return (link);
		}
		link = link->next;
	}
	return (NIL (EntryListEntryP));
}

/*--------------------------------------------------------------------------*/

void
entry_list_init(EntryListP list)
{
	list->head = NIL (EntryListEntryP);
	list->tail = &(list->head);
}

void
entry_list_copy(EntryListP to, EntryListP from)
{
	EntryListEntryP ptr;
	
	to->head = NIL (EntryListEntryP);
	to->tail = &(to->head);
	for (ptr = from->head; ptr; ptr = ptr->next) {
		entry_list_add (to, ptr->entry);
	}
}

void
entry_list_add(EntryListP list, EntryP entry)
{
	EntryListEntryP link = ALLOCATE (EntryListEntryT);
	
	link->next    = NIL (EntryListEntryP);
	link->entry   = entry;
	*(list->tail) = link;
	list->tail    = &(link->next);
}

void
entry_list_add_if_missing(EntryListP list,
						  EntryP entry)
{
	if (entry_list_find (list, entry) == NIL (EntryListEntryP)) {
		entry_list_add (list, entry);
	}
}

BoolT
entry_list_contains(EntryListP list, EntryP entry)
{
	return (entry_list_find (list, entry) != NIL (EntryListEntryP));
}

BoolT
entry_list_includes(EntryListP list1, EntryListP list2)
{
	EntryListEntryP ptr;
	
	for (ptr = list2->head; ptr; ptr = ptr->next) {
		if (entry_list_find (list1, ptr->entry) == NIL (EntryListEntryP)) {
			return (FALSE);
		}
	}
	return (TRUE);
}

void
entry_list_intersection(EntryListP to, EntryListP list1,
						EntryListP list2)
{
	EntryListEntryP ptr;
	
	entry_list_init (to);
	for (ptr = list1->head; ptr; ptr = ptr->next) {
		if (entry_list_find (list2, ptr->entry) != NIL (EntryListEntryP)) {
			entry_list_add_if_missing (to, ptr->entry);
		}
	}
}

void
entry_list_unlink_used(EntryListP to, EntryListP from)
{
	EntryListEntryP ptr;
	
	to->tail = &(to->head);
	while ((ptr = *(to->tail)) != NIL (EntryListEntryP)) {
		if (entry_list_find (from, ptr->entry) != NIL (EntryListEntryP)) {
			*(to->tail) = ptr->next;
			DEALLOCATE (ptr);
		} else {
			to->tail = &(ptr->next);
		}
	}
}

void
entry_list_append(EntryListP to, EntryListP from)
{
	EntryListEntryP ptr;
	
	for (ptr = from->head; ptr; ptr = ptr->next) {
		entry_list_add_if_missing (to, ptr->entry);
	}
}

#ifdef FS_FAST
#undef entry_list_is_empty
#endif /* defined (FS_FAST) */
BoolT
entry_list_is_empty(EntryListP list)
{
	return (list->head == NIL (EntryListEntryP));
}
#ifdef FS_FAST
#define entry_list_is_empty(e) ((e)->head == NIL (EntryListEntryP))
#endif /* defined (FS_FAST) */

void
entry_list_save_state(EntryListP list, SaveListP state)
{
	state->last_ref = list->tail;
}

void
entry_list_restore_state(EntryListP list,
						 SaveListP state)
{
	EntryListEntryP ptr = *(state->last_ref);
	
	*(state->last_ref) = NIL (EntryListEntryP);
	list->tail         = state->last_ref;
	while (ptr) {
		EntryListEntryP tmp = ptr;
		
		ptr = ptr->next;
		DEALLOCATE (tmp);
	}
}

void
entry_list_iter(EntryListP list, void (*proc)(EntryP, void *),
		void *closure)
{
	EntryListEntryP ptr;
	
	for (ptr = list->head; ptr; ptr = ptr->next) {
		(*proc) (ptr->entry, closure);
	}
}

void
entry_list_iter_table(EntryListP list, BoolT full,
		void (*proc)(EntryP, void *), void *closure)
{
	EntryListEntryP ptr;
	
	for (ptr = list->head; ptr; ptr = ptr->next) {
		entry_iter (ptr->entry, full, proc, closure);
	}
}

void
entry_list_destroy(EntryListP list)
{
	EntryListEntryP ptr = list->head;
	
	while (ptr) {
		EntryListEntryP tmp = ptr;
		
		ptr = ptr->next;
		DEALLOCATE (tmp);
	}
}

void
write_entry_list(OStreamP ostream, EntryListP list)
{
	EntryListEntryP ptr = list->head;
	char *sep = "";
	
	while (ptr) {
		write_cstring (ostream, sep);
		write_char (ostream, '\'');
		write_key (ostream, entry_key (ptr->entry));
		write_char (ostream, '\'');
		if ((ptr = ptr->next) && (ptr->next)) {
			sep = " & ";
		} else {
			sep = ", ";
		}
	}
}