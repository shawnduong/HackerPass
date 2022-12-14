# Contribution Guide

HackerPass is currently in early development and only accepts contributions from approved contributors. In the future, HackerPass may accept outside contributions.

## Contents

- [Quick Start](#quick-start)
- [Commit Convention](#commit-convention)
- [Code Convention](#code-convention)
- [Outstanding Tasks](#outstanding-tasks)

## Quick Start

To get started, [fork](https://github.com/shawnduong/HackerPass/fork) and [clone](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository) the HackerPass repository. When forking, be sure to include all branches.

Make sure that all your commit messages follow the [commit convention](#commit-convention) and your code follows the [code convention](#code-convention).

You may work on your own branches or in the existing branches. When sending a PR, be sure to send a PR to the appropriate branch, never main.

Not sure what contributions to make? Check out the list of [outstanding tasks](#outstanding-tasks) that need to be done!

## Commit Convention

You should commit after every deliverable change you do. Avoid doing multiple changes and aggregating them all into one commit, as this makes it more difficult to track your progress and contributions.

You should never commit code that doesn't compile or work. If someone checks out the repository at a prior commit, they must be able to compile and run the project.

Commit messages should be short and succinct and grammatically correct. Commit messages should be in past tense and detail the changes made in the commit.

Commits should only contain files relevant to the commit. Avoid adding unnecessary or irrelevant files. Be sure to run `git diff` and `git status` often, and if possible, `git add` files manually instead of `git add .` (unless you are absolutely certain).

**Examples**

DO: "Added debugging mode CLI option."
- Past tense.
- Proper capitalization.
- Contains punctuation.

DON'T: "adds debugging mode cli option"
- Present tense.
- Start of sentence and "cli" should be capitalized.
- Missing punctuation.

DO: "Added options to auto-open with Firefox or Chromium."
- Details what the commit did.

DON'T: "did stuff"
- Doesn't detail what the commit did.

Failure to comply with the commit convention may result in your PR being rejected, though it is more likely that your PR will be accepted and you'll just be asked to follow the commit convention next time.

## Code Convention

Tabs should be used for indentation, and anything else after that should be spaces. This is in order to stay indentation-agnostic! Some people may prefer 4-column indentation and others may prefer 8-column or something else, and using tabs allows us to respect everyone's indentation width preference.

Variables, attributes, and parameters should be named in `lowerCamelCase`; functions and methods in `snake_case`; classes and objects in `PascalCase`; and constants in `SCREAMING_SNAKE_CASE`.

Allman braces should be used. This is where braces are on their own lines.

DO:

```c
int main()
{
	for (uint8_t i = 0; i < 16; i++)
	{
		puts("HELLO THERE!");
	}

	return 0;
}
```

DON'T:

```c
int main() {
	for (uint8_t i = 0; i < 16; i++) {
		puts("HELLO THERE!");
	}

	return 0;
}
```

When possible, use `uintx_t` and `intx_t` instead of `int` types.

`//` comments should be used for commenting out code or trailing annotations. Otherwise, `/* */` comments should be used.

DO:

```c
/* The code below is commented out! */
//	for (uint8_t i = 0; i < 16; i++) {}

x = 14;  // Trailing comments are fine in this style.

/* Two-line comments can be written like this. Lorem ipsum
   dolor sit amet. */

/* Multi-line really long comments can be written like
 * this. Lorem ipsum dolor sit amet, consectetur
 * adipiscing elit. Nunc nibh eros, porta nec rhoncus quis.
 */
```

DON'T:

```c
/*
	for (uint8_t i = 0; i < 16; i++) {}
*/

/*	for (uint8_t i = 0; i < 16; i++) {} */

x = 14;  /* This isn't super bad... just not preferable. */

// Please don't write single-line or multi-line comments
// like this.
```

If you don't follow the code convention, you may be asked to resubmit your PR with another commit where you reformat your code.

Many IDEs convert tabs to 4 spaces. Make sure your IDE is not doing this. Tabs are tabs!

All of these conventions are to have uniform and predictable patterns, as well as to be indentation-agnostic and IDE-agnostic.

## Outstanding Tasks

This task list may be incomplete and will be updated. Completed tasks will be removed from this list.

- `web`: Create front-end where users can check their points.
