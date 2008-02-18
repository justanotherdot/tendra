/*
 * Copyright (c) 2002, The Tendra Project <http://www.ten15.org/>
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
 *	(1) Its Recipients shall ensure that this Notice is
 *	reproduced upon any copies or amended versions of it;
 *
 *	(2) Any amended version of it shall be clearly marked to
 *	show both the nature of and the organisation responsible
 *	for the relevant amendment or amendments;
 *
 *	(3) Its onward transfer from a recipient to another
 *	party shall be deemed to be that party's acceptance of
 *	these conditions;
 *
 *	(4) DERA gives no warranty or assurance as to its
 *	quality or suitability for any purpose and DERA accepts
 *	no liability whatsoever in relation to any use to which
 *	it may be put.
 *
 * $TenDRA$
 */

#define HPPATRANS_CODE
/*
 *  Translation is controlled by translate() in this module. *  Code generation follows the following phases:
 *
 *  1. The TDF is read in, applying bottom-up optimisations.
 *  2. Top-down optimisations are performed.
 *  3. Register allocation is performed, and TDF idents introduced
 *     so code generation can be performed with no register spills.
 *  4. Code is generated for each procedure, and global declarations processed.
 *  5. Currently assembler source is generated directly, and the
 *     assembler optimiser (as -O) used for delay slot filling,
 *     instruction scheduling and peep-hole optimisation.
 *
 *  In a little more detail:
 *
 *  1) During the TDF reading process for tag declarations and tag
 *  definitions, applications of tokens are expanded as they are
 *  encountered, using the token definitions.  Every token used must have
 *  been previously defined in the bitstream.
 *
 *  The reading of the tag definitions creates a data structure in memory
 *  which directly represents the TDF.  At present, all the tag definitions
 *  are read into memory in this way before any further translation is
 *  performed.  This will shortly be changed, so that translation is
 *  performed in smaller units.  The translator is set up already so that
 *  the change to translate in units of single global definitions (or
 *  groups of these) can easily be made.
 *
 *  During the creation of the data structure bottom-up optimisations
 *  are performed.  These are the optimisations which can be done when a
 *  complete sub-tree of TDF is present, and are independent of the context
 *  in which the sub-tree is used.  They are defined in check.c and
 *  check_id.c.  These optimisation do such things as use the commutative
 *  and associative laws for plus to collect together and evaluate
 *  constants.  More ambitious examples of these bottom-up optimisations
 *  include all constant evaluation, elimination of inaccessible code, and
 *  forward propagation of assignments of constants.
 *
 *  2) After reading in the TDF various optimisations are performed which
 *  cannot be done until the whole context is present.  For example,
 *  constants cannot be extracted from a loop when we just have the loop
 *  itself, because we cannot tell whether the variables used in it are
 *  aliased.
 *
 *  These optimisations are invoked by opt_all_exps which is defined in
 *  indep.c.  They include extraction of constants from loops, common
 *  expression elimination on them and strength reduction for indexing.
 *
 *  3) Allocatable registers are partitioned into two sets, the s regs
 *  which are preserved over calls, and the t regs which are not.
 *
 *  The TDF is scanned introducing TDF idents so that expressions can be
 *  evaluated within the available t regs with no spills.  These new idents
 *  may be later allocated to a s reg later, if the weighting algorithm
 *  (below) considers this worth while.  Otherwise they will be on the stack.
 *
 *  Information is collected to help in global register allocation.  During
 *  a forward recursive scan of the TDF the number of accesses to each
 *  variable is computed (making assumptions about the frequency of
 *  execution of loops).  Then on the return scan of this recursion, for
 *  each declaration, the number of registers which must be free for it to
 *  be worthwhile to use a register is computed, and put into the TDF as
 *  the"break" point.  The procedures to do this are defined in weights.c.
 *
 *  Suitable idents not alive over a procedure call are allocated to a t reg,
 *  and others to s regs.  At the same time stack space requirements are
 *  calculated, so this is known before code for a procedure is generated.
 *
 *  4) Finally the code is generated without register spills.  The code is
 *  generated by make_code() in makecode.c, and make_XXX_code() in proc.c.
 *
 *  Note that procedure inlining and loop unrolling optimisations are not
 *  currently implemented.  Library procedures such as memcpy() and
 *  strcpy() are not treated specially.  Integer multiply, divide and
 *  remainder use the standard support procedures .mul, .div, .rem and
 *  unsigned variants.
 */


