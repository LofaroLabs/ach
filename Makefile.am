ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	spin

AM_DISTCHECK_CONFIGURE_FLAGS=--enable-dkms --disable-dkms-build --disable-kbuild --with-python --with-java

# dist_bin_SCRIPTS = achpipe achlog

dist_noinst_SCRIPTS = achtooltest src/run-benchmarks test-achcop


AM_CPPFLAGS = -I$(top_srcdir)/include

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ach.pc


include_HEADERS = include/ach.h include/Ach.hpp
pkginclude_HEADERS = \
	include/ach/generic.h \
	include/ach/experimental.h \
	include/ach/klinux_generic.h

noinst_HEADERS = \
	include/achutil.h \
	include/achd.h \
	include/ach/private_posix.h \
	include/libach_private.h \
	include/achtest.h \
	include/libach/vtab.h


###############
## LIBRARIES ##
###############

lib_LTLIBRARIES = libach.la
libach_la_SOURCES = \
	src/libach.c \
	src/libach_posix.c \
	src/libach_klinux.c
# This is a libtool version -- CURRENT:REVISION:AGE
# Is /NOT/ major.minor.patch and the relationship is nontrivial
# Does not correspond to the package version
# The cmake versioning needs to be updated when this line changes
libach_la_LDFLAGS = -version-info 4:0:0 -export-symbols-regex '^ach_'


lib_LTLIBRARIES += libach-experimental.la
libach_experimental_la_SOURCES = src/event.c
libach_experimental_la_LDFLAGS = -version-info 0:0:0 -export-symbols-regex '^ach_'
libach_experimental_la_LIBADD = libach.la

noinst_LTLIBRARIES = libachutil.la
libachutil_la_SOURCES = src/achutil.c src/pipe.c src/dns.c

##############
## PROGRAMS ##
##############

bin_PROGRAMS = ach

ach_SOURCES = src/achtool.c
ach_LDADD = libach.la libachutil.la


bin_PROGRAMS += achcop
achcop_SOURCES = src/achcop.c
achcop_LDADD = libachutil.la

# achpipe_bin_SOURCES = src/achpipe-bin.c
# achpipe_bin_LDADD = libach.la libachutil.la
bin_PROGRAMS += achlog
achlog_SOURCES = src/achlog.c
achlog_LDADD = libach.la libachutil.la

bin_PROGRAMS += achcat
achcat_SOURCES = src/achcat.c
achcat_LDADD = libach.la libachutil.la

bin_PROGRAMS += achd
achd_SOURCES = src/achd/achd.c   \
               src/achd/client.c \
               src/achd/io.c \
               src/achd/transport.c
achd_LDADD = libach.la libachutil.la


bin_PROGRAMS += achbench
achbench_SOURCES = src/ach-bench.c
achbench_LDADD = libach.la libachutil.la

noinst_PROGRAMS = ach-example
ach_example_SOURCES = src/ach-example.c
ach_example_LDADD = libach.la -lm

noinst_PROGRAMS += ach-poll-example
ach_poll_example_SOURCES = src/ach-poll-example.c
ach_poll_example_LDADD = libach.la -lm


##########
## DATA ##
##########

if INSTALL_ZSH
dist_zshf_DATA = data/zsh/_ach
endif

###########
## Tests ##
###########

if ENABLE_TESTS


TESTS = achtooltest test-achcop

noinst_LTLIBRARIES += libachtest.la
libachtest_la_SOURCES = src/test/test.c

TESTS += achtest
noinst_PROGRAMS += achtest
achtest_SOURCES = src/test/achtest.c
achtest_LDADD = libach.la libachtest.la

TESTS += canceltest
noinst_PROGRAMS += canceltest
canceltest_SOURCES = src/test/canceltest.c
canceltest_LDADD = libach.la

TESTS += clocktest
noinst_PROGRAMS += clocktest
clocktest_SOURCES = src/test/clocktest.c
clocktest_LDADD = libach.la libachtest.la

# TESTS += transfertest
# noinst_PROGRAMS += transfertest
# transfertest_SOURCES = src/test/transfertest.c
# transfertest_LDADD = libach.la


noinst_PROGRAMS += klinuxtest
klinuxtest_SOURCES = src/test/klinuxtest.c
klinuxtest_LDADD = libach.la libachtest.la


noinst_PROGRAMS += mask
mask_SOURCES = src/test/mask.c
mask_LDADD = libach.la libachtest.la

noinst_PROGRAMS += holedigger
holedigger_SOURCES = src/test/holedigger.c
holedigger_LDADD = libach.la libachtest.la

