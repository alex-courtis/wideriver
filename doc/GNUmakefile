include GNUmakefile

all: README.md man/wideriver.1
	rm doc/help*.out doc/*pandoc.md

doc/help.out:
	./wideriver --help > $(@)

doc/help-defaults.out:
	./wideriver --help-defaults > $(@)

doc/readme.pandoc.md: doc/help.out doc/help-defaults.out
	cat doc/templ/*readme*md | sed \
		-e "/@HELP@/{r doc/help.out" \
		-e "d}" \
		-e "/@HELP_DEFAULTS@/{r doc/help-defaults.out" \
		-e "d}" \
		> $(@)

doc/man.pandoc.md: doc/help-defaults.out
	cat doc/templ/*man*md | sed \
		-e "s#@DATE@#`date +%Y/%m/%d`#g" \
		-e "/@HELP_DEFAULTS@/{r doc/help-defaults.out" \
		-e "d}" \
		> $(@)

README.md: doc/readme.pandoc.md
	pandoc -s --wrap=none --from=markdown --to=gfm --output $(@) doc/readme.pandoc.md
	markdown-toc --maxdepth=3 -i $(@)

man/wideriver.1: doc/man.pandoc.md
	pandoc -s --wrap=none --from=markdown --to=man --shift-heading-level-by -1 --output $(@) doc/man.pandoc.md

.PHONY: all README.md man/river-xmonadwh.1