#include "config.h"
#include "fmm.h"
#include "tenapp.h"

#include "common_types.h"
#include "myassert.h"
#include "flpt.h"
#include "frames.h"
#include "tags.h"
#include "exptypes.h"
#include "exp.h"
#include "shapemacs.h"
#include "tempdecs.h"
#include "weights.h"
#include "proctypes.h"
#include "procrec.h"
#include "regalloc.h"
#include "codehere.h"
#include "makecode.h"
#include "eval.h"
#include "toktypes.h"
#include "flags.h"
#include "basicread.h"
#include "tags.h"
#include "bitsmacs.h"
#include "needscan.h"
#include "getregs.h"
#include "regmacs.h"
#include "labels.h"
#include "comment.h"
#include "hppadiags.h"
#include "installglob.h"
#include "externs.h"
#include "locate.h"
#include "out.h"
#include "translat.h"
#include "version.h"
#include "inst_fmt.h"
#include "optimise.h"
#include "getregs.h"
#include "special.h"
#include "oprators.h"
#include "time.h"



int optim_level;		/* optimisation level from -O# option */
int maxfix_tregs;		/* the number of t regs allocatable */

char *proc_name;
char export[128];
labexp current,first;

int nexps;

FILE *outf = NULL;/* assembler output file */
dec **main_globals;
int main_globals_index;

procrec *procrecs,*cpr;

dec *diag_def = NULL;	/* diagnostics definition */

#define is_zero(e) is_comm(e)
#define TRANSLATE_GLOBALS_FIRST 1

ash
ashof(shape s)
{
	ash a;

	a.ashsize = shape_size(s);
	a.ashalign = shape_align(s);
	return a;
}


/* is shape 'sha' of unknown size? */
static bool
varsize(shape sha)
{
	switch (name(sha))
	{
	case SH_COMPOUND:
	{
		shape t = son(sha);

		if (t == nilexp)
			return 1;
		if (varsize(sh(t)))
			return 1;
		while (!last(t))
		{
			t = bro(t);
			if (varsize(sh(t)))
				return 1;
		}
		return 0;
	}

	case SH_NOF:
		return 1;

	default:
		return 0;
	}
}

void
insection(enum section s)
{
	static enum section current_section = no_section;

	if (s == current_section)
		return;

	current_section = s;

	switch (s)
	{
	case shortdata_section:
	{
		outs("\t.SHORTDATA\n");
		return;
	}
	case data_section:
	{
		outs("\t.DATA\n");
		return;
	}
	case text_section:
	{
		outs("\t.CODE\n");
		return;
	}
	case bss_section:
	case shortbss_section:
	{
		if (gcc_assembler)
		{
			/* gcc does not recognise .BSS directive */
			outs("\t.SPACE\t$PRIVATE$\n");
			outs("\t.SUBSPA\t$BSS$\n");
		}
		else
			outs("\t.BSS\n");
		return;
	}
	case no_section:
	{
		current_section = no_section;
		return;
	}
	case rodata_section:
	default: {}
	}
	failer("bad \".section\" name");
}

void
mark_unaliased(exp e)
{
	exp p = pt(e);
	bool ca = 1;
	assert(!separate_units);	/* don't know about aliases in other units */
	while (p != nilexp && ca)
	{
		if (bro(p) == nilexp)
		{
			ca = 0;
		}
		else
		{
			if (!(last(p) && name(bro(p)) == cont_tag) &&
				!(!last(p) && last(bro(p)) && name(bro(bro(p))) == ass_tag))
				ca = 0;
		}
		p = pt(p);
	};
	if (ca)
		setcaonly(e);
	return;
}

baseoff
find_tg(char *n)
{
	int i;
	for (i=0;i<main_globals_index;i++)
	{
		exp tg = main_globals[i] -> dec_u.dec_val.dec_exp;
		char *id = main_globals[i] -> dec_u.dec_val.dec_id;
		if (strcmp(id,n)==0)
		{
			return boff(tg);
		}
	}
	failer("Extension name not declared ");
	exit(EXIT_FAILURE);
}


