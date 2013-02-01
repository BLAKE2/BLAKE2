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
	public static class Blake2B
	{
		public static Hasher Create()
		{
			return Create(new Blake2BConfig());
		}

		public static Hasher Create(Blake2BConfig config)
		{
			return new Blake2BHasher(config);
		}

		/*public static Hasher CreateParallel(int parallelism = 4)
		{
			return CreateParallel(null, parallelism);
		}

		public static Hasher CreateParallel(Blake2Config config, int parallelism = 4)
		{
			if (parallelism < 2)
				throw new ArgumentOutOfRangeException("parallelism", "parallism must be at least 2");
			throw new NotImplementedException();
		}

		public static Hasher CreateTreeHasher(Blake2BConfig config, Blake2TreeConfig treeConfig)
		{
		}

		public static NodeHasher CreateNodeHasher(Blake2BConfig config, Blake2TreeConfig treeConfig)
		{
		}*/

		public static byte[] ComputeHash(byte[] data, int start, int count)
		{
			return ComputeHash(data, start, count, null);
		}

		public static byte[] ComputeHash(byte[] data)
		{
			return ComputeHash(data, 0, data.Length, null);
		}

		public static byte[] ComputeHash(byte[] data, Blake2BConfig config)
		{
			return ComputeHash(data, 0, data.Length, config);
		}

		public static byte[] ComputeHash(byte[] data, int start, int count, Blake2BConfig config)
		{
			var hasher = Create(config);
			hasher.Update(data, start, count);
			return hasher.Finish();
		}
		//public static byte[] ComputeParallelHash(byte[] data);
		//public static byte[] ComputeParallelHash(byte[] data, Blake2Config config);
	}
}
