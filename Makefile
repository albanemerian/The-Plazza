##
## EPITECH PROJECT, 2025
## B-CCP-400-NAN-4-1-theplazza-albane.merian
## File description:
## Makefile
##

RESET	=	\033[0m
YELLOW	=	\033[0;93m
GREEN   =	\033[0;92m

.PHONY: all clean fclean re debug unit_tests tests_run coverage


all:
	@$(MAKE) re -C lib/
	@$(MAKE) all -C common/processes/
	@$(MAKE) all -C src/

clean:
	@-rm plazza.log
	@$(MAKE) clean -C src/
	@$(MAKE) clean -C lib/
	@$(MAKE) clean -C common/processes/

fclean:
	@-rm plazza.log
	@$(MAKE) fclean -C src/
	@$(MAKE) fclean -C lib/
	@$(MAKE) clean -C tests/
	@$(MAKE) fclean -C common/processes/
	-@rm plugins/*.so
	-@rm lib/*.a

re: fclean all

# Unit test compilation

unit_tests: fclean
	@$(MAKE) -C common/processes/
	@$(MAKE) unit_tests -C tests/

tests_run:
	@$(MAKE) tests_run -C tests/

coverage:
	@$(MAKE) coverage -C tests/


# Debug compilation

debug:
	@$(MAKE) debug -C src/