noinst_PROGRAMS += achstress
achstress_SOURCES = src/test/achstress.c
achstress_LDADD = libach.la libachtest.la

robusttest_SOURCES = src/test/robusttest.c
noinst_PROGRAMS += robusttest
robusttest_LDADD = libach.la

endif


#########
## C++ ##
#########

bin_PROGRAMS += cpptest

cpptest_SOURCES = cpp/cpptest.cpp
cpptest_LDADD = libach.la

############
## PYTHON ##
############

if HAVE_PYTHON

python_PYTHON = python/ach.py
pyexec_LTLIBRARIES = ach_py.la
ach_py_la_SOURCES = python/ach_py.c
ach_py_la_LDFLAGS = -avoid-version -module -lach
ach_py_la_CPPFLAGS = $(PYTHON_CPPFLAGS)

endif

##########
## JAVA ##
##########

JSOURCES =                                \
	$(top_srcdir)/java/Lib.java       \
	$(top_srcdir)/java/Status.java    \
	$(top_srcdir)/java/Channel.java   \
	$(top_srcdir)/java/Test.java
EXTRA_DIST += $(JSOURCES)

if BUILD_JAVA

BUILT_SOURCES = org_golems_ach_Lib.h


JCLASSES =                                \
	org/golems/ach/Status.class       \
	org/golems/ach/Channel.class      \
	org/golems/ach/Test.class

org/golems/ach/Lib.class: $(JSOURCES)
	$(JAVAC) -d $(top_builddir) $(JSOURCES)

org/golems/ach/Status.class: org/golems/ach/Lib.class
org/golems/ach/Channel.class: org/golems/ach/Lib.class
org/golems/ach/Test.class: org/golems/ach/Lib.class

org_golems_ach_Lib.h: $(top_builddir)/org/golems/ach/Lib.class
	$(JAVAH) org.golems.ach.Lib
	touch $@

org.golems.ach.jar: $(JCLASSES)
	$(JAR) cf $@ $(JCLASSES)


# install the jar file somewhere reasonable
javadir = $(datadir)/java
java_DATA = org.golems.ach.jar

lib_LTLIBRARIES += libachj.la
libachj_la_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS) $(JNICPPFLAGS)
libachj_la_SOURCES = java/achj.c
libachj_la_LIBADD = libach.la

endif

if INSTALL_JAVADOC
EXTRA_DIST += doc/javadoc

# TODO: What's the best way to install javadoc?  The following line
# doesn't work:
#
# doc_DATA=doc/javadoc
#
# and install hooks that cp break distcheck

endif

if HAVE_JAVADOC
# Javadocs
doc/javadoc/index.html: $(JSOURCES)
	javadoc -d doc/javadoc $(JSOURCES)
	touch $@

all: doc/javadoc
endif

#########################
## Linux Kernel Module ##
#########################

# All of life's problems are solveable with recursion.

# Automake's uninstall-hook runs after the files are uninstalled.
# DKMS can't removed the ach-klinux module after automake deletes its
# installed sources.  Use a recursive make to force the correct
# ordering.
SUBDIRS = src/klinux/dkms-pre . src/klinux/dkms-post doc

## KBUILD ##
if BUILD_KBUILD
SUBDIRS += src/klinux
endif

## DKMS ##
if INSTALL_DKMS
dkmsdir=@prefix@/src/ach-klinux-@PACKAGE_VERSION@
dkmsincludedir=$(dkmsdir)
dkmspkgincludedir=$(dkmsincludedir)/ach

dkmsinclude_HEADERS = include/ach_klinux.h
dkmspkginclude_HEADERS = \
	 include/ach/generic.h \
	 include/ach/impl_generic.h \
	 include/ach/klinux_generic.h \
	 include/ach/private_generic.h \
	 include/ach/private_klinux.h
dkms_DATA = src/klinux/dkms/dkms.conf src/klinux/ach_klinux.c src/klinux/dkms/Kbuild

endif

###########
## LOCAL ##
###########

clean-local:
	-rm -rf debian/tmp debian/files debian/libach? debian/libach-dev debian/*.log debian/*.substvars texput.log debian/*.debhelper debian/usr/* debian/DEBIAN/* *.so *.so.* org org_golems_ach_Lib.h org.golems.ach.jar $(module_builddir)

distclean-local:
	-rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake .nfs*

maintainer-clean-local:
	-rm -rf  *.tar.gz doc/javadoc doc/.javadoc.stamp


include doxygen.am
