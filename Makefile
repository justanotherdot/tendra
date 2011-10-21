# $Id$

# Note to package maintainers: this top-level Makefile is provided for the
# convenience of people checking out the entire repository, who just want to
# try it out. It is not intended to be packaged; rather, each project is
# intended to stand alone, and those ought to be packaged separately.
#
# The process for bootstrapping (building tendra with the system compiler) and
# rebuilding (using itself), which is performed by this makefile, is the same
# process that separate packages should undertake. The depencencies between
# packages have been arranged in order to make that as painless as possible.

OBJ_DIR?=     ${.CURDIR}/obj    # XXX: unused
OBJ_WWW?=     ${.CURDIR}/obj-www
OBJ_DOC?=     ${.CURDIR}/obj-doc
OBJ_BPREFIX?= ${.CURDIR}/obj-bootstrap
OBJ_REBUILD?= ${.CURDIR}/obj-rebuild
OBJ_REGEN?=   ${.CURDIR}/obj-regen
OBJ_TEST?=    ${OBJ_BPREFIX}/test
OBJ_BOOT?=    ${OBJ_BPREFIX}/obj

# TODO: this should probably be the actual $PREFIX
OBJ_RPREFIX?=	${OBJ_REBUILD}


all: bootstrap

doc:

clean:

# TODO: depend on rebuild, and install to $PREFIX
install:

install-doc:


bootstrap: ${BOOTSTRAP_DEPS}
	mkdir -p "${OBJ_BPREFIX}/bin"
.for project in installers-dra producers-dra tld tnc tpl tspec
	@echo "===> bootstrapping ${project} into ${OBJ_BPREFIX}"
	cd ${.CURDIR}/${project} && ${MAKE} -DNODOCS \
	    OBJ_DIR=${OBJ_BOOT}/${project}           \
	    PREFIX=${OBJ_BPREFIX} install
.endfor
	# TODO: these mkdirs are to be removed pending work on tcc
	mkdir -p "${OBJ_BPREFIX}/tmp"
	mkdir -p "${OBJ_BPREFIX}/lib/tcc/api"
	mkdir -p "${OBJ_BPREFIX}/lib/tcc/lpi"
	mkdir -p "${OBJ_BPREFIX}/lib/tcc/sys"
	@echo "===> bootstrapping tcc into ${OBJ_BPREFIX}"
	cd ${.CURDIR}/tcc && ${MAKE} -DNODOCS   \
	    OBJ_DIR=${OBJ_BOOT}/tcc             \
	    PREFIX_INCLUDE=                     \
	    PREFIX_MAN=                         \
	    PREFIX_TMP=${OBJ_BPREFIX}/tmp       \
	    PREFIX=${OBJ_BPREFIX} install
	@echo "===> bootstrapping osdep into ${OBJ_BPREFIX}"
	cd ${.CURDIR}/osdep && ${MAKE} -DNODOCS \
	    OBJ_DIR=${OBJ_BOOT}/osdep           \
	    PREFIX=${OBJ_BPREFIX}               \
	    TSPEC_PREFIX=${TSPEC_BPREFIX}       \
	    TCC=${OBJ_BPREFIX}/bin/tcc          \
	    TPL=${OBJ_BPREFIX}/bin/tpl          \
	    TNC=${OBJ_BPREFIX}/bin/tnc          \
	    TLD=${OBJ_BPREFIX}/bin/tld          \
	    install

bootstrap-test: ${OBJ_BPREFIX}/bin/tcc
.for project in osdep
	cd ${.CURDIR}/${project} && ${MAKE} -DNODOCS \
	    OBJ_DIR=${OBJ_TEST}/${project}           \
	    PREFIX=${OBJ_BPREFIX}                    \
	    TSPEC_PREFIX=${TSPEC_BPREFIX}            \
	    TCC=${OBJ_BPREFIX}/bin/tcc               \
	    TPL=${OBJ_BPREFIX}/bin/tpl               \
	    TNC=${OBJ_BPREFIX}/bin/tnc               \
	    TLD=${OBJ_BPREFIX}/bin/tld               \
	    test
.endfor


bootstrap-rebuild:
	@echo "===> rebuilding with bootstrap from ${OBJ_BOOT} into ${OBJ_REBUILD}"
.for project in tspec tcc tpl tnc producers-dra installers-dra
	cd ${.CURDIR}/${project} && ${MAKE} -DNODOCS \
	    TCC=${OBJ_BPREFIX}/bin/tcc               \
	    OBJ_DIR=${OBJ_REBUILD}/${project}        \
	    PREFIX=${OBJ_RPREFIX} install
.endfor

bootstrap-regen:
	@echo "===> bootstraping into ${OBJ_REGEN} for source regeneration"
.for project in sid calculus make_err
	@echo "===> bootstrapping ${project} into ${OBJ_BPREFIX}"
	cd ${.CURDIR}/${project} && ${MAKE} -DNODOCS \
	    OBJ_DIR=${OBJ_REGEN}/obj/${project}      \
	    PREFIX=${OBJ_REGEN} install
.endfor
.for project in calculus lexi make_err make_tdf producers-dra sid tpl tspec
	@echo "===> regenerating for ${project}"
	cd ${.CURDIR}/${project} && ${MAKE} -DNODOCS \
	    OBJ_DIR=${OBJ_REGEN}/obj/${project}      \
	    SID=${OBJ_REGEN}/bin/sid                 \
	    CALCULUS=${OBJ_REGEN}/bin/calculus       \
	    MAKE_ERR=${OBJ_REGEN}/bin/make_err       \
	    PREFIX=${OBJ_REGEN} regen-clean regen
.endfor
.for project in calculus lexi make_err make_tdf producers-dra sid tpl tspec
	@echo "===> rebuilding for ${project}"
	cd ${.CURDIR}/${project} && ${MAKE} -DNODOCS \
	    OBJ_DIR=${OBJ_REGEN}/obj/${project}      \
	    PREFIX=${OBJ_REGEN}
.endfor


test-doc:
	@echo "===> validating documents"
.for project in calculus disp installers-dra lexi make_err make_tdf \
	osdep producers-dra sid tcc tendra-doc tld tnc tpl tspec
	cd ${.CURDIR}/${project}/doc && ${MAKE} test
.endfor

# XXX: just temporary
# XXX: need the per-project prefix that -DWEBSITE uses
doc:
	@echo "===> building documents"
.for project in calculus disp installers-dra lexi make_err make_tdf \
	osdep producers-dra sid tcc tendra-doc tld tnc tpl tspec
	cd ${.CURDIR}/${project}/doc && ${MAKE} \
	    OBJ_DIR=${OBJ_DOC}/${project}
.endfor