/* translate the TDF */
void
translate_capsule(void)
{
	int noprocs;
	int procno;
	int i;
	dec *crt_def,**proc_def_trans_order;
	int *src_line=0,next_proc_def;
	space tempregs;
	int r;
	static int capn=0;
	capn++;

	/* mark the as output as TDF compiled */
	outs("\t;  Produced by the ");
	fprintf(outf,"%s %s", progdesc, progvers);
	outnl();
	outnl();
	outnl();
	outs("\t.SPACE  $TEXT$,SORT=8\n");
	outs("\t.SUBSPA $CODE$,QUAD=0,ALIGN=8,ACCESS=44,CODE_ONLY,SORT=24\n");
	outnl();
	outs("\t.SPACE  $PRIVATE$,SORT=16\n");
	outs("\t.SUBSPA $DATA$,QUAD=1,ALIGN=8,ACCESS=31,SORT=16\n\n");
	outs("\t.IMPORT\t$$dyncall,CODE\n");
	if (do_profile)
		outs("\t.IMPORT\t_mcount,CODE\n");
	outs("\t.IMPORT\t$global$,DATA\n");
	outnl();

#if 0
	outs("LB\t.MACRO\tTARGET\n");
	outs("\tldil\tL'TARGET,%r1\n");
	outs("\tldo\tR'TARGET(%r1),%r1\n");
	outs("\tbv\t0(%r1)\n");
	outs("\tnop\n");
	outnl();
#endif

	/* Begin diagnostics if necessary. */
	if (diagnose)
	{
		outs("\t.CODE\n");
		outnl();
		init_stab_aux();
		outnl();
		outnl();
	}

	setregalt(nowhere.answhere, 0);
	nowhere.ashwhere.ashsize = 0;
	nowhere.ashwhere.ashsize = 0;

	if (!diagnose)
		opt_all_exps();  /* optimise */
	/* mark static unaliased; count procs */
	noprocs = 0;
	for (crt_def = top_def; crt_def != (dec *) 0; crt_def = crt_def->def_next)
	{
		exp crt_exp = crt_def->dec_u.dec_val.dec_exp;
		exp scexp = son(crt_exp);
		if (scexp != nilexp)
		{
			if (!diagnose && !separate_units &&
				!crt_def->dec_u.dec_val.extnamed && isvar(crt_exp))
				mark_unaliased(crt_exp);
			if (name(scexp) == proc_tag || name(scexp)== general_proc_tag)
			{
				noprocs++;
				if (!strncmp("__I.TDF",crt_def->dec_u.dec_val.dec_id,7))
				{
					char *s;
					static char dyn = 0;
					if (!dyn)
					{
						outs("\t.SPACE  $PRIVATE$,SORT=16\n");
						outs("\t.SUBSPA $DYNDATA$,QUAD=1,ALIGN=4,ACCESS=31,SORT=16\n");
						outnl();
						dyn = 1;
					}
					s = (char*) xcalloc(64,sizeof(char));
					sprintf(s,"_GLOBAL_$I%d",capn);
					strcat(s,crt_def->dec_u.dec_val.dec_id+7);
					crt_def->dec_u.dec_val.dec_id = s;
					if (!gcc_assembler)
						fprintf(outf,"\t.WORD\t%s\n",s);
				}
			}
		}
	};
	outnl();

	/* alloc memory */
	procrecs = (procrec *) xcalloc(noprocs, sizeof (procrec));

	proc_def_trans_order = (dec**) xcalloc(noprocs, sizeof (dec*));
	if (xdb)
	{
		src_line = (int*) xcalloc(noprocs,sizeof(int));
	}

	/* number proc defs */
	procno = 0;
	for (crt_def = top_def; crt_def != (dec *) 0; crt_def = crt_def->def_next)
	{
		exp crt_exp = crt_def->dec_u.dec_val.dec_exp;

		if (son(crt_exp) != nilexp && (name(son(crt_exp)) == proc_tag ||
									   name(son(crt_exp))== general_proc_tag))
		{
			procrec *pr = &procrecs[procno];
			proc_def_trans_order[procno] = crt_def;
			if (xdb)
			{
				/* Retrieve diagnostic info neccessary to comply with xdb's
				 *	    requirement that procedures be compiled in source file order. */
				diag_descriptor * dd =  crt_def -> dec_u.dec_val.diag_info;
				if (dd != (diag_descriptor*)0)
				{
					sourcemark *sm = &dd -> data.id.whence;
					src_line[procno] = sm->line_no.nat_val.small_nat;
				}
				else
					src_line[procno] = 0;
			}
			pr->nameproc = bro(crt_exp);
			no(son(crt_exp)) = procno++;/* index into procrecs in no(proc) */
		}
	}


	/*
	 * Scan to put everything in HP_PA form, and calculate register and stack
	 * space needs.
	 */

	/*
	 *      First work out which fixed point t-regs, i.e. those not preserved
	 *  over calls, can be used. This needs to be done before scan() which
	 *  adds idents so temp reg needs are within available temp reg set.
	 *
	 */

	/* initial reg sets */
	tempregs.fixed = PROC_TREGS;
	tempregs.flt = PROC_FLT_TREGS;

	/* GR0,GR1,SP,DP are NEVER allocatable */
	tempregs.fixed |= RMASK(GR0);
	tempregs.fixed |= RMASK(GR1);
	tempregs.fixed |= RMASK(SP);
	tempregs.fixed |= RMASK(DP);
	if (PIC_code)
	{
		tempregs.fixed |= RMASK(GR19); /* %r19 is reserved in PIC mode */
	}

	/* count t fixed point regs we can use, and set the global maxfix_tregs */
	maxfix_tregs = 0;
	for (r = R_FIRST; r <= R_LAST; r++)
	{
		/* bit clear means allocatable */
		if (IS_TREG(r) && (tempregs.fixed & RMASK(r)) == 0)
			maxfix_tregs++;
	}
	comment4("maxfix_tregs=%d(%#x) maxfloat_tregs=%d(%#x)",
			 maxfix_tregs, tempregs.fixed, MAXFLOAT_TREGS, tempregs.flt);

	/* scan all the procs, to put everything in HP_PA form */
	nexps = 0;
	for (crt_def = top_def; crt_def != (dec *) 0; crt_def = crt_def->def_next)
	{
		exp crt_exp = crt_def->dec_u.dec_val.dec_exp;
		if (son(crt_exp) != nilexp && (name(son(crt_exp)) == proc_tag ||
									   name(son(crt_exp))== general_proc_tag))
		{
			procrec *pr = &procrecs[no(son(crt_exp))];
			exp *st = &son(crt_exp);
			cpr = pr;
			cpr->Has_ll = 0;
			cpr->Has_checkalloc = 0;
			builtin=0;
			pr->needsproc = scan(st, &st);
			pr->callee_sz = callee_sz;
			pr->needsproc.builtin=builtin;
		}
	}

	/* calculate the break points for register allocation */
	for (crt_def = top_def; crt_def != (dec *) 0; crt_def = crt_def->def_next)
	{
		exp crt_exp = crt_def->dec_u.dec_val.dec_exp;

		if (son(crt_exp) != nilexp && (name(son(crt_exp)) == proc_tag ||
									   name(son(crt_exp))== general_proc_tag))
		{
			procrec *pr = &procrecs[no(son(crt_exp))];
			needs * ndpr = & pr->needsproc;
			long pprops = (ndpr->propsneeds);
			bool leaf = (pprops & anyproccall) == 0;
			weights w;
			spacereq forrest;
			int freefixed, freefloat;
			int No_S = (!leaf && proc_uses_crt_env(son(crt_exp)) && proc_has_lv(son(crt_exp)));
			proc_name = crt_def->dec_u.dec_val.dec_id;

			setframe_flags(son(crt_exp),leaf);

			/* free s registers = GR3,GR4,..,GR18 */
			freefixed = 16;

			if (Has_fp) /* Has frame pointer */
			{
				freefixed--;
				/* reserve GR3 as frame pointer (i.e. points to bottom of stack) */
			}
			if (Has_vsp) /* Has variable stack pointer */
			{
				freefixed--;
				/* reserve GR4 for use as copy of the original stack pointer */
			}
			if (is_PIC_and_calls)
			{
				freefixed--;
				/* best reserve GR5 for use as a copy of GR19 */
			}
			if (Has_vcallees)
			{
				pr->callee_sz = 0; /*  Don't know callee_sz  */
			}

			real_reg[1] = GR4;
			real_reg[2] = GR5;
			if (Has_fp)
			{
				if (is_PIC_and_calls && !Has_vsp)
					real_reg[2] = GR4;
			}
			else
			{
				if (Has_vsp)
				{
					if (is_PIC_and_calls)
						real_reg[2] = GR3;
					else
						real_reg[1] = GR3;
				}
				else
					if (is_PIC_and_calls)
					{
						real_reg[1] = GR3;
						real_reg[2] = GR4;
					}
			}

			/* +++ create float s regs for leaf? */
			freefloat = 0;		/* none, always the same */

			/* estimate usage of tags in body of proc */
			if (!No_S)
				w = weightsv(UNITWEIGHT, bro(son(son(crt_exp))));

			/* reg and stack allocation for tags */
			forrest = regalloc(bro(son(son(crt_exp))), freefixed, freefloat, 0);

			/* reg and stack allocation for tags */
			pr->spacereqproc = forrest;

			set_up_frame(son(crt_exp));
		}
	}


	/*  Set up main_globals and output global definitions. */
	i = 0;
	for (crt_def=top_def; crt_def!=(dec*)0; crt_def=crt_def->def_next)
	{
		i++;
	}
	main_globals_index = i;
	main_globals = (dec**) xcalloc(main_globals_index,sizeof(dec*));
	i = 0;
	for (crt_def = top_def; crt_def != (dec *) 0; crt_def = crt_def->def_next)
	{
		main_globals[i] = crt_def;
		main_globals[i]->dec_u.dec_val.sym_number = i;
		i++;
	}

	for (crt_def = top_def; crt_def != (dec *) 0; crt_def = crt_def->def_next)
	{
		exp tg = crt_def->dec_u.dec_val.dec_exp;
		char *id = crt_def->dec_u.dec_val.dec_id;
		bool extnamed = (bool) crt_def->dec_u.dec_val.extnamed;
		if (son(tg)==nilexp && no(tg)!=0 && extnamed)
		{
			outs("\t.IMPORT\t");
			outs(id);
			outs(name(sh(tg))==SH_PROC ? (isvar(tg) ? ",DATA\n" : ",CODE\n") : ",DATA\n");
		}
		else
			if (son(tg) != nilexp && (extnamed || no(tg) != 0))
			{
				if (name(son(tg)) != proc_tag && name(son(tg)) != general_proc_tag)
				{
					/* evaluate all outer level constants */
					instore is;
					long symdef = crt_def->dec_u.dec_val.sym_number + 1;
					if (isvar (tg))
						symdef = -symdef;
					if (extnamed && !(is_zero(son(tg))))
					{
						outs("\t.EXPORT\t");
						outs(id);
						outs(",DATA\n");
					}
					is = evaluated(son(tg),symdef);
					if (diagnose)
					{
						diag_def = crt_def;
						stab_global(son(tg), id, extnamed);
					}

					if (is.adval)
					{
						setvar(tg);
					}
				}
			}
	}


	/* Uninitialized data local to module. */

	for (crt_def=top_def; crt_def != (dec *) 0; crt_def=crt_def->def_next)
	{
		exp tg = crt_def->dec_u.dec_val.dec_exp;
		char *id = crt_def->dec_u.dec_val.dec_id;
		bool extnamed = (bool) crt_def->dec_u.dec_val.extnamed;
		if (son(tg) == nilexp && no(tg)!=0 && !extnamed)
		{
			shape s = crt_def->dec_u.dec_val.dec_shape;
			ash a;
			long size;
			int align;
			a = ashof(s);
			size = (a.ashsize + 7) >> 3;
			align = ((a.ashalign > 32 || a.ashsize > 32) ? 8 : 4);
			if (size>8)
				insection(bss_section);
			else
				insection(shortbss_section);
			outs("\t.ALIGN\t");
			outn(align);
			outs(id);
			outs("\t.BLOCKZ\t");
			outn(size);
		}
	}

	/* Translate the procedures. */

	if (xdb)
	{
		/*  XDB requires the procedures to be translated in the order
		 *	 that they appear in the c source file.  */
		int n,j;
		for (n=0; n<noprocs; n++)
			for (j=n+1; j<noprocs; j++)
			{
				if (src_line[n] > src_line[j])
				{
					int srcl = src_line[n];
					dec *pdef;
					src_line[n] = src_line[j];
					src_line[j] = srcl;
					pdef = proc_def_trans_order[n];
					proc_def_trans_order[n] = proc_def_trans_order[j];
					proc_def_trans_order[j] = pdef;

				}
			}
	}
	else
	{
#if TRANSLATE_GLOBALS_FIRST
		/*  Translate the global procedures first.  */
		int fstat = 0, lglob = noprocs-1;
		while (fstat<lglob)
		{
			while (fstat<noprocs && proc_def_trans_order[fstat]->dec_u.dec_val.extnamed)
				fstat++;
			while (lglob>0 && !proc_def_trans_order[lglob]->dec_u.dec_val.extnamed)
				lglob--;
			if (fstat<lglob)
			{
				dec *pdef;
				pdef = proc_def_trans_order[fstat];
				proc_def_trans_order[fstat] = proc_def_trans_order[lglob];
				proc_def_trans_order[lglob] = pdef;
				fstat++;
				lglob--;
			}
		}
#endif
	}

	for (next_proc_def=0; next_proc_def < procno; next_proc_def++)
	{
		exp tg, crt_exp;
		char *id;
		bool extnamed;
		procrec *pr;
		crt_def = proc_def_trans_order[next_proc_def];
		tg = crt_def->dec_u.dec_val.dec_exp;
		id = crt_def->dec_u.dec_val.dec_id;
		extnamed = (bool) crt_def->dec_u.dec_val.extnamed;

		if (no(tg)!=0 || extnamed)
		{
			crt_exp = crt_def->dec_u.dec_val.dec_exp;
			pr = & procrecs[no(son(crt_exp))];
			insection(text_section);
			outnl();
			outnl();
			if (diagnose)
			{
				diag_def = crt_def;
				stab_proc(son(tg), id, extnamed);
			}
			seed_label();		/* reset label sequence */
			settempregs(son(tg));	/* reset getreg sequence */

			first = (labexp) malloc(sizeof(struct labexp_t));
			first->e = (exp) 0;
			first->next = (labexp) 0;
			current = first;

			proc_name=id;
			code_here(son(tg), tempregs, nowhere);

			outs("\t.PROCEND\n\t;");
			outs(id);
			if (xdb)
			{
#if _SYMTAB_INCLUDED
				close_function_scope(res_label);
				outnl();
				outs("_");
				outs(id);
				outs("_end_");
#endif
			}
			outnl();
			outnl();
			if (extnamed)
			{
				outs("\t.EXPORT ");
				outs(id);
				outs(",ENTRY");
				outs(export);
				outnl();
				outnl();
				outnl();
			}
			if (first->next != (labexp)0)
			{
				exp e,z;
				labexp p,next;
				ash a;
				int lab,byte_size;
				outs("\n\n");
				next = first->next;
				do
				{
					e = next->e;
					z = e;
					a = ashof(sh(e));
					lab = next->lab;
					if (is_zero(e))
					{
						byte_size = (a.ashsize+7) >> 3;
						if (byte_size>8)
							insection(bss_section);
						else
							insection(shortbss_section);
						if (a.ashalign > 32 || a.ashsize > 32)
							set_align(64);
						else
							set_align(32);
						outs(ext_name(lab));
						outs("\t.BLOCK\t");
						outn(byte_size);
						outnl();
					}
					else
					{
						insection(data_section);
						if (a.ashalign > 32 || a.ashsize > 32)
							set_align(64);
						else
							set_align(32);
						outs(ext_name(lab));
						outnl();
						evalone(z,0);
						if (a.ashalign>32)
							set_align(64);
					}
					next = next->next;
				}
				while (next!=0);
				next = first;
				do
				{
					p = next->next;
					free(next);
					next = p;
				}
				while (next!=0);
				outs("\t.CODE\n\n\n");
			}
			else
				free(first);
		}
	}

	return;
}

void
translate_tagdef(void)
{
	return;
}

void
translate_unit(void)
{
	if (separate_units)
	{
		dec *crt_def;

		translate_capsule();
		crt_def = top_def;
		while (crt_def != (dec *) 0)
		{
			exp crt_exp = crt_def->dec_u.dec_val.dec_exp;

			no(crt_exp) = 0;
			pt(crt_exp) = nilexp;
			crt_def = crt_def->def_next;
		};
		crt_repeat = nilexp;
		repeat_list = nilexp;
	};
	return;
}



/*
 *    EXIT TRANSLATOR
 */

void
exit_translator(void)
{
	outnl();
	outnl();
	outnl();
	outnl();
	import_millicode();
#if use_long_double
	import_long_double_lib();
#endif
	outnl();
	outnl();
	if (xdb)
	{
#ifdef _SYMTAB_INCLUDED
		output_DEBUG();
		outnl();
		outnl();
#endif
	}
	outs("\t.END\n");
	return;
}


















