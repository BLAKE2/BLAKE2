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
#if false
	public sealed partial class Blake2BCore
	{
		partial void Compress(byte[] block, int start)
		{
			var h = _h;
			var m = _m;

			if (BitConverter.IsLittleEndian)
			{
				Buffer.BlockCopy(block, start, m, 0, BlockSizeInBytes);
			}
			else
			{
				for (int i = 0; i < 16; ++i)
					m[i] = BytesToUInt64(block, start + (i << 3));
			}

			var v0 = h[0];
			var v1 = h[1];
			var v2 = h[2];
			var v3 = h[3];
			var v4 = h[4];
			var v5 = h[5];
			var v6 = h[6];
			var v7 = h[7];

			var v8 = IV0;
			var v9 = IV1;
			var v10 = IV2;
			var v11 = IV3;
			var v12 = IV4 ^ _counter0;
			var v13 = IV5 ^ _counter1;
			var v14 = IV6 ^ _finaliziationFlag0;
			var v15 = IV7 ^ _finaliziationFlag1;

			for (int r = 0; r < NumberOfRounds; ++r)
			{
				// G(r,0,v0,v4,v8,v12)
				v0 = v0 + v4 + m[Sigma[16 * r + 2 * 0 + 0]];
				v12 ^= v0;
				v12 = ((v12 >> 32) | (v12 << (64 - 32)));
				v8 = v8 + v12;
				v4 ^= v8;
				v4 = ((v4 >> 24) | (v4 << (64 - 24)));
				v0 = v0 + v4 + m[Sigma[16 * r + 2 * 0 + 1]];
				v12 ^= v0;
				v12 = ((v12 >> 16) | (v12 << (64 - 16)));
				v8 = v8 + v12;
				v4 ^= v8;
				v4 = ((v4 >> 63) | (v4 << (64 - 63)));

				// G(r,1,v1,v5,v9,v13)
				v1 = v1 + v5 + m[Sigma[16 * r + 2 * 1 + 0]];
				v13 ^= v1;
				v13 = ((v13 >> 32) | (v13 << (64 - 32)));
				v9 = v9 + v13;
				v5 ^= v9;
				v5 = ((v5 >> 24) | (v5 << (64 - 24)));
				v1 = v1 + v5 + m[Sigma[16 * r + 2 * 1 + 1]];
				v13 ^= v1;
				v13 = ((v13 >> 16) | (v13 << (64 - 16)));
				v9 = v9 + v13;
				v5 ^= v9;
				v5 = ((v5 >> 63) | (v5 << (64 - 63)));

				// G(r,2,v2,v6,v10,v14)
				v2 = v2 + v6 + m[Sigma[16 * r + 2 * 2 + 0]];
				v14 ^= v2;
				v14 = ((v14 >> 32) | (v14 << (64 - 32)));
				v10 = v10 + v14;
				v6 ^= v10;
				v6 = ((v6 >> 24) | (v6 << (64 - 24)));
				v2 = v2 + v6 + m[Sigma[16 * r + 2 * 2 + 1]];
				v14 ^= v2;
				v14 = ((v14 >> 16) | (v14 << (64 - 16)));
				v10 = v10 + v14;
				v6 ^= v10;
				v6 = ((v6 >> 63) | (v6 << (64 - 63)));

				// G(r,3,v3,v7,v11,v15)
				v3 = v3 + v7 + m[Sigma[16 * r + 2 * 3 + 0]];
				v15 ^= v3;
				v15 = ((v15 >> 32) | (v15 << (64 - 32)));
				v11 = v11 + v15;
				v7 ^= v11;
				v7 = ((v7 >> 24) | (v7 << (64 - 24)));
				v3 = v3 + v7 + m[Sigma[16 * r + 2 * 3 + 1]];
				v15 ^= v3;
				v15 = ((v15 >> 16) | (v15 << (64 - 16)));
				v11 = v11 + v15;
				v7 ^= v11;
				v7 = ((v7 >> 63) | (v7 << (64 - 63)));

				// G(r,4,v0,v5,v10,v15)
				v0 = v0 + v5 + m[Sigma[16 * r + 2 * 4 + 0]];
				v15 ^= v0;
				v15 = ((v15 >> 32) | (v15 << (64 - 32)));
				v10 = v10 + v15;
				v5 ^= v10;
				v5 = ((v5 >> 24) | (v5 << (64 - 24)));
				v0 = v0 + v5 + m[Sigma[16 * r + 2 * 4 + 1]];
				v15 ^= v0;
				v15 = ((v15 >> 16) | (v15 << (64 - 16)));
				v10 = v10 + v15;
				v5 ^= v10;
				v5 = ((v5 >> 63) | (v5 << (64 - 63)));

				// G(r,5,v1,v6,v11,v12)
				v1 = v1 + v6 + m[Sigma[16 * r + 2 * 5 + 0]];
				v12 ^= v1;
				v12 = ((v12 >> 32) | (v12 << (64 - 32)));
				v11 = v11 + v12;
				v6 ^= v11;
				v6 = ((v6 >> 24) | (v6 << (64 - 24)));
				v1 = v1 + v6 + m[Sigma[16 * r + 2 * 5 + 1]];
				v12 ^= v1;
				v12 = ((v12 >> 16) | (v12 << (64 - 16)));
				v11 = v11 + v12;
				v6 ^= v11;
				v6 = ((v6 >> 63) | (v6 << (64 - 63)));

				// G(r,6,v2,v7,v8,v13)
				v2 = v2 + v7 + m[Sigma[16 * r + 2 * 6 + 0]];
				v13 ^= v2;
				v13 = ((v13 >> 32) | (v13 << (64 - 32)));
				v8 = v8 + v13;
				v7 ^= v8;
				v7 = ((v7 >> 24) | (v7 << (64 - 24)));
				v2 = v2 + v7 + m[Sigma[16 * r + 2 * 6 + 1]];
				v13 ^= v2;
				v13 = ((v13 >> 16) | (v13 << (64 - 16)));
				v8 = v8 + v13;
				v7 ^= v8;
				v7 = ((v7 >> 63) | (v7 << (64 - 63)));

				// G(r,7,v3,v4,v9,v14)
				v3 = v3 + v4 + m[Sigma[16 * r + 2 * 7 + 0]];
				v14 ^= v3;
				v14 = ((v14 >> 32) | (v14 << (64 - 32)));
				v9 = v9 + v14;
				v4 ^= v9;
				v4 = ((v4 >> 24) | (v4 << (64 - 24)));
				v3 = v3 + v4 + m[Sigma[16 * r + 2 * 7 + 1]];
				v14 ^= v3;
				v14 = ((v14 >> 16) | (v14 << (64 - 16)));
				v9 = v9 + v14;
				v4 ^= v9;
				v4 = ((v4 >> 63) | (v4 << (64 - 63)));
			}

			h[0] ^= v0 ^ v8;
			h[1] ^= v1 ^ v9;
			h[2] ^= v2 ^ v10;
			h[3] ^= v3 ^ v11;
			h[4] ^= v4 ^ v12;
			h[5] ^= v5 ^ v13;
			h[6] ^= v6 ^ v14;
			h[7] ^= v7 ^ v15;
		}
	}
#endif
}
