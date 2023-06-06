CC = gcc
OBJS1 = event_manager.o event.o date.o node.o pair.o student.o event_manager_tests.o
OBJS2 = priority_queue.o priority_queue_tests.o
EXEC1 = event_manager
EXEC2 = priority_queue
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror --pedantic-errors -DNDEBUG


$(EXEC1) : $(OBJS1)
	$(CC) $(OBJS1) -o $@ -L. -lpriority_queue
$(EXEC2) : $(OBJS2)
	$(CC) $(OBJS2) -o $@
event_manager_tests.o : tests/event_manager_tests.c tests/test_utilities.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) tests/$*.c
priority_queue_tests.o : tests/priority_queue_tests.c tests/test_utilities.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) tests/$*.c
event_manager.o : event_manager.c priority_queue.h event_manager.h date.h event.h node.h student.h pair.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c
node.o : node.c node.h pair.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c
pair.o : pair.c pair.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c
date.o : date.c date.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c
student.o : student.c student.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c
event.o : event.c event.h date.h node.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c
priority_queue.o: priority_queue.c priority_queue.h node.h pair.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean :
	rm -f $(OBJS2) $(EXEC2) $(OBJS1) $(EXEC1)