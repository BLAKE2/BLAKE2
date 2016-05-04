// BLAKE2 reference source code package - C# implementation

// Written in 2012 by Christian Winnerlein  <codesinchaos@gmail.com>

// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.

// You should have received a copy of the CC0 Public Domain Dedication along with
// this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

using System;
using System.Text;

namespace Blake2Sharp
{
	public sealed class Blake2BConfig : ICloneable
	{
		public byte[] Personalization { get; set; }
		public byte[] Salt { get; set; }
		public byte[] Key { get; set; }
		public int OutputSizeInBytes { get; set; }
		public int OutputSizeInBits
		{
			get { return OutputSizeInBytes * 8; }
			set
			{
				if (value % 8 != 0)
					throw new ArgumentException("Output size must be a multiple of 8 bits");
				OutputSizeInBytes = value / 8;
			}
		}

		public Blake2BConfig()
		{
			OutputSizeInBytes = 64;
		}

		public Blake2BConfig Clone()
		{
			var result = new Blake2BConfig();
			result.OutputSizeInBytes = OutputSizeInBytes;
			if (Key != null)
				result.Key = (byte[])Key.Clone();
			if (Personalization != null)
				result.Personalization = (byte[])Personalization.Clone();
			if (Salt != null)
				result.Salt = (byte[])Salt.Clone();
			return result;
		}

		object ICloneable.Clone()
		{
			return Clone();
		}
	}
}
