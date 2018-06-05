/*
 * Copyright (C) 2008-2018 Dynare Team
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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "MacroDriver.hh"

MacroDriver::MacroDriver()
= default;

MacroDriver::~MacroDriver()
{
  for (auto value : values)
    delete value;
}

void
MacroDriver::parse(const string &f, const string &fb, const string &modfiletxt,
                   ostream &out, bool debug, bool no_line_macro_arg, map<string, string> defines,
                   vector<string> path)
{
  file = f;
  basename = fb;
  no_line_macro = no_line_macro_arg;

  /*
    Copy the file into a stringstream, and add an extra end-of-line. This is a
    workaround for trac ticket #73: with this workaround, MOD files ending with
    an @#endif or an @#endfor - but no newline - no longer trigger an error.
  */
  stringstream file_with_endl;
  for (auto & define : defines)
    try
      {
        boost::lexical_cast<int>(define.second);
        file_with_endl << "@#define " << define.first << " = " << define.second << endl;
      }
    catch (boost::bad_lexical_cast &)
      {
        if (!define.second.empty() && define.second.at(0) == '[' && define.second.at(define.second.length()-1) == ']')
          // If the input is an array. Issue #1578
          file_with_endl << "@#define " << define.first << " = " << define.second << endl;
        else
          file_with_endl << "@#define " << define.first << " = \"" << define.second << "\"" << endl;
      }
  file_with_endl << modfiletxt << endl;

  lexer = new MacroFlex(&file_with_endl, &out, no_line_macro, path);
  lexer->set_debug(debug);

  Macro::parser parser(*this, out);
  parser.set_debug_level(debug);

  // Output first @#line statement
  if (!no_line_macro)
    out << "@#line \"" << file << "\" 1" << endl;

  // Launch macro-processing
  parser.parse();

  delete lexer;
}

void
MacroDriver::error(const Macro::parser::location_type &l, const string &m) const
{
  cerr << "ERROR in macro-processor: " << l << ": " << m << endl;
  exit(EXIT_FAILURE);
}

void
MacroDriver::set_variable(const string &name, const MacroValue *value)
{
  env[name] = value;
}

const MacroValue *
MacroDriver::get_variable(const string &name) const noexcept(false)
{
  auto it = env.find(name);
  if (it == env.end())
    throw UnknownVariable(name);
  return it->second;
}

void
MacroDriver::init_loop(const string &name, const MacroValue *value) noexcept(false)
{
  const auto *mv1 = dynamic_cast<const ArrayMV<int> *>(value);
  const auto *mv2 = dynamic_cast<const ArrayMV<string> *>(value);
  if (!mv1 && !mv2)
    throw MacroValue::TypeError("Argument of @#for loop must be an array expression");
  loop_stack.emplace(name, make_pair(value, 0));
}

bool
MacroDriver::iter_loop()
{
  if (loop_stack.empty())
    throw "No loop on which to iterate!";

  int &i = loop_stack.top().second.second;
  const MacroValue *mv = loop_stack.top().second.first;
  string name = loop_stack.top().first;

  const auto *mv1 = dynamic_cast<const ArrayMV<int> *>(mv);
  if (mv1)
    {
      if (i >= (int) mv1->values.size())
        {
          loop_stack.pop();
          return false;
        }
      else
        {
          env[name] = new IntMV(*this, mv1->values[i++]);
          return true;
        }
    }
  else
    {
      const auto *mv2 = dynamic_cast<const ArrayMV<string> *>(mv);
      if (i >= (int) mv2->values.size())
        {
          loop_stack.pop();
          return false;
        }
      else
        {
          env[name] = new StringMV(*this, mv2->values[i++]);
          return true;
        }
    }
}

void
MacroDriver::begin_if(const MacroValue *value) noexcept(false)
{
  const auto *ival = dynamic_cast<const IntMV *>(value);
  if (!ival)
    throw MacroValue::TypeError("Argument of @#if must be an integer");
  last_if = (bool) ival->value;
}

void
MacroDriver::begin_ifdef(const string &name)
{
  try
    {
      get_variable(name);
      begin_if(new IntMV(*this, 1));
    }
  catch (UnknownVariable &)
    {
      begin_if(new IntMV(*this, 0));
    }
}

void
MacroDriver::begin_ifndef(const string &name)
{
  try
    {
      get_variable(name);
      begin_if(new IntMV(*this, 0));
    }
  catch (UnknownVariable &)
    {
      begin_if(new IntMV(*this, 1));
    }
}

void
MacroDriver::echo(const Macro::parser::location_type &l, const MacroValue *value) const noexcept(false)
{
  const auto *sval = dynamic_cast<const StringMV *>(value);
  if (!sval)
    throw MacroValue::TypeError("Argument of @#echo must be a string");

  cerr << "ECHO in macro-processor: " << l << ": " << sval->value << endl;
}

void
MacroDriver::error(const Macro::parser::location_type &l, const MacroValue *value) const noexcept(false)
{
  const auto *sval = dynamic_cast<const StringMV *>(value);
  if (!sval)
    throw MacroValue::TypeError("Argument of @#error must be a string");

  error(l, sval->value);
}

string
MacroDriver::printvars(const Macro::parser::location_type &l, const bool tostdout) const
{
  if (tostdout)
    {
      cout << "Macroprocessor: Printing macro variable values from " << file
           << " at line " << l.begin.line << endl;
      for (const auto & it : env)
        cout << "    " << it.first << " = " << it.second->print() << endl;
      cout << endl;
      return "";
    }

  stringstream intomfile;
  if (!no_line_macro)
    intomfile << "@#line \"" << file << "\" " << l.begin.line << endl;

  for (const auto & it : env)
    intomfile<< "options_.macrovars_line_" << l.begin.line << "." << it.first << " = " << it.second->print() << ";" << endl;
  return intomfile.str();
}