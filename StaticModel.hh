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

#ifndef _STATIC_MODEL_HH
#define _STATIC_MODEL_HH

using namespace std;

#include <fstream>

#include "ModelTree.hh"

//! Stores a static model
class StaticModel : public ModelTree
{
private:
  typedef map<pair<int, int>, int> deriv_id_table_t;
  //! Maps a pair (symbol_id, lag) to a deriv ID
  deriv_id_table_t deriv_id_table;
  //! Maps a deriv ID to a pair (symbol_id, lag)
  vector<pair<int, int> > inv_deriv_id_table;

  //! Temporary terms for the file containing parameters dervicatives
  temporary_terms_type params_derivs_temporary_terms;

  //! Temporary terms for block decomposed models
  vector<vector<temporary_terms_type> > v_temporary_terms;

  vector<temporary_terms_inuse_type> v_temporary_terms_inuse;


  typedef map< pair< int, pair< int, int> >, NodeID> first_chain_rule_derivatives_type;
  first_chain_rule_derivatives_type first_chain_rule_derivatives;

  //! Writes static model file (standard Matlab version)
  void writeStaticMFile(const string &static_basename) const;

  //! Writes the static function calling the block to solve (Matlab version)
  void writeStaticBlockMFSFile(const string &basename) const;


  //! Writes static model file (C version)
  /*! \todo add third derivatives handling */
  void writeStaticCFile(const string &static_basename) const;

  //! Writes the Block reordred structure of the model in M output
  void writeModelEquationsOrdered_M(const string &dynamic_basename) const;

  //! Writes the code of the Block reordred structure of the model in virtual machine bytecode
  void writeModelEquationsCodeOrdered(const string file_name, const string bin_basename, map_idx_type map_idx) const;

  //! Computes jacobian and prepares for equation normalization
  /*! Using values from initval/endval blocks and parameter initializations:
    - computes the jacobian for the model w.r. to contemporaneous variables
    - removes edges of the incidence matrix when derivative w.r. to the corresponding variable is too close to zero (below the cutoff)
  */
  void evaluateJacobian(const eval_context_type &eval_context, jacob_map *j_m, bool dynamic);

  map_idx_type map_idx;

  void computeTemporaryTermsOrdered();
  //! Write derivative code of an equation w.r. to a variable
  void compileDerivative(ofstream &code_file, int eq, int symb_id, int lag, map_idx_type &map_idx) const;
  //! Write chain rule derivative code of an equation w.r. to a variable
  void compileChainRuleDerivative(ofstream &code_file, int eq, int var, int lag, map_idx_type &map_idx) const;

  //! Get the type corresponding to a derivation ID
  virtual SymbolType getTypeByDerivID(int deriv_id) const throw (UnknownDerivIDException);
  //! Get the lag corresponding to a derivation ID
  virtual int getLagByDerivID(int deriv_id) const throw (UnknownDerivIDException);
  //! Get the symbol ID corresponding to a derivation ID
  virtual int getSymbIDByDerivID(int deriv_id) const throw (UnknownDerivIDException);
  //! Compute the column indices of the static Jacobian
  void computeStatJacobianCols();
  //! return a map on the block jacobian
  map<pair<pair<int, pair<int, int> >, pair<int, int> >, int> get_Derivatives(int block);
  //! Computes chain rule derivatives of the Jacobian w.r. to endogenous variables
  void computeChainRuleJacobian(t_blocks_derivatives &blocks_derivatives);
  //! Collect only the first derivatives
  map<pair<int, pair<int, int> >, NodeID> collect_first_order_derivatives_endogenous();

  //! Helper for writing the Jacobian elements in MATLAB and C
  /*! Writes either (i+1,j+1) or [i+j*no_eq] */
  void jacobianHelper(ostream &output, int eq_nb, int col_nb, ExprNodeOutputType output_type) const;

