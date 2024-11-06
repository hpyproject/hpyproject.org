# hpyproject.org webpages

This is the source repository for the [hpyproject.org](https://www.hpyproject.org) webpages.

## Working with the repo to generate new content

The webpages are generated using the static site generator _nikola_ (https://getnikola.com/) and is driven with a Makefile.
To create a new post, first create a virtualenv with `make
venv_nikola/bin/nikola`, then `./venv_nikola/bin/nikola new_post`.

!! The content of `public` (with all helper directories, like js, css, ...)
is not written by hand,
**please do not modify** - it will be overwritten !!

After you make changes, you can do `make build` to regenerate the pages in
`public/` for local viewing, but **do not commit these**, they will be
rebuilt and commited via a CI deploy step. You can also do `make auto` to
start a server that will serve the pages, and rebuild them when any changes are
made to the sources.

PRs previews will be generated with Netlify. After pushing a PR, a CI run will
have a "Deploy preview ready!" run, clicking on the "details" link will show the
newly-rendered site preview.

### Comments

Comments to blog posts are generated via the [utterances](https://utteranc.es/)
javascript plugin. The comments appear as issues in the repo.
When viewing the site, a query is made to fetch the comments to the issue with
that name. To comment, users must authorize the utterances app to post on their
behalf using the [GitHub
OAuth](https://developer.github.com/v3/oauth/#web-application-flow) flow.
Alternatively, users can comment on the GitHub issue directly.

## Build requirements

Python 3.10 or 3.11 is required to build the web pages.

## Deployment

Any changes to the main branch (including merging PRs) regenerates and pushes
to the gh-pages branch, which is a copy of the `public` directory.
