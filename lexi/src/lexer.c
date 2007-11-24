/* 
 *  Copyright (c) 2002-2007 The TenDRA Project <http://www.tendra.org/>.
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of The TenDRA Project nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific, prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 *  IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 *     		 Crown Copyright (c) 1997
 *     
 *     This TenDRA(r) Computer Program is subject to Copyright
 *     owned by the United Kingdom Secretary of State for Defence
 *     acting through the Defence Evaluation and Research Agency
 *     (DERA).  It is made available to Recipients with a
 *     royalty-free licence for its use, reproduction, transfer
 *     to other parties and amendment for any purpose not excluding
 *     product development provided that any such use et cetera
 *     shall be deemed to be acceptance of the following conditions:-
 *     
 *         (1) Its Recipients shall ensure that this Notice is
 *         reproduced upon any copies or amended versions of it;
 *     
 *         (2) Any amended version of it shall be clearly marked to
 *         show both the nature of and the organisation responsible
 *         for the relevant amendment or amendments;
 *     
 *         (3) Its onward transfer from a recipient to another
 *         party shall be deemed to be that party's acceptance of
 *         these conditions;
 *     
 *         (4) DERA gives no warranty or assurance as to its
 *         quality or suitability for any purpose and DERA accepts
 *         no liability whatsoever in relation to any use to which
 *         it may be put.
 * 
 * $Id$
 * 
 */

/*
 *  AUTOMATICALLY GENERATED BY lexi VERSION 2.0
 */

#include "lexer.h"

#include <assert.h>
#include <stdint.h>

struct lexi_lexer_state_tag {
	int (*zone_function)(struct lexi_lexer_state_tag*);
};
/* LOOKUP TABLE */

typedef uint8_t lookup_type;
static lookup_type lookup_tab[257] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
	0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x0a,
	0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00
};

/*
 * Lexi's buffer is a simple stack. The size is calculated as
 * max(mapping) - 1 + max(token) - 1
 */
static int lexi_buffer[5 - 1];
static int lexi_buffer_index;

/* Push a character to lexi's buffer */
void lexi_push(const int c) {
	assert(lexi_buffer_index < sizeof lexi_buffer / sizeof *lexi_buffer);
	lexi_buffer[lexi_buffer_index++] = c;
}

/* Pop a character from lexi's buffer */
int lexi_pop(void) {
	assert(lexi_buffer_index > 0);
	return lexi_buffer[--lexi_buffer_index];
}

/* Flush lexi's buffer */
void lexi_flush(void) {
	lexi_buffer_index = 0;
}

/* Read a character */
int lexi_readchar(void) {
	if(lexi_buffer_index) {
		return lexi_pop();
	}

	return read_char();
}

int lexi_group(enum lexi_groups group, int c) {
	return lookup_tab[c] & group;
}



/* KEYWORDS */
#include <string.h>
int lexi_keyword(const char *identifier, int notfound) {
	if(!strcmp(identifier, "COPYRIGHT")) return lex_copyright;
	if(!strcmp(identifier, "DEFAULT")) return lex_default;
	if(!strcmp(identifier, "ELSE")) return lex_else;
	if(!strcmp(identifier, "GROUP")) return lex_group;
	if(!strcmp(identifier, "IF")) return lex_if;
	if(!strcmp(identifier, "KEYWORD")) return lex_keyword;
	if(!strcmp(identifier, "MAPPING")) return lex_mapping;
	if(!strcmp(identifier, "TOKEN")) return lex_token;
	if(!strcmp(identifier, "ZONE")) return lex_zone;
	if(!strcmp(identifier, "white")) return lex_white;
	return notfound;
}
/* PRE-PASS ANALYSERS */

lexi_lexer_state lexi_current_lexer_state_v={&lexi_read_token};
lexi_lexer_state* lexi_current_lexer_state=&lexi_current_lexer_state_v;/* ZONES PASS ANALYSER PROTOTYPES*/

static int lexi_read_token_line_comment(struct lexi_lexer_state_tag* state);
static int lexi_read_token_comment(struct lexi_lexer_state_tag* state);
/* MAIN PASS ANALYSERS */

/* MAIN PASS ANALYSER for zone line_comment*/

static int
lexi_read_token_line_comment(lexi_lexer_state* state)
{
	start: {
	int c0 = lexi_readchar();
	if (lexi_group(lexi_group_line_comment_white, c0)) goto start;
	if (c0 == '\n') {
	    state->zone_function=&lexi_read_token;
	    return(lexi_read_token(state));
	}
	goto start;
	}
}
/* MAIN PASS ANALYSER for zone comment*/

