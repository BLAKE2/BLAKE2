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
		private ulong[] _v = new ulong[16];

		private static ulong RotateRight(ulong value, int nBits)
		{
			return (value >> nBits) | (value << (64 - nBits));
		}

		private void G(int a, int b, int c, int d, int r, int i)
		{
			int p = (r << 4) + i;
			int p0 = Sigma[p];
			int p1 = Sigma[p + 1];
			var v = _v;
			var m = _m;

			v[a] += v[b] + m[p0];
			v[d] = RotateRight(v[d] ^ v[a], 32);
			v[c] += v[d];
			v[b] = RotateRight(v[b] ^ v[c], 24);
			v[a] += v[b] + m[p1];
			v[d] = RotateRight(v[d] ^ v[a], 16);
			v[c] += v[d];
			v[b] = RotateRight(v[b] ^ v[c], 63);
		}

		partial void Compress(byte[] block, int start)
		{
			var v = _v;
			var h = _h;
			var m = _m;

			for (int i = 0; i < 16; ++i)
				m[i] = BytesToUInt64(block, start + (i << 3));

			v[0] = h[0];
			v[1] = h[1];
			v[2] = h[2];
			v[3] = h[3];
			v[4] = h[4];
			v[5] = h[5];
			v[6] = h[6];
			v[7] = h[7];

			v[8] = IV0;
			v[9] = IV1;
			v[10] = IV2;
			v[11] = IV3;
			v[12] = IV4 ^ _counter0;
			v[13] = IV5 ^ _counter1;
			v[14] = IV6 ^ _finaliziationFlag0;
			v[15] = IV7 ^ _finaliziationFlag1;

			for (int r = 0; r < NumberOfRounds; ++r)
			{
				G(0, 4, 8, 12, r, 0);
				G(1, 5, 9, 13, r, 2);
				G(2, 6, 10, 14, r, 4);
				G(3, 7, 11, 15, r, 6);
				G(3, 4, 9, 14, r, 14);
				G(2, 7, 8, 13, r, 12);
				G(0, 5, 10, 15, r, 8);
				G(1, 6, 11, 12, r, 10);
			}

			for (int i = 0; i < 8; ++i)
				h[i] ^= v[i] ^ v[i + 8];
		}
	}
#endif
}
