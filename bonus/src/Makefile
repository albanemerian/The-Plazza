##
## EPITECH PROJECT, 2025
## B-OOP-400-NAN-4-1-raytracer-albane.merian
## File description:
## Makefile
##

NAME	=	plazza

CC	=	g++

CFLAGS	=	-std=c++20 -Wall -Wextra -Werror -Wshadow -g

DEBUGFLAG	=	-g

FPIC 	=	-fPIC

LDFLAGS = -pthread -ldl

RESET	=	\033[0m
GREEN	=	\033[1;92m
BLUE	 =	\033[1;94m
YELLOWBIG	=	\033[1;93m
RED			=	\033[1;91m
CYAN	=	\033[1;96m
MAGENTA =	\033[1;95m

SRC		=	main.cpp 							\
			Plazza.cpp							\
			ErrorParsing.cpp

SRC		+=	../common/Ingridient.cpp			\
			../common/APizza.cpp				\
			../common/Socket.cpp				\
			../common/ErrorSocket.cpp

SRC		+=	../common/messages/AStatus.cpp		\
			../common/messages/DoneStatus.cpp	\
			../common/messages/Inactivity.cpp	\
			../common/messages/Order.cpp		\
			../common/messages/Queue.cpp		\
			../common/messages/RefillStatus.cpp	\
			../common/messages/CookStatus.cpp

SRC		+=	reception/Reception.cpp				\
			reception/CommandParser.cpp			\
			reception/ErrorReception.cpp		\
			reception/ReceiveMessageKitchen.cpp


SRC     +=  kitchen/Kitchen.cpp					\
			kitchen/SendingMessageKitchen.cpp	\
			kitchen/ErrorKitchen.cpp

SRC		+=	cooks/Cooks.cpp						\
			cooks/ErrorCooks.cpp

SRC     +=	utils/Utils.cpp

SRC		+=  ../lib/DLLoader.cpp


PLUGINS 	= 	plazza_american.so		\
				plazza_regina.so		\
				plazza_margherita.so	\
				plazza_fantasia.so	\
				plazza_carbonara.so		\
				plazza_pesto.so			\
				plazza_bolognese.so		\
				plazza_arrabiata.so		\
				plazza_paffo.so			\
				plazza_lasagna.so


SRC_AMERICANA 	=	recipes/pizza/Americana.cpp			\

SRC_REGINA		=	recipes/pizza/Regina.cpp			\

SRC_MARGARITA	=	recipes/pizza/Margarita.cpp			\

SRC_FANTASIA	=	recipes/pizza/Fantasia.cpp			\

SRC_COMMON		=	../common/Ingridient.cpp			\
					../common/APizza.cpp				\
					../common/APasta.cpp

plazza_american.so: $(SRC_AMERICANA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/


plazza_regina.so: $(SRC_REGINA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/


plazza_margherita.so: $(SRC_MARGARITA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/

plazza_fantasia.so: $(SRC_FANTASIA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/



SRC_CARBONARA	=	recipes/pasta/Carbonara.cpp			\

SRC_PESTO		=	recipes/pasta/Pesto.cpp				\

SRC_BOLOGNESE	=	recipes/pasta/Bolognese.cpp			\

SRC_ARRABIATA	=	recipes/pasta/Arrabiata.cpp			\

SRC_PAFFO		=	recipes/pasta/Paffo.cpp				\

SRC_LASAGNA		=	recipes/pasta/Lasagna.cpp			\

plazza_carbonara.so: $(SRC_CARBONARA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/

plazza_pesto.so: $(SRC_PESTO) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/

plazza_bolognese.so: $(SRC_BOLOGNESE) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/

plazza_arrabiata.so: $(SRC_ARRABIATA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/

plazza_paffo.so: $(SRC_PAFFO) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/

plazza_lasagna.so: $(SRC_LASAGNA) $(SRC_COMMON)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $< $(SRC_COMMON)
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../plugins/



OBJ		=	$(SRC:.cpp=.o)


all: $(PLUGINS)  $(NAME)

$(NAME): $(OBJ)
	@echo -e "$(GREEN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo -e "$(BLUE)Compilation of $@ done!$(RESET)"
	mv -f $(NAME) ../

clean:
	-@rm -rf $(OBJ)
	@echo -e "$(YELLOWBIG)Core object files cleaned!$(RESET)"

fclean: clean
	-@rm -f $(NAME) $(TEST_NAME)
	-@rm -f ../$(NAME)
	-@rm -fr *.o *.so
	-@rm -f recipes/pizza/*.o recipes/pasta/*.o
	-@rm *.o
	-@rm *.a
	@echo -e "$(YELLOWBIG)Core binary and test files cleaned!$(RESET)"

re: fclean all

debug:
	@echo -e "$(RED)Compiling in debug mode...$(RESET)"
	$(CC) $(CFLAGS) $(DEBUGFLAG) $(SRC) -o $(NAME) $(LDFLAGS)
	@echo -e "$(BLUE)Compilation of $@ done!$(RESET)"
	cp -f $(NAME) ../