  //! Helper for writing the sparse Hessian elements in MATLAB and C
  /*! Writes either (i+1,j+1) or [i+j*NNZDerivatives[1]] */
  void hessianHelper(ostream &output, int row_nb, int col_nb, ExprNodeOutputType output_type) const;

  //! Write chain rule derivative of a recursive equation w.r. to a variable
  void writeChainRuleDerivative(ostream &output, int eq, int var, int lag, ExprNodeOutputType output_type, const temporary_terms_type &temporary_terms) const;

  //! Collecte the derivatives w.r. to endogenous of the block, to endogenous of previouys blocks and to exogenous
  void collect_block_first_order_derivatives();

protected:
  //! Indicate if the temporary terms are computed for the overall model (true) or not (false). Default value true
  bool global_temporary_terms;

  //! vector of block reordered variables and equations
  vector<int> equation_reordered, variable_reordered, inv_equation_reordered, inv_variable_reordered;

  //! Vector describing equations: BlockSimulationType, if BlockSimulationType == EVALUATE_s then a NodeID on the new normalized equation
  t_equation_type_and_normalized_equation equation_type_and_normalized_equation;

  //! for each block contains pair< Simulation_Type, pair < Block_Size, Recursive_part_Size > >
  t_block_type_firstequation_size_mfs block_type_firstequation_size_mfs;

  //! for all blocks derivatives description
  t_blocks_derivatives blocks_derivatives;

  //! The jacobian without the elements below the cutoff
  dynamic_jacob_map dynamic_jacobian;

  //! Vector indicating if the block is linear in endogenous variable (true) or not (false)
  vector<bool> blocks_linear;

  //! Map the derivatives for a block pair<lag, make_pair(make_pair(eq, var)), NodeID>
  typedef map<pair< int, pair<int, int> >, NodeID> t_derivative;
  //! Vector of derivative for each blocks
  vector<t_derivative> derivative_endo, derivative_other_endo, derivative_exo, derivative_exo_det;

  //!List for each block and for each lag-leag all the other endogenous variables and exogenous variables
  typedef set<int> t_var;
  typedef map<int, t_var> t_lag_var;
  vector<t_lag_var> other_endo_block, exo_block, exo_det_block;

  //!Maximum lead and lag for each block on endogenous of the block, endogenous of the previous blocks, exogenous and deterministic exogenous
  vector<pair<int, int> > endo_max_leadlag_block, other_endo_max_leadlag_block, exo_max_leadlag_block, exo_det_max_leadlag_block, max_leadlag_block;

public:
  StaticModel(SymbolTable &symbol_table_arg, NumericalConstants &num_constants);

  //! Writes information on block decomposition when relevant
  void writeOutput(ostream &output, bool block) const;

  //! Absolute value under which a number is considered to be zero
  double cutoff;
  //! Compute the minimum feedback set in the static model:
  /*!   0 : all endogenous variables are considered as feedback variables
				1 : the variables belonging to a non linear equation are considered as feedback variables
        2 : the variables belonging to a non normalizable non linear equation are considered as feedback variables
        default value = 0 */
  int mfs;
  //! the file containing the model and the derivatives code
  ofstream code_file;
  //! Execute computations (variable sorting + derivation)
  /*!
    \param jacobianExo whether derivatives w.r. to exo and exo_det should be in the Jacobian (derivatives w.r. to endo are always computed)
    \param hessian whether 2nd derivatives w.r. to exo, exo_det and endo should be computed (implies jacobianExo = true)
    \param thirdDerivatives whether 3rd derivatives w.r. to endo/exo/exo_det should be computed (implies jacobianExo = true)
    \param paramsDerivatives whether 2nd derivatives w.r. to a pair (endo/exo/exo_det, parameter) should be computed (implies jacobianExo = true)
    \param eval_context evaluation context for normalization
    \param no_tmp_terms if true, no temporary terms will be computed in the static files
  */
  void computingPass(const eval_context_type &eval_context, bool no_tmp_terms, bool hessian, bool block);

