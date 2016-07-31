
**﻿BLAKE2 reference source code package - C# implementation**

```
	Written in 2012 by Samuel Neves <sneves@dei.uc.pt>
	Written in 2012 by Christian Winnerlein <codesinchaos@gmail.com>
	Written in 2016 by Uli Riehm <metadings@live.de>

	To the extent possible under law, the author(s) have dedicated all copyright
	and related and neighboring rights to this software to the public domain
	worldwide. This software is distributed without any warranty.

	You should have received a copy of the CC0 Public Domain Dedication along with
	this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
```

**Usage**

```
using Blake2;
using System;
using System.Text;

byte[] bytes = Encoding.UTF8.GetBytes(text);
byte[] value;

using (var hash = new Blake2B()) value = hash.ComputeHash(bytes);
```

**Examples**

```
metadings@metadings-mint ~/Blake2.cs/bin/Debug $ mono ./Blake2.exe HelloWorldOfDonaldDuckAndMickeyM
ff982bbcce1ff9d24ea1d9c06f0568cb4c612053c93876cac0ab91b3ce00a5e3b4708a146d41cb1e55e77a8863ce42298e22de0c34c85ee92dbcc9dab9092c6c

metadings@metadings-mint ~/Blake2.cs/bin/Debug $ mono ./Blake2.exe HHHHAAAALLLLOOOOWWWWEEEELLLLTTTT
bbc9e82dbf9a8897a5ec2f6836c381dbe27ac0b8ecd9912afa67459ef9474d70a52bf24ad5dcf29dbb8004d19a387b6516cc47ffae99d59d52efc013456c6b48
```

Ask questions on [stackoverflow](http://stackoverflow.com/questions/tagged/c%23+blake2) using tags `C#``Blake2` !

