/*
 * Copyright (C) 2003-2009 Dynare Team
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

#ifndef _VARIABLETABLE_HH
#define _VARIABLETABLE_HH

using namespace std;

#include <map>
#include <vector>

#include "SymbolTable.hh"

//! Used to keep track of variables in the sense of the models, i.e. pairs (symbol, lead/lag)
/*! Warning: some methods access variables through the pair (symbol_id, lag), but internally the class uses a lexicographic order over (lag, symbol_id) */
class VariableTable
{
private:
  //! A reference to the symbol table
  const SymbolTable &symbol_table;
  //! A variable is a pair (lag, symbol_id)
  typedef pair<int, int> var_key_type;

  typedef map<var_key_type, int> variable_table_type;
  //! Maps a pair (lag, symbol_id) to a variable ID
  variable_table_type variable_table;

  //! Maps a variable ID to a pair (lag, symbol_id)
  vector<var_key_type> inv_variable_table;

  //! Number of dynamic endogenous variables inside the model block
  int var_endo_nbr;
  //! Number of dynamic exogenous variables inside the model block
  int var_exo_nbr;
  //! Number of dynamic deterministic exogenous variables inside the model block
  int var_exo_det_nbr;

  //! Contains the columns indices for the dynamic jacobian (indexed by variable IDs)
  vector<int> dyn_jacobian_cols_table;
public:
  VariableTable(const SymbolTable &symbol_table_arg);
  //! Maximum lag over all types of variables (positive value)
  int max_lag;
  //! Maximum lead over all types of variables
  int max_lead;
  //! Maximum lag over endogenous variables (positive value)
  int max_endo_lag;
  //! Maximum lead over endogenous variables
  int max_endo_lead;
  //! Maximum lag over exogenous variables (positive value)
  int max_exo_lag;
  //! Maximum lead over exogenous variables
  int max_exo_lead;
  //! Maximum lag over deterministic exogenous variables (positive value)
  int max_exo_det_lag;
  //! Maximum lead over deterministic exogenous variables
  int max_exo_det_lead;
  //! Thrown when trying to access an unknown variable by (symb_id, lag)
  class UnknownVariableKeyException
  {
  public:
    int symb_id, lag;
    UnknownVariableKeyException(int symb_id_arg, int lag_arg) : symb_id(symb_id_arg), lag(lag_arg) {}
  };
  //! Thrown when trying to access an unknown variable by var_id
  class UnknownVariableIDException
  {
  public:
    //! Variable ID
    int id;
    UnknownVariableIDException(int id_arg) : id(id_arg) {}
  };
  //! Thrown when getDynJacobianCol() called before computeDynJacobianCols()
  class DynJacobianColsNotYetComputedException
  {
  };
  //! Thrown when computeDynJacobianCols() or addVariable() called after computeDynJacobianCols()
  class DynJacobianColsAlreadyComputedException
  {
  };
  //! Adds a variable in the table, and returns its (newly allocated) variable ID
  /*! Also works if the variable already exists */
  int addVariable(int symb_id, int lag) throw (DynJacobianColsAlreadyComputedException);
  //! Return variable ID
  inline int getID(int symb_id, int lag) const throw (UnknownVariableKeyException);
  //! Return lag of variable
  inline int getLag(int var_id) const throw (UnknownVariableIDException);
  //! Return symbol ID of variable
  inline int getSymbolID(int var_id) const throw (UnknownVariableIDException);
  //! Get variable type
  inline SymbolType getType(int var_id) const throw (UnknownVariableIDException);
  //! Get number of variables
  inline int size() const;
  //! Get column index in dynamic jacobian
  inline int getDynJacobianCol(int var_id) const throw (DynJacobianColsNotYetComputedException, UnknownVariableIDException);
  //! Computes column indices in dynamic jacobian
  void computeDynJacobianCols() throw (DynJacobianColsAlreadyComputedException);
  //! Get the number of columns of dynamic jacobian (depending on whether we compute derivatives w.r. to exogenous)
  inline int getDynJacobianColsNbr(bool computeJacobianExo) const;
};

inline int
VariableTable::getDynJacobianCol(int var_id) const throw (DynJacobianColsNotYetComputedException, UnknownVariableIDException)
{
  if (dyn_jacobian_cols_table.size() == 0)
    throw DynJacobianColsNotYetComputedException();

  if (var_id < 0 || var_id >= size())
    throw UnknownVariableIDException(var_id);

  return dyn_jacobian_cols_table[var_id];
}

inline int
VariableTable::getID(int symb_id, int lag) const throw (UnknownVariableKeyException)
{
  variable_table_type::const_iterator it = variable_table.find(make_pair(lag, symb_id));
  if (it == variable_table.end())
    throw UnknownVariableKeyException(symb_id, lag);
  else
    return it->second;
}

inline SymbolType
VariableTable::getType(int var_id) const throw (UnknownVariableIDException)
{
  if (var_id < 0 || var_id >= size())
    throw UnknownVariableIDException(var_id);

  return symbol_table.getType(inv_variable_table[var_id].second);
}

inline int
VariableTable::getSymbolID(int var_id) const throw (UnknownVariableIDException)
{
  if (var_id < 0 || var_id >= size())
    throw UnknownVariableIDException(var_id);

  return inv_variable_table[var_id].second;
}

inline int
VariableTable::getLag(int var_id) const throw (UnknownVariableIDException)
{
  if (var_id < 0 || var_id >= size())
    throw UnknownVariableIDException(var_id);

  return inv_variable_table[var_id].first;
}

inline int
VariableTable::size() const
{
  return variable_table.size();
}

inline int 
VariableTable::getDynJacobianColsNbr(bool computeJacobianExo) const
{
  if (computeJacobianExo)
    return var_endo_nbr + symbol_table.exo_nbr() + symbol_table.exo_det_nbr();
  else
    return var_endo_nbr;
}

#endif