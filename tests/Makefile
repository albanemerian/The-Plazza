##
## EPITECH PROJECT, 2025
## B-CCP-400-NAN-4-1-theplazza-albane.merian
## File description:
## Makefile
##

TEST_NAME =	unit_tests

TEST_FLAGS	=	--coverage -lcriterion


RESET	=	\033[0m
YELLOW	=	\033[0;93m

SRC =	../src/utils/Utils.cpp						\
		../common/APizza.cpp						\
		../common/Ingridient.cpp					\
		../common/Socket.cpp						\
		../common/ErrorSocket.cpp					\
		../src/recipes/pizza/Regina.cpp				\
		../common/messages/AStatus.cpp				\
		../common/messages/CookStatus.cpp			\
		../common/messages/DoneStatus.cpp			\
		../common/messages/Inactivity.cpp			\
		../common/messages/Order.cpp				\
		../common/messages/Queue.cpp				\
		../common/messages/RefillStatus.cpp			\
		../src/reception/Reception.cpp				\
		../src/reception/CommandParser.cpp			\
		../src/reception/ErrorReception.cpp			\
		../src/reception/ReceiveMessageKitchen.cpp	\
		../src/kitchen/Kitchen.cpp					\
		../src/kitchen/SendingMessageKitchen.cpp	\
		../src/kitchen/ErrorKitchen.cpp				\
		../src/cooks/Cooks.cpp						\
		../src/cooks/ErrorCooks.cpp



TESTSRC = 	Helper-test.cpp	\
			Pizza-test.cpp	\
			Ingridient-test.cpp	\
			Socket-test.cpp	\
			Regina-test.cpp	\
			AException-test.cpp	\
			Reception-test.cpp	\
			AStatus-test.cpp	\
			Inactivity-test.cpp	\
			Order-test.cpp	\
			Queue-test.cpp	\
			Lib-loader-test.cpp	\
			Cook-test.cpp	\
			Kitchen-test.cpp	\
			CommandParser-test.cpp	\
			CookStatus-test.cpp	\


clean:
	@echo -e "$(YELLOW)Cleaning up...$(RESET)"
	-@rm -rf *.gc*
	-@rm -rf *.gcov
	-@rm -f *.gcno *.gcda
	-@rm -f $(TEST_NAME)

unit_tests: clean
	g++ -o $(TEST_NAME) $(SRC) $(TESTSRC) $(TEST_FLAGS) $(LIBS)


tests_run: unit_tests
	@echo -e "$(YELLOW)Running tests...$(RESET)"
	@./$(TEST_NAME)

coverage: tests_run
	@gcovr --root=.. --exclude='.*-test.cpp' --exclude-directories='tests'
