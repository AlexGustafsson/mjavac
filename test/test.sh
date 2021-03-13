#!/usr/bin/env bash

DIRECTORY="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

mjavac="$1"
if [[ -z "$mjavac" ]]; then
  echo "Usage: $0 ./path/to/mjavac"
  exit 1
fi

function test_file() {
  file="$1"
  test_header="$(head -n 1 "$file" | sed 's!^// !!')"

  parse=ignore
  parse_description="no description"

  semantics=ignore
  semantics_description="no description"

  expected_output=""

  if [[ "$test_header" = "mjavac test file header" ]]; then
    lines=$(head -n 2 "$file" | tail -1 | sed 's!^// header: !!')
    header="$(head -n "$lines" "$file" | tail -n +3 | sed 's!^// !!g')"

    parse_value="$(echo "$header" | grep '^parse:' | sed 's/parse: //' | cut -d ':' -f2 | xargs)"
    parse="$(echo "$parse_value" | cut -d'-' -f1 | xargs)"
    parse_description="$(echo "$parse_value" | cut -d'-' -f2 | xargs)"

    semantics_value="$(echo "$header" | grep '^semantics:' | sed 's/semantics: //' | cut -d ':' -f2 | xargs)"
    semantics="$(echo "$semantics_value" | cut -d'-' -f1 | xargs)"
    semantics_description="$(echo "$semantics_value" | cut -d'-' -f2 | xargs)"

    expected_output_lines="$(echo "$header" | grep '^output:' | sed 's/output: //')"
    expected_output="$(echo "$header" | tail -n "0$expected_output_lines")"
  else
    parse=succeed
    echo "warning: missing test header in $1, assuming parse only"
  fi

  if [[ -n "$parse" ]] && [[ ! "$parse" = "ignore" ]]; then
    output="$($mjavac --parse-only "$file" 2>&1)"
    exit_code="$?"
    if [[ "$parse" = "succeed" ]] && [[ "$exit_code" -eq 0 ]]; then
      echo -e "✔ \e[32mcan parse $file\e[0m"
    elif [[ "$parse" = "fail" ]] && [[ "$exit_code" -gt 0 ]]; then
      echo -e "✔ \e[32mcannot parse $file\e[0m"
    else
      echo -e "✗ \e[31mexpected parsing to $parse ($parse_description): $file\e[0m"
      echo "$output"
      return
    fi
  fi

  if [[ -n "$semantics" ]] && [[ ! "$semantics" = "ignore" ]]; then
    output="$($mjavac --semantics-only "$file" 2>&1)"
    exit_code="$?"
    if [[ "$semantics" = "succeed" ]] && [[ "$exit_code" -eq 0 ]]; then
      echo -e "✔ \e[32mcan validate semantics of $file\e[0m"
    elif [[ "$semantics" = "fail" ]] && [[ "$exit_code" -gt 0 ]]; then
      echo -e "✔ \e[32mcannot validate semantics of $file\e[0m"
    else
      echo -e "✗ \e[31mexpected semantics validation to $semantics ($semantics_description): $file\e[0m"
      echo "$output"
      return
    fi
  fi

  # Skip execution - not implemented
  if [[ -n "$expected_output" ]] && false; then
    output="$($mjavac --execute "$file" 2>&1)"
    exit_code="$?"
    if [[ "$output" = "$expected_output" ]]; then
      echo -e "✔ \e[32mcan execute $file\e[0m"
    else
      echo -e "✗ \e[31mexpected execution to $semantics: $file\e[0m"
      echo "=== Expected ==="
      echo "$expected_output"
      echo "===   Got    ==="
      echo "$output"
      echo "================"
      return
    fi
  fi
}

function test_files() {
  files="$*"
  for file in $files; do
    test_file "$file"
    # if "$mjavac" "$file" &> /dev/null; then
    #   echo -e "✔ \e[32m$file\e[0m"
    # else
    #   echo -e "✗ \e[31m$file\e[0m"
    # fi
  done
}

test_files "$DIRECTORY/correct/"*.java
test_files "$DIRECTORY/examples/"*.java
test_files "$DIRECTORY/incorrect/"*.java
test_files "$DIRECTORY/grading/"*.java
test_files "$DIRECTORY/grading2/"*.java
