# Zen

Zen is a general purpose programming language designed to build simple, reliable and efficient programs.

## Example

```python
import zen.core.System.*

function factorial(n)
    return n <= 1 then 1 else n * factorial(n - 1)

function main(...arguments)
    var n = promptInteger('Enter an integer: ')
    var result = factorial(n)
    printf('%d! = %d', n, result)
```

The above example generates the following output:

```
Enter an integer: 5
5! = 120
```

## Contributing

We welcome all contributors.

Zen was created with a vision to help programmers, like you and I, write code
better. With your contributions, we can get there sooner. We appreciate your help!

To contribute, please contact the original author Samuel Rowe (<samuelrowe1999@gmail.com>).

## License

Copyright 2017-2019 Samuel Rowe

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
