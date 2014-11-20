## Markdown extension (e.g. md, markdown, mdown).
MEXT = md

## All markdown files in the working directory
SRC = $(wildcard *.$(MEXT))

## Location of Pandoc support files.
PANDOC_PREFIX = $(PWD)/.pandoc

## Location of your working bibliography file
BIB = $(PWD)/bibs/fpbib-pandoc.bib

## CSL stylesheet (located in the csl folder of the PREFIX directory).
CSL = apsa


PDFS=$(SRC:.md=.pdf)
HTML=$(SRC:.md=.html)
TEX=$(SRC:.md=.tex)


#all:	$(PDFS) $(HTML) $(TEX)
all:	abstract.pdf

pdf:	clean $(PDFS)
html:	clean $(HTML)
tex:	clean $(TEX)

%.html:	%.md
	pandoc -r markdown+simple_tables+table_captions+yaml_metadata_block -w html -S --template=$(PANDOC_PREFIX)/templates/html.template --css=$(PANDOC_PREFIX)/marked/kultiad-serif.css --filter pandoc-citeproc --csl=$(PANDOC_PREFIX)/csl/$(CSL).csl --bibliography=$(BIB) -o $@ $<

%.tex:	%.md
	pandoc -r markdown+simple_tables+table_captions+yaml_metadata_block -w latex -s -S --latex-engine=pdflatex --template=$(PANDOC_PREFIX)/templates/latex.template --filter pandoc-citeproc --csl=$(PANDOC_PREFIX)/csl/ajps.csl --bibliography=$(BIB) -o $@ $<


%.pdf:	%.md
	pandoc -r markdown+simple_tables+table_captions+yaml_metadata_block -s -S --latex-engine=pdflatex --template=$(PANDOC_PREFIX)/templates/latex.template --filter pandoc-citeproc --csl=$(PANDOC_PREFIX)/csl/$(CSL).csl --bibliography=$(BIB) -o $@ $<

clean:
	rm -f *.html abstract.pdf *.tex *.aux *.out *.log
