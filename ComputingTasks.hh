/*
 * Copyright (C) 2003-2011 Dynare Team
 *
 * This file is part of Dynare.
 *
 * Dynare is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dynare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dynare.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _COMPUTINGTASKS_HH
#define _COMPUTINGTASKS_HH

#include <ostream>

#include "SymbolList.hh"
#include "SymbolTable.hh"
#include "Statement.hh"
#include "StaticModel.hh"
#include "DynamicModel.hh"

class SteadyStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  SteadyStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class CheckStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  CheckStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class SimulStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  SimulStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class ModelInfoStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  ModelInfoStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class StochSimulStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
public:
  StochSimulStatement(const SymbolList &symbol_list_arg,
                      const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class ForecastStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
public:
  ForecastStatement(const SymbolList &symbol_list_arg,
                    const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class RamseyPolicyStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
public:
  RamseyPolicyStatement(const SymbolList &symbol_list_arg,
                        const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class RplotStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
public:
  RplotStatement(const SymbolList &symbol_list_arg,
                 const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class UnitRootVarsStatement : public Statement
{
private:
  const SymbolList symbol_list;
public:
  UnitRootVarsStatement(const SymbolList &symbol_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class PeriodsStatement : public Statement
{
private:
  const int periods;
public:
  PeriodsStatement(int periods_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class DsampleStatement : public Statement
{
private:
  const int val1, val2;
public:
  DsampleStatement(int val1_arg);
  DsampleStatement(int val1_arg, int val2_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class EstimationStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
  const SymbolTable &symbol_table;
public:
  EstimationStatement(const SymbolList &symbol_list_arg,
                      const OptionsList &options_list_arg,
                      const SymbolTable &symbol_table);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class DynareSensitivityStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  DynareSensitivityStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class ObservationTrendsStatement : public Statement
{
public:
  typedef map<string, expr_t> trend_elements_t;
private:
  const trend_elements_t trend_elements;
  const SymbolTable &symbol_table;
public:
  ObservationTrendsStatement(const trend_elements_t &trend_elements_arg,
                             const SymbolTable &symbol_table_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class OsrParamsStatement : public Statement
{
private:
  const SymbolList symbol_list;
public:
  OsrParamsStatement(const SymbolList &symbol_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class OsrStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
public:
  OsrStatement(const SymbolList &symbol_list_arg,
               const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class DynaTypeStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const string filename;
public:
  DynaTypeStatement(const SymbolList &symbol_list_arg,
                    const string &filename_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class DynaSaveStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const string filename;
public:
  DynaSaveStatement(const SymbolList &symbol_list_arg,
                    const string &filename_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class ModelComparisonStatement : public Statement
{
public:
  typedef vector<pair<string, string> > filename_list_t;
private:
  filename_list_t filename_list;
  OptionsList options_list;
public:
  ModelComparisonStatement(const filename_list_t &filename_list_arg,
                           const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

//! Temporary structure used when parsing estimation_params* statements
class EstimationParams
{
public:
  int type;
  string name, name2, prior;
  expr_t init_val, low_bound, up_bound, mean, std, p3, p4, jscale;

  void
  init(const DataTree &datatree)
  {
    type = 0;
    name = "";
    name2 = "";
    prior = "NaN";
    init_val = datatree.NaN;
    low_bound = datatree.MinusInfinity;
    up_bound = datatree.Infinity;
    mean = datatree.NaN;
    std = datatree.NaN;
    p3 = datatree.NaN;
    p4 = datatree.NaN;
    jscale = datatree.NaN;
  }
};

class EstimatedParamsStatement : public Statement
{
private:
  const vector<EstimationParams> estim_params_list;
  const SymbolTable &symbol_table;
public:
  EstimatedParamsStatement(const vector<EstimationParams> &estim_params_list_arg,
                           const SymbolTable &symbol_table_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class EstimatedParamsInitStatement : public Statement
{
private:
  const vector<EstimationParams> estim_params_list;
  const SymbolTable &symbol_table;
public:
  EstimatedParamsInitStatement(const vector<EstimationParams> &estim_params_list_arg,
                               const SymbolTable &symbol_table_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class EstimatedParamsBoundsStatement : public Statement
{
private:
  const vector<EstimationParams> estim_params_list;
  const SymbolTable &symbol_table;
public:
  EstimatedParamsBoundsStatement(const vector<EstimationParams> &estim_params_list_arg,
                                 const SymbolTable &symbol_table_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class OptimWeightsStatement : public Statement
{
public:
  typedef map<string, expr_t> var_weights_t;
  typedef map<pair<string, string>, expr_t> covar_weights_t;
private:
  const var_weights_t var_weights;
  const covar_weights_t covar_weights;
  const SymbolTable &symbol_table;
public:
  OptimWeightsStatement(const var_weights_t &var_weights_arg,
                        const covar_weights_t &covar_weights_arg,
                        const SymbolTable &symbol_table_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

/*! \todo Make model_tree a member instead of a pointer */
class PlannerObjectiveStatement : public Statement
{
private:
  StaticModel *model_tree;
public:
  //! Constructor
  /*! \param model_tree_arg the model tree used to store the objective function.
    It is owned by the PlannerObjectiveStatement, and will be deleted by its destructor */
  PlannerObjectiveStatement(StaticModel *model_tree_arg);
  virtual ~PlannerObjectiveStatement();
  /*! \todo check there are only endogenous variables at the current period in the objective
    (no exogenous, no lead/lag) */
  virtual void checkPass(ModFileStructure &mod_file_struct);
  /*! \todo allow for the possibility of disabling temporary terms */
  virtual void computingPass();
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class BVARDensityStatement : public Statement
{
private:
  const int maxnlags;
  const OptionsList options_list;
public:
  BVARDensityStatement(int maxnlags_arg, const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class BVARForecastStatement : public Statement
{
private:
  const int nlags;
  const OptionsList options_list;
public:
  BVARForecastStatement(int nlags_arg, const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class SBVARStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  SBVARStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class MS_SBVARStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  MS_SBVARStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class IdentificationStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  IdentificationStatement(const OptionsList &options_list_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class WriteLatexDynamicModelStatement : public Statement
{
private:
  const DynamicModel &dynamic_model;
public:
  WriteLatexDynamicModelStatement(const DynamicModel &dynamic_model_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class WriteLatexStaticModelStatement : public Statement
{
private:
  const StaticModel &static_model;
public:
  WriteLatexStaticModelStatement(const StaticModel &static_model_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class ShockDecompositionStatement : public Statement
{
private:
  const SymbolList symbol_list;
  const OptionsList options_list;
public:
  ShockDecompositionStatement(const SymbolList &symbol_list_arg,
                              const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class ConditionalForecastStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  ConditionalForecastStatement(const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class PlotConditionalForecastStatement : public Statement
{
private:
  //! A value of -1 indicates that the user didn't specify a value
  const int periods;
  const SymbolList symbol_list;
public:
  PlotConditionalForecastStatement(int periods_arg, const SymbolList &symbol_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class SvarIdentificationStatement : public Statement
{
public:
  typedef map<pair<int, int>, vector<int> > svar_identification_exclusion_t;
private:
  const svar_identification_exclusion_t exclusion;
  const bool upper_cholesky_present;
  const bool lower_cholesky_present;
  const SymbolTable &symbol_table;
  int getMaxLag() const;
public:
  SvarIdentificationStatement(const svar_identification_exclusion_t &exclusion_arg,
                              const bool &upper_cholesky_present_arg,
                              const bool &lower_cholesky_present_arg,
                              const SymbolTable &symbol_table_arg);
  virtual void checkPass(ModFileStructure &mod_file_struct);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class MarkovSwitchingStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  MarkovSwitchingStatement(const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

class SvarStatement : public Statement
{
private:
  const OptionsList options_list;
public:
  SvarStatement(const OptionsList &options_list_arg);
  virtual void writeOutput(ostream &output, const string &basename) const;
};

#endif
