/* $Id$ */

/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#include <stddef.h>

#include <shared/check.h>

#include <local/exptypes.h>
#include <local/expmacs.h>

#include <reader/basicread.h>
#include <reader/externs.h>

#include <construct/exp.h>
#include <construct/tags.h>
#include <construct/installglob.h>
#include <construct/me_fns.h>
#include <construct/install_fns.h>
#include <construct/shapemacs.h>

#include <refactor/refactor.h>
#include <refactor/optimise.h>

static int unroll_complex(exp, int, exp, int, exp, int);

/* MACROS */

#define LIMIT 55
#define SIMPLE_LIMIT 0
#define UNROLL_MAX 16

#define UNROLL_BY 4


/* VARIABLES */
/* All variables initialised */

/* records the uses of the control variable */
static exp names[LIMIT];	/* no init needed */
static int names_index;		/* no init needed */
/* permit removal of internal test */
static int allow_double;	/* no init needed */
static int jumps_out;		/* no init needed */

/* PROCEDURES */

static int
uc_list(exp e, int n, exp control, int lia, exp ul, int decr)
{
	int c = unroll_complex(e, n, control, lia, ul, decr);
	if (c < 0 || last(e)) {
		return c;
	}
	return uc_list(bro(e), c, control, lia, ul, decr);
}


static int
unroll_complex(exp e, int n, exp control, int lia, exp ul, int decr)
{
	/* e = body - repeated statement less label */
	/* n = complexity maximum */
	/* control = variable declaration for control variable */
	/* lia = boolean, limit is aliased */
	/* ul = variable declaration for limit if not aliased */
	/* decr = unit to decrement by */
	if (n < 0) {
		return - 1;	/* complexity exceeded */
	}

	if (son(e) == NULL) {
		if (name(e) == goto_tag) {
			/* prevent removal of internal test */
			allow_double = 0;
		}
		return n;
	}

	switch (name(e)) {
	case test_tag:
	case testbit_tag:
		if (!isunroll(pt(e))) {
			/* flag set and cleared by cond_tag below */
			/* prevent removal of internal test; jump out of loop */
			allow_double = 0;
		}
		return uc_list(son(e), n - decr, control, lia, ul, decr);
	case goto_tag:
		if (!isunroll(pt(e))) {
			/* flag set and cleared by cond_tag below */
			/* prevent removal of internal test; jump out of loop */
			allow_double = 0;
		}
		return n - 1;
	case cond_tag: {
		int t;
		setunroll(bro(son(e)));		/* mark internal label */
		if (name(sh(son(e))) == bothd) {
			t = unroll_complex(son(e), n - (4 * decr), control, lia,
					   ul, 0);
			t = unroll_complex(bro(son(e)), t - decr, control, lia,
					   ul, decr);
		} else {
			t = unroll_complex(son(e), n - decr, control, lia, ul,
					   decr);
			t = unroll_complex(bro(son(e)), t - decr, control, lia,
					   ul, decr);
		}
		clearunroll(bro(son(e)));	/* unmark it */
		return t;
	}
	case ass_tag:
	case assvol_tag: {
		exp assdest = son(e);	/* destination of assignment */
		if (name(assdest) == name_tag && son(assdest) == ul) {
			/* prevent removal of internal test; assigning to
			 * limit */
			allow_double = 0;
		}
		if (lia) {
			if (name(assdest) == name_tag && !isvar(son(assdest))) {
				/* prevent removal of internal test; perhaps
				 * assigning to limit */
				allow_double = 0;
			}
			if (name(assdest) == name_tag &&
			    !iscaonly(son(assdest))) {
				/* prevent removal of internal test; perhaps
				 * assigning to limit */
				allow_double = 0;
			}
		}
		return uc_list(son(e), n - decr, control, lia, ul, decr);
	}
	case name_tag:
		/* is this the control variable? */
		if (son(e) == control) {
			exp t;
			if (!last(e) || name(bro(e)) != cont_tag) {
				/* any use but contents -> no test elim */
				allow_double = 0;
			} else {
				/* it is a cont */
				t = bro(e);
			}
#if isalpha
				if (!last(t) || name(bro(t)) != chvar_tag ||
				    last(bro(t)) ||
				    name(bro(bro(t))) != val_tag ||
				    !last(bro(bro(t))) ||
				    name(bro(bro(bro(t)))) != offset_mult_tag) {
					/* not offset_mult -> no test elim */
					allow_double = 0;
				} else {
					/* record the use */
					names[names_index++] = bro(e);
				}
#else
				if (last(t) || name(bro(t)) != val_tag ||
				    !last(bro(t)) ||
				    name(bro(bro(t))) != offset_mult_tag) {
					/* not offset_mult -> no test elim */
					allow_double = 0;
				} else {
					/* record the use */
					names[names_index++] = bro(e);
				}
#endif
			}
		return n - decr;
	case apply_tag:
	case solve_tag:
		return -1;	/* no unroll */
	case case_tag:
		return unroll_complex(son(e), n - decr, control, lia, ul, decr);
	case string_tag:
	case env_offset_tag:
	case general_env_offset_tag:
		return n - decr;	/* decrease the complexity count */
	case top_tag:
	case prof_tag:
	case clear_tag:
		return n;
	case labst_tag:
		return unroll_complex(bro(son(e)), n, control, lia, ul, decr);
	case seq_tag:
		return uc_list(son(e), n, control, lia, ul, decr);
	case round_tag:
	case fplus_tag:
	case fminus_tag:
	case fmult_tag:
	case fdiv_tag:
	case fabs_tag:
	case fneg_tag:
	case fpower_tag:
	case fmax_tag:
	case fmin_tag:
	case float_tag:
	case chfl_tag:
		return uc_list (son(e), n - (16 * decr), control, lia, ul,
				decr);	/* heavy flpt ops */
	default:
		return uc_list (son(e), n - decr, control, lia, ul, decr);	/* other ops decrease complexity by 1 */
	}
}


