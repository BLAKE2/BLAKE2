// BLAKE2 reference source code package - C# implementation

// Written in 2012 by Christian Winnerlein  <codesinchaos@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with
// this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

using System;
using System.Collections.Generic;
using System.Text;

namespace Blake2Sharp
{
	/*public class Blake2BNodeHasher : NodeHasher
	{
		ulong[] rawConfig;
		byte[] key;
		Blake2BCore core = new Blake2BCore();

		public override void Init(int depth, long nodeOffset)
		{
			throw new NotImplementedException();
		}

		public override byte[] Finish(bool isEndOfLayer)
		{
			throw new NotImplementedException();
		}

		public override void Update(byte[] data, int start, int count)
		{
			throw new NotImplementedException();
		}

		public Blake2BNodeHasher(Blake2BConfig config, Blake2BTreeConfig treeConfig)
		{
			if (config == null)
				config = DefaultConfig;
			rawConfig = Blake2IvBuilder.ConfigB(config, null);
			if (config.Key != null && config.Key.Length != 0)
			{
				key = new byte[128];
				Array.Copy(config.Key, key, config.Key.Length);
			}
			Init(0, 0);
		}
	}*/
}
