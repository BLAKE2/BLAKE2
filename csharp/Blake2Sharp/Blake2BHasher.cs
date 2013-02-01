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
	internal class Blake2BHasher : Hasher
	{
		private readonly Blake2BCore core = new Blake2BCore();
		private readonly ulong[] rawConfig;
		private readonly byte[] key;
		private readonly int outputSizeInBytes;
		private static readonly Blake2BConfig DefaultConfig = new Blake2BConfig();

		public override void Init()
		{
			core.Initialize(rawConfig);
			if (key != null)
			{
				core.HashCore(key, 0, key.Length);
			}
		}

		public override byte[] Finish()
		{
			var fullResult = core.HashFinal();
			if (outputSizeInBytes != fullResult.Length)
			{
				var result = new byte[outputSizeInBytes];
				Array.Copy(fullResult, result, result.Length);
				return result;
			}
			else return fullResult;
		}

		public Blake2BHasher(Blake2BConfig config)
		{
			if (config == null)
				config = DefaultConfig;
			rawConfig = Blake2IvBuilder.ConfigB(config, null);
			if (config.Key != null && config.Key.Length != 0)
			{
				key = new byte[128];
				Array.Copy(config.Key, key, config.Key.Length);
			}
			outputSizeInBytes = config.OutputSizeInBytes;
			Init();
		}

		public override void Update(byte[] data, int start, int count)
		{
			core.HashCore(data, start, count);
		}
	}
}
