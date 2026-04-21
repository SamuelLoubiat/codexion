NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

THREAD_DIR    = thread
PARSER_DIR  = parser
HEADER_DIR  = header
INCLUDES    = -I$(HEADER_DIR)

MAKEFLAGS += --no-print-directory

# ================= COLORS ================= #

RESET   = \033[0m
BOLD    = \033[1m
DIM     = \033[2m

RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
MAGENTA = \033[35m
CYAN    = \033[36m
WHITE   = \033[37m

# ========================================== #

THREAD_SRC    = $(addprefix $(THREAD_DIR)/,\
					thread.c \
					thread_utils.c)

PARSER_SRC  = $(addprefix $(PARSER_DIR)/,\
					parser.c \
					parser_utils.c)

SRC = main.c cleanup.c init.c monitor.c monitor_utils.c mutex_utils.c utils.c $(THREAD_SRC) $(PARSER_SRC)

OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

TOTAL_FILES := $(words $(SRC))
COMPILED := 0

# ========================================== #

define progress_bar
	$(eval COMPILED=$(shell echo $$(($(COMPILED)+1))))
	$(eval PERCENT=$(shell echo $$(($(COMPILED)*100/$(TOTAL_FILES)))))
	@printf "\r$(CYAN)Compiling: [$(GREEN)%3d%%$(CYAN)] $(RESET)" $(PERCENT)
endef

# ========================================== #

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) -g3 $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\n$(GREEN)✔ $(NAME) has compiled !$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning objects...$(RESET)"
	@rm -f $(OBJ) $(DEP) $(OBJ_BONUS) $(DEP_BONUS)
	@echo "$(GREEN)✔ Clean finished$(RESET)"

fclean: clean
	@echo "$(RED)Suppression de $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✔ Full clean finished$(RESET)"

re: fclean all

only: all clean

-include $(DEP)

.PHONY: all clean fclean re only