void
simple_unroll(exp candidate, exp body, exp inc, exp te)
{
	/* candidate = rep_tag */
	/* body = repeated statement less label, assignment and test */
	/* inc = the single assignment to the control variable */
	/* te = the final test - only jump to repeat label */
	/* repeated statement less label, assignment and test */
	exp second_body = copy(body);
	exp second_inc = copy(inc);	/* assignment to control */
	exp second_test = copy(te);
	exp z = getexp(f_top, te, 0, NULL, NULL, 0, 0, 0);
	exp seq = getexp(f_top, bro(son(candidate)), 1, z, NULL, 0, 0,
			 seq_tag);
	exp cond_labst;
	exp cl1, mt;
	exp cond, f;
	exp *point;
	float freq = fno(bro(son(candidate)));

	/* decrease label count (increased by copy(te)) */
	no(son(bro(son(candidate))))--;

	setlast(second_inc);
	bro(second_inc) = z;
	clearlast(second_body);
	bro(second_body) = second_inc;
	clearlast(second_test);
	bro(second_test) = second_body;
	clearlast(inc);
	bro(inc) = second_test;
	clearlast(body);
	bro(body) = inc;
	son(z) = body;
	setlast(te);
	bro(te) = seq;
	bro(son(bro(son(candidate)))) = seq;

	/*
	   candidate
	   rep
	   x	labst
	   1 use	seq
	   0	te
	   body	inc	second_test	second_body	second_inc
	 */

	cond_labst = getexp(f_top, NULL, 1, NULL, NULL, 0, 0, labst_tag);
	fno(cond_labst) = (float)(freq / 20.0);
	mt = getexp(f_top, cond_labst, 1, NULL, NULL, 0, 0, top_tag);
	cl1 = getexp(f_top, mt, 0, NULL, NULL, 0, 1, clear_tag);
	son(cond_labst) = cl1;

	pt(second_test) = cond_labst;
	settest_number(second_test, (int)int_inverse_ntest[test_number(te)]);

	cond = getexp(f_top, bro(candidate), (int)(last(candidate)), candidate,
		      NULL, 0, 0, cond_tag);
	bro(cond_labst) = cond;

	f = father(candidate);
	point = refto(f, candidate);
	*point = cond;

	clearlast(candidate);
	bro(candidate) = cond_labst;

	/*
	   cond
	   cond
	   rep				labst(ln)
	   x	labst			1 use	top
	   1 use	seq
	   0	te
	   body	inc	second_test(invert, ln)	second_body	second_inc

	 */

	setunrolled(candidate);
	return;
}


