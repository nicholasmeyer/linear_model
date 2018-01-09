linear_model: linear_model.o
	gcc linear_model.o -o linear_model
linear_model.o: linear_model.c
	gcc -c linear_model.c
.PHONY: clean
	rm -f *.o
