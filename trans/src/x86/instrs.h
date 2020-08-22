/*
 * Copyright 2008-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#ifndef INSTRS_H
#define INSTRS_H

#define push_fl temp_push_fl()
#define pop_fl temp_pop_fl()

#define adcb	"adcb"
#define adcl	"adcl"
#define adcw	"adcw"
#define addb	"addb"
#define addl	"addl"
#define addw	"addw"
#define andb	"andb"
#define andl	"andl"
#define andw	"andw"
#define call	"call"
#define cltd	"cltd"
#define cmpb	"cmpb"
#define cmpl	"cmpl"
#define cmpsb	"cmpsb"
#define cmpw	"cmpw"
#define decb	"decb"
#define decl	"decl"
#define decw	"decw"
#define divb	"divb"
#define divl	"divl"
#define divw	"divw"
#define fabs	"fabs"
#define fadd	"fadd"
#define faddp	"faddp"
#define faddl	"faddl"
#define fadds	"fadds"
#define fchs	"fchs"
#define fclex	"fclex"
#define fcoml	"fcoml"
#define fcomp	"fcomp"
#define fcompp	"fcompp"
#define fdiv	"fdiv"
#define fdivp	"fdivp"
#define fdivl	"fdivl"
#define fdivrl	"fdivrl"
#define fdivr	"fdivr"
#define fdivrp	"fdivrp"
#define fdivrs	"fdivrs"
#define fdivs	"fdivs"
#define fild	"fild"
#define fildl	"fildl"
#define fildll	"fildll"
#define finit	"finit"
#define fistp	"fistp"
#define fistpl	"fistpl"
#define fistpll	"fistpll"
#define fld	"fld"
#define fld1	"fld1"
#define fldl	"fldl"
#define fldcw	"fldcw"
#define flds	"flds"
#define fldt	"fldt"
#define fldz	"fldz"
#define fmul	"fmul"
#define fmulp	"fmulp"
#define fmull	"fmull"
#define fmuls	"fmuls"
#define fnstsw	"fnstsw"
#define frndint	"frndint"
#define fst	"fst"
#define fstcw	"fstcw"
#define fstsw	"fstsw"
#define fstl	"fstl"
#define fstp	"fstp"
#define fstpl	"fstpl"
#define fstps	"fstps"
#define fstpt	"fstpt"
#define fsts	"fsts"
#define fstt	"fstt"
#define fsub	"fsub"
#define fsubp	"fsubp"
#define fsubl	"fsubl"
#define fsubrl	"fsubrl"
#define fsubr	"fsubr"
#define fsubrp	"fsubrp"
#define fsubrs	"fsubrs"
#define fsubs	"fsubs"
#define ftst	"ftst"
#define fwait	"fwait"
#define idivb	"idivb"
#define idivl	"idivl"
#define idivw	"idivw"
#define imulb	"imulb"
#define imull	"imull"
#define imulw	"imulw"
#define incb	"incb"
#define incl	"incl"
#define incw	"incw"
#define into	"into"
#define ja	"ja"
#define jae	"jae"
#define jb	"jb"
#define jbe	"jbe"
#define je	"je"
#define jg	"jg"
#define jge	"jge"
#define jl	"jl"
#define jle	"jle"
#define jmp	"jmp"
#define jne	"jne"
#define jno	"jno"
#define jns	"jns"
#define jo	"jo"
#define jpe	"jpe"
#define jpo	"jpo"
#define js	"js"
#define leal	"leal"
#define leave	"leave"
#define movb	"movb"
#define movl	"movl"
#define movsbl	"movsbl"
#define movsbw	"movsbw"
#define movsb	"movsb"
#define movsl	"movsl"
#define movsw	"movsw"
#define movswl	"movswl"
#define movw	"movw"
#define movzbl	"movzbl"
#define movzbw	"movzbw"
#define movzwl	"movzwl"
#define mulb	"mulb"
#define mull	"mull"
#define mulw	"mulw"
#define negb	"negb"
#define negl	"negl"
#define negw	"negw"
#define nop	"nop"
#define notb	"notb"
#define notl	"notl"
#define notw	"notw"
#define orb	"orb"
#define orl	"orl"
#define orw	"orw"
#define popeax	"pop %eax"
#define popedx	"pop %edx"
#define popebx	"pop %ebx"
#define popecx	"pop %ecx"
#define popedi	"pop %edi"
#define popesi	"pop %esi"
#define popebp	"pop %ebp"
#define popl	"popl"
#define pusheax	"push %eax"
#define pushedx	"push %edx"
#define pushecx	"push %ecx"
#define pushesi	"push %esi"
#define pushedi	"push %edi"
#define pushl	"pushl"
#define rdtsc	"rdtsc"
#define rep	"rep"
#define ret	"ret"
#define rorb	"rorb"
#define rolb	"rolb"
#define rorw	"rorw"
#define rolw	"rolw"
#define rorl	"rorl"
#define roll	"roll"
#define sahf	"sahf"
#define salb	"salb"
#define sall	"sall"
#define salw	"salw"
#define sarb	"sarb"
#define sarl	"sarl"
#define sarw	"sarw"
#define sbbb	"sbbb"
#define sbbw	"sbbw"
#define sbbl	"sbbl"
#define seta	"seta"
#define setae	"setae"
#define setb	"setb"
#define setbe	"setbe"
#define sete	"sete"
#define setg	"setg"
#define setge	"setge"
#define setl	"setl"
#define setle	"setle"
#define setmp	"setmp"
#define setne	"setne"
#define shlb	"shlb"
#define shll	"shll"
#define shlw	"shlw"
#define shldl	"shldl"
#define shrb	"shrb"
#define shrl	"shrl"
#define shrw	"shrw"
#define shrdl	"shrdl"
#define stc	"stc"
#define subb	"subb"
#define subw	"subw"
#define subl	"subl"
#define subbl	"subbl"
#define testb	"testb"
#define testl	"testl"
#define testw	"testw"
#define xchg	"xchg"
#define xorb	"xorb"
#define xorl	"xorl"
#define xorw	"xorw"

#endif