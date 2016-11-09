
/// BLAKE2 reference source code package - C# implementation

/// Written in 2012 by Christian Winnerlein  <codesinchaos@gmail.com>

/// To the extent possible under law, the author(s) have dedicated all copyright
/// and related and neighboring rights to this software to the public domain
/// worldwide. This software is distributed without any warranty.

/// You should have received a copy of the CC0 Public Domain Dedication along with
/// this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Blake2Sharp.CompressionCodeGen
{
	class Program
	{
		private const int NumberOfRounds = 12;

		private static readonly int[] Sigma = new int[NumberOfRounds * 16] {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3,
			11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4,
			7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8,
			9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13,
			2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9,
			12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11,
			13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10,
			6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5,
			10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3
		};

		static void Round(int r)
		{
			Console.WriteLine("// ##### Round({0}) #####", r);
			G(r, 0, 0, 4, 8, 12);
			G(r, 1, 1, 5, 9, 13);
			G(r, 2, 2, 6, 10, 14);
			G(r, 3, 3, 7, 11, 15);
			G(r, 4, 0, 5, 10, 15);
			G(r, 5, 1, 6, 11, 12);
			G(r, 6, 2, 7, 8, 13);
			G(r, 7, 3, 4, 9, 14);
			Console.WriteLine();
		}

		static void G(int r, int i, int a, int b, int c, int d)
		{
			int p = (r << 4) + 2 * i;
			int p0 = Sigma[p];
			int p1 = Sigma[p + 1];

			string s = @"// G(r, i, a, b, c, d)
				a = a + b + m[" + p0 + @"];
				d ^= a;
				d = " + RotateRight("d", 32) + @";
				c = c + d;
				b ^= c;
				b = " + RotateRight("b", 24) + @";
				a = a + b + m[" + p1 + @"];
				d ^= a;
				d = " + RotateRight("d", 16) + @";
				c = c + d;
				b ^= c;
				b = " + RotateRight("b", 63) + @";";
			s = s.Replace("a", "v" + a);
			s = s.Replace("b", "v" + b);
			s = s.Replace("c", "v" + c);
			s = s.Replace("d", "v" + d);
			s = s.Replace("r", r.ToString());
			s = s.Replace("i", i.ToString());
			s = s.Replace("\t", "");
			Console.WriteLine(s);
			Console.WriteLine();
		}

		static string RotateRight(string name, int offset)
		{
			return "((" + name + " >>" + offset + ")|(" + name + " << (64-" + offset + ")))";
		}

		static void Main(string[] args)
		{
			for (int r = 0; r < NumberOfRounds; r++)
				Round(r);
		}
	}
}
