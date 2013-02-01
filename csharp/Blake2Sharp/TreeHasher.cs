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
	/*public class TreeHasher : Hasher
	{
		NodeHasher nodeHasher;
		int maxDepth;
		int maxLeafSize;
		int currentLeafSize;
		int fanOut;
		List<byte[]>[] intermediateHashes;
		long[] counts;

		public override void Init()
		{
			intermediateHashes = new List<byte[]>[maxDepth];
			counts = new long[maxDepth];
		}

		public override byte[] Finish()
		{
			for (int layer = 0; layer < intermediateHashes.Length; layer++)
			{
				if (intermediateHashes[layer].Count > 0)
				{
					nodeHasher.Init(layer, counts[layer]);
					foreach (var hash in intermediateHashes[layer])
						nodeHasher.Update(hash);
				}
			}
			intermediateHashes = null;
		}

		public override void Update(byte[] data, int start, int count)
		{
			while (count > 0)
			{
				int toHash = Math.Min(maxLeafSize - currentLeafSize, count);
				nodeHasher.Update(data, start, toHash);
				start += toHash;
				count -= toHash;
				if (count > 0)
				{
					intermediateHashes[0].Add(nodeHasher.Finish(false));
					for (int layer = 0; layer < intermediateHashes.Length; layer++)
					{
						if ((layer + 1 < maxDepth) && (intermediateHashes[layer].Count == fanOut))
						{
							nodeHasher.Init(layer, counts[layer]);
							foreach (var hash in intermediateHashes[layer])
								nodeHasher.Update(hash);
							intermediateHashes[layer + 1].Add(nodeHasher.Finish);
							intermediateHashes[layer].Clear();
							counts[layer + 1]++;
						}
					}
					counts[0]++;
					nodeHasher.Init(0, counts[0]);
				}
			}
		}
	}*/
}
