## Aulavik Code Style Guide
The recommended style of C sources in this project is heavily inspired by the
[Linux kernel coding style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html).
Contributors are encouraged to read and be familiar with this style, as well to
consider it the de-facto standard should any part of this specification be 
missing or unclear.

However, the two do differ, and as such, contributors are still expected to 
read and be familiar with the Aulavik style in particular. 

### This document is obviously highly work in progress.

### Switch statements

Switch cases should be aligned with the switch label itself. It is pointless, 
and a waste of space, to indent code within a switch twice - particularly with
eight-character indentation. 

Ordering switch cases alphabetically/numerically is strongly preferred whenever
possible. The default is strongly preferred to be last. Should a situation 
arise where an alternative ordering make more sense in the context, it 
should be used instead.

Switch cases that fall through to another branch shall be explicitly commented
as "fall through". This makes the fall-through immediately clear to the reader, 
and additionally makes it clear that it is a deliberate choice, and that the 
behaviour is intended.

```c
switch (sequence) {
case "a":
        dummyvar = 13;
        break;
case "n":
        dummyvar = 2011;
        /* fall through */
case "r":
        dummyvar = 621;
        break;
default:
        dummyvar = last_value;
}
```

### Including header files

#includes should be proceeded by a comment, documenting what is being used 
from the file. This helps improve code readability, as well as making it more
obvious when a header file is no longer being used and the #include is no 
longer needed.

If the usage of the #include is obvious and will always be needed (ie, 
`#include <foo.h>` in a file named `foo.c`, implementing `foo.h`), this is not 
necessary.

If the #include is used widely enough that this comment isn't practical, the
usage of the header should already be obvious in the context.

```c
#include <stdint.h> /* SIZE_MAX, uint16_t, uint8_t */
#include <kernel/tty.h> /* terminal_init() */
```

Includes should be broken up into three categories of descending priority: 
standard library headers, library headers, and project files. Includes should
be grouped together by their category, with these groups ordered by priority, 
and a blank line between them. 

Where it makes sense to do so, it is strongly preferred that includes are 
ordered alphabetically within their category.

If a file is solely an implementation of a header, that header should be the 
first #include.

```c
#include <math.h> /* atan(), asin(), INFINITY */
#include <stdint.h> /* uint16_t */

#include <dummylib/bar.h> 
#include <dummylib/foo.h> /* celestia() */
#include <samplelib/foo.h>

#include "example.h" /* magic() */
```