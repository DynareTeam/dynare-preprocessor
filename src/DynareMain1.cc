/*
 * Copyright (C) 2015-2017 Dynare Team
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

#include <sstream>
#include <fstream>

#include "macro/MacroDriver.hh"

bool compareNewline (int i, int j) {
  return i == '\n' && j == '\n';
}

void
main1(string &modfile, string &basename, string &modfiletxt, bool debug, bool save_macro, string &save_macro_file,
      bool no_line_macro, bool no_empty_line_macro, map<string, string> &defines, vector<string> &path, stringstream &macro_output)
{
  // Do macro processing
  MacroDriver m;

  m.parse(modfile, basename, modfiletxt, macro_output, debug, no_line_macro, defines, path);
  if (save_macro)
    {
      if (save_macro_file.empty())
        save_macro_file = basename + "-macroexp.mod";
      ofstream macro_output_file(save_macro_file);
      if (macro_output_file.fail())
        {
          cerr << "Cannot open " << save_macro_file << " for macro output" << endl;
          exit(EXIT_FAILURE);
        }

      string str (macro_output.str());
      if (no_empty_line_macro)
        str.erase(unique(str.begin(), str.end(), compareNewline), str.end());
      macro_output_file << str;
      macro_output_file.close();
    }
}
