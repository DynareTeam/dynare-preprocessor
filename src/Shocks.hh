/*
 * Copyright (C) 2003-2017 Dynare Team
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

#ifndef _SHOCKS_HH
#define _SHOCKS_HH

using namespace std;

#include <string>
#include <vector>
#include <map>

#include "Statement.hh"
#include "SymbolTable.hh"
#include "ExprNode.hh"

class AbstractShocksStatement : public Statement
{
public:
  struct DetShockElement
  {
    int period1;
    int period2;
    expr_t value;
  };
  //The boolean element indicates if the shock is a surprise (false) or a perfect foresight (true) shock.
  //This boolean is used only in case of conditional forecast with extended path method (simulation_type = deterministic).
  using det_shocks_t = map<int, vector<DetShockElement>>;
protected:
  //! Is this statement a "mshocks" statement ? (instead of a "shocks" statement)
  const bool mshocks;
  //! Does this "shocks" statement replace the previous ones?
  const bool overwrite;
  const det_shocks_t det_shocks;
  const SymbolTable &symbol_table;
  void writeDetShocks(ostream &output) const;
  void writeJsonDetShocks(ostream &output) const;

  AbstractShocksStatement(bool mshocks_arg, bool overwrite_arg,
                          det_shocks_t det_shocks_arg,
                          const SymbolTable &symbol_table_arg);
};

class ShocksStatement : public AbstractShocksStatement
{
public:
  using var_and_std_shocks_t = map<int, expr_t>;
  using covar_and_corr_shocks_t = map<pair<int, int>, expr_t>;
private:
  const var_and_std_shocks_t var_shocks, std_shocks;
  const covar_and_corr_shocks_t covar_shocks, corr_shocks;
  void writeVarOrStdShock(ostream &output, var_and_std_shocks_t::const_iterator &it, bool stddev) const;
  void writeVarAndStdShocks(ostream &output) const;
  void writeCovarOrCorrShock(ostream &output, covar_and_corr_shocks_t::const_iterator &it, bool corr) const;
  void writeCovarAndCorrShocks(ostream &output) const;
  bool has_calibrated_measurement_errors() const;
public:
  ShocksStatement(bool overwrite_arg,
                  const det_shocks_t &det_shocks_arg,
                  var_and_std_shocks_t var_shocks_arg,
                  var_and_std_shocks_t std_shocks_arg,
                  covar_and_corr_shocks_t covar_shocks_arg,
                  covar_and_corr_shocks_t corr_shocks_arg,
                  const SymbolTable &symbol_table_arg);
  void checkPass(ModFileStructure &mod_file_struct, WarningConsolidation &warnings) override;
  void writeOutput(ostream &output, const string &basename, bool minimal_workspace) const override;
  void writeJsonOutput(ostream &output) const override;
};

class MShocksStatement : public AbstractShocksStatement
{
public:
  MShocksStatement(bool overwrite_arg,
                   const det_shocks_t &det_shocks_arg,
                   const SymbolTable &symbol_table_arg);
  void writeOutput(ostream &output, const string &basename, bool minimal_workspace) const override;
};

class ConditionalForecastPathsStatement : public Statement
{
private:
  const AbstractShocksStatement::det_shocks_t paths;
  const SymbolTable &symbol_table;
  int path_length;
public:
  ConditionalForecastPathsStatement(AbstractShocksStatement::det_shocks_t paths_arg,
                                    const SymbolTable &symbol_table_arg);
  void checkPass(ModFileStructure &mod_file_struct, WarningConsolidation &warnings) override;
  void writeOutput(ostream &output, const string &basename, bool minimal_workspace) const override;
};

class MomentCalibration : public Statement
{
public:
  struct Constraint
  {
    int endo1, endo2;
    string lags;
    string lower_bound, upper_bound;
  };
  using constraints_t = vector<Constraint>;
private:
  constraints_t constraints;
  const SymbolTable &symbol_table;
public:
  MomentCalibration(constraints_t constraints_arg,
                    const SymbolTable &symbol_table_arg);
  void writeOutput(ostream &output, const string &basename, bool minimal_workspace) const override;
  void writeJsonOutput(ostream &output) const override;
};

class IrfCalibration : public Statement
{
public:
  struct Constraint
  {
    int endo;
    int exo;
    string periods, lower_bound, upper_bound;
  };
  using constraints_t = vector<Constraint>;
private:
  constraints_t constraints;
  const SymbolTable &symbol_table;
  const OptionsList options_list;
public:
  IrfCalibration(constraints_t constraints_arg,
                 const SymbolTable &symbol_table_arg,
                 OptionsList options_list_arg);
  void writeOutput(ostream &output, const string &basename, bool minimal_workspace) const override;
  void writeJsonOutput(ostream &output) const override;
};

class ShockGroupsStatement : public Statement
{
public:
  struct Group
  {
    string name;
    vector<string> list;
  };
  using group_t = vector<Group>;
private:
  group_t shock_groups;
  vector<string> group_names;
  string name;
public:
  ShockGroupsStatement(group_t shock_groups_arg, string name_arg);
  void writeOutput(ostream &output, const string &basename, bool minimal_workspace) const override;
};

#endif