static exp
inc_offset(exp var, shape sha, exp konst, exp body, int i)
{
	exp sum, t;
	exp id = son(var);
	exp rest = pt(id);
	body = copy(body);
	if (names_index > 0) {
		/* count of offset_mult uses of control variable */
		t = pt(id);
		sum = me_u3(sha, copy(var), cont_tag);
		sum = hold_refactor(me_b3(sha, sum, me_shint(sha, i * no(konst)),
				       plus_tag));	/* variable + i */

		for (i = 0; i < names_index; ++i) {
			exp q = pt(t);
			exp b = bro(t);
			/* replace the offset_mults in body */
			replace(bro(t), copy(sum), body);
			kill_exp(b, b);
			t = q;
		}
		if (t != rest) {
			failer("unroll failure");
		}

		kill_exp(sum, sum);
	}
	return body;
}


void
unroll_trans(exp candidate, exp body, exp inc, exp te, exp limit, int nt,
	     exp var, exp konst, exp reps, int times)
{
	/* candidate = rep_tag */
	/* body = repeated statement less label, assignment and test */
	/* inc = the single assignment to the control variable */
	/* te = the final test - only jump to repeat label */
	/* limit = the limit exp */
	/* nt = the test number */
	/* var = name_tag for control variable */
	/* konst = the value added to the control variable */
	/* reps = current element of the repeat list */
	/* times = no of times to unroll */
	float freq = fno(bro(son(candidate)));
	if (allow_double && no(konst) == 1 &&
	    /* allow_double==0 prevents test elimination */
	    (nt == (int)f_greater_than || nt == (int)f_greater_than_or_equal) &&
	    /* the permitted tests - we are counting upwards */
	    ((name(limit) == name_tag && !isvar(son(limit))) ||
	     name(limit) == val_tag ||
	     (name(limit) == cont_tag && name(son(limit)) == name_tag &&
	      isvar(son(son(limit)))))	/* permitted forms of limit */
	   ) {
		/* unroll and remove the internal increment and test */

		int i;
		shape sha = sh(konst);
		/* 0 - (times - 2) are preliminaries (times - 1) is test out
		 * times is the loop (times + 1) is the end */
		exp branches [UNROLL_MAX + 2];
		/* used to jump out after < times */
		exp test_out = copy(te);
		exp temp, temp1, bc, repeater, lrep, res, id, temp2;
		/* used to increment the control variable */
		exp new_c = me_shint(sha, times * no(konst));

		settest_number(test_out,
			       (int)int_inverse_ntest[test_number(test_out)]);

		for (i = 0; i < times + 2; ++i) {
			/* set up labst for branches */
			exp lia = me_shint(sha, (((i > 1) &&
						  (i < (times - 1))) ||
						 i >= times) ? 2 : 1);
			exp li = getexp(f_bottom, NULL, 0, lia, NULL, 0, 0,
					labst_tag);
			fno(li) = (float)(freq / 20.0);
			name(lia) = clear_tag;
			clearlast(lia);
			branches[i] = li;
		}
		SET(branches);
		sh(branches[times+1]) = f_top;


		for (i = 0; i < times - 1; ++ i) {
			/* set up preliminaries */
			exp sub = me_b3(f_top, copy(body), copy(inc), 0);
			exp seq = me_b3(f_bottom, sub,
					getexp(f_bottom, NULL, 0, NULL,
					       branches[i + 1], 0, 0, goto_tag),
					seq_tag);
			bro(son(branches[i])) = seq;
			setlast(seq);
			bro(seq) = branches[i];
		}

		pt(test_out) = branches[times+1];
		temp = me_u3(f_top, test_out, 0);
		temp = me_b3(f_bottom, temp,
			     getexp(f_bottom, NULL, 0, NULL,
				    branches[times], 0, 0, goto_tag), seq_tag);
		bro(son(branches[times - 1])) = temp;
		setlast(temp);
		bro(temp) = branches[times - 1];

		temp = copy(body);
		temp1 = temp;
		if (jumps_out) {
			bro(temp1) = copy(inc);
			clearlast(temp1);
			temp1 = bro(temp1);
		}
		for (i = 1; i < times - 1; ++i) {
			if (jumps_out) {
				bro(temp1) = copy(body);
			} else {
				bro(temp1) = inc_offset(var, sha, konst, body,
							i);
			}
			clearlast(temp1);
			temp1 = bro(temp1);
			if (jumps_out) {
				bro(temp1) = copy(inc);
				clearlast(temp1);
				temp1 = bro(temp1);
			}
		}
		bc = getexp(f_top, NULL, 0, temp, NULL, 0, 0, 0);
		setlast(temp1);
		bro(temp1) = bc;
		if (jumps_out) {
			bc = me_b3(f_top, bc, copy(body), seq_tag);
		} else {
			bc = me_b3(f_top, bc, inc_offset(var, sha, konst, body,
							 i), seq_tag);
			kill_exp(body, body);
		}

		if (jumps_out) {
			kill_exp(new_c, new_c);
		} else {
			/* replace konst by times * konst */
			replace(bro(son(bro(var))), new_c, new_c);
		}

		temp = me_b3(f_top, bc, inc, 0);
		temp = me_b3(f_top, temp, te, seq_tag);
		lrep = me_b3(f_top, me_shint(sha, 1), temp, labst_tag);
		fno(lrep) = freq / (float)times;
		name(son(lrep)) = clear_tag;
		repeater = me_b3(f_top, f_make_top(), lrep, rep_tag);
		son(reps) = repeater;
		pt(te) = lrep;	/* label in repeater */
		pt(test_out) = branches[times+1];

		temp = f_make_top();
		bro(son(branches[times + 1])) = temp;
		setlast(temp);
		bro(temp) = branches[times + 1];

		temp = me_u3(f_top, repeater, 0);
		temp = me_b3(f_bottom, temp,
			     getexp(f_bottom, NULL, 0, NULL,
				    branches[times + 1], 0, 0, goto_tag),
			     seq_tag);
		bro(son(branches[times])) = temp;
		setlast(temp);
		bro(temp) = branches[times];

		temp = me_u3(sha, copy(var), cont_tag);
		temp1 = copy(limit);
		sh(temp1) = sha;
		temp = hold_refactor(me_b3(sha, temp1, temp, minus_tag));
		if (nt == (int)f_greater_than) {
			temp = hold_refactor(me_b3(sha, temp, me_shint(sha, 1),
						plus_tag));
		}
		temp = hold_refactor(me_b3(sha, temp, me_shint(sha, times - 1),
					and_tag));

		id = me_startid(sha, temp, 0);
		temp = getexp(f_top, NULL, 0, me_obtain(id), branches[times],
			      0, 0, test_tag);
		settest_number(temp, f_not_equal);
		bro(son(temp)) = me_shint(sha, 0);
		setlast(bro(son(temp)));
		bro(bro(son(temp))) = temp;
		temp1 = temp;

		for (i = 1; i < (times - 1); ++i) {
			temp2 = getexp(f_top, NULL, 0, me_obtain(id),
				       branches[times - i - 1], 0, 0, test_tag);
			settest_number(temp2, f_not_equal);
			bro(son(temp2)) = me_shint(sha, i);
			setlast(bro(son(temp2)));
			bro(bro(son(temp2))) = temp2;
			settest_number(temp, f_not_equal);
			clearlast(temp1);
			bro(temp1) = temp2;
			temp1 = temp2;
		}

		bc = getexp(f_top, NULL, 0, temp, NULL, 0, 0, 0);
		setlast(temp1);
		bro(temp1) = bc;
		bc = me_b3(f_bottom, bc,
			   getexp(f_bottom, NULL, 0, NULL, branches[0], 0,
				  0, goto_tag), seq_tag);
		id = me_complete_id(id, bc);

		temp1 = id;
		for (i = 0; i < (times + 2); ++i) {
			bro(temp1) = branches[i];
			clearlast(temp1);
			temp1 = bro(temp1);
		}
		res = getexp(f_top, NULL, 0, id, NULL, 0, 0, solve_tag);
		setlast(temp1);
		bro(temp1) = res;
		setunrolled(repeater);

		replace(candidate, res, res);
	}
#if TRANS_80x86
	else {
		simple_unroll(candidate, body, inc, te);
	}
#endif
	return;
}


