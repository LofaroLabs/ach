EXTRA_DIST = ach-manual.xml achd.inc

# Rules to build
if HAVE_HELP2MAN
man/ach.1: $(top_srcdir)/src/achtool.c $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "manipulate ach channels" $(top_builddir)/ach$(EXEEXT) -o $@

man/achbench.1: $(top_srcdir)/src/ach-bench.c $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "benchmark and tune ach" $(top_builddir)/achbench$(EXEEXT) -o $@

man/achcat.1: $(top_srcdir)/src/achcat.c $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "ach debugging tool" $(top_builddir)/achcat$(EXEEXT) -o $@

man/achd.1: $(top_srcdir)/src/achd/achd.c $(top_srcdir)/configure.ac achd.inc
	$(HELP2MAN) -h -\? -v -V -i achd.inc --no-info -n "send ach messages over streams" $(top_builddir)/achd$(EXEEXT) -o $@

man/achlog.1: $(top_srcdir)/src/achlog.c
	$(HELP2MAN) -h -h -v -V --no-info -n "log ach messages" $(top_builddir)/achlog$(EXEEXT) -o $@

man/achcop.1: $(top_srcdir)/src/achcop.c
	$(HELP2MAN) -h -\? -v -V --no-info -n "Watchdog for ach daemons" $(top_builddir)/achcop$(EXEEXT) -o $@

if HAVE_MAN2HTML
.1.html:
	$(MAN2HTML) -r $< | sed -e '1d;2d;8d;s/@\|&amp;/{at}/g' > $@
endif
endif

if HAVE_DB2HTML
manual/ach-manual.html: ach-manual.xml
	db2html -u -o manual $<
endif

# test what to install
if HAVE_MANPAGES
dist_man_MANS = man/ach.1 man/achbench.1 man/achcat.1 man/achlog.1 man/achd.1 man/achcop.1
endif

if HAVE_MANHTML
MAN_HTML = man/ach.html man/achbench.html man/achcat.html man/achlog.html man/achd.html man/achcop.html
endif

if HAVE_MANUAL
DOCBOOK_HTML =  manual/ach-manual.html
endif

dist_doc_DATA = $(DOCBOOK_HTML) $(MAN_HTML)


maintainer-clean-local:
	-rm -rf *.1 *.html man/*.html man/*.1
