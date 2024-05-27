# cegex
This is basic c regex engine.

> [!WARNING]
> This is an educational project, **DO NOT USE** in prod!

**Table of Contents**:

- [Usage](#usage)
- [contributing](#contributing)
- [license](#license)


## Usage

```c
include "cegex.h"


int main(){
    char *regex = "o.w(l|d)+";
         *text  = "hello wldddll abcd!",
    //                 ^ match ^

    //{int index, int length}                v Flags
    CegexValue res = cegex_match(regex, text, NULL);

    // Get matched part from text
    char matched_word[res.length+1];
    memcpy(&matched_word, text + res.index, sizeof(char) * res.length);
    matched_word[res.length] = '\0'; // null terminitaion

    printf("%s\n", matched_word);
    return 0;
}
```

**Supported operations**:
|symbol| name        | description                           | usage    | example    |status|
|------|-------------|---------------------------------------|----------|------------|-----|
| `?`  | question    | zero or one repeat                    | `a?`     | `aa`, `a`  | [x] |
| `*`  | kleene star | zero or more repeat                   | `a*`     | `aaa`,` `  | [x] |
| `+`  | plus        | one or more repeat                    | `a+`     | `a`, `aa`  | [x] |
| `\|` | pipe        | one of right or left                  | `a\|b`   | `a`, `b`   | [x] |
| `.`  | dot         | any char except new line              | `a.`     | `aX`       | [x] |
| `\`  | escape      | for using regex char as a plain text  | `a\.`    | `a\.`      | [x] |
| `( )`| group       | group regex expressions               | `(a\|b)+`| `aab`      | [x] |
|  `^` | start       | regex start here                      | `^a`     | `abcd`     | [ ] |
| `[ ]`| any         | any of given chars                    | `[ab]`   | `a`        | [ ] |
| `{ }`| time        | multiply char given times             | `a{3}`   | `aaa`      | [ ] |
| `\d` | decimal     | decimal character                     | `a\d`    | `a1`       | [ ] |
| `\D` | not decimal | not a decimal character               | `a\D`    | `aX`       | [ ] |
| `\w` | word        | alphanumerical character              | `a\w`    | `a?`,`as`  | [ ] |
| `\W` | not word    | not alphanumerical character          | `a\W`    | `a1`       | [ ] |
| `\s` | space       | space or tab character                | `a\s`    | `a `, `a  `| [ ] |
| `\S` | not space   | not space character                   | `a\S`    | `a`        | [ ] |

**IMPORTANT NOTE**:
Currently, we dont backtrace the regex stack so, maybe some operator combinations
like `a.*b` can't correct work.

For example:
Normally `a.*b` matchs with `axxxb` but cegex doesn't parse like `a`, `.*`, `b`.
Cegex's stack loaded like this: `a`, `.`, `*`, `b`.


## Contributing
This project accepts the open source and free software mentality in its main terms.
Development and distribution are free within the framework of the rules specified
in the license section, BUT the course and mentality of the project depends entirely
on my discretion.

**Please respect this mentality and contributing rules**

## License
This project is under the [GNU GPLv3](./LICENSE) license.