  //! Adds informations for simulation in a binary file
  void Write_Inf_To_Bin_File(const string &static_basename, const string &bin_basename, const int &num,
                             int &u_count_int, bool &file_open) const;

  //! Writes static model file
  void writeStaticFile(const string &basename, bool block, bool bytecode) const;

  //! Writes LaTeX file with the equations of the static model
  void writeLatexFile(const string &basename) const;

  //! Writes initializations in oo_.steady_state for the auxiliary variables
  void writeAuxVarInitval(ostream &output) const;

  virtual int getDerivID(int symb_id, int lag) const throw (UnknownDerivIDException);

  //! Return the number of blocks
  virtual unsigned int getNbBlocks() const {return(block_type_firstequation_size_mfs.size());};
  //! Determine the simulation type of each block
  virtual BlockSimulationType getBlockSimulationType(int block_number) const {return(block_type_firstequation_size_mfs[block_number].first.first);};
  //! Return the first equation number of a block
  virtual unsigned int getBlockFirstEquation(int block_number) const {return(block_type_firstequation_size_mfs[block_number].first.second);};
  //! Return the size of the block block_number
  virtual unsigned int getBlockSize(int block_number) const {return(block_type_firstequation_size_mfs[block_number].second.first);};
  //! Return the number of feedback variable of the block block_number
  virtual unsigned int getBlockMfs(int block_number) const {return(block_type_firstequation_size_mfs[block_number].second.second);};
  //! Return the maximum lag in a block
  virtual unsigned int getBlockMaxLag(int block_number) const {return(block_lag_lead[block_number].first);};
  //! Return the maximum lead in a block
  virtual unsigned int getBlockMaxLead(int block_number) const {return(block_lag_lead[block_number].second);};
  //! Return the type of equation (equation_number) belonging to the block block_number
  virtual EquationType getBlockEquationType(int block_number, int equation_number) const {return( equation_type_and_normalized_equation[equation_reordered[block_type_firstequation_size_mfs[block_number].first.second+equation_number]].first);};
  //! Return true if the equation has been normalized
  virtual bool isBlockEquationRenormalized(int block_number, int equation_number) const {return( equation_type_and_normalized_equation[equation_reordered[block_type_firstequation_size_mfs[block_number].first.second+equation_number]].first == E_EVALUATE_S);};
  //! Return the NodeID of the equation equation_number belonging to the block block_number
  virtual NodeID getBlockEquationNodeID(int block_number, int equation_number) const {return( equations[equation_reordered[block_type_firstequation_size_mfs[block_number].first.second+equation_number]]);};
  //! Return the NodeID of the renormalized equation equation_number belonging to the block block_number
  virtual NodeID getBlockEquationRenormalizedNodeID(int block_number, int equation_number) const {return( equation_type_and_normalized_equation[equation_reordered[block_type_firstequation_size_mfs[block_number].first.second+equation_number]].second);};
  //! Return the original number of equation equation_number belonging to the block block_number
  virtual int getBlockEquationID(int block_number, int equation_number) const {return( equation_reordered[block_type_firstequation_size_mfs[block_number].first.second+equation_number]);};
  //! Return the original number of variable variable_number belonging to the block block_number
  virtual int getBlockVariableID(int block_number, int variable_number) const {return( variable_reordered[block_type_firstequation_size_mfs[block_number].first.second+variable_number]);};
  //! Return the position of equation_number in the block number belonging to the block block_number
  virtual int getBlockInitialEquationID(int block_number, int equation_number) const {return((int)inv_equation_reordered[equation_number] - (int)block_type_firstequation_size_mfs[block_number].first.second);};
  //! Return the position of variable_number in the block number belonging to the block block_number
  virtual int getBlockInitialVariableID(int block_number, int variable_number) const {return((int)inv_variable_reordered[variable_number] - (int)block_type_firstequation_size_mfs[block_number].first.second);};


};

#endif
