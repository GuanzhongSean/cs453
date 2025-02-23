PKG := pkg1

submit:
	zip -r - ${PKG}/* | curl --data-binary @- ugster72d.student.cs.uwaterloo.ca:9000/submit

clean:
	sudo rm -rf pkg*/output-*/
