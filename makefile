#
#
#

.PHONY: all
all: src

.PHONY: src
src:
	cd src; $(MAKE)

.PHONY: install
install:
	cd src; $(MAKE) install	

.PHONY: clean
clean:
	cd src; $(MAKE) clean

.PHONY: cleanall
cleanall: 
	cd src; $(MAKE) cleanall

.PHONY: clbk
clbk:
	cd src; $(MAKE) clbk 


.PHONY: dos2unix
dos2unix:
	cd src; $(MAKE) dos2unix

.PHONY: unix2dos
unix2dos:
	cd src; $(MAKE) unix2dos