void
unroller(void)
{
  exp reps = repeat_list;
  exp candidate;
  exp labst;
  exp rb;


  while (reps != NULL) {
    if (no(reps) == 0 && son(reps) != NULL && name(son(reps)) == rep_tag) {
      /* this is a leaf repeat node */
      candidate = son(reps);		/* this is the repeat */
      labst = bro(son(candidate));	/* the repeated statement */
      rb = bro(son(labst));		/* the repeated statement less label */

      if (name(son(candidate)) == top_tag && no(son(labst)) == 1 &&
	  name(rb) == seq_tag && name(bro(son(rb))) == seq_tag) {
/*
	rep_tag
	top_tag	labst_tag
	count		seq_tag
	0	seq_tag
*/
	exp final = bro(son(rb));
	exp body = son(son(rb));
	exp ass = son(son(final));
	exp te = bro(son(final));
	if (name(ass) == ass_tag && name(te) == test_tag) {
/*
	rep_tag
	top_tag	labst_tag
	count		seq_tag
	0	seq_tag = final
	body	0	test_tag = te
	ass_tag = ass
*/
	  exp dest = son(ass);
	  exp val = bro(dest);
	  if (name(dest) == name_tag && isvar(son(dest)) &&
	      iscaonly(son(dest)) && shape_size(sh(val)) == 32) {
/*

	rep_tag
	top_tag	labst_tag
	count		seq_tag
	0	seq_tag = final
	body	0	test_tag = te
	ass_tag = ass
	name = dest	val (32)
	var & ca
*/
	    if (name(val) == plus_tag && name(son(val)) == cont_tag &&
		name(son(son(val))) == name_tag &&
		son(son(son(val))) == son(dest) &&
		name(bro(son(val))) == val_tag) {
/*

	rep_tag
	top_tag	labst_tag
	count		seq_tag
	0	seq_tag = final
	body	0	test_tag = te
	ass_tag = ass
	name = dest	plus_tag =val (32)
	var & ca	cont_tag	val_tag
	name_tag -> dest
*/
	      exp konst = bro(son(val));
	      int nt = (int)test_number(te);
	      if (name(son(te)) == cont_tag && name(son(son(te))) == name_tag &&
		  pt(te) == labst && son(son(son(te))) == son(dest)) {
/*
	rep_tag
	top_tag	labst_tag
	count	seq_tag
	0	seq_tag = final
	body	0	test_tag(nt, labst) = te
	ass_tag = ass	cont_tag
	name = dest	plus_tag =val (32)	name_tag -> dest
	var & ca	cont_tag val_tag = konst
	name_tag -> dest
*/
		int count;
		exp limit = bro(son(te));
		exp unaliased_limit = NULL;
		int limit_is_aliased = 0;

		if (name(limit) == cont_tag && name(son(limit)) == name_tag &&
		    isvar(son(son(limit)))) {
/*
	rep_tag
	top_tag	labst_tag
	count	seq_tag
	0	seq_tag = final
	body	0	test_tag(nt, labst) = te
	ass_tag = ass	cont_tag	cont_tag = limit
	name = dest	plus_tag =val (32)	name_tag -> dest
	var & ca	cont_tag val_tag = konst
	name_tag -> dest
*/
		  if (iscaonly(son(son(limit)))) {
		    unaliased_limit = son(son(limit));
		  } else {
		    limit_is_aliased = 1;
		  }
		}

		names_index = 0;
		allow_double = 1;
		jumps_out = 0;
		count = unroll_complex(body, LIMIT, son(dest), limit_is_aliased,
				       unaliased_limit, 1);
		if (count >= 0) {
		  unroll_trans(candidate, body, ass, te, limit, nt, dest, konst,
			       reps, UNROLL_BY);
		}
	      }
	    } else {
	      int count;
	      names_index = 0;
	      allow_double = 0;
	      count = unroll_complex(body, SIMPLE_LIMIT, NULL, 0, NULL, 1);
	      if (count >= 0) {
	        simple_unroll(candidate, body, ass, te);
	      }
  	    }
	  }
	}
      }
    }
    reps = pt(reps);
  }
  return;
}
