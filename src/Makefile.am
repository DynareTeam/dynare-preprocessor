SUBDIRS = macro

BUILT_SOURCES = DynareBison.hh stack.hh position.hh location.hh DynareBison.cc DynareFlex.cc FlexLexer.h

bin_PROGRAMS = dynare_m

# We don't put BUILT_SOURCES in dynare_m_SOURCES, otherwise DynareBison.o and DynareFlex.o will be linked two times (Automake translates DynareFlex.ll and DynareBison.yy into their respective .o); so BUILT_SOURCES is in EXTRA_DIST
dynare_m_SOURCES = \
	DynareFlex.ll \
	DynareBison.yy \
	ComputingTasks.cc \
	ComputingTasks.hh \
	ModelTree.cc \
	ModelTree.hh \
	StaticModel.cc \
	StaticModel.hh \
	DynamicModel.cc \
	DynamicModel.hh \
	NumericalConstants.cc \
	NumericalConstants.hh \
	NumericalInitialization.cc \
	NumericalInitialization.hh \
	Shocks.cc \
	Shocks.hh \
	SigmaeInitialization.cc \
	SigmaeInitialization.hh \
	SymbolTable.cc \
	SymbolTable.hh \
	SymbolList.cc \
	SymbolList.hh \
	ParsingDriver.cc \
	ParsingDriver.hh \
	DataTree.cc \
	DataTree.hh \
	ModFile.cc \
	ModFile.hh \
	ConfigFile.cc \
	ConfigFile.hh \
	Statement.cc \
	Statement.hh \
	ExprNode.cc \
	ExprNode.hh \
	MinimumFeedbackSet.cc \
	MinimumFeedbackSet.hh \
	DynareMain.cc \
	DynareMain1.cc \
	DynareMain2.cc \
	CodeInterpreter.hh \
	ExternalFunctionsTable.cc \
	ExternalFunctionsTable.hh \
	ModelEquationBlock.hh \
	ModelEquationBlock.cc \
	WarningConsolidation.hh \
	WarningConsolidation.cc \
	ExtendedPreprocessorTypes.hh \
	SubModel.cc \
	SubModel.hh


ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	Doxyfile \
	$(BUILT_SOURCES)

# The -I. is for <FlexLexer.h>
dynare_m_CPPFLAGS = $(BOOST_CPPFLAGS) -I.
dynare_m_LDFLAGS = $(BOOST_LDFLAGS)
dynare_m_LDADD = macro/libmacro.a $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)

DynareFlex.cc FlexLexer.h: DynareFlex.ll
	$(LEX) -o DynareFlex.cc DynareFlex.ll
	cp $(LEXINC)/FlexLexer.h . || test -f ./FlexLexer.h

DynareBison.cc DynareBison.hh location.hh stack.hh position.hh: DynareBison.yy
	$(YACC) -W -o DynareBison.cc DynareBison.yy

all-local: $(PROGRAMS)

if HAVE_DOXYGEN
html-local:
	$(DOXYGEN)
endif

clean-local:
	rm -rf doc/html/
