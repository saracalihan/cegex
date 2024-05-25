# cegex
This is basic c regex matcher.

**Supported operations**:
|symbol|name        |description                            |example|
|------|------------|---------------------------------------|-------|
| `?`  | question   |zero or one repeat                     | `a?`  |
| `*`  | kleene star|zero or more repeat                    | `a*`  |
| `+`  | plus       |one or more repeat                     | `a+`  |
| `\|` | pipe       |one of right or left                   | `a\|b`|
|  `.` | dot        |any char except new line               | `a.`  |
|  `\` | escape     |for using regex char as a plain text   | `a\.` |

## License
This project is under the [GNU GPLv3](./LICENSE) license.
