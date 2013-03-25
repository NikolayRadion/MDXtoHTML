MDXtoHTML
=========

## A simple translator from my extension MarkDown by some TeX abilities.

### Using
MDXtoHTML -i input_file -o output_file
By default output_file = out.html

## Description MDX
I make MDX becouse I need some kind of writting university lectures, but LaTeX tremendous and basic MarkDown is limited for written lectures.
### Supported
+ from `#` to `####` - headers.
+ `---` - part separator.
+ `Thm:` (without `Proof` yet) - Theorem statement and (in future) proof.
+ `Def:` - definition.
### ToDo
+ `+` - list
+ `*some text*` and `**some text**` - text style
+ `Ex:` - example
+ `Prop:` - properties
+ TeX chars like `\in`, `\subset` and `\phi`, of course
+ and so and forth