using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;

namespace Blake2
{
	internal static class Program
	{
		static void Main(string[] args) 
		{
			int argsI; // string command;
			Dictionary<string, string> dictionary
				= ReadConsoleArguments(args, out argsI); //, out command);

			var concatArgs = string.Empty;
			var concatArgsSB = new StringBuilder();
			for (int j = 0; argsI < args.Length; ++argsI, ++j)
			{
				if (j > 0) concatArgsSB.Append(' ');
				concatArgsSB.Append(args[argsI]);
			}
			concatArgs = concatArgsSB.ToString();

			byte[] bytes = Encoding.UTF8.GetBytes(concatArgs);
			byte[] value;

			using (var hash = new Blake2B()) value = hash.ComputeHash(bytes);

			foreach (byte v in value) Console.Write("{0:x2}", v);
			Console.WriteLine();
		}

		static Dictionary<string, string> ReadConsoleArguments(string[] args, out int argsI) // , out string command)
		{
			argsI = 0;

			int nameI, dashs;
			string arg, argName;
			var dictionary = new Dictionary<string, string>(StringComparer.CurrentCultureIgnoreCase);
			do
			{
				if (args.Length == 0) break;

				arg = args[argsI];
				nameI = arg.IndexOf('=');
				dashs = arg.StartsWith("--") ? 2 : (arg.StartsWith("-") ? 1 : 0);

				if (dashs > 0)
				{
					if (nameI == -1)
					{
						argName = arg.Substring(dashs);
						dictionary.Add(argName, string.Empty);
						continue;
					}
					else
					{
						argName = arg.Substring(dashs, nameI - dashs);
						arg = arg.Substring(nameI + 1);
						dictionary.Add(argName, arg);
						continue;
					}
				}

				break;
			} while (++argsI < args.Length);

			/* command = string.Empty;
			if (argsI < args.Length)
			{
				command = args[++argsI];
			} /**/

			return dictionary;
		}
	}
}

