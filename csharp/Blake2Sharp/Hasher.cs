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
	public abstract class Hasher
	{
		public abstract void Init();
		public abstract byte[] Finish();
		public abstract void Update(byte[] data, int start, int count);

		public void Update(byte[] data)
		{
			Update(data, 0, data.Length);
		}

		public HashAlgorithm AsHashAlgorithm()
		{
			return new HashAlgorithmAdapter(this);
		}

		internal class HashAlgorithmAdapter : HashAlgorithm
		{
			private readonly Hasher _hasher;

			protected override void HashCore(byte[] array, int ibStart, int cbSize)
			{
				_hasher.Update(array, ibStart, cbSize);
			}

			protected override byte[] HashFinal()
			{
				return _hasher.Finish();
			}

			public override void Initialize()
			{
				_hasher.Init();
			}

			public HashAlgorithmAdapter(Hasher hasher)
			{
				_hasher = hasher;
			}
		}
	}
}
