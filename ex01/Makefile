# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcros <tcros@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/20 11:03:25 by tcros             #+#    #+#              #
#    Updated: 2026/03/10 14:34:25 by tcros            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### VARIABLES ###
# NOM #
EXEC	= RPN

# COMPILATION #
CC 		= c++
INC		= includes/
CFLAGS 	= -Wshadow -Wall -Wextra -Werror -g3 -I$(INC) -std=c++98

# SOURCES #
SRC 	= src/coloring.cpp \
		  src/RPN.cpp \
		  main.cpp

# OBJETS #
OBJ_DIR	= obj
OBJ		= $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

# TOTAL DE FICHIERS #
TOTAL	= $(words $(SRC))

### COULEURS ###
GREEN	= \033[3;32m
RED		= \033[0;31m
ORANGE	= \033[1;33m
BLUE	= \033[0;34m
NC		= \033[0m


### REGLES D'INFERENCES ###
# REGLES PHONY #
.PHONY: all clean fclean re

all: $(EXEC)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ || { \
		printf "$(RED)[ERREUR] Échec de la compilation de $<\n$(NC)"; exit 1; }
	@COUNT=$$(ls -1 $(OBJ) 2>/dev/null | wc -l); \
	RATIO=$$(expr $$COUNT \* 100 / $(TOTAL)); \
	BARS=$$(expr $$RATIO / 10); \
	BAR="["; \
	for i in $$(seq 1 $$BARS); do BAR="$$BAR#"; done; \
	for i in $$(seq $$BARS 10); do BAR="$$BAR-"; done; \
	BAR="$$BAR]"; \
	printf "$(ORANGE)\r$$BAR Compilation de fichier $$COUNT/$(TOTAL) : $<$(NC)\n"

$(EXEC): $(OBJ)
#	@printf "$(BLUE)[INFO] Compilation des dépendances libft...$(NC)"
#	@make -sC $(LIBFT)
	@printf "$(BLUE)[INFO] Liaison finale de $(EXEC)...\n$(NC)"
	@$(CC) -o $(EXEC) $(OBJ) || { \
		printf "$(RED)[ERREUR] Échec de la création de $(EXEC)\n$(NC)"; exit 1; }
	@printf "$(GREEN)[SUCCÈS] $(EXEC) compilé avec succès. ✅\n$(NC)"

clean:
	@rm -rf $(OBJ_DIR)
#	@make clean -sC $(LIBFT)
	@printf "$(ORANGE)[CLEAN] Fichiers objets supprimés.\n$(NC)"

fclean: clean
	@rm -f $(EXEC)
#	@make fclean -sC $(LIBFT)
	@printf "$(RED)[FCLEAN] Binaire supprimé.\n$(NC)"

re: fclean all
