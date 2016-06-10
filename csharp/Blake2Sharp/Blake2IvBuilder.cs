// BLAKE2 reference source code package - C# implementation

// Written in 2012 by Christian Winnerlein  <codesinchaos@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with
// this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

using System;

namespace Blake2Sharp
{
	internal static class Blake2IvBuilder
	{
		private static readonly Blake2BTreeConfig SequentialTreeConfig = new Blake2BTreeConfig() { IntermediateHashSize = 0, LeafSize = 0, FanOut = 1, MaxHeight = 1 };

		public static ulong[] ConfigB(Blake2BConfig config, Blake2BTreeConfig treeConfig)
		{
			bool isSequential = treeConfig == null;
			if (isSequential)
				treeConfig = SequentialTreeConfig;
			var rawConfig = new ulong[8];
			var result = new ulong[8];

			//digest length
			if (config.OutputSizeInBytes <= 0 | config.OutputSizeInBytes > 64)
				throw new ArgumentOutOfRangeException("config.OutputSize");
			rawConfig[0] |= (ulong)(uint)config.OutputSizeInBytes;

			//Key length
			if (config.Key != null)
			{
				if (config.Key.Length > 64)
					throw new ArgumentException("config.Key", "Key too long");
				rawConfig[0] |= (ulong)((uint)config.Key.Length << 8);
			}
			// FanOut
			rawConfig[0] |= (uint)treeConfig.FanOut << 16;
			// Depth
			rawConfig[0] |= (uint)treeConfig.MaxHeight << 24;
			// Leaf length
			rawConfig[0] |= ((ulong)(uint)treeConfig.LeafSize) << 32;
			// Inner length
			if (!isSequential && (treeConfig.IntermediateHashSize <= 0 || treeConfig.IntermediateHashSize > 64))
				throw new ArgumentOutOfRangeException("treeConfig.TreeIntermediateHashSize");
			rawConfig[2] |= (uint)treeConfig.IntermediateHashSize << 8;
			// Salt
			if (config.Salt != null)
			{
				if (config.Salt.Length != 16)
					throw new ArgumentException("config.Salt has invalid length");
				rawConfig[4] = Blake2BCore.BytesToUInt64(config.Salt, 0);
				rawConfig[5] = Blake2BCore.BytesToUInt64(config.Salt, 8);
			}
			// Personalization
			if (config.Personalization != null)
			{
				if (config.Personalization.Length != 16)
					throw new ArgumentException("config.Personalization has invalid length");
				rawConfig[6] = Blake2BCore.BytesToUInt64(config.Personalization, 0);
				rawConfig[7] = Blake2BCore.BytesToUInt64(config.Personalization, 8);
			}

			return rawConfig;
		}

		public static void ConfigBSetNode(ulong[] rawConfig, byte depth, ulong nodeOffset)
		{
			rawConfig[1] = nodeOffset;
			rawConfig[2] = (rawConfig[2] & ~0xFFul) | depth;
		}
	}
}