static int
lexi_read_token_comment(lexi_lexer_state* state)
{
	start: {
	int c0 = lexi_readchar();
	if (lexi_group(lexi_group_comment_white, c0)) goto start;
	if (c0 == '*') {
	    int c1 = lexi_readchar();
	    if (c1 == '/') {
		state->zone_function=&lexi_read_token;
		return(lexi_read_token(state));
	    }
	    lexi_push(c1);
	}
	goto start;
	}
}
/* MAIN PASS ANALYSER for zone global*/

int
lexi_read_token(lexi_lexer_state *state)
{
	if(state->zone_function!=&lexi_read_token)
		return ((*state->zone_function)(state));
	start: {
	int c0 = lexi_readchar();
	if (lexi_group(lexi_group_white, c0)) goto start;
	switch (c0) {
	    case '"': {
		return(get_string(c0));
	    }
	    case '#': {
		int c1 = lexi_readchar();
		if (c1 == '#') {
		    return(lex_arg_Hchar_Hlist);
		} else if (c1 == '$') {
		    return(lex_arg_Hchar_Hvoid);
		} else if (c1 == '*') {
		    return(lex_arg_Hchar_Hstring);
		}
		if (lexi_group(lexi_group_digit, c1)) {
		    return(read_arg_char_nb(c0, c1));
		}
		lexi_push(c1);
		break;
	    }
	    case '$': {
		int c1 = lexi_readchar();
		if (c1 == '$') {
		    return(lex_nothing_Hmarker);
		}
		if (lexi_group(lexi_group_alpha, c1)) {
		    return(get_sid_ident(c0, c1));
		}
		lexi_push(c1);
		break;
	    }
	    case '(': {
		return(lex_open);
	    }
	    case ')': {
		return(lex_close);
	    }
	    case '+': {
		return(lex_plus);
	    }
	    case ',': {
		return(lex_comma);
	    }
	    case '-': {
		int c1 = lexi_readchar();
		if (c1 == '>') {
		    return(lex_arrow);
		}
		lexi_push(c1);
		break;
	    }
	    case '.': {
		int c1 = lexi_readchar();
		if (c1 == '.') {
		    int c2 = lexi_readchar();
		    if (c2 == '.') {
			return(lex_range);
		    }
		    lexi_push(c2);
		}
		lexi_push(c1);
		break;
	    }
	    case '/': {
		int c1 = lexi_readchar();
		if (c1 == '*') {
		    state->zone_function=&lexi_read_token_comment;
		    return(lexi_read_token(state));
		} else if (c1 == '/') {
		    state->zone_function=&lexi_read_token_line_comment;
		    return(lexi_read_token(state));
		}
		lexi_push(c1);
		break;
	    }
	    case ':': {
		return(lex_colon);
	    }
	    case ';': {
		return(lex_semicolon);
	    }
	    case '=': {
		return(lex_equal);
	    }
	    case '{': {
		int c1 = lexi_readchar();
		if (c1 == '0') {
		    int c2 = lexi_readchar();
		    if (c2 == '-') {
			int c3 = lexi_readchar();
			if (c3 == '9') {
			    int c4 = lexi_readchar();
			    if (c4 == '}') {
				return(lex_digit);
			    }
			    lexi_push(c4);
			}
			lexi_push(c3);
		    }
		    lexi_push(c2);
		} else if (c1 == 'A') {
		    int c2 = lexi_readchar();
		    if (c2 == '-') {
			int c3 = lexi_readchar();
			if (c3 == 'Z') {
			    int c4 = lexi_readchar();
			    if (c4 == '}') {
				return(lex_upper);
			    }
			    lexi_push(c4);
			}
			lexi_push(c3);
		    }
		    lexi_push(c2);
		} else if (c1 == 'a') {
		    int c2 = lexi_readchar();
		    if (c2 == '-') {
			int c3 = lexi_readchar();
			if (c3 == 'z') {
			    int c4 = lexi_readchar();
			    if (c4 == '}') {
				return(lex_lower);
			    }
			    lexi_push(c4);
			}
			lexi_push(c3);
		    }
		    lexi_push(c2);
		}
		lexi_push(c1);
		return(lex_open_Hbrace);
	    }
	    case '}': {
		return(lex_close_Hbrace);
	    }
	    case LEX_EOF: {
		return(lex_eof);
	    }
	}
	if (lexi_group(lexi_group_alpha, c0)) {
	    return(get_identifier(c0));
	}
	return(unknown_token(c0));
	}
}
