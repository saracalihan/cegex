# cegex
This is basic c regex engine.

**Supported operations**:
|symbol|name        |description                            |example| status |
|------|------------|---------------------------------------|-------|--------|
| `?`  | question   |zero or one repeat                     | `a?`  |  [x]   |
| `*`  | kleene star|zero or more repeat                    | `a*`  |  [x]   |
| `+`  | plus       |one or more repeat                     | `a+`  |  [x]   |
| `\|` | pipe       |one of right or left                   | `a\|b`|  [ ]   |
|  `.` | dot        |any char except new line               | `a.`  |  [x]   |
|  `\` | escape     |for using regex char as a plain text   | `a\.` |  [X]   |
|  `^` | start      |regex start here                       | `^a`  |  [ ]   |
|`( )` | group      |group regex expressions                | `(a\|b)+` | [ ]   |
|`[ ]` | any        | any of given chars                    | `[ab]` | [ ]   |
|`{ }` | time       | multiply char given times             | `a{3}` | [ ]   |

## License
This project is under the [GNU GPLv3](./LICENSE) license.
