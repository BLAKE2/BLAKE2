/*	BLAKE2 reference source code package - C# implementation

	Written in 2012 by Samuel Neves <sneves@dei.uc.pt>
	Written in 2012 by Christian Winnerlein <codesinchaos@gmail.com>
	Written in 2016 by Uli Riehm <metadings@live.de>

	To the extent possible under law, the author(s) have dedicated all copyright
	and related and neighboring rights to this software to the public domain
	worldwide. This software is distributed without any warranty.

	You should have received a copy of the CC0 Public Domain Dedication along with
	this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

namespace Blake2
{
	public partial class Blake2B : HashAlgorithm
	{
		private ulong[] rawConfig;

		private byte[] _Personalization;

		public byte[] Personalization 
		{ 
			get { return _Personalization; }
			set { 
				_Personalization = value; 
				_isInitialized = false;
			}
		}

		private byte[] _Salt;

		public byte[] Salt 
		{ 
			get { return _Salt; }
			set { 
				_Salt = value; 
				_isInitialized = false;
			}
		}

		private byte[] _Key;

		public byte[] Key
		{ 
			get { return _Key; }
			set { 
				_Key = value; 
				_isInitialized = false;
			}
		}

		private Blake2BTreeConfig _TreeConfig;

		public Blake2BTreeConfig TreeConfig
		{ 
			get { return _TreeConfig; }
			set { 
				_TreeConfig = value; 
				_isInitialized = false;
			}
		}

		private int _outputSizeInBytes;

		public int OutputSizeInBytes 
		{ 
			get { return _outputSizeInBytes; }
			protected set 
			{ 
				if (value <= 0 || value > 64)
					throw new IndexOutOfRangeException("OutputSizeInBytes");
				if (value % 8 != 0)
					throw new IndexOutOfRangeException("OutputSizeInBytes must be a multiple of 8");

				_outputSizeInBytes = value;
			}
		}

		public override int HashSize { get { return OutputSizeInBytes * 8; } }

		public Blake2B() : this(64) { }

		public Blake2B(int outputSizeInBytes)
		{			
			OutputSizeInBytes = outputSizeInBytes;
		}

		private bool _isInitialized = false;

		private int _bufferFilled;
		private byte[] _buf = new byte[128];
		private ulong[] _h = new ulong[8];
		private ulong[] _m = new ulong[16];
		private ulong _counter0;
		private ulong _counter1;
		private ulong _finalizationFlag0;
		private ulong _finalizationFlag1;

		private const int NumberOfRounds = 12;
		private const int BlockSizeInBytes = 128;

		const ulong IV0 = 0x6A09E667F3BCC908UL;
		const ulong IV1 = 0xBB67AE8584CAA73BUL;
		const ulong IV2 = 0x3C6EF372FE94F82BUL;
		const ulong IV3 = 0xA54FF53A5F1D36F1UL;
		const ulong IV4 = 0x510E527FADE682D1UL;
		const ulong IV5 = 0x9B05688C2B3E6C1FUL;
		const ulong IV6 = 0x1F83D9ABFB41BD6BUL;
		const ulong IV7 = 0x5BE0CD19137E2179UL;

		private static readonly int[] Sigma = new int[NumberOfRounds * 16] {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3,
			11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4,
			7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8,
			9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13,
			2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9,
			12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11,
			13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10,
			6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5,
			10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3
		};

		public static ulong BytesToUInt64(byte[] buf, int offset)
		{
			return
				((ulong)buf[offset + 7] << 7 * 8 |
				((ulong)buf[offset + 6] << 6 * 8) |
				((ulong)buf[offset + 5] << 5 * 8) |
				((ulong)buf[offset + 4] << 4 * 8) |
				((ulong)buf[offset + 3] << 3 * 8) |
				((ulong)buf[offset + 2] << 2 * 8) |
				((ulong)buf[offset + 1] << 1 * 8) |
				((ulong)buf[offset]));
		}

		public static void UInt64ToBytes(ulong value, byte[] buf, int offset)
		{
			buf[offset + 7] = (byte)(value >> 7 * 8);
			buf[offset + 6] = (byte)(value >> 6 * 8);
			buf[offset + 5] = (byte)(value >> 5 * 8);
			buf[offset + 4] = (byte)(value >> 4 * 8);
			buf[offset + 3] = (byte)(value >> 3 * 8);
			buf[offset + 2] = (byte)(value >> 2 * 8);
			buf[offset + 1] = (byte)(value >> 1 * 8);
			buf[offset] = (byte)value;
		}

		public override void Initialize()
		{
			if (rawConfig != null)
				Array.Clear(rawConfig, 0, rawConfig.Length);
			else
				rawConfig = new ulong[8];

			// digest length
			rawConfig[0] |= (ulong)(uint)OutputSizeInBytes;

			// Key length
			if (Key != null)
			{
				if (Key.Length > 64)
					throw new ArgumentException("Key", "Key too long");
				rawConfig[0] |= (ulong)((uint)Key.Length << 8);
			}

			bool isSequential = TreeConfig == null;
			if (isSequential)
			{
				// FanOut
				rawConfig[0] |= (uint)1 << 16;
				// Depth
				rawConfig[0] |= (uint)1 << 24;
				// Leaf length
				rawConfig[0] |= (uint)0 << 32;
				// Inner length
				rawConfig[2] |= (uint)0 << 8;
			}
			else
			{
				if (TreeConfig.IntermediateHashSize <= 0 || TreeConfig.IntermediateHashSize > 64)
					throw new ArgumentOutOfRangeException("TreeConfig.IntermediateHashSize");

				// FanOut
				rawConfig[0] |= (uint)TreeConfig.FanOut << 16;
				// Depth
				rawConfig[0] |= (uint)TreeConfig.MaxHeight << 24;
				// Leaf length
				rawConfig[0] |= ((ulong)(uint)TreeConfig.LeafSize) << 32;
				// Inner length
				rawConfig[2] |= (uint)TreeConfig.IntermediateHashSize << 8;
			}
			// Salt
			if (Salt != null)
			{
				if (Salt.Length != 16)
					throw new ArgumentException("Salt has invalid length");

				rawConfig[4] = Blake2B.BytesToUInt64(Salt, 0);
				rawConfig[5] = Blake2B.BytesToUInt64(Salt, 8);
			}
			// Personalization
			if (Personalization != null)
			{
				if (Personalization.Length != 16)
					throw new ArgumentException("Personalization has invalid length");

				rawConfig[6] = Blake2B.BytesToUInt64(Personalization, 0);
				rawConfig[7] = Blake2B.BytesToUInt64(Personalization, 8);
			}

			Initialize(rawConfig);
		}

		/* public static void ConfigBSetNode(ulong[] rawConfig, byte depth, ulong nodeOffset)
		{
			rawConfig[1] = nodeOffset;
			rawConfig[2] = (rawConfig[2] & ~0xFFul) | depth;
		} */

		public virtual void Initialize(ulong[] config)
		{
			if (config == null)
				throw new ArgumentNullException("config");
			if (config.Length != 8)
				throw new ArgumentException("config length must be 8 words");
			
			_isInitialized = true;

			_h[0] = IV0;
			_h[1] = IV1;
			_h[2] = IV2;
			_h[3] = IV3;
			_h[4] = IV4;
			_h[5] = IV5;
			_h[6] = IV6;
			_h[7] = IV7;

			_counter0 = 0;
			_counter1 = 0;
			_finalizationFlag0 = 0;
			_finalizationFlag1 = 0;

			_bufferFilled = 0;

			Array.Clear(_buf, 0, _buf.Length);

			for (int i = 0; i < 8; i++)
				_h[i] ^= config[i];
			
			if (Key != null)
			{
				HashCore(Key, 0, Key.Length);
			}
		}

		partial void Compress(byte[] block, int start);

		protected override void HashCore(byte[] array, int start, int count)
		{
			if (array == null)
				throw new ArgumentNullException("array");
			if (start < 0)
				throw new ArgumentOutOfRangeException("start");
			if (count < 0)
				throw new ArgumentOutOfRangeException("count");
			if ((long)start + (long)count > array.Length)
				throw new ArgumentOutOfRangeException("start+count");

			if (!_isInitialized) Initialize();

			int offset = start;
			int bufferRemaining = BlockSizeInBytes - _bufferFilled;

			if ((_bufferFilled > 0) && (count > bufferRemaining))
			{
				Array.Copy(array, offset, _buf, _bufferFilled, bufferRemaining);
				_counter0 += BlockSizeInBytes;
				if (_counter0 == 0)
					_counter1++;
				Compress(_buf, 0);
				offset += bufferRemaining;
				count -= bufferRemaining;
				_bufferFilled = 0;
			}

			while (count > BlockSizeInBytes)
			{
				_counter0 += BlockSizeInBytes;
				if (_counter0 == 0)
					_counter1++;
				Compress(array, offset);
				offset += BlockSizeInBytes;
				count -= BlockSizeInBytes;
			}

			if (count > 0)
			{
				Array.Copy(array, offset, _buf, _bufferFilled, count);
				_bufferFilled += count;
			}
		}

		protected override byte[] HashFinal ()
		{
			return HashFinal(false);
		}

		protected virtual byte[] HashFinal(bool isEndOfLayer)
		{
			if (!_isInitialized) Initialize();

			_isInitialized = false;

			//Last compression
			_counter0 += (uint)_bufferFilled;
			_finalizationFlag0 = ulong.MaxValue;
			if (isEndOfLayer)
				_finalizationFlag1 = ulong.MaxValue;
			for (int i = _bufferFilled; i < _buf.Length; i++)
				_buf[i] = 0;
			Compress(_buf, 0);

			//Output
			byte[] hash = new byte[64];
			for (int i = 0; i < 8; ++i)
				UInt64ToBytes(_h[i], hash, i << 3);
			return hash;
		}

		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				_isInitialized = false;
				
				if (rawConfig != null)
				{
					Array.Clear(rawConfig, 0, rawConfig.Length);
					rawConfig = null;
				}
				if (_buf != null)
				{
					Array.Clear(_buf, 0, _buf.Length);
					_buf = null;
				}
				if (_h != null)
				{
					Array.Clear(_h, 0, _h.Length);
					_h = null;
				}
				if (Personalization != null)
				{
					Array.Clear(Personalization, 0, Personalization.Length);
					Personalization = null;
				}
				if (Salt != null)
				{
					Array.Clear(Salt, 0, Salt.Length);
					Salt = null;
				}
				if (Key != null)
				{
					Array.Clear(Key, 0, Key.Length);
					Key = null;
				}
			}
			base.Dispose(disposing);
		}
	}
}
