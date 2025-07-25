##
## EPITECH PROJECT, 2025
## B-CCP-400-NAN-4-1-theplazza-albane.merian
## File description:
## Makefile
##


CC	=	g++

FPIC 	=	-fPIC

CFLAGS	= -std=c++20 -Wall -Wextra -Werror -Wshadow -g

DEBUGFLAG =		-g


RESET	=	\033[0m
CYAN	=	\033[1;96m
MAGENTA =	\033[1;95m
YELLOWBIG	=	\033[1;93m
RED			=	\033[1;91m


SRC_FORK	=	ForkProcess.cpp

PLUGGINS	=	plazza_fork.so


all: $(PLUGGINS)


plazza_fork.so: $(SRC_FORK)
	@echo -e "$(CYAN)Compiling $@...$(RESET)"
	$(CC) $(CFLAGS) $(FPIC) -shared -o $@ $<
	@echo -e "$(MAGENTA)Compilation of $@ done!$(RESET)"
	cp -f *.so ../../plugins/


clean:
	-@rm -fr *.o *.so
	@echo -e "$(YELLOWBIG)PLUGGINS object files cleaned!$(RESET)"

fclean: clean
	-@rm -f *.so
	@echo -e "$(YELLOWBIG)PLUGGINS libraries cleaned!$(RESET)"

re: fclean all

debug:
	@echo -e "$(RED)Debug mode activated!$(RESET)"
	@echo -e "$(YELLOWBIG)Compiling with debug flags...$(RESET)"
	$(CC) $(CFLAGS) $(DEBUGFLAG) $(FPIC) -shared -o \
		plazza_fork.so $(SRC_FORK)
	cp -f *.so ../../plugins/
