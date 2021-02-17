// mjavac test file header
// header: 3
// parse: fail - invalid syntax
class 1'test {
  -- Whilst this is a Lua comment which is not valid Java,
  -- it is troublesome to detect it during lexing without a parser
}
