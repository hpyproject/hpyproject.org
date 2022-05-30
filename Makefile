SHELL := bash 
TARGET ?= origin
# hpy static page and blog makefile
# type `make help` to see all options 

all: build

.PHONY: clean build help


venv_nikola/bin/nikola:  ## create a virtualenv to build the website
	@virtualenv -ppython3 ./venv_nikola
	@venv_nikola/bin/python -mpip install nikola==8.2.2 jinja2 aiohttp watchdog ruamel.yaml feedparser 
	@venv_nikola/bin/nikola plugin -i sidebar
	@venv_nikola/bin/nikola plugin -i localsearch

build: venv_nikola/bin/nikola  ## build the website if needed, the result is in ./public
	venv_nikola/bin/nikola build
	@for f in $$(grep "include.*sidebar-en.inc" -lr public/) ;do \
	   sed -i -e'/<!-- include sidebar-en.inc -->/r public/sidebar-en.inc' $$f; \
	   sed -i -e'/<!-- include sidebar-en.inc -->/d' $$f; \
	   echo $${f}; \
	done;

auto: venv_nikola/bin/nikola ## build and serve the website, autoupdate on changes
	venv_nikola/bin/nikola auto -a 0.0.0.0

clean:  venv_nikola/bin/nikola  ## clean the website, usually not needed at all
	venv_nikola/bin/nikola clean

# Add help text after each target name starting with '\#\#'
help:   ## Show this help.
	@echo "\nHelp for building the website, based on nikola"
	@echo "Possible commands are:"
	@grep -h "##" $(MAKEFILE_LIST) | grep -v grep | sed -e 's/\(.*\):.*##\(.*\)/    \1: \2/'
 
