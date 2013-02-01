
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

namespace Blake2Sharp.Tests
{
	/*class DebugNodeHasher : NodeHasher
	{
		StringBuilder data = new StringBuilder();

		public override void Init(int depth, long nodeOffset)
		{
			data.AppendFormat("({0}-{1}", depth, nodeOffset);
		}

		public override byte[] Finish(bool isEndOfLayer)
		{
			data.Append(")");
			return Encoding.ASCII.GetBytes(data.ToString());
		}

		public override void Update(byte[] data, int start, int count)
		{
			data.Append(Encoding.ASCII.GetString(data, start, count));
		}
	}*/
}
