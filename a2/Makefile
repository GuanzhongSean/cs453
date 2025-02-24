PKG := pkg1

submit:
	zip -r - ${PKG}/* | curl --data-binary @- ugster72d.student.cs.uwaterloo.ca:9000/submit

prepare:
	git clone https://github.com/meng-xu-cs/cs453-program-analysis-platform.git

build: prepare
	cd cs453-program-analysis-platform && ./scripts/ugster-up.sh

zip: clean
	mkdir j76xiao && \
	cp -r pkg* j76xiao/ && \
	cp writeup/WRITEUP1 j76xiao/pkg1/WRITEUP && \
	cp writeup/WRITEUP2 j76xiao/pkg2/WRITEUP && \
	cp writeup/WRITEUP3 j76xiao/pkg3/WRITEUP && \
	cp writeup/WRITEUP4 j76xiao/pkg4/WRITEUP && \
	cp writeup/WRITEUP5 j76xiao/pkg5/WRITEUP && \
	cp writeup/WRITEUP6 j76xiao/pkg6/WRITEUP && \
	cd j76xiao && zip -qr19 ../a2.zip *

clean:
	sudo rm -rf pkg*/output-*/ a2.zip j76xiao/
