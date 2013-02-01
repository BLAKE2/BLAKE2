// BLAKE2 reference source code package - C# implementation

// Written in 2012 by Christian Winnerlein  <codesinchaos@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with
// this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

namespace Blake2Sharp
{
	public abstract class NodeHasher
	{
		public abstract void Init(int depth, long nodeOffset);
		public abstract byte[] Finish(bool isEndOfLayer);
		public abstract void Update(byte[] data, int start, int count);

		public void Update(byte[] data)
		{
			Update(data, 0, data.Length);
		}
	}
}
